from flask import Blueprint, jsonify
import ctypes

memory = Blueprint('memory', __name__)

lib = ctypes.CDLL('./libmemory.so')

lib.get_memory_info.restype = ctypes.c_double

@memory.route('/usage', methods=['GET'])
def get_memory_usage():
    memory_usage = lib.get_memory_info()
    return jsonify({'memory_usage': memory_usage})