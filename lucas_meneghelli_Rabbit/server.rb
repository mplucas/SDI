#!/usr/bin/env ruby
require 'bunny'
require 'json'

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('sdi_lucas')

receiveID = 1

begin
  
  puts ' [*] Esperando mensagens. Para sair use CTRL+C'
  
  queue.subscribe(block: true) do |_delivery_info, _properties, body|
    
    message = JSON.parse(body)

    puts " [x] Recebida mensagem de #{message['nickname']}: #{message['content']}"

    fileName = message['nickname'] + '-' + receiveID.to_s.rjust(2, '0') + '.serv'
    file = File.open(fileName, 'w')
    file.write(message['content'])
    file.close

    puts "Mensagem salva em #{fileName}"

    receiveID = receiveID + 1

  end

rescue Interrupt => _

  connection.close

  exit(0)

end
