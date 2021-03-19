#!/usr/bin/env ruby
require 'bunny'
require 'json'

connection = Bunny.new(automatically_recover: false)
connection.start

channel = connection.create_channel
queue = channel.queue('hello')

begin

    while true

        message = {:nickname => 'lucas', :content => 'Hello World!'}

        channel.default_exchange.publish(message.to_json, routing_key: queue.name)
        puts " [x] Sent 'Hello World!'"

    end

rescue Interrupt => _

    connection.close
    
    puts "Rescued"

    exit(0)
    
end
