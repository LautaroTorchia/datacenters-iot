# models.py
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Guardia(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    nombre = db.Column(db.String(100), nullable=False)
    dia = db.Column(db.String(10), nullable=False)
    chat_id = db.Column(db.String(50), nullable=False)
    telefono = db.Column(db.String(20), nullable=False)