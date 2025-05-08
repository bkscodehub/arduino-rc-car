import paho.mqtt.client as mqtt

#BROKER = "broker.hivemq.com"
BROKER = ""<HiveMQ MQTT BROKER HOST URL>"
CMD_TOPIC = "car/command"
STATUS_TOPIC = "car/status"

def on_message(client, userdata, msg):
    print(f"[{msg.topic}] {msg.payload.decode()}")

client = mqtt.Client()
client.on_message = on_message
#client.connect(BROKER, 1883, 60)
client.connect(BROKER, 8883, 60)
client.subscribe(STATUS_TOPIC)

client.loop_start()

# Send a command to Arduino via ESP-01
while True:
    cmd = input("Enter command (F/B/L/R): ")
    if cmd.upper() in ['F', 'B', 'L', 'R']:
        client.publish(CMD_TOPIC, cmd.upper())
    else:
        print("Invalid command.")
