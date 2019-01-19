#!/usr/bin/env ruby

$pool = Hash.new
$pool.default = -3
(0..9).each{|i| (0..9).each {|j| $pool[[i, j]] = 0}}
STDOUT.sync = true
$turn = gets.to_i
$answer = ($turn == 1) ? "00" : "99"
$pool[[$answer[0].to_i, $answer[1].to_i]] = $turn

def bfs(i0, j0, type, finish)
        visited = {[i0, j0] => true}
        q = [[i0, j0]]
        while (!q.empty?)
                top = q.shift
                (top[0]-1..top[0]+1).each do |i|
                        (top[1]-1..top[1]+1).each do |j|
                                if (!visited[[i, j]] and type.include? $pool[[i, j]])
                                        return true if $pool[[i, j]] == finish
                                        q.push [i, j]
                                        visited[[i, j]] = true
                                end
                        end
                end
        end
        false
end

def valid(i, j)
        case
                when $pool[[i, j]] == 0 then bfs(i, j, [$turn], $turn)
                when $pool[[i, j]] == 3 - $turn then bfs(i, j, [$turn, -(3 - $turn)], $turn)
                else false
        end
end

def random_move
        k = 0
        while $answer.length < 6 and k < 1000
                i, j = rand(10), rand(10)
                if valid(i, j)
                        $pool[[i, j]] = ($pool[[i, j]] == 3 - $turn) ? -(3 - $turn) : $turn
                        $answer += i.to_s + j.to_s
                end
                k += 1
        end
        return $answer
end

if $turn == 1
        print random_move + "\n"
        $answer = ""
end
while true
        s = gets.chomp
        3.times do |t|
                i, j = s[t*2].to_i, s[t*2+1].to_i
                $pool[[i, j]] = ($pool[[i, j]] == $turn) ? -$turn : 3 - $turn
        end
        print random_move + "\n"
        $answer = ""
end
