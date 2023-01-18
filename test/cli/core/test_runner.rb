require 'open3'
require 'tmpdir'

class TestRunner
	RED = "\033[0;91m"
	GREEN = "\033[0;92m"
	RESET = "\u001b[0m"

	def run_test(test)
		main_dir = Dir.getwd

		workdir = Dir.mktmpdir
		cmd = generate_cmd(test)

		Dir.chdir(workdir) do
			unless test.copy_res_name.nil?
				FileUtils.cp(File.join(main_dir, 'test/cli/res', test.copy_res_name), test.copy_res_name)
			end

			unless test.preparation_script.nil?
				system(test.preparation_script)
			end

			stdout, _ = Open3.capture2(cmd)
			stdout = stdout[0..-2]

			if not test.expected_stdout.nil?
				success = test.expected_stdout == stdout
				result = stdout
			elsif not test.expected_file_name.nil?
				begin
					file_content = IO.read(test.expected_file_name)
				rescue
					file_content = '(error)'
				end
				success = test.expected_file_content == file_content
				result = file_content
			else
				puts_red('Test does not declare any expectations!')
				success = true
				result = '(error)'
			end

			if success
				print_ok(test)
			else
				print_ko(test, cmd, result)
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

	private def print_ko(test, cmd, result)
		puts_red("[cli@#{test.id}] KO")
		puts_red("// ================================================================================================================== //")
		puts_red("-- COMMAND ---------------------------------------------------------------------------------------------------------- //")
		puts_red(cmd)
		if test.expected_file_name.nil?
			puts_red("-- ACTUAL OUTPUT ---------------------------------------------------------------------------------------------------- //")
		else
			puts_red("-- ACTUAL FILE CONTENT ---------------------------------------------------------------------------------------------- //")
		end
		puts_red(result)
		if test.expected_file_name.nil?
			puts_red("-- EXPECTED OUTPUT -------------------------------------------------------------------------------------------------- //")
			puts_red(test.expected_stdout)
		else
			puts_red("-- EXPECTED FILE CONTENT -------------------------------------------------------------------------------------------- //")
			puts_red(test.expected_file_content)
		end
		puts_red("// ================================================================================================================== //")
	end

	private def puts_green(str)
		puts GREEN + str + RESET
	end

	private def puts_red(str)
		puts RED + str + RESET
	end
end