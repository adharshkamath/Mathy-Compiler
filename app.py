import os
import sys
import datetime
from flask import Flask, request
from flask_cors import CORS
from pygments import highlight
from pygments.lexers import CLexer, CppLexer
from pygments.formatters import HtmlFormatter

app = Flask("Compiler App Backend")
CORS(app)


@app.route("/build")
def build():
    if request.method != "POST":
        return '<h3> Hello! Nothing to see here :/<br><br> Might wanna check <a href="https://github.com/adharshkamath/Mathy-Compiler">this</a> out</h3>'
    try:
        os.system("cd system; make")
        return "Build Succeeded!"
    except:
        error = sys.exc_info()[0]
        print(error)
        return "Build failed due to " + str(error)


@app.route("/compile", methods=["POST"])
def compile():
    if request.method != "POST":
        return '<h3> Hello! Nothing to see here :/<br><br> Might wanna check <a href="https://github.com/adharshkamath/Mathy-Compiler">this</a> out</h3>'
    input_code = request.form.get("code")
    if (input_code is None) or (len(input_code) == 0):
        return "<h4> <b> Please provide some input :P </b> </h4>"
    with open("logs.txt", "a") as logs:
        log_str = "{0} ============= \n {1} \n========================================\n".format(
            datetime.datetime.now().strftime("%c"), input_code
        )
        logs.write(log_str)
    with open("input.txt", "w") as inputfile:
        inputfile.write(input_code)
    os.system("rm output.c")
    output = os.popen("./source/compile input.txt -o ./output").read()
    if len(output) > 0:
        return "<h4> <b> " + output + " </b> </h4>"
    with open("output.c", "r") as outputfile:
        outputstr = outputfile.read()
    lexer = CppLexer()
    formatter = HtmlFormatter(full=True, style="vs")
    result = highlight(outputstr, lexer, formatter)
    return result


@app.route("/", defaults={"u_path": ""})
@app.route("/<path:u_path>")
def catch_all(u_path):
    return '<h3> Hello! Nothing to see here :/<br><br> Might wanna check <a href="https://github.com/adharshkamath/Mathy-Compiler">this</a> out</h3>'


if __name__ == "__main__":
    app.run()
