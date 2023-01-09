require_relative 'core/test_collector'
require_relative 'core/test_runner'
require_relative 'tests/md5_test_suite'
require_relative 'tests/sha256_test_suite'
require_relative 'tests/base64_test_suite'
require_relative 'tests/des_test_suite'

test_collector = TestCollector.new
test_runner = TestRunner.new

test_collector.find_all_tests_in_all_suites.each { |config| test_runner.run_test config }