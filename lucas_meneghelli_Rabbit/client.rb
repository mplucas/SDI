#!/usr/bin/env ruby
require 'bunny'
require 'json'

class ChatClient

    attr_accessor :call_id, :response, :lock, :condition, :connection,
    :channel, :server_queue_name, :reply_queue, :exchange, :nickname,
    :sendID, :receiveID

    def initialize(server_queue_name, nickname)

        @nickname = nickname
        @clientID = -1
        @sendID = 1
        @receiveID = 1

        @connection = Bunny.new(automatically_recover: false)
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

        if result != 'NOK'
            puts "Nickname já utilizado. Abortando."
            exit(0)
        else
            puts "Nickname autorizado"
            @clientID = result
        end

    end
    
    def stardSendAndReceive

        while true

            sendMessage
            pollMessages
    
            sleep(5)
    
        end

    end

    private

    def sendMessage
    
        fileName = nickname + '-' + sendID.to_s.rjust(2, '0') + '.chat'
        puts "Verificando se o arquivo '#{fileName}' existe..."

        if File.exists?(fileName)

            file = File.open(fileName)
            file_data = file.read
            file.close
            
            message = {:type => 'message', :nickname => nickname, :content => file_data}
            puts " [x] Enviada mensagem #{message} para o Server"
            result = send(message)

            if result == 'OK'
                
                @sendID = sendID + 1

            else

                puts " [x] Recebida resposta errada #{result}."

            end

        elsif

            puts "Arquivo #{fileName} não existe, verificando novamente em 5 segundos..."

        end

    end

    def pollMessages
    
        serverMessageID = requestServerMessageID

        while receiveID <= serverMessageID
        
            message = requestMessageFromServer(receiveID)

            if message['nickname'] != nickname

                fileName = nickname + '-' + receiveID.to_s.rjust(2, '0') + '.client' + clientID.to_s.rjust(2, '0')
                file = File.open(fileName, 'w')
                file.write(file_data)
                file.close

            else
                puts "Mensagem #{receiveID} é sua, não salvando."
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

if ARGV.length != 1

    puts "Utilização errada do arquivo, execute da seguinte maneira: ruby client.rb <nickname>"
    exit(0)

end

nickname = ARGV[0]

begin
    
    client = ChatClient.new('sdi_lucas', nickname)

    client.requestNicknameAuthorization
    
    client.stardSendAndReceive

rescue Interrupt => _

    client.stop
    exit(0)
    
end