#!/usr/bin/env ruby
require 'bunny'
require 'json'

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('sdi_lucas')

sendID = 0

begin

    while true

        puts "Verificando se o arquivo '#{sendID}' existe..."

        if File.exists?(sendID.to_s)

            file = File.open(sendID.to_s)

            file_data = file.read

            file.close
            
            message = {:nickname => 'lucas', :content => file_data}

            channel.default_exchange.publish(message.to_json, routing_key: queue.name)
            puts " [x] Enviada mensagem #{message} para o Server"

            sendID = sendID + 1

        elsif

            puts "Arquivo #{sendID} não existe, verificando novamente em 5 segundos..."

        end

        sleep(5)

    end

rescue Interrupt => _

    connection.close
    
    exit(0)
    
end
