Rails.application.routes.draw do
  resources :user_sessions
  resources :users

  get 'login' => 'user_sessions#new', :as => :login
  post 'logout' => 'user_sessions#destroy', :as => :logout

  post 'games/new_file' => 'games#new_file'
  post 'games/delete_file' => 'games#delete_file'
  post 'games/download' => 'games#download_file'
  post 'games/download_html' => 'games#download_html'
  post 'games/download_pdf' => 'games#download_pdf'

  post 'games/new_match' => 'games#new_match'
  post 'games/delete_game' => 'games#delete_game'

  get 'show_game' => 'users#show', :as => :show_game
  get '/' => 'users#show'

  get 'delay/:id/:seconds' => 'users#delay', :as => :delay
  get 'undelay/:id' => 'users#undelay', :as => :undelay

  post 'round_tournament' => 'tournaments#round_tournament'

  get 'pight' => 'users#show'
  get 'reversi' => 'users#show'
  get 'tictactoe' => 'users#show'
  get 'virus' => 'users#show'
  get 'seabattle' => 'users#show'

  get 'runners' => 'runners#index', :as => 'runners'
  get 'runners/unload' => 'runners#unload'
  post 'runners/new' => 'runners#new'
  post 'runners/delete' => 'runners#delete'

  get 'tickets' => 'tickets#index', :as => 'tickets'
  post 'tickets/new' => 'tickets#new'
  post 'tickets/delete' => 'tickets#delete'
  post 'tickets/save' => 'tickets#save'

  get 'games/troubleshooting.html' => 'games#troubleshooting', :as => 'troubleshooting'

  root 'user_sessions#new'
end
