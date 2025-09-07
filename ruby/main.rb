#!/usr/bin/env ruby
require 'json'

puts '=== Ruby demo ==='

User = Struct.new(:id, :name)
users = [User.new(1, 'Ada'), User.new(2, 'Alan')]
puts users.map { |u| "#{u.id},#{u.name}" }.join("\n")
puts JSON.pretty_generate(users.map { |u| { id: u.id, name: u.name } })

text = 'Hello hello world'
counts = Hash.new(0)
text.downcase.scan(/[a-z]+/) { |w| counts[w] += 1 }
p counts

Dir.entries('.').first(5).each { |e| puts "- #{e}" }