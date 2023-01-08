class DesTestSuite
	def test_enc_ecb_basic_1
		TestConfig.new('des@1')
				  .pass_stdin('aaaaaaa')
				  .pass_args('des-ecb -k 1 -a')
				  .expect_stdout('FDtiA16pLphU3mS1OQVhsw==')
	end

	def test_enc_ecb_basic_2
		TestConfig.new('des@2')
				  .pass_stdin('aaaaaaa')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('5KnD4zBc5uGeMxRQ89l8TA==')
	end

	def test_enc_ecb_basic_3
		TestConfig.new('des@3')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 1 -a')
				  .expect_stdout('XgF7ZG5wTc9U3mS1OQVhsw==')
	end

	def test_enc_ecb_basic_4
		TestConfig.new('des@4')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 6162636461626364 -a')
				  .expect_stdout('YZF3QKaabXUwxNg3obhMfw==')
	end

	def test_enc_ecb_basic_5
		expected_stdout = <<~EOS.chomp
			8SwcnJ+SGX7aomLcBGT2MXRm8pNcB58zGLABl9GE8GZ+WCIYEEc7O0LqdLNbs2Bz
			CNXFfbsyNlSeMxRQ89l8TA==
		EOS

		TestConfig.new('des@5')
				  .pass_stdin('Lorem ipsum dolor sit amet, consectetur adipiscing elit')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout(expected_stdout)
	end

	def test_enc_ecb_padding_1
		TestConfig.new('des@6')
				  .pass_stdin('abc')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('lSYfEDxC2q0=')
	end

	def test_enc_ecb_padding_2
		TestConfig.new('des@7')
				  .pass_stdin('abc abc abc abc abc')
				  .pass_args('des-ecb -k 123 -a')
				  .expect_stdout('aXmOuwxQ8BBpeY67DFDwEJUmHxA8Qtqt')
	end

	def test_enc_ecb_input_file_1
		TestConfig.new('des@8')
				  .prepare('echo "hello world" > in.txt')
				  .pass_args('des-ecb -k 123 -a -i in.txt')
				  .expect_stdout('KAvMSx7m5erShPb8cd9h6w==')
	end

	def test_enc_ecb_output_file_1
		TestConfig.new('des@9')
				  .pass_stdin('foo bar')
				  .pass_args('des-ecb -k 123 -a -o out.txt')
				  .expect_file('out.txt', 'LwQAsdaAVYKeMxRQ89l8TA==')
	end

	def test_enc_cbc_basic_1
		TestConfig.new('des@100')
				  .pass_stdin('aaaaaaa')
				  .pass_args('des-cbc -k 1 -v 1 -a')
				  .expect_stdout('XpYZc1ckPE+2U7Gan+424g==')
	end

	def test_enc_cbc_basic_2
		TestConfig.new('des@101')
				  .pass_stdin('foo bar')
				  .pass_args('des-cbc -k 123 -v 123 -a')
				  .expect_stdout('nI282mp+aQEhb41vHJeKHw==')
	end

	def test_enc_cbc_basic_3
		expected_stdout = <<~EOS.chomp
			4RrD7kayc24oYjZlKWufTSzhTV25O5xXViGFi61pi4KEafPDTqO7kICj42meRWqK
			O8SRV9a/jYKIYjy/kFx4Xg==
		EOS

		TestConfig.new('des@102')
				  .pass_stdin('Lorem ipsum dolor sit amet, consectetur adipiscing elit')
				  .pass_args('des-cbc -k 123 -v 123 -a')
				  .expect_stdout(expected_stdout)
	end

	def test_enc_cbc_reference
		TestConfig.new('des@103')
				  .pass_stdin('one deep secret')
				  .pass_args('des-cbc -a -k 6162636461626364 -v 0011223344556677')
				  .expect_stdout('zqYWONX68rWNxl7msIdGC67Uh2HfVEBo')
	end

	def test_dec_ecb_1
		TestConfig.new('des@1001')
				  .pass_stdin('FDtiA16pLphU3mS1OQVhsw==')
				  .pass_args('des-ecb -k 1 -a -d')
				  .expect_stdout('aaaaaaa')
	end

	def test_dec_ecb_2
		TestConfig.new('des@1002')
				  .pass_stdin('5KnD4zBc5uGeMxRQ89l8TA==')
				  .pass_args('des-ecb -k 123 -a -d')
				  .expect_stdout('aaaaaaa')
	end

	def test_dec_ecb_3
		TestConfig.new('des@1003')
				  .pass_stdin('XgF7ZG5wTc9U3mS1OQVhsw==')
				  .pass_args('des-ecb -k 1 -a -d')
				  .expect_stdout('foo bar')
	end

	def test_dec_ecb_4
		plaintext = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit'

		ciphertext = <<~EOS.chomp
			8SwcnJ+SGX7aomLcBGT2MXRm8pNcB58zGLABl9GE8GZ+WCIYEEc7O0LqdLNbs2Bz
			CNXFfbsyNlSeMxRQ89l8TA==
		EOS

		TestConfig.new('des@1004')
				  .prepare("echo '#{ciphertext}' > in.txt")
				  .pass_args('des-ecb -k 123 -a -d -i in.txt')
				  .expect_stdout(plaintext)
	end

	def test_dec_cbc_1
		TestConfig.new('des@1101')
				  .pass_stdin('iiqJOx6VTrU=')
				  .pass_args('des-cbc -k 123 -v 123 -a -d')
				  .expect_stdout('a')
	end

	def test_dec_cbc_2
		TestConfig.new('des@1102')
				  .pass_stdin('XpYZc1ckPE+2U7Gan+424g==')
				  .pass_args('des-cbc -k 1 -v 1 -a -d')
				  .expect_stdout('aaaaaaa')
	end

	def test_dec_cbc_3
		plaintext = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit'

		ciphertext = <<~EOS.chomp
			4RrD7kayc24oYjZlKWufTSzhTV25O5xXViGFi61pi4KEafPDTqO7kICj42meRWqK
			O8SRV9a/jYKIYjy/kFx4Xg==
		EOS

		TestConfig.new('des@1103')
				  .pass_stdin(ciphertext)
				  .pass_args('des-cbc -k 123 -v 123 -a -d')
				  .expect_stdout(plaintext)
	end

	def test_dec_cbc_reference
		TestConfig.new('des@1104')
				  .pass_stdin('zqYWONX68rWNxl7msIdGC67Uh2HfVEBo')
				  .pass_args('des-cbc -a -k 6162636461626364 -v 0011223344556677 -d')
				  .expect_stdout('one deep secret')
	end
end