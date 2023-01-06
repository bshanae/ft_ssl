class DesTestSuite
	def test_ecb_basic_1
		TestConfig.new('des@1')
				  .pass_stdin('aaaaaaa')
				  .pass_args('des-ecb -k 1 -a')
				  .expect_stdout('FDtiA16pLphU3mS1OQVhsw==')
	end

	def test_ecb_basic_2
		TestConfig.new('des@2')
				  .pass_stdin('aaaaaaa')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('5KnD4zBc5uGeMxRQ89l8TA==')
	end

	def test_ecb_basic_3
		TestConfig.new('des@3')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 1 -a')
				  .expect_stdout('XgF7ZG5wTc9U3mS1OQVhsw==')
	end

	def test_ecb_basic_4
		TestConfig.new('des@4')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 6162636461626364 -a')
				  .expect_stdout('YZF3QKaabXUwxNg3obhMfw==')
	end

	def test_ecb_basic_5
		expected_stdout = <<~EOS.chomp
			8SwcnJ+SGX7aomLcBGT2MXRm8pNcB58zGLABl9GE8GZ+WCIYEEc7O0LqdLNbs2Bz
			CNXFfbsyNlSeMxRQ89l8TA==
		EOS

		TestConfig.new('des@5')
				  .pass_stdin('Lorem ipsum dolor sit amet, consectetur adipiscing elit')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout(expected_stdout)
	end

	def test_ecb_padding_1
		TestConfig.new('des@6')
				  .pass_stdin('abc')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('lSYfEDxC2q0=')
	end

	def test_ecb_padding_2
		TestConfig.new('des@7')
				  .pass_stdin('abc abc abc abc abc')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('aXmOuwxQ8BBpeY67DFDwEJUmHxA8Qtqt')
	end

	def test_ecb_input_file_1
		TestConfig.new('des@8')
				  .prepare('echo "hello world" > in.txt')
				  .pass_args('des-ecb -k 123 -a -i in.txt')
				  .expect_stdout('KAvMSx7m5erShPb8cd9h6w==')
	end

	def test_ecb_output_file_1
		TestConfig.new('des@9')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 123 -a -o out.txt')
				  .expect_file('out.txt', 'LwQAsdaAVYKeMxRQ89l8TA==')
	end
end