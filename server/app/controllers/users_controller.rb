class UsersController < ApplicationController
    skip_before_action :require_login, only: [:new, :create]
  
    def index
        if current_user.email == "admin"
            @users = User.all
            @users.each do |user|
                if Time.now.to_i >= user.next_send_timestamp.to_time.to_i
                    if user.last_send_timestamp.day != DateTime.now.day
                        user.update_attribute(:sends_today, 0)
                    end
                end
            end
        else
            redirect_to current_user
        end
    end
  
    def new
        if current_user
            if current_user.email == "admin" || Rails.application.config.allow_external_registration
                @user = User.new
            else
                redirect_to current_user
            end
        else
            if Rails.application.config.allow_external_registration
                @user = User.new
            else
                redirect_to login_path
            end
        end
    end

    def show
        @games_list = Dir.entries(Rails.root.to_s + "/static-files/sources").keep_if{|s| Rails.application.config.enabled_games[s]}.sort
        index = @games_list.index(request.original_url.split('?')[0].split('/')[-1])

        if current_user.email == "admin" and params[:id] and params[:id] != current_user.id
            view_as_admin = true
            @user = User.find(params[:id])
            index = @user.tab_number
        else
            @user = current_user
        end

        if index or view_as_admin
            if current_user.tab_number != index
                current_user.update_attribute(:tab_number, index)
            end

            @current_game = @games_list[index]
            @source_root = "static-files/sources/#{@current_game}/users/#{@user.email}/"
            @match_root = "static-files/games/#{@current_game}/#{@user.email}/"

            FileUtils.mkdir_p(@source_root)
            FileUtils.mkdir_p(@match_root)

            @files_array = Dir.entries(@source_root).keep_if{|s| s[0] != "."}.sort.map{|s| @user.email + "/" + s}

            @test_array = Dir.entries("static-files/sources/" + @current_game + "/test/").keep_if{|s| s[0] != "."}.sort.map{|s| "test/" + s}
            @test_array << ['---------'] if @files_array.size > 0
            @test_array += @files_array

            (2012..2018).each do |year|
                if Dir.exists?("static-files/sources/" + @current_game + "/combalg/#{year}/")
                    combalg_array = Dir.entries("static-files/sources/" + @current_game + "/combalg/#{year}/").keep_if{|s| s[0] != "."}.sort.map{|s| "combalg/#{year}/" + s}
                    @test_array << ['---------'] if combalg_array.size > 0
                    @test_array += combalg_array
                end
            end

            if params[:show_tournaments]
                @show_tournaments = true
                @tournaments_root = Rails.root.to_s + "/static-files/tournaments/#{@current_game}/"
            end

            @send_delay_info = 0
            if Time.now.to_i >= current_user.next_send_timestamp.to_time.to_i
                if current_user.last_send_timestamp.day != DateTime.now.day
                    current_user.update_attribute(:sends_today, 0)
                end
            else
                if current_user.sends_today >= Rails.application.config.sends_per_day
                    @send_delay_info = 1
                else
                    @send_delay_info = 2
                end
            end

        else
            if params[:show_tournaments]
                redirect_to '/' + @games_list[@user.tab_number] + '?show_tournaments=1'
            else
                redirect_to '/' + @games_list[@user.tab_number]
            end
        end
    end
  
    def edit
        @user = User.find(params[:id])
        if current_user.email != @user.email and current_user.email != "admin"
            redirect_to current_user
        end
    end
  
    def create
        if (not Rails.application.config.allow_external_registration) and current_user and current_user.email != 'admin'
            redirect_to current_user
        end

        @user = User.new(user_params)
  
        if @user.save
            @user.update_attribute(:tab_number, 0)
            @user.update_attribute(:last_send_timestamp, DateTime.now)
            @user.update_attribute(:next_send_timestamp, DateTime.now)
            @user.update_attribute(:sends_today, 0)
            @user.update_attribute(:sends_total, 0)
            @user.save

            if current_user
                redirect_to users_path
            else
                if @user = auto_login(@user)
                    redirect_to show_game_path
                else
                    render :new, notice: 'Login failed'
                end
            end
        else
            render :new
        end
    end
  
    def update
        @user = User.find(params[:id])

        if @user.update(user_params)
            redirect_to edit_user_path, notice: 'User was successfully updated'
        else
            render :edit
        end
    end

    def undelay
        if current_user.email != 'admin'
            redirect_to current_user
        end

        user = User.find(params[:id])
        user.update_attribute(:next_send_timestamp, DateTime.now)
        redirect_to users_path
    end

    def delay
        if current_user.email != 'admin'
            redirect_to current_user
        end

        user = User.find(params[:id])
        user.update_attribute(:next_send_timestamp, [DateTime.now, user.next_send_timestamp].max + params[:seconds].to_i.seconds)
        redirect_to users_path
    end
  
    def destroy
        if current_user.email != 'admin'
            redirect_to current_user
        end

        @user = User.find(params[:id])
        @user.destroy
        redirect_to users_path
    end
  
    private
    # Never trust parameters from the scary internet, only allow the white list through.
    def user_params
        params.require(:user).permit(:username, :email, :password, :password_confirmation)
    end
end
