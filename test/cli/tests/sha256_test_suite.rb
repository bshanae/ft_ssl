class Sha256TestSuite
	def test1
		TestConfig.new('sha2565@1')
				  .prepare('echo "https://www.42.fr/" > website')
				  .pass_args('sha256 -q website')
				  .expect_stdout('1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf')
	end

	def test2
		TestConfig.new('sha2565@2')
				  .pass_args('sha256 -s "42 is nice"')
				  .expect_stdout('SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f')
	end
end