# test_script.py
import os
import signal
import subprocess
import time

if __name__ == "__main__":
    # Start the process
    # fix the path to app
    print(os.getcwd())
    process = subprocess.Popen(['../build/app/crc32daemon', '-directory', './', '-timeout', '10000'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time.sleep(3)
    # Get the PID of the forked process
    child_pid = None
    for pid in set(map(int, os.popen('pgrep -x {}'.format('crc32daemon')).read().split("\n")[:-1])):
        child_pid = pid
        break
    if child_pid:
        print("Child PID:", child_pid)
        # Send SIGTERM signal to the forked process
        os.kill(child_pid, signal.SIGTERM)
        print("Sent SIGTERM signal to the child process")
    else:
        print("No child process found")
    time.sleep(3)
    file_name = 'result.json'

    check_file = os.path.isfile(file_name)

    print('file {} exists: {}'.format(file_name, check_file))

