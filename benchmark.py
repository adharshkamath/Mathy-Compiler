import os
import subprocess

os.chdir("./benchmark")
files = os.listdir("./source")
for sourcefile in files:
    subprocess.call(["../source/test", "./source/"+
                    sourcefile, "-o" ,"./output/"+(sourcefile.split("."))[0]])
