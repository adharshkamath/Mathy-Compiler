"""
    This script is used to generate outputs for all PolyBench/C programs.
    The inputs are taken from benchmark/source and the outputs will be 
    written to benchmark/build
"""

import os
import subprocess

os.chdir("./benchmark")
files = os.listdir("./source")
if not os.path.exists('./build'):
    os.makedirs('build')
for sourcefile in files:
    subprocess.call(["../source/test", "./source/" +
                     sourcefile, "-o", "./build/" + (sourcefile.split("."))[0]])
