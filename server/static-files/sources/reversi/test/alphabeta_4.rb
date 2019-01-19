#!/usr/bin/env ruby

def go_direction pool, start, i, j, turn, to_change
    count, go = 0, [0, 0]
    go[0] = start[0] + i
    go[1] = start[1] + j

    while pool[go] != 0 and pool[go] != turn
        go[0] += i
        go[1] += j
        count += 1
    end

    return 0 if pool[go] == 0
    return count if not to_change

    go[0] -= i
    go[1] -= j

    while pool[go] != pool[start]
        pool[go] = turn
        go[0] -= i
        go[1] -= j
    end

    return count
end

class Hash
    def valid s, turn, to_change
        return false if s.length != 2
        return false if !('a'..'h').include?(s[0])
        return false if !('1'..'8').include?(s[1])
        m = [s[0].ord - 'a'.ord + 1, s[1].to_i]
        return false if self[m] != 0
        self[m] = turn if to_change
        
        sum = 0
        [-1, 0, 1].each do |i|
            [-1, 0, 1].each do |j|
                sum += go_direction(self, m, i, j, turn, to_change) if i != 0 || j != 0
            end
        end
        return sum > 0
    end

    def formprint
        w = {1=>'a', 2=>'b', 3=>'c', 4=>'d', 5=>'e', 6=>'f', 7=>'g', 8=>'h'}
        STDERR.puts " 12345678"
        (1..8).each do |i|
            STDERR.print w[i]
            (1..8).each do |j|
                STDERR.print "*" if self[[i, j]] == 1
                STDERR.print "&" if self[[i, j]] == 2
                STDERR.print "_" if self[[i, j]] == 0
            end
            STDERR.puts
        end
        sleep 0.5
    end
end

def eval_pool pool
    sc = 0
    (1..8).each do |i|
        (1..8).each do |j|
            sc -= 1 if pool[[i, j]] == 1
            sc += 1 if pool[[i, j]] == 2
        end
    end
    return sc
end

def update_pool pool, move, turn
    pool.valid(move, turn, true)
end


def make_move pool, turn, depth, a, b
    return eval_pool(pool) if depth == 4

    valid_moves = []
    ('a'..'h').each do |a|
        ('1'..'8').each do |b|
            valid_moves << a + b if pool.valid(a + b, turn, false)
        end
    end

    if turn == 1
        best_move = "Skip"
        valid_moves.each do |move|
            new_pool = pool.dup
            update_pool(new_pool, move, turn)

            ans = make_move(new_pool, 3 - turn, depth + 1, a, b)
            b, best_move = ans, move if ans < b
            break if b <= a
        end
        return valid_moves[rand(valid_moves.size)] if valid_moves.size > 0 and best_move == "Skip" and depth == 0
        return ((depth == 0) ? best_move : b)
    else
        best_move = "Skip"
        valid_moves.each do |move|
            new_pool = pool.dup
            update_pool(new_pool, move, turn)

            ans = make_move(new_pool, 3 - turn, depth + 1, a, b)
            a, best_move = ans, move if ans > a
            break if b <= a
        end
        return valid_moves[rand(valid_moves.size)] if valid_moves.size > 0 and best_move == "Skip" and depth == 0
        return ((depth == 0) ? best_move : a)
    end
end

pool = Hash.new(0)
STDOUT.sync = true
turn, winner, score = 1, nil, [0, 0, 0]

pool[[4, 4]] = 2
pool[[5, 5]] = 2
pool[[4, 5]] = 1
pool[[5, 4]] = 1

turn = gets.chomp.to_i
if turn == 1
    m = make_move(pool, turn, 0, -10000, 10000)
    puts m
    update_pool(pool, m, turn)
end
loop do
    update_pool(pool, gets.chomp, 3 - turn)
    m = make_move(pool, turn, 0, -10000, 10000)
    puts m
    update_pool(pool, m, turn)
end
