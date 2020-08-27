class TournamentsController < ApplicationController
  def round_tournament
    if not current_user or current_user.email != 'admin'
        redirect_to show_game_path
        return
    end

    if not params[:name] or params[:name] == ""
        redirect_to show_game_path
        return
    end

    if Dir.exist?(Rails.root.to_s + "/static-files/tournaments/#{params[:game]}/#{params[:name]}")
        redirect_to show_game_path
        return
    end

    sources = []
    sources_dir = Rails.root.to_s + "/static-files/sources/#{params[:game]}/users/"

    params.each do |k, v|
        sources << sources_dir + v if k.index("source")
    end

    if sources.size < 2
        redirect_to show_game_path
        return
    end

    cgi = {}

    if params['cgi']
        params['cgi'].split('&').each do |par|
            par_split = par.split('=')
            cgi[par_split[0]] = par_split[1] if par_split.size > 1
        end
    end

    config_number = (cgi['config_number'] || '1').to_i

    t_path = Rails.root.to_s + "/static-files/tournaments/#{params[:game]}/#{params[:name]}/"
    FileUtils.mkdir_p(t_path + 'games')

    f = File.open(t_path + "trnfull.tmp", 'w')
    f.puts(sources.size * (sources.size - 1))
    f.close

    n = 0
    (0..sources.size - 1).each do |i|
        (i + 1..sources.size - 1).each do |j|
            name_1 = sources[i].split("/")[-1]
            name_2 = sources[j].split("/")[-1]
            RunGameJob.perform_later(params[:game], sources[i], sources[j], name_1, name_2, t_path + 'games/', n * 2, true, config_number)
            RunGameJob.perform_later(params[:game], sources[j], sources[i], name_2, name_1, t_path + 'games/', n * 2 + 1, true, config_number)
            n += 1
        end
    end

    CollectRoundTournamentResultJob.perform_later(t_path)

    redirect_to show_game_path
  end
end
