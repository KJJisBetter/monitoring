from flask import Blueprint, jsonify
import ctypes

disk = Blueprint('disk', __name__)

lib = ctypes.CDLL('./backend/libsystem_data.so')

lib.diskio_data.restype = ctypes.c_int

@disk.route('/usage', methods=['GET'])
def get_disk_usage():
    disk_usage = lib.diskio_data()
    disk_format = f"{disk_usage / 1024:.2f} seconds"
    return jsonify({'disk_usage': disk_format})