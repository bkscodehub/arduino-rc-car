import paho.mqtt.client as mqtt

#BROKER = "broker.hivemq.com"
BROKER = "<HiveMQ MQTT BROKER HOST URL>"
TOPIC = "car/command"

# Called when connected to the broker
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(TOPIC)

# Called when a message is received from the server
def on_message(client, userdata, msg):
    print(f"Message received on topic '{msg.topic}': {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

print(f"Connecting to broker at {BROKER}...")
#client.connect(BROKER, 1883, 60)
client.connect(BROKER, 8883, 60)

# Blocking loop that handles reconnection and message dispatch
client.loop_forever()
