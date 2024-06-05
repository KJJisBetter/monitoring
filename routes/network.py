from flask import Blueprint, jsonify
import ctypes

network = Blueprint('network', __name__)

class NetworkUsage(ctypes.Structure):
    #this was done with the help of chatgpt
    _fields_ = [("inbound", ctypes.c_double),
                ("outbound", ctypes.c_double)]

lib = ctypes.CDLL('./backend/libsystem_data.so')

lib.network_data.restype = ctypes.c_double
lib.network_data.argtypes = [ctypes.c_int, ctypes.POINTER(NetworkUsage)]

@network.route('/usage/<int:interval>', methods=['GET'])
def get_network_usage(interval):
    usage = NetworkUsage()
    lib.network_data(interval, ctypes.byref(usage))

    return jsonify({
        # I would have never come up with this without the help chatgpt
        'network_in': usage.inbound,
        'network_out': usage.outbound
    })

# copilot seems to be insanely good at just knowing whats next after doing it once.