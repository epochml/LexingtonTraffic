from __main__ import app
from flask import Flask, request
import json
from markupsafe import escape

@app.route("/")
def welcome():
    return "<p>Hello, use the api pls at /api or check docs at /docs</p>"

@app.route("/api/lastx")
def get_last_x():
    num = int(request.args.get('num'))
    if num is None:
        return "ERR: Missing parameter", 400

    with open('../dummydata.json', 'r') as file: # switch to the actual data when we need to
        data = json.load(file)  
    
    data = list(data.items())[-num:]
    data.reverse()
    return dict(data)
@app.route("/api/getrange")
def get_range():
    start = int(request.args.get('start'))
    stop = int(request.args.get('stop'))
    if start is None or stop is None:
        return "ERR: Missing parameter", 400

    with open('../dummydata.json', 'r') as file: # switch to the actual data when we need to
        data = json.load(file)  
    
    data = list(data.items())[start:stop + 1]
    data.reverse()
    return dict(data)

@app.route("/docs")
def get_docs():
    return "<h1>Docs</h1><h2>/api/lastx</h2><p>returns the last x pieces of data. access by using /api/lastx?num=z where z is the last z pieces of data you want.</p><h2>/api/getrange</h2><p>returns the pieces of data in range y z. access by using /api/getrange?start=y&stop=z where y is the first piece of data and z is the last.</p>"