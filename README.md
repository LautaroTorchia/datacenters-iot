# Trabajo integrador IoT

## Autores

- Lautaro Torchia
- Gregorio Maclen

## Trabajo Refrigeración Datacenters

El proyecto se enfoca en el monitoreo de aires acondicionados en datacenters e incluye los siguientes aspectos:

- Desarrollo de nodo sensor con sensores de temperatura, detección de agua y emisores infrarrojos como actuadores.
- Control de equipos multimarca.
- Comunicación inalámbrica.

## Arquitectura elegida

### MQTT Broker

Se decidió utilizar un MQTT Broker para el intercambio de mensajes entre las distintas tecnologías. Se utilizan 3 tópicos en los cuales existen nodos que se suscriben y otros que publican en ellos:

- `datacenter/temperature`: recolección y almacenamiento de datos de temperatura en el datacenter.
- `datacenter/humidity`: recolección y almacenamiento de datos de humedad en el datacenter.
- `datacenter/water`: recolección y detección de agua en el datacenter.
- `datacenter/actions`: códigos de acción enviados a los nodos para ejecutar funcionalidades.

### ESP-32

Se decidió utilizar ESP-32 como tecnología para el desarrollo e implementación de los nodos encargados de la recolección de la información de temperatura, humedad y agua, así como del manejo de los dispositivos del datacenter. Los nodos se dividen en 2 categorías:

#### Worker

Los workers son los nodos ubicados en distintos sitios estratégicos en el datacenter y se encargan de la recolección de todos los datos de temperatura, humedad y agua para su almacenamiento. Se integran con sensores de agua y otros de temperatura, como el DHT22.

#### Controller

El nodo controller es el nodo ESP-32 encargado de la manipulación de los distintos dispositivos de ayuda dentro de un datacenter. Su tarea principal es recibir órdenes y encender o apagar el aire acondicionado del datacenter según corresponda, utilizando su sensor infrarrojo.

### Node-RED

Se decidió utilizar Node-RED para la creación de flujos de acción y controlar todo el flujo del sistema. Node-RED se conecta con todas las demás tecnologías y es responsable de interconectarlas. También se utiliza para enviar alertas mediante SMS y Telegram a la persona de guardia en el datacenter.

### InfluxDB/Grafana

Se decidió utilizar InfluxDB y Grafana como sistema de almacenamiento y análisis de los datos de temperatura, humedad y agua recolectados por los nodos workers. También se generaron varios gráficos en Grafana para analizar y utilizar eficientemente la información recolectada, así como para el monitoreo.

### Python/Javascript

Se utilizaron scripts y servicios desplegados en ambas tecnologías para el soporte y mejora del sistema. Especialmente, se desarrolló una API en Python para el manejo de los turnos de guardia. Esta API es utilizada por el sistema (principalmente Node-RED) para obtener la persona encargada del turno de guardia del datacenter y utilizarla como contacto en caso de potenciales errores.

## Servidor donde se encuentran los servicios de monitoreo y flujo

- IP: 137.184.125.122

- Grafana: [137.184.125.122:3000](http://137.184.125.122:3000)
- MQTT Broker: [137.184.125.122:1883](mqtt://137.184.125.122:1883)
- Node-RED Flujo: [137.184.125.122:1880](http://137.184.125.122:1880)

## Contacto

Para contactarnos para obtener credenciales de acceso o mas preguntas hablarnos aqui:

- Lautaro Torchia: torchia.lautaro2@gmail.com
- Gregorio Maclen: gregorio.maclen@gmail.com
