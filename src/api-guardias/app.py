import os
import json
from flask import Flask, jsonify, request

app = Flask(__name__)

file_path = 'personas_guardia.json'

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

@app.route('/guardia', methods=['POST'])
def cargar_guardia():
    try:
        datos_guardia = request.get_json()
        personas_guardia["guardias"].append(datos_guardia)
        guardar_datos()
        return jsonify({'message': 'Guardia cargada exitosamente'})
    except Exception as e:
        return jsonify({'error': f'Error al cargar la guardia: {e}'})

@app.route('/guardia/<int:dia>', methods=['GET'])
def obtener_guardia(dia):
    try:
        guardia = next((persona for persona in personas_guardia["guardias"] if persona['dia'] == dia), None)
        if guardia:
            return jsonify({'nombre': guardia['nombre'], 'chat_id': guardia['chat_id']})
        else:
            return jsonify({'error': 'No se encontró guardia para el día especificado'})
    except Exception as e:
        return jsonify({'error': f'Error al obtener la guardia: {e}'})

def guardar_datos():
    try:
        with open(file_path, 'w') as file:
            json.dump(personas_guardia, file)
    except Exception as e:
        print(f'Error al guardar los datos en el archivo JSON: {e}')

if __name__ == '__main__':
    app.run(debug=True, port=5000)