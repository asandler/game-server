#!/usr/bin/env ruby

$pool = Hash.new
$pool.default = false
STDOUT.sync = true
$turn = 1

$snakes = [[], [[3, 0], [2, 0], [1, 0], [0, 0]], [[26, 39], [27, 39], [28, 39], [29, 39]]]
$directions = [[], [1, 0], [-1, 0]]
$r_rot = {[1, 0] => [0, -1], [0, -1] => [-1, 0], [-1, 0] => [0, 1], [0, 1] => [1, 0]}
$l_rot = {[1, 0] => [0, 1], [0, 1] => [-1, 0], [-1, 0] => [0, -1], [0, -1] => [1, 0]}
$grow_queues = [[], [], []]

class Array
    def coord_sum a
        ans = []
        (0..self.length - 1).each{ |i| ans << (self[i] + a[i]) }
        ans
    end
end

def update_pool s, turn
    $directions[turn] = $l_rot[$directions[turn]] if s == 'L'
    $directions[turn] = $r_rot[$directions[turn]] if s == 'R'
    head = $snakes[turn][0].coord_sum $directions[turn]
    $grow_queues[turn][$snakes[turn].length + $grow_queues[turn].compact.length] = $pool[head] == 1 ? head : nil
    $pool[head] = 0
    $snakes[turn].pop if not $grow_queues[turn].shift
    $snakes[turn].unshift head
end

def free i0, j0, type, finish
    visited = {[i0, j0] => true}
    visited.default = false
    q = [[i0, j0]]
    ans = 0
    while q.any?
        t = q.shift
        [[t[0], t[1] + 1], [t[0], t[1] - 1], [t[0] + 1, t[1]], [t[0] - 1, t[1]]].each do |i, j|
            if not visited[[i, j]] and type.include? $pool[[i, j]] and not finish.include? [i, j]
                q.push [i, j]
                ans += 1
                return ans if ans > 50
            end
            visited[[i, j]] = true
        end
    end
    return ans
end

def measure head, turn
    ans = 100000
    return 200000 if free(head[0], head[1], [0, 1], $snakes[1] + $snakes[2]) < 50
    return 300000 if $snakes[1].include? head or $snakes[2].include? head or not $pool[head]
    (0..29).each do |i|
        (0..39).each do |j|
            ans = [ans, (i - head[0]).abs + (j - head[1]).abs].min if $pool[[i, j]] == 1
            return 100000 if (i - head[0]).abs + (j - head[1]).abs < 4 and $snakes[3 - turn].include? [i, j]
        end
    end
    ans
end

def make_move turn
    head_l = $snakes[turn][0].coord_sum $l_rot[$directions[turn]]
    head_r = $snakes[turn][0].coord_sum $r_rot[$directions[turn]]
    head_f = $snakes[turn][0].coord_sum $directions[turn]
    [[measure(head_l, turn), "L"], [measure(head_r, turn), "R"], [measure(head_f, turn), "F"]].sort[0][1]
end

$turn = gets.chomp.to_i
30.times do |i|
    gets.chomp.each_char.map{|c| c.to_i}.each_with_index do |val, j|
        $pool[[i, j]] = val
    end
end

if $turn == 1
    m = make_move $turn
    puts m
    update_pool m, $turn
end
loop do
    update_pool gets.chomp, 3 - $turn
    m = make_move $turn
    puts m
    update_pool m, $turn
end
