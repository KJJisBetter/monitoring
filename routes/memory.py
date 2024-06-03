from flask import Blueprint, jsonify
import ctypes

memory = Blueprint('memory', __name__)

lib = ctypes.CDLL('./backend/libsystem_data.so')

lib.memory_data.restype = ctypes.c_int

@memory.route('/usage', methods=['GET'])
def get_memory_usage():
    memory_usage = lib.memory_data()
    return jsonify({'memory_usage': memory_usage})