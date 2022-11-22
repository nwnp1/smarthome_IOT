import time
import paho.mqtt.client as paho
import picamera

broker = "192.168.43.19"
recvData = ""


def on_message(client, userdata, message):
    time.sleep(1)
    recvData = str(message.payload.decode("utf-8"))
    print(recvData)
    if recvData == ("1"):
        print("camera")
        camera = picamera.PiCamera()
        camera.start_recording('final1.h264')
        time.sleep(7)
        camera.stop_recording()
        camera.close()


client = paho.Client()
client.on_message = on_message

while True:
    print(broker)
    client.connect(broker)
    client.loop_start()
    print("subscribing ")
    client.subscribe("camera")
    time.sleep(5)
    client.disconnect()
    client.loop_stop()
