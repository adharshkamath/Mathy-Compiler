import os
import subprocess

os.chdir("./benchmark")
files = os.listdir("./source")
if not os.path.exists('./build'):
    os.makedirs('build')
for sourcefile in files:
    subprocess.call(["../source/test", "./source/" +
                     sourcefile, "-o", "./build/" + (sourcefile.split("."))[0]])
