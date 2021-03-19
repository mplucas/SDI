#!/usr/bin/env ruby
require 'bunny'
require 'json'

if ARGV.length != 1

    puts "Utilização errada do arquivo, execute da seguinte maneira: ruby client.rb <nickname>"
    exit(0)

end

nickname = ARGV[0]

sendID = 1

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('sdi_lucas')

begin

    while true

        fileName = nickname + '-' + sendID.to_s.rjust(2, '0') + '.chat'
        puts "Verificando se o arquivo '#{fileName}' existe..."

        if File.exists?(fileName)

            file = File.open(fileName)

            file_data = file.read

            file.close
            
            message = {:nickname => 'lucas', :content => file_data}

            channel.default_exchange.publish(message.to_json, routing_key: queue.name)
            puts " [x] Enviada mensagem #{message} para o Server"

            sendID = sendID + 1

        elsif

            puts "Arquivo #{fileName} não existe, verificando novamente em 5 segundos..."

        end

        sleep(5)

    end

rescue Interrupt => _

    connection.close
    
    exit(0)
    
end
