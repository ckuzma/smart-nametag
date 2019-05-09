from flask import Flask, request

## Setup Flask
application = Flask(__name__, static_url_path='')

## Setup space to save the lines
application.top    = ''
application.bottom = ''

## Define our routes
@application.route('/', methods=['GET'])
def root():
    return 'Hello, world!'

@application.route('/iot', methods=['GET'])
def iot():
    return application.top + '|' + application.bottom

@application.route('/update', methods=['GET'])
def update():
    return application.send_static_file('submit-form.html')

@application.route('/submit', methods=['POST'])
def submit():
    data = request.form
    application.top = data['top'][0:16]
    application.bottom = data['bottom'][0:16]
    return 'OK'

## Run the app
if __name__ == '__main__':
    application.run(host='0.0.0.0')
