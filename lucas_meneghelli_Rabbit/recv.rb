#!/usr/bin/env ruby
require 'bunny'
require 'json'

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('hello')

begin
  
  puts ' [*] Waiting for messages. To exit press CTRL+C'
  
  queue.subscribe(block: true) do |_delivery_info, _properties, body|
    
    puts " [x] Received #{body}"

    message = JSON.parse(body)

    puts " [x] Received from #{message['nickname']}: #{message['content']}"

  end

rescue Interrupt => _

  connection.close

  exit(0)
  
end
