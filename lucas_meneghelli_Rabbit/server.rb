#!/usr/bin/env ruby
require 'bunny'
require 'json'

class ChatServer

    attr_accessor :receiveID

    def initialize

        @receiveID = 1
        
        @connection = Bunny.new(automatically_recover: false)
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

    attr_reader :channel, :exchange, :queue, :connection

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

        if message['type'] == 'message'

            saveInFile(message)

            return 'OK'

        else

            puts "Recebida mensagem com type não reconhecido. type: #{message['type']}"

            return 'NOK'

        end

    end

    def saveInFile(message)

        puts " [x] Recebida mensagem de #{message['nickname']}: #{message['content']}"

        fileName = message['nickname'] + '-' + receiveID.to_s.rjust(2, '0') + '.serv'
        file = File.open(fileName, 'w')
        file.write(message['content'])
        file.close

        puts "Mensagem salva em #{fileName}"

        @receiveID = receiveID + 1

    end

end

begin

    server = ChatServer.new

    puts ' [*] Esperando mensagens. Para sair use CTRL+C'
    server.start('sdi_lucas')
    server.loop_forever

rescue Interrupt => _
    
    server.stop
    exit(0)

end