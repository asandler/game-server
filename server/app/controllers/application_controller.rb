class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  before_action :require_login
  protect_from_forgery with: :exception
  after_action :set_csrf_cookie_for_ng

  def set_csrf_cookie_for_ng
    cookies['XSRF-TOKEN'] = form_authenticity_token if protect_against_forgery?
  end

protected
    # In Rails 4.2 and above
    def verified_request?
        super || valid_authenticity_token?(session, request.headers['X-XSRF-TOKEN'])
    end

    # In Rails 4.1 and below
    def verified_request?
        super || form_authenticity_token == request.headers['X-XSRF-TOKEN']
    end

private
    def not_authenticated
        redirect_to login_path
    end
end
