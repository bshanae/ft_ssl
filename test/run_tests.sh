#!/bin/sh

PURPLE='\033[0;96m'
RESET='\033[0m'

echo "${PURPLE}BUILDING TESTS /--------------------------------------------------------------//${RESET}"
make

echo "${PURPLE}RUNNING ALGO TESTS /----------------------------------------------------------//${RESET}"
./build/ft_ssl_algo_tests

echo "${PURPLE}RUNNING CLI TESTS /-----------------------------------------------------------//${RESET}"
python3 test/cli/ft_ssl_cli_tests.py
