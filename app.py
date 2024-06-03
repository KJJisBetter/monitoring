from asyncio import subprocess
from flask import Flask, jsonify, render_template, request # type: ignore

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')


if __name__ == '__main__':
    app.run(debug=True)