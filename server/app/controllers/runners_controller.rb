class RunnersController < ApplicationController
  def index
    if current_user.email == "admin"
        @runners = Runner.all
    else
        redirect_to current_user
    end
  end

  def new
    if current_user.email == "admin"
        runner = Runner.new(host: params['host'][0], status: params['status'])
        runner.save
        redirect_to runners_path
    else
        redirect_to current_user
    end
  end

  def delete
    if current_user.email == "admin"
        runner = Runner.find(params[:id])
        runner.destroy
        redirect_to runners_path
    else
        redirect_to current_user
    end
  end

  def unload
    if current_user.email == "admin"
        runner = Runner.find(params[:host_id])
        runner.status = 'free'
        runner.save
        redirect_to runners_path
    else
        redirect_to current_user
    end
  end
end
