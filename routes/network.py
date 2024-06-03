from flask import Blueprint, jsonify
import ctypes

network = Blueprint('network', __name__)

lib = ctypes.CDLL('./libnetwork.so')

lib.get_network_info.restype = ctypes.c_double

@network.route('/usage', methods=['GET'])
def get_network_usage():
    network_usage = lib.get_network_info()
    return jsonify({'network_usage': network_usage})