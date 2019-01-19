class TicketsController < ApplicationController
  def index
    @tickets = Ticket.all
  end

  def new
    ticket = Ticket.new(text: params[:text][0], status: 'new', author: params[:author])
    ticket.save
    redirect_to tickets_path
  end

  def delete
    ticket = Ticket.find(params[:id])
    if ["admin", "asandler", ticket.author].include? current_user.email
        ticket.destroy
        redirect_to tickets_path
    else
        redirect_to current_user
    end
  end

  def save
    if ["admin", "asandler"].include? current_user.email
        ticket = Ticket.find(params[:ticket_id])
        ticket.status = params[:status]
        ticket.save
    end
    redirect_to tickets_path
  end
end
