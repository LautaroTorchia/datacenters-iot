import os
import json
from flask import Flask, jsonify, request

app = Flask(__name__)

# Verificar si existe un archivo de datos guardados
if os.path.isfile('personas_guardia.json'):
    with open('personas_guardia.json') as file:
        personas_guardia = json.load(file)
else:
    personas_guardia = {"guardias": []}

@app.route('/guardia', methods=['POST'])
def cargar_guardia():
    datos_guardia = request.get_json()
    personas_guardia["guardias"].append(datos_guardia)
    guardar_datos()
    return jsonify({'message': 'Guardia cargada exitosamente'})

@app.route('/guardia/<int:dia>', methods=['GET'])
def obtener_guardia(dia):
    guardia = next((persona for persona in personas_guardia["guardias"] if persona['dia'] == dia), None)
    if guardia:
        return jsonify({'nombre': guardia['nombre'], 'chat_id': guardia['chat_id']})
    else:
        return jsonify({'error': 'No se encontró guardia para el día especificado'})

def guardar_datos():
    with open('personas_guardia.json', 'w') as file:
        json.dump(personas_guardia, file)

if __name__ == '__main__':
    app.run(debug=True)