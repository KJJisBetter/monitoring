from flask import Blueprint, jsonify
import ctypes

cpu = Blueprint('cpu', __name__)

lib = ctypes.CDLL('./backend/libsystem_data.so')

lib.cpu_data.restype = ctypes.c_int

# copilot seems to be good at getting this api stuff. learning lots
@cpu.route('/usage', methods=['GET'])
def get_cpu_usage():
    cpu_usage = lib.cpu_data()
    return jsonify({'cpu_usage': cpu_usage})