import os
from flask import Flask
app = Flask(__name__)


@app.route('/')
def hello():
    os.system("cd source; make")    
    return "Hello World!"

if __name__ == '__main__':
    app.run()
