from flask import Blueprint, jsonify
import ctypes

network = Blueprint('network', __name__)

lib = ctypes.CDLL('./backend/libsystem_data.so')

lib.network_data.restype = ctypes.c_int

@network.route('/usage/<int:interval>', methods=['GET'])
def get_network_usage(interval):
    network_usage = lib.network_data(interval)

    net_format = f"{network_usage:.2f} KB"


    return jsonify({
        'network': net_format
    })

# copilot seems to be insanely good at just knowing whats next after doing it once.