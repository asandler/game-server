# Be sure to restart server after modifying this file.

Rails.application.config.allow_external_registration = true

# ==== Send delays ====

Rails.application.config.send_delays = Hash.new(60 * 10) # default delay

# delays for different matches counts
Rails.application.config.send_delays[1] = 60 * 5 # seconds
Rails.application.config.send_delays[2] = 60 * 6 # seconds
Rails.application.config.send_delays[3] = 60 * 7 # seconds
Rails.application.config.send_delays[4] = 60 * 8 # seconds
Rails.application.config.send_delays[5] = 60 * 9 # seconds

Rails.application.config.sends_per_day = 50

# ==== List of enabled games ====

Rails.application.config.enabled_games = {}

Rails.application.config.enabled_games["tictactoe"] = true
Rails.application.config.enabled_games["virus"] = true
Rails.application.config.enabled_games["pight"] = true
Rails.application.config.enabled_games["reversi"] = true
Rails.application.config.enabled_games["seabattle"] = true

# ==== Time limits for games ====

Rails.application.config.time_limits = {}

Rails.application.config.time_limits["tictactoe.sec"] = 1
Rails.application.config.time_limits["tictactoe.usec"] = 0

Rails.application.config.time_limits["virus.sec"] = 5
Rails.application.config.time_limits["virus.usec"] = 0

Rails.application.config.time_limits["pight.sec"] = 2
Rails.application.config.time_limits["pight.usec"] = 0

Rails.application.config.time_limits["reversi.sec"] = 2
Rails.application.config.time_limits["reversi.usec"] = 0

Rails.application.config.time_limits["seabattle.sec"] = 2
Rails.application.config.time_limits["seabattle.usec"] = 0

# ==== Total number of game configs for games ====

Rails.application.config.total_configs_number = {}

Rails.application.config.total_configs_number["tictactoe"] = 1
Rails.application.config.total_configs_number["virus"] = 1
Rails.application.config.total_configs_number["pight"] = 2
Rails.application.config.total_configs_number["reversi"] = 1
Rails.application.config.total_configs_number["seabattle"] = 4
