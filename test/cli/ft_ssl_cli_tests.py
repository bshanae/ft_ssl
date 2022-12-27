import os
import re
import shutil
import subprocess
import tempfile
from enum import Enum
from typing import Optional, List


class Test:
    id: str
    setup: str
    args: str
    stdin: str
    stdout: str


class TermColor:
    RED = '\033[0;91m'
    GREEN = '\033[0;92m'
    RESET = '\u001b[0m'


def extract_id_from_test_name(name: str) -> int:
    return int(re.findall(r'test_(\d*)', name)[0])


def parse_test(path: str) -> Optional[Test]:
    class ReadMode(Enum):
        SETUP = 1
        ARGS = 2
        INPUT = 3
        OUTPUT = 4
        ERROR = 5

    test = Test()
    test.id = 'cli@' + str(extract_id_from_test_name(path))
    test.args = ''
    test.stdin = ''

    setup_lines = []
    stdout_lines = []

    read_mode = 0
    with open(path) as lines:
        for line in lines:
            line = line.rstrip()
            if line[0] == '#':
                if line == '#SETUP':
                    read_mode = ReadMode.SETUP
                elif line == '#ARGS':
                    read_mode = ReadMode.ARGS
                elif line == '#STDIN':
                    read_mode = ReadMode.INPUT
                elif line == '#STDOUT':
                    read_mode = ReadMode.OUTPUT
            else:
                if read_mode == ReadMode.SETUP:
                    setup_lines.append(line)
                elif read_mode == ReadMode.ARGS:
                    test.args = line
                elif read_mode == ReadMode.INPUT:
                    test.stdin = line
                elif read_mode == ReadMode.OUTPUT:
                    stdout_lines.append(line)

    test.setup = '\n'.join(setup_lines)
    test.stdout = '\n'.join(stdout_lines)

    return test


def run_test(test: Test):
    cmd = ''
    if test.stdin != '':
        cmd += f'echo "{test.stdin}" | '
    cmd += os.path.abspath('build/ft_ssl')
    if test.args != '':
        cmd += ' ' + test.args

    workdir = tempfile.mkdtemp()

    if test.setup != '':
        subprocess.Popen(test.setup, shell=True, cwd=workdir)

    process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=workdir)
    stdout = process.stdout.read().decode()[:-1]

    log_prefix = f'[{test.id}] '
    if stdout == test.stdout:
        print(TermColor.GREEN, end='')
        print(log_prefix + 'OK')
    else:
        print(TermColor.RED, end='')
        print(log_prefix + 'EXPECTED STDOUT:')
        print(test.stdout)
        print(log_prefix + 'ACTUAL STDOUT:')
        print(stdout)

    print(TermColor.RESET, end='')

    shutil.rmtree(workdir)


config_dir = 'test/cli/configs'
for file in sorted(os.listdir(config_dir), key=extract_id_from_test_name):
    filename = os.fsdecode(file)

    test = parse_test(os.path.join(config_dir, filename))
    run_test(test)
