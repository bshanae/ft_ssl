class Base64TestSuite
	@@lorem_ipsum_20 = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. In purus magna, elementum non accumsan at, lacinia nec leo. Donec pellentesque.'
	@@lorem_ipsum_20_b64 = <<~EOS.chomp
		TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2Np
		bmcgZWxpdC4gSW4gcHVydXMgbWFnbmEsIGVsZW1lbnR1bSBub24gYWNjdW1zYW4g
		YXQsIGxhY2luaWEgbmVjIGxlby4gRG9uZWMgcGVsbGVudGVzcXVlLgo=
	EOS

	def test_1
		TestConfig.new('base64@1')
				  .pass_stdin('123')
				  .pass_args('base64 -e')
				  .expect_stdout('MTIzCg==')
	end

	def test_2
		TestConfig.new('base64@2')
				  .pass_stdin('MTIzCg==')
				  .pass_args('base64 -d')
				  .expect_stdout("123\n")
	end

	def test_3
		TestConfig.new('base64@3')
				  .pass_stdin(@@lorem_ipsum_20)
				  .pass_args('base64 -e')
				  .expect_stdout(@@lorem_ipsum_20_b64)
	end

	def test_4
		TestConfig.new('base64@4')
				  .pass_stdin(@@lorem_ipsum_20_b64)
				  .pass_args('base64 -d')
				  .expect_stdout(@@lorem_ipsum_20 + "\n")
	end

	def test_5
		TestConfig.new('base64@5')
				  .prepare('echo 123 > in.txt')
				  .pass_args('base64 -e -i in.txt -o out.txt')
				  .expect_file('out.txt', 'MTIzCg==')
	end

	def test_6
		TestConfig.new('base64@6')
				  .prepare('echo MTIzCg== > in.txt')
				  .pass_args('base64 -d -i in.txt -o out.txt')
				  .expect_file('out.txt', "123\n")
	end

	def test_7
		TestConfig.new('base64@7')
				  .pass_stdin("repeat after me ' encoding is not encryption'")
				  .pass_args('base64 -e')
				  .expect_stdout('cmVwZWF0IGFmdGVyIG1lICcgZW5jb2RpbmcgaXMgbm90IGVuY3J5cHRpb24nCg==')
	end
end