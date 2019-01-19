require 'fileutils'

class GamesController < ApplicationController
  def new_file
    notice = ""
    errors = false

    if not (params[:upload] and params[:upload][:datafile])
        redirect_to show_game_path
        return
    end

    if params[:upload][:datafile].original_filename.length >= 42
        notice += "\nFile name must not exceed 42 symbols. "
        errors = true
    end
    if params[:upload][:datafile].tempfile.size == 0 or params[:upload][:datafile].tempfile.size >= 256 * 1024
        notice += "\nFile size must not exceed 256 KB and be greater than zero. "
        errors = true
    end

    if errors
        @msg = notice
        @user = current_user
        render :show
        return
    end

    src_path = params[:upload][:datafile].tempfile.path
    dst_path = Rails.root.to_s + "/static-files/sources/" + params[:game] + "/users/" + current_user.email + "/" + params[:upload][:datafile].original_filename
    FileUtils.copy_file(src_path, dst_path)
    redirect_to show_game_path
  end

  def download_file
    name = params[:path]
    if name.index(current_user.email) || current_user.email == "admin" || name.index("tournaments")
        filename = File.join(Rails.root.to_s, params[:path])
        send_file(filename, :type => 'text/plain', :disposition => 'inline')
    else
        redirect_to show_game_path
    end
  end

  def download_html
    name = params[:path]
    filename = File.join(Rails.root.to_s, params[:path])
    send_file(filename, :type => 'text/html', :disposition => 'inline')
  end

  def download_pdf
    name = params[:path]
    filename = File.join(Rails.root.to_s, params[:path])
    send_file(filename, :type => 'application/pdf', :disposition => 'inline')
  end

  def new_match
    if not params[:count] or not ('1'..'5').include? params[:count][0]
        redirect_to show_game_path
        return
    end
    if (not params[:first]) or (params[:first] == "") or (params[:first].index("-----"))
        redirect_to show_game_path
        return
    end
    if (not params[:second]) or (params[:second] == "") or (params[:second].index("-----"))
        redirect_to show_game_path
        return
    end

    match_num = Dir.entries(Rails.root.to_s + "/static-files/games/#{params[:game]}/#{current_user.email}/").map{|s| s.to_i}.max || -1
    match_num = (match_num + 1).to_s
    match_path = Rails.root.to_s + "/static-files/games/#{params[:game]}/#{current_user.email}/#{match_num}/"
    FileUtils.mkdir_p(match_path)

    File.open(match_path + "match.output.tmp", "w") do |f|
        f.puts('match: ' + params[:first].split('/')[-1] + ' vs. ' + params[:second].split('/')[-1])
        f.puts('game: ' + params[:game])
        f.puts('games_count: ' + (2 * params[:count][0].to_i).to_s)
    end

    player_1 = Rails.root.to_s + "/static-files/sources/#{params[:game]}/"
    player_2 = Rails.root.to_s + "/static-files/sources/#{params[:game]}/"

    player_1 += "users/" if not (params[:first].index('test/') or params[:first].index('combalg'))
    player_2 += "users/" if not (params[:second].index('test/') or params[:second].index('combalg'))

    player_1 += params[:first]
    player_2 += params[:second]

    params[:count][0].to_i.times do |n|
        config_number = rand(Rails.application.config.total_configs_number[params[:game]]) + 1

        RunGameJob.perform_later(params[:game], player_1, player_2, match_path, n * 2, false, config_number)
        RunGameJob.perform_later(params[:game], player_2, player_1, match_path, n * 2 + 1, false, config_number)
    end

    CollectMatchResultJob.perform_later(match_path)

    current_user.update_attribute(:sends_today, current_user.sends_today + 1)
    current_user.update_attribute(:sends_total, current_user.sends_total + 1)
    current_user.update_attribute(:last_send_timestamp, DateTime.now)

    if current_user.sends_today >= Rails.application.config.sends_per_day
        current_user.update_attribute(:next_send_timestamp, (DateTime.now + 1.days).midnight)
    else
        current_user.update_attribute(:next_send_timestamp, DateTime.now + Rails.application.config.send_delays[params[:count][0].to_i].seconds)
    end

    redirect_to show_game_path
  end

  def delete_file
    FileUtils.remove_file(params[:filename])
    redirect_to show_game_path
  end

  def delete_game
    if (params[:dir].to_s.index(current_user.email)) || (current_user.email == "admin")
        FileUtils.remove_dir(params[:dir])
    end
    redirect_to show_game_path
  end

  def troubleshooting
  end
end
