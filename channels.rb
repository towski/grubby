class Encoding::UTF_7
end
puts Encoding.list
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

begin
  dwarf = Dwarf.find(1)
rescue
end

Channel.receive
sleep 1
Channel.send
