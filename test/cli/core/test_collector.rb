require_relative 'test_config'

class TestCollector
	def find_all_tests_in_all_suites
		all_tests = Array.new

		ObjectSpace.each_object(Class).each do |_class|
			class_name = _class.name
			if not class_name.nil? and _class.name.include?('TestSuite')
				find_all_tests_in_suite(all_tests, Object::const_get(class_name).new)
			end
		end

		all_tests
	end

	private def find_all_tests_in_suite(all_tests, test_suite)
		test_suite.methods.each do |method_name|
			method_name = method_name.to_s
			if method_name.include?('test')
				all_tests << test_suite.send(method_name)
			end
		end
	end
end