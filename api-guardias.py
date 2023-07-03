import requests

base_url = 'http://137.184.125.122:5000/guardia'  # Replace with the appropriate base URL of your Flask API

dias_semana = ['lunes', 'martes', 'miercoles', 'jueves', 'viernes', 'sabado', 'domingo']

def obtener_guardia(dia):
    url = f"{base_url}/{dia}"
    response = requests.get(url)
    data = response.json()
    
    if response.status_code == 200:
        print(response.json())
        print(f"Guardia encontrada para el día {dia}:")
        print(f"Nombre: {data['nombre']}")
        print(f"Chat ID: {data['chat_id']}")
        print(f"Teléfono: {data['telefono']}")
    else:
        print(data.get('error', 'Error al obtener la guardia'))

def agregar_guardia():
    nombre = input("Nombre: ")
    dia = int(input("Día (1-7): "))
    chat_id = input("Chat ID: ")
    telefono = input("Teléfono: ")
    
    payload = {
        'nombre': nombre,
        'dia': dias_semana[dia - 1],
        'chat_id': chat_id,
        'telefono': telefono
    }
    
    response = requests.post(base_url, json=payload)
    data = response.json()
    
    if response.status_code == 200:
        print("Guardia agregada exitosamente")
    else:
        print(data.get('error', 'Error al agregar la guardia'))

def eliminar_guardia(dia):
    url = f"{base_url}/{dia}"
    response = requests.delete(url)
    data = response.json()
    
    if response.status_code == 200:
        print("Guardia eliminada exitosamente")
    else:
        print(data.get('error', 'Error al eliminar la guardia'))

def print_menu():
    print("1. Obtener guardia")
    print("2. Agregar guardia")
    print("3. Eliminar guardia")
    print("0. Salir")

def main():
    while True:
        print("\n--- Menú Principal ---")
        print_menu()
        choice = input("Seleccione una opción: ")
        
        if choice == '1':
            dia = int(input("Ingrese el día (1-7): "))
            obtener_guardia(dia)
        elif choice == '2':
            agregar_guardia()
        elif choice == '3':
            dia = int(input("Ingrese el día (1-7): "))
            eliminar_guardia(dia)
        elif choice == '0':
            break
        else:
            print("Opción inválida. Intente nuevamente.")

if __name__ == '__main__':
    main()