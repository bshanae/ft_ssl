class TestConfig
	attr_reader :id
	attr_reader :preparation_script
	attr_reader :stdin
	attr_reader :args
	attr_reader :expected_stdout

	def initialize(id)
		@id = id
		self
	end

	def prepare(script)
		@preparation_script = script
		self
	end

	def pass_stdin(stdin)
		@stdin = stdin
		self
	end

	def pass_args(args)
		@args = args
		self
	end

	def expect_stdout(stdout)
		@expected_stdout = stdout
		self
	end
end