class CollectMatchResultJob < ActiveJob::Base
  queue_as :default
  sidekiq_options retry: 0

  def perform(match_path)
    tmp_match_file = File.open(match_path + 'match.output.tmp', 'r')

    first_line = tmp_match_file.readline
    game = tmp_match_file.readline

    player_1 = first_line.split[1].split('/')[-1]
    player_2 = first_line.split[3].split('/')[-1]

    games_count = tmp_match_file.readline.split[1].to_i
    tmp_match_file.close

    output_file_list = []

    while output_file_list.size < games_count
      output_file_list = Dir.entries(match_path).keep_if{|s| s.index('.output')}.keep_if{|s| not s.index('.tmp')}
      sleep(0.5)
    end

    ans = File.open(match_path + 'match.output', 'w')
    ans.puts first_line
    ans.puts game
    ans.puts

    scores = {player_1 => [0, 0], player_2 => [0, 0]}
    pl_1, pl_2 = "", ""

    has_empty_game_files = false
    output_file_list.sort.each do |file|
        if File.zero?(match_path + file)
            has_empty_game_files = true
            break
        end
    end

    if not has_empty_game_files
        output_file_list.sort.each do |file|
            File.open(match_path + file, 'r').readlines[1..-1].each do |line|
                ans.puts line

                if line.index('player1:')
                    pl_1 = line.split[1]
                end
                if line.index('player2:')
                    pl_2 = line.split[1]
                end

                if line.index("0 >> Score:")
                    s = line[line.index(':') + 2..-1]
                    main_score = s.split(' ')[0]
                    second_score = s.split(' ')[1].gsub('(', '').gsub(')', '')

                    scores[pl_1][0] += main_score.split(':')[0].to_i
                    scores[pl_1][1] += second_score.split(':')[0].to_i

                    scores[pl_2][0] += main_score.split(':')[1].to_i
                    scores[pl_2][1] += second_score.split(':')[1].to_i
                end
            end
            ans.puts
        end

        if (scores[player_1][0] > scores[player_2][0]) or (scores[player_1][0] == scores[player_2][0] and scores[player_1][1] > scores[player_2][1])
            ans.puts "winner: 1"
        elsif (scores[player_1][0] < scores[player_2][0]) or (scores[player_1][0] == scores[player_2][0] and scores[player_1][1] < scores[player_2][1])
            ans.puts "winner: 2"
        else
            ans.puts "winner: 0"
        end
        ans.puts "score: #{scores[player_1][0]}:#{scores[player_2][0]} (#{scores[player_1][1]}:#{scores[player_2][1]})"
    end
    ans.close

    err = File.open(match_path + 'match.err', 'w')
    err_file_list = Dir.entries(match_path).keep_if{|s| s.index('.err')}

    err_file_list.sort.each do |file|
      File.open(match_path + file, 'r').readlines.each do |line|
        err.puts line
      end
      err.puts
    end
    err.close

    FileUtils.rm(match_path + "match.output.tmp")
  end
end
