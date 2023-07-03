# app.py
from flask import Flask
from database import initialize_db
from routes import register_routes

def create_app():
    app = Flask(__name__)
    db_user = 'datacenter'
    db_password = 'datacenter'
    db_name = 'guardias'

    app.config['SQLALCHEMY_DATABASE_URI'] = f'postgresql://{db_user}:{db_password}@192.168.100.8/{db_name}'
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

    initialize_db(app)
    register_routes(app)

    return app

if __name__ == '__main__':
    app = create_app()
    app.run(debug=True, port=5000, host='0.0.0.0')