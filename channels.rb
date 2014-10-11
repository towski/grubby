# UTF_8 = Encoding.find('UTF-8')
require 'rubygems'
require 'active_record'
require 'mysql2'

ActiveRecord::Base.establish_connection(:database => "dwarfomatic", :username => "root", :adapter => "mysql2", :password => "mysql")
class Dwarf < ActiveRecord::Base
end

puts Dwarf.count

Channel.receive do |value|
  puts value
  sleep 1
  Channel.send({:hey => "you"})
  sleep 3
  Channel.send({:hey => "you"})
end
