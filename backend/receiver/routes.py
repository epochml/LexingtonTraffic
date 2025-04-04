## Receiver for ESP32 data.
from __main__ import app
from flask import request
from flask import jsonify 
import json

@app.route("/upload_data", methods=["POST"])
def upload_data():
    with open('../dummydata.json', 'r') as file: # switch to the actual data when we need to
        data = json.load(file)  
    print("stored data" + data)
    print(str("get json" + request.get_json()))
    print(str("req.data" + request.data))
    print(str("form" + request.form))
    got_data = jsonify(request.form.to_dict(flat=False))
    print(str( "as dict" + got_data))
    data.push(got_data)
    print("new stored data" + data)
    json.dump(data, file, ensure_ascii=False, indent=4)
    return "<h1>:3</h1><h2>:3</h2><h3>:3</h3>"