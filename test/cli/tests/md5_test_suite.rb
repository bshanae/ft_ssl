class Md5TestSuite
	def test1
		TestConfig.new('md5@1')
				  .pass_stdin('42 is nice')
				  .pass_args('md5')
				  .expect_stdout('(stdin)= 35f1d6de0302e2086a4e472266efb3a9')
	end

	def test2
		TestConfig.new('md5@2')
				  .pass_stdin('42 is nice')
				  .pass_args('md5 -p')
				  .expect_stdout('("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9')
	end

	def test3
		TestConfig.new('md5@3')
				  .pass_stdin('Pity the living.')
				  .pass_args('md5 -q -r')
				  .expect_stdout('e20c3b973f63482a778f3fd1869b7f25')
	end

	def test4
		TestConfig.new('md5@4')
				  .prepare('echo "And above all," > file')
				  .pass_args('md5 file')
				  .expect_stdout('MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a')
	end

	def test5
		TestConfig.new('md5@5')
				  .prepare('echo "And above all," > file')
				  .pass_args('md5 -r file')
				  .expect_stdout('53d53ea94217b259c11a5a2d104ec58a file')
	end

	def test6
		TestConfig.new('md5@6')
				  .pass_args('md5 -s "pity those that aren\'t following baerista on spotify."')
				  .expect_stdout('MD5 ("pity those that aren\'t following baerista on spotify.") = a3c990a1964705d9bf0e602f44572f5f')
	end

	def test7
		expected_stdout = <<~EOS.chomp
		    ("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c
		    MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a
		EOS

		TestConfig.new('md5@7')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('be sure to handle edge cases carefully')
				  .pass_args('md5 -p file')
				  .expect_stdout(expected_stdout)
	end

	def test8
		TestConfig.new('md5@8')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('some of this will not make sense at first')
				  .pass_args('md5 file')
				  .expect_stdout('MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a')
	end

	def test9
		expected_stdout = <<~EOS.chomp
		    ("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281
		    53d53ea94217b259c11a5a2d104ec58a file
		EOS

		TestConfig.new('md5@9')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('but eventually you will understand')
				  .pass_args('md5 -p -r file')
				  .expect_stdout(expected_stdout)
	end

	def test10
		expected_stdout = <<~EOS.chomp
            ("GL HF let's go")= d1e3cc342b6da09480b27ec57ff243e2
            MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8
            MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a
		EOS

		TestConfig.new('md5@10')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('GL HF let\'s go')
				  .pass_args('md5 -p -s "foo" file')
				  .expect_stdout(expected_stdout)
	end

	def test11
		expected_stdout = <<~EOS.chomp
            ("one more thing")= a0bd1876c6f011dd50fae52827f445f5
            acbd18db4cc2f85cedef654fccc4a4d8 "foo"
            53d53ea94217b259c11a5a2d104ec58a file
		EOS

		TestConfig.new('md5@11')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('one more thing')
				  .pass_args('md5 -r -p -s "foo" file -s "bar"')
				  .expect_stdout(expected_stdout)
	end

	def test12
		expected_stdout = <<~EOS.chomp
            just to be extra clear
            3ba35f1ea0d170cb3b9a752e3360286c
            acbd18db4cc2f85cedef654fccc4a4d8
            53d53ea94217b259c11a5a2d104ec58a
		EOS

		TestConfig.new('md5@12')
				  .prepare('echo "And above all," > file')
				  .pass_stdin('just to be extra clear')
				  .pass_args('md5 -r -q -p -s "foo" file')
				  .expect_stdout(expected_stdout)
	end
end