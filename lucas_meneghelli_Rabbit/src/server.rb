#!/usr/bin/env ruby
require 'bunny'
require 'json'

class ChatServer

    attr_accessor :receiveID

    def initialize(host, noFlood)

        @host = host
        @noFlood = noFlood

        @receiveID = 0
        @usedNicknames = []
        @savedFilesNames = []
        @currentClientID = 1
        
        @connection = Bunny.new(automatically_recover: false, :host => host)
        @connection.start
        @channel = @connection.create_channel

    end

    def start(queue_name)
    
        @queue = channel.queue(queue_name)
        @exchange = channel.default_exchange
        subscribe_to_queue
    
    end

    def stop
        channel.close
        connection.close
    end

    def loop_forever
        loop { sleep 5 }
    end

    private

    attr_reader :channel, :exchange, :queue, :connection, :usedNicknames,
    :currentClientID, :savedFilesNames, :noFlood

    def subscribe_to_queue

        queue.subscribe do |_delivery_info, properties, payload|

            result = getResultFor(payload)

            exchange.publish(
                result.to_json,
                routing_key: properties.reply_to,
                correlation_id: properties.correlation_id
            )

        end

    end


    def getResultFor(payload)

        message = JSON.parse(payload)

        if message['type'] == 'nicknameAuth'

            return nicknameCanBeUsed(message['nickname'])

        elsif message['type'] == 'message'

            saveInFile(message)
            return 'OK'

        elsif message['type'] == 'requestMessageID'

            return receiveID
        
        elsif message['type'] == 'requestMessage'

            return readMessageWith(message['messageID'])
        
        elsif message['type'] == 'unlockNickname'

            unlockNickname(message['nickname'])

        else

            puts "Recebida mensagem com type não reconhecido. type: #{message['type']}"
            return 'NOK'

        end

    end

    def nicknameCanBeUsed(nickname)

        puts " [x] Requisitada utilização do nickname #{nickname}"

        if !usedNicknames.include?(nickname)
            
            puts "Autorizado"
            usedNicknames.push(nickname)

            clientID = currentClientID
            @currentClientID = currentClientID + 1
            return clientID

        else
            puts "Não autorizado"
            return 'NOK'
        end

    end

    def saveInFile(message)

        notification = " [x] Recebida mensagem de #{message['nickname']}" + (noFlood ? "" : ": #{message['content']}")
        puts notification

        fileName = message['nickname'] + '-' + (receiveID + 1).to_s.rjust(2, '0') + '.serv'
        file = File.open(fileName, 'w')
        file.write(message['content'])
        file.close

        puts "Mensagem salva em #{fileName}"

        savedFilesNames.push(fileName)

        @receiveID = receiveID + 1

    end

    def readMessageWith(messageID)

        arrayIndex = messageID - 1

        file = File.open(savedFilesNames[arrayIndex])
        file_data = file.read
        file.close

        nickname = savedFilesNames[arrayIndex][0..(savedFilesNames[arrayIndex].index('-') - 1)]
        message = {:nickname => nickname, :content => file_data}

        return message

    end

    def unlockNickname(nickname)
        
        if usedNicknames.delete(nickname) != nil
            puts " [x] Liberado nickname #{nickname}"
        end
    end

end

if ARGV.length < 1 or ARGV.length > 2 or (ARGV.length == 2 and ARGV[1] != "--no-flood")

    puts "Utilização errada do arquivo, execute da seguinte maneira: ruby server.rb <host> [--no-flood]"
    exit(0)

end

host = ARGV[0]
noFlood = ARGV.length == 2 ? true : false

begin

    server = ChatServer.new(host, noFlood)

    puts ' [*] Esperando mensagens. Para sair use CTRL+C'
    server.start('sdi_lucas')
    server.loop_forever

rescue Interrupt => _
    
    server.stop
    exit(0)

end