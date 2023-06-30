# database.py
from flask import Flask
from models import db

def initialize_db(app: Flask):
    db.init_app(app)
    with app.app_context():
        db.create_all()