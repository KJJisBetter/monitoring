from flask import Blueprint, jsonify
import ctypes

cpu = Blueprint('cpu', __name__)

lib = ctypes.CDLL('./libcpu.so')

lib.get_cpu_info.restype = ctypes.c_double

# copilot seems to be good at getting this api stuff. learning lots
@cpu.route('/usage', methods=['GET'])
def get_cpu_usage():
    cpu_usage = lib.get_cpu_info()
    return jsonify({'cpu_usage': cpu_usage})