from flask import Flask, request

## Setup Flask
app = Flask(__name__, static_url_path='')
app.top    = ''
app.bottom = ''

@app.route('/', methods=['GET'])
def root():
    return 'Hello, world!'

@app.route('/iot', methods=['GET'])
def iot():
    return app.top + '|' + app.bottom

@app.route('/update', methods=['GET'])
def update():
    return app.send_static_file('submit-form.html')

@app.route('/submit', methods=['POST'])
def submit():
    data = request.form
    app.top = data['top'][0:16]
    app.bottom = data['bottom'][0:16]
    return 'OK'
