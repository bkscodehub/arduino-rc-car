import paho.mqtt.client as mqtt
import ssl

# HiveMQ Cloud details
BROKER = "<HiveMQ MQTT BROKER HOST URL>"
PORT = 8883
USERNAME = "<HiveMQ USERID>"
PASSWORD = "<HiveMQ PASSWORD>"
TOPIC = "car/command"

client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.tls_set(ca_certs="hivemq-com-chain.pem", cert_reqs=ssl.CERT_REQUIRED)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connected to HiveMQ Cloud!")
        client.subscribe(TOPIC)
        print(f"📥 Subscribed to topic '{TOPIC}'")
    else:
        print("❌ Connection failed with code", rc)

def on_message(client, userdata, msg):
    print(f"📨 Message on {msg.topic}: {msg.payload.decode()}")

client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT)
client.loop_forever()
