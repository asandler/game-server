require 'sinatra'
require 'fileutils'

post '/run' do
    if not params['game']
        return "Name of the game is required (\"game\" field in request)\n"
    end
    if not params['player1'] or not params['player2']
        return "At least two players are required (\"player1\", \"player2\" fields in request)\n"
    end

    incoming_source_files = []

    counter = 1
    while params["player#{counter}"] and counter < 3
        incoming_source_files << params["player#{counter}"][:tempfile].path
        counter += 1
    end

    time = Time.now
    tempdir = '../runs/' + time.to_s.split[0..1].join('/') + '_' + time.tv_nsec.to_s[0..3]
    FileUtils.mkdir_p(tempdir)

    source_files = []

    incoming_source_files.each_with_index do |source, index|
        file_ext = source[source.rindex('.')..-1]
        source_path = tempdir + "/player#{index + 1}" + file_ext
        FileUtils.cp(source, source_path)
        source_files << source_path
    end

    err_str = ""

    source_files.each_with_index do |source, index|
        prefix = tempdir + "/player#{index + 1}"
        file_ext = source[source.rindex('.')..-1]

        compiled_n_ready = true
        good_interpreter = true

        if [".cpp", ".c", ".pas"].include? file_ext
            if file_ext == ".cpp"
                compiled_n_ready &= system("LC_ALL=C g++ -std=c++0x -O2 " + source + " -o #{prefix} 2> #{prefix}.compilation_errors")
            elsif file_ext == ".c"
                compiled_n_ready &= system("LC_ALL=C gcc " + source + " -o #{prefix} 2> #{prefix}.compilation_errors")
            elsif file_ext == ".pas"
                compiled_n_ready &= system("LC_ALL=C fpc -Tlinux " + source + " -o#{prefix} 2> #{prefix}.compilation_errors")
            else
                err_str += "ERROR (player#{index + 1}): unsupported language\nCheck the list of available languages on the site.\n"
            end

            if not compiled_n_ready
                err_str += "ERROR (player#{index + 1}): Compilation errors\n" + File.read("#{prefix}.compilation_errors") + "\n"
            end
        else
            compiled_n_ready &= system("mv " + source + " #{prefix} && chmod +x #{prefix}")

            if compiled_n_ready
                first_line = File.open("#{prefix}", "r").readline.chomp
                good_interpreter &= (["#!/usr/bin/env python", "#!/usr/bin/env python3", "#!/usr/bin/env ruby"].include? first_line)

                if not good_interpreter
                    err_str += "ERROR (player#{index + 1}): bad interpreter\n"
                    err_str += "Check the first line of your program. Available interpreters are:\n"
                    err_str += "#!/usr/bin/env python\n#!/usr/bin/env python3\n#!/usr/bin/env ruby\n"
                end
            else
                err_str += "ERROR (player#{index + 1}): internal system error\nPlease, report it to the system administrator (Andrew Sandler)\n"
            end
        end
    end

    return err_str if not err_str.empty?

    game = params['game']
    sec = params['sec'] || '1'
    usec = params['usec'] || '0'

    tournament_game = ((params['tournament_game'].to_i == 1) ? true : false)
    config_number = params['config_number'] || '1'

    name1 = params['name1'] || params['player1'][:filename]
    name2 = params['name2'] || params['player2'][:filename]

    system("cd ../sandbox && ./run_game.sh #{game} #{tempdir}/player1 #{tempdir}/player2 #{sec} #{usec} #{tempdir} #{name1} #{name2} #{config_number} > #{tempdir}/run_log 2> #{tempdir}/run_log_err")

    if (%x[wc -l #{tempdir}/run_log_err]).to_s.split[0] == "0"
        if not tournament_game
            return File.open("#{tempdir}/play.output", "r").read + "\nSTDERR:\n" + File.open("#{tempdir}/play.err", "r").read + "\n"
        else
            return File.open("#{tempdir}/play.output", "r").read + "\n"
        end
    else
        return "ERROR: internal system error\nPlease, report it to the system administrator (Andrew Sandler)\n"
    end
end
