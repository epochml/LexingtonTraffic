from flask import Flask, request
import json
from markupsafe import escape

app = Flask(__name__)

import receiver.routes
import transmitter.routes

if __name__ == '__main__':
    app.run()