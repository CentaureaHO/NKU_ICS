import subprocess
import time
import glob
import os
import signal

unpassed = []

def run_test(test_name):
    process = subprocess.Popen(
        ["make", "ARCH=x86-nemu", "ALL={}".format(test_name), "run"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True
    )
    
    try:
        process.stdin.write("c\n")
        process.stdin.flush()
        
        process.stdin.write("q\n")
        process.stdin.flush()
        
        stdout, stderr = process.communicate(timeout=5)
        return stdout, stderr, False
        
    except subprocess.TimeoutExpired:
        process.kill()
        stdout, stderr = process.communicate()
        return stdout, stderr, True


def check_result(test_name, stdout, timeout):
    if timeout:
        print("\t{} timeout (>5s)".format(test_name))
        unpassed.append(test_name)
    elif "nemu: HIT GOOD TRAP" in stdout:
        print("\t{} passed".format(test_name))
    elif "nemu: HIT BAD TRAP" in stdout:
        print("\t{} failed".format(test_name))
        unpassed.append(test_name)
    else:
        print("\t{} incomplete".format(test_name))
        unpassed.append(test_name)


def main():
    c_files = glob.glob("./tests/*.c")
    
    for c_file in c_files:
        test_name = os.path.splitext(os.path.basename(c_file))[0]
        print("Running test: {}".format(test_name))
        stdout, stderr, timeout = run_test(test_name)
        check_result(test_name, stdout, timeout)

    if len(unpassed) > 0:
        print("\nUnpassed tests:")
        for test_name in unpassed:
            print('\t', test_name)
    else:
        print("\nAll tests passed!")


if __name__ == "__main__":
    main()