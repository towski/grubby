class Encoding::UTF_7
end
class Encoding
  UTF_8 = Encoding.find('UTF-8')
end
require 'enc/utf_16be'
require 'enc/utf_16le'
require 'enc/utf_32be'
require 'enc/utf_32le'
require 'rubygems'
require 'active_record'
require 'mysql2'

ActiveRecord::Base.establish_connection(:database => "dwarfomatic", :username => "root", :adapter => "mysql2", :password => "mysql")
class Dwarf < ActiveRecord::Base
end

puts Dwarf.count

value = Channel.receive
puts value
sleep 1
Channel.send({:hey => "you"}.to_json)
sleep 3
Channel.send({:hey => "you"}.to_json)
