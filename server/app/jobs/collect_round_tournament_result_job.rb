class String
    def formprint
        if self.length < 14
            self + " " * (15 - self.length)
        else
            self[0..13] + " "
        end
    end
end

class CollectRoundTournamentResultJob < ActiveJob::Base
  queue_as :default

  def perform(tournament_path)
    games_count = File.open(tournament_path + 'trnfull.tmp', 'r').readline.to_i

    output_file_list = []

    while output_file_list.size < games_count
      output_file_list = Dir.entries(tournament_path + 'games').keep_if{|s| s.index('.output')}.keep_if{|s| not s.index('.tmp')}
      sleep(0.5)
    end

    ans = File.open(tournament_path + 'trnfull', 'w')

    has_empty_game_files = false
    output_file_list.sort.each do |file|
        if File.zero?(tournament_path + 'games/' + file)
            has_empty_game_files = true
            break
        end
    end

    if not has_empty_game_files
        table = {}
        names = []
        scores1 = Hash.new(0)
        scores2 = Hash.new(0)
        pl_1, pl_2 = "", ""

        output_file_list.sort.each do |file|
            File.open(tournament_path + 'games/' + file, 'r').readlines[1..-1].each do |line|
                ans.puts line

                if line.index('player1:')
                    pl_1 = line.split[1]
                    names << pl_1
                end
                if line.index('player2:')
                    pl_2 = line.split[1]
                    names << pl_2
                end

                if line.index("0 >> Score:")
                    scores = line[line.index(':') + 2..-1]
                    m = scores.split(' ')[0].split(':')
                    s = scores.split(' ')[1].gsub('(', '').gsub(')', '').split(':')

                    scores1[pl_1] += m[0].to_i
                    scores2[pl_1] += s[0].to_i

                    scores1[pl_2] += m[1].to_i
                    scores2[pl_2] += s[1].to_i

                    scores1[[pl_1, pl_2]] += m[0].to_i
                    scores1[[pl_2, pl_1]] += m[1].to_i

                    scores2[[pl_1, pl_2]] += s[0].to_i
                    scores2[[pl_2, pl_1]] += s[1].to_i
                end
            end
            ans.puts

            FileUtils.rm(tournament_path + 'games/' + file.gsub('.output', '.err'))
        end

        names = names.uniq.compact.sort{|b, a| scores1[a] == scores1[b] ? scores2[a] <=> scores2[b] : scores1[a] <=> scores1[b] }

        names.each do |n1|
            names.each do |n2|
                if n1 == n2
                    table[[n1, n2]] = ""
                else
                    table[[n1, n2]] = scores1[[n1, n2]].to_s + ":" + scores1[[n2, n1]].to_s + " (" + scores2[[n1, n2]].to_s + ":" + scores2[[n2, n1]].to_s + ")"
                end
            end
        end

        table_file = File.open(tournament_path + 'table.txt', 'w')

        line = "".formprint
        names.each{ |s| line += s.formprint }
        line += "score".formprint
        line += "bonus".formprint

        table_file.puts line

        names.each do |a|
            line = a.formprint
            names.each do |b|
                line += table[[a, b]].formprint
            end
            line += scores1[a].to_s.formprint
            line += scores2[a].to_s.formprint
            table_file.puts line
        end

        table_file.close
    else
        ans.puts "Has some empty output game files"
    end
    ans.close

    FileUtils.rm(tournament_path + "trnfull.tmp")
    system("cd #{tournament_path} && zip -r games games/* && rm -r games")
  end
end
