while s = gets
    charset = Array('A'..'Z') + Array('a'..'z')
    puts "#{s.chomp} #{charset.sample(10).join} http://84.201.184.249:3000/login"
end
