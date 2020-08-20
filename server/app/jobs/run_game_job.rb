class RunGameJob < ActiveJob::Base
  queue_as :default
  sidekiq_options retry: 0

  def perform(name, game_file_1, game_file_2, name_1, name_2, output_path, game_num, is_tournament, config_number=nil)
    File.open(output_path + "#{game_num}.output.tmp", "w") do |f|
        f.puts(game_file_1.split('/')[-1] + ' ' + game_file_2.split('/')[-1])
    end
    
    s = Redis::Semaphore.new(:runner_semaphore, :host => "localhost")
    free_runner = nil

    while true
        s.lock do
            ActiveRecord::Base.connection.clear_query_cache
            free_runner = Runner.find_by(:status => 'free')

            if free_runner
                free_runner.status = 'load'
                free_runner.save
            end
        end

        break if free_runner
        sleep(0.1)
    end

    result = RestClient::Request.execute(
        :method => :post,
        :url => free_runner.host + '/run',
        :timeout => nil,
        :open_timeout => nil,
        :payload => {
            :game => name,
            :player1 => File.new(game_file_1),
            :player2 => File.new(game_file_2),
            :name1 => name_1,
            :name2 => name_2,
            :count => 1,
            :sec => Rails.application.config.time_limits[name + ".sec"],
            :usec => Rails.application.config.time_limits[name + ".usec"],
            :tournament_game => (is_tournament ? 1 : 0),
            :config_number => config_number || 'random'
        }
    )

    output = ""
    err = ""

    if result.index("ERROR")
        output = ""
        err = result
    else
        if result.index("STDERR:")
            output = result[0..result.index("STDERR:") - 1]
            err = result[result.index("STDERR:")..-1]
        else
            output = result
            err = ""
        end
    end

    File.open(output_path + "#{game_num}.output", "w"){|f| f.write(output)}
    File.open(output_path + "#{game_num}.err", "w"){|f| f.write(err)}
    FileUtils.rm(output_path + "#{game_num}.output.tmp")

    s.lock do
        free_runner.status = 'free'
        free_runner.save
    end
  end
end
