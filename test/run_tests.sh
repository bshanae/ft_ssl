#!/bin/sh

PURPLE='\033[0;96m'
RESET='\033[0m'

make 1> /dev/null

echo "${PURPLE}RUNNING ALGO TESTS /----------------------------------------------------------//${RESET}"
./build/ft_ssl_algo_tests

echo "${PURPLE}RUNNING CLI TESTS /-----------------------------------------------------------//${RESET}"
ruby test/cli/ft_ssl_cli_tests.rb
