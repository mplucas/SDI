#!/usr/bin/env ruby
require 'bunny'
require 'json'

class ChatClient

    attr_accessor :call_id, :response, :lock, :condition, :connection,
    :channel, :server_queue_name, :reply_queue, :exchange, :nickname,
    :sendID, :receiveID, :clientID, :noFlood, :host

    def initialize(server_queue_name, nickname, host, noFlood)

        @nickname = nickname
        @host = host
        @noFlood = noFlood

        @clientID = -1
        @sendID = 0
        @receiveID = 1

        @connection = Bunny.new(automatically_recover: false, :host => host)
        @connection.start

        @channel = connection.create_channel
        @exchange = channel.default_exchange
        @server_queue_name = server_queue_name

        setup_reply_queue

    end
    
    def stop
        unlockNickname
        channel.close
        connection.close
    end

    def send(message, hasToLock=true)

        @call_id = generate_uuid

        exchange.publish(message.to_json,
            routing_key: server_queue_name,
            correlation_id: call_id,
            reply_to: reply_queue.name
        )

        if hasToLock
            # wait for the signal to continue the execution
            lock.synchronize { condition.wait(lock) }

            return response
        end

        return 0

    end

    def requestNicknameAuthorization
    
        message = {:type => 'nicknameAuth', :nickname => nickname}

        puts " [x] Requisitando uso do nickname #{nickname} para o Server"
        result = send(message)

        if result == 'NOK'
            puts "Nickname já utilizado. Abortando."
            exit(0)
        else
            puts "Nickname autorizado"
            @clientID = result
        end

    end
    
    def startSendAndReceive

        while true

            sendMessage
            pollMessages

            puts " [*] Envio e polling de mensagens concluidos. Executando novamente em 5 segundos...\n\n"
    
            sleep(5)
    
        end

    end

    private

    def sendMessage
    
        hasMessagesToSend = true

        while hasMessagesToSend
        
            fileName = nickname + '-' + (sendID + 1).to_s.rjust(2, '0') + '.chat'
            puts " [x] Verificando se o arquivo '#{fileName}' existe para o enviar como mensagem ao server"

            if File.exists?(fileName)

                file = File.open(fileName)
                file_data = file.read
                file.close
                
                message = {:type => 'message', :nickname => nickname, :content => file_data}
                notification = " [x] Enviada mensagem para o Server de #{fileName}" + (noFlood ? "" : ": #{message}")
                puts notification
                result = send(message)

                if result == 'OK'
                    
                    @sendID = sendID + 1

                else

                    puts " [x] Recebida resposta errada #{result} do server após envio, tentará enviar novamente a mensagem na próxima checagem"
                    hasMessagesToSend = false

                end

            else

                puts "Arquivo #{fileName} não existe, não enviando mensagens ao server"
                hasMessagesToSend = false

            end

        end

    end

    def pollMessages
    
        puts " [x] Verificando se há novas mensagens no Server"
        serverMessageID = requestServerMessageID

        if receiveID > serverMessageID
            puts "Nenhuma mensagem nova no Server"
        end

        while receiveID <= serverMessageID
        
            puts " [x] Requisitando mensagem #{receiveID} do Server"
            message = requestMessageFromServer(receiveID)

            if message['nickname'] != nickname

                notification = "Recebida mensagem de #{message['nickname']}" + (noFlood ? "" : ": #{message['content']}")
                puts notification
                
                fileName = nickname + '-' + receiveID.to_s.rjust(2, '0') + '.client' + clientID.to_s.rjust(2, '0')
                file = File.open(fileName, 'w')
                file.write(message['content'])
                file.close
                
                puts "Mensagem salva em #{fileName}"

            else
                puts "Mensagem #{receiveID} é sua, não salvando"
            end

            @receiveID = receiveID + 1
        
        end
    
    end

    def requestServerMessageID
    
        message = {:type => 'requestMessageID'}
        result = send(message)
        return result
    
    end

    def requestMessageFromServer(messageID)

        message = {:type => 'requestMessage', :messageID => messageID}
        result = send(message)
        return result

    end

    def setup_reply_queue

        @lock = Mutex.new
        @condition = ConditionVariable.new
        that = self
        @reply_queue = channel.queue('', exclusive: true)

        reply_queue.subscribe do |_delivery_info, properties, payload|

            if properties[:correlation_id] == that.call_id
                
                that.response = JSON.parse(payload)
                # sends the signal to continue the execution of #send
                that.lock.synchronize { that.condition.signal }

            end

        end

    end

    def generate_uuid
        # very naive but good enough for code examples
        "#{rand}#{rand}#{rand}"
    end

    def unlockNickname
    
        message = {:type => 'unlockNickname', :nickname => nickname}
        puts " [x] Liberado nickname #{nickname} no Server"
        result = send(message, false)
    
    end

end

if ARGV.length < 2 or ARGV.length > 3 or (ARGV.length == 3 and ARGV[2] != "--no-flood")

    puts "Utilização errada do arquivo, execute da seguinte maneira: ruby client.rb <nickname> <host> [--no-flood]"
    exit(0)

end

nickname = ARGV[0]
host = ARGV[1]
noFlood = ARGV.length == 3 ? true : false

begin
    
    client = ChatClient.new('sdi_lucas', nickname, host, noFlood)

    client.requestNicknameAuthorization
    
    client.startSendAndReceive

rescue Interrupt => _

    client.stop
    exit(0)
    
end