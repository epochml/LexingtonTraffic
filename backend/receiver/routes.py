## Receiver for ESP32 data.
from __main__ import app

@app.route("/upload_data", methods=["POST"])
def upload_data():
    return ""