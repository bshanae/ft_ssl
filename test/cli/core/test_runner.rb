require 'open3'
require 'tmpdir'

class TestRunner
	RED = "\033[0;91m"
	GREEN = "\033[0;92m"
	RESET = "\u001b[0m"

	def run_test(test)
		workdir = Dir.mktmpdir
		cmd = generate_cmd(test)

		Dir.chdir(workdir) do
			unless test.preparation_script.nil?
				system(test.preparation_script)
			end

			stdout, _ = Open3.capture2(cmd)
			stdout = stdout[0..-2]

			if test.expected_stdout == stdout
				print_ok(test)
			else
				print_ko(test, cmd, stdout)
			end
		end

		FileUtils.rm_rf(workdir)
	end

	private def generate_cmd(test)
		cmd = ''
		cmd += "echo \"#{test.stdin}\" | " unless test.stdin.nil?
		cmd += File.expand_path('build/ft_ssl')
		cmd += " #{test.args}"
		cmd
	end

	private def print_ok(test)
		puts_green("[cli@#{test.id}] OK")
	end

	private def print_ko(test, cmd, stdout)
		puts_red("[cli@#{test.id}] KO")
		puts_red("## COMMAND ##")
		puts_red(cmd)
		puts_red("## ACTUAL OUTPUT ##")
		puts_red(stdout)
		puts_red("## EXPECTED OUTPUT ##")
		puts_red(test.expected_stdout)
	end

	private def puts_green(str)
		puts GREEN + str + RESET
	end

	private def puts_red(str)
		puts RED + str + RESET
	end
end