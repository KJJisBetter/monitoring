from asyncio import subprocess
from flask import Flask, jsonify, render_template, request # type: ignore
from routes.cpu import cpu
from routes.disk import disk
from routes.memory import memory
from routes.network import network

app = Flask(__name__)

app.register_blueprint(cpu, url_prefix='/cpu')
app.register_blueprint(disk, url_prefix='/disk')
app.register_blueprint(memory, url_prefix='/memory')
app.register_blueprint(network, url_prefix='/network')

@app.route('/')
def index():
    return render_template('index.html')


if __name__ == '__main__':
    app.run(debug=True)