# routes.py
from flask import Blueprint, jsonify, request
from models import Guardia, db

bp = Blueprint('guardia', __name__, url_prefix='/guardia')
dias_semana=['lunes', 'martes', 'miercoles', 'jueves', 'viernes', 'sabado', 'domingo']

@bp.get('/<int:dia>')
def obtener_guardia(dia):
    try:
        if dia < 1 or dia > 7:
            return jsonify({'error': 'Los dias de la semana son entre el 1 y el 7'}), 400
        
        target_day = dias_semana[dia - 1]
        guardia = Guardia.query.filter_by(dia=target_day.lower()).first()
        
        if guardia:
            return jsonify({'nombre': guardia.nombre, 'chat_id': guardia.chat_id, 'telefono': guardia.telefono})
        else:
            return jsonify({'error': 'No se encontro guardia para el dia especificado'}, 404)
    except Exception as e:
        return jsonify({'error': f'Error al obtener la guardia: {e}'})

@bp.post('')
def agregar_guardia():
    try:
        data = request.get_json()
        
        required_fields = ['nombre', 'dia', 'chat_id', 'telefono']
        if not all(field in data for field in required_fields):
            return jsonify({'error': 'Faltan campos obligatorios'}), 400
        
        target_day = data['dia'].lower()
        if target_day not in dias_semana:
            return jsonify({'error': 'El dia especificado no es valido'}), 400
        
        existing_guardia = Guardia.query.filter_by(dia=target_day).first()
        if existing_guardia:
            return jsonify({'error': 'Ya hay una guardia asignada para el dia especificado'}), 400
        
        guardia = Guardia(nombre=data['nombre'], dia=target_day, chat_id=data['chat_id'], telefono=data['telefono'])
        db.session.add(guardia)
        db.session.commit()
        
        return jsonify({'message': 'Guardia agregada exitosamente'})
    except Exception as e:
        return jsonify({'error': f'Error al agregar la guardia: {e}'})

@bp.delete('/<int:dia>')
def eliminar_guardia(dia):
    try:
        if dia < 1 or dia > 7:
            return jsonify({'error': 'Los dias de la semana son entre el 1 y el 7'}), 400
        
        target_day = dias_semana[dia - 1]
        guardia = Guardia.query.filter_by(dia=target_day.lower()).first()
        
        if guardia:
            db.session.delete(guardia)
            db.session.commit()
            
            return jsonify({'message': 'Guardia eliminada exitosamente'})
        else:
            return jsonify({'error': 'No se encontro guardia para el dia especificado'})
    except Exception as e:
        return jsonify({'error': f'Error al eliminar la guardia: {e}'})

def register_routes(app):
    app.register_blueprint(bp)