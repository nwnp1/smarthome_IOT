import paho.mqtt.client as mqtt
import requests, json

server = "3.223.138.218"
url="http://3.223.138.218:8086/write?db=sensorDB"
def on_connect(client, userdata, flags, rc):
    print("Connected with RC : " + str(rc))
    client.subscribe("#")
def on_message(client, userdata, msg):
    print(msg.topic+" "+msg.payload.decode('utf-8'))
    res= requests.post(url,msg.payload.decode('utf-8'))
    if msg.topic=="On":
        if int(msg.payload.decode('utf-8')) < 100:
            client.publish("relay","1")
        else :
            client.publish("relay","0")

    if msg.topic=="fire":
        if int(msg.payload.decode('utf-8')) < 100:
            client.publish("speaker","1")
        else :
            client.publish("speaker","0")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(server, 1883, 60)
client.loop_forever()
