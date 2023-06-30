import os
import json
from flask import Flask, jsonify, request, abort

app = Flask(__name__)

file_path = 'personas_guardia.json'
dias_semana = ["lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"]

# Verificar si existe un archivo de datos guardados
if os.path.isfile(file_path):
    try:
        with open(file_path) as file:
            personas_guardia = json.load(file)
    except (IOError, json.JSONDecodeError) as e:
        print(f'Error al cargar el archivo JSON: {e}')
        personas_guardia = {"guardias": []}
else:
    personas_guardia = {"guardias": []}

@app.route('/guardia/<int:dia>', methods=['GET'])
def obtener_guardia(dia):
    try:
        # Ensure the given dia is within the valid range
        if dia < 1 or dia > 7:
            return jsonify({'error': 'Los dias de la semana son entre el 1 y el 7'}), 400
        
        # Get the corresponding day of the week
        target_day = dias_semana[dia - 1]
        
        # Find the guardia for the target day
        guardia = next((persona for persona in personas_guardia["guardias"] if persona['dia'].lower() == target_day), None)
        
        if guardia:
            return jsonify({'nombre': guardia['nombre'], 'chat_id': guardia['chat_id'], 'telefono': guardia['telefono']})
        else:
            return jsonify({'error': 'No se encontro guardia para el dia especificado'})
    except Exception as e:
        return jsonify({'error': f'Error al obtener la guardia: {e}'})

@app.route('/guardia', methods=['POST'])
def agregar_guardia():
    try:
        data = request.get_json()
        
        # Validate request data
        required_fields = ['nombre', 'dia', 'chat_id', 'telefono']
        if not all(field in data for field in required_fields):
            return jsonify({'error': 'Faltan campos obligatorios'}), 400
        
        # Ensure the given dia is within the valid range
        target_day = data['dia'].lower()
        if target_day not in dias_semana:
            return jsonify({'error': 'El dia especificado no es valido'}), 400
        
        # Check if guardia already exists for the target day
        existing_guardia = next((persona for persona in personas_guardia["guardias"] if persona['dia'].lower() == target_day), None)
        if existing_guardia:
            return jsonify({'error': 'Ya hay una guardia asignada para el dia especificado'}), 400
        
        # Add new guardia
        personas_guardia["guardias"].append(data)
        
        # Save the updated data
        guardar_datos()
        
        return jsonify({'message': 'Guardia agregada exitosamente'})
    except Exception as e:
        return jsonify({'error': f'Error al agregar la guardia: {e}'})

@app.route('/guardia/<int:dia>', methods=['DELETE'])
def eliminar_guardia(dia):
    try:
        # Ensure the given dia is within the valid range
        if dia < 1 or dia > 7:
            return jsonify({'error': 'Los dias de la semana son entre el 1 y el 7'}), 400
        
        # Get the corresponding day of the week
        target_day = dias_semana[dia - 1]
        
        # Find the guardia for the target day
        guardia = next((persona for persona in personas_guardia["guardias"] if persona['dia'].lower() == target_day), None)
        
        if guardia:
            # Remove the guardia
            personas_guardia["guardias"].remove(guardia)
            
            # Save the updated data
            guardar_datos()
            
            return jsonify({'message': 'Guardia eliminada exitosamente'})
        else:
            return jsonify({'error': 'No se encontro guardia para el dia especificado'})
    except Exception as e:
        return jsonify({'error': f'Error al eliminar la guardia: {e}'})

def guardar_datos():
    try:
        with open(file_path, 'w') as file:
            json.dump(personas_guardia, file)
    except Exception as e:
        print(f'Error al guardar los datos en el archivo JSON: {e}')

if __name__ == '__main__':
    app.run(debug=True, port=5000, host='0.0.0.0')