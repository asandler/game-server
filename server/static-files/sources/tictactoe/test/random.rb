#!/usr/bin/env ruby

$pool = Array.new(9){|i| i}

def random_move
        r = rand($pool.size)
        p $pool[r]
        $pool.delete($pool[r])
end

random_move if (t = gets.to_i) == 1
while true
        t = gets.to_i
        $pool.delete(t)
        random_move if $pool.size > 0
end
