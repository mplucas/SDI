#!/usr/bin/env ruby
require 'bunny'
require 'json'

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('sdi_lucas')

begin
  
  puts ' [*] Esperando mensagens. Para sair use CTRL+C'
  
  queue.subscribe(block: true) do |_delivery_info, _properties, body|
    
    message = JSON.parse(body)

    puts " [x] Recebida mensagem de #{message['nickname']}: #{message['content']}"

  end

rescue Interrupt => _

  connection.close

  exit(0)

end
