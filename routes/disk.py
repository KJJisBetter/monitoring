from flask import Blueprint, jsonify
import ctypes

disk = Blueprint('disk', __name__)

lib = ctypes.CDLL('./libdisk.so')

lib.get_disk_info.restype = ctypes.c_double

@disk.route('/usage', methods=['GET'])
def get_disk_usage():
    disk_usage = lib.get_disk_info()
    return jsonify({'disk_usage': disk_usage})