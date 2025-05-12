import paho.mqtt.client as mqtt
import ssl

# HiveMQ Cloud details
BROKER = os.environ["MQTT_BROKER"]
PORT = int(os.environ.get("MQTT_PORT", 8883))
USERNAME = os.environ["MQTT_USERNAME"]
PASSWORD = os.environ["MQTT_PASSWORD"]
TOPIC = "car/command"

# Create the client
client = mqtt.Client()

# Set credentials
client.username_pw_set(USERNAME, PASSWORD)

# Configure TLS
client.tls_set(ca_certs="hivemq-com-chain.pem", cert_reqs=ssl.CERT_REQUIRED)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connected to HiveMQ Cloud!")
        # Send test message
        client.publish(TOPIC, "F")  # Example: send "Forward" command
        print(f"📤 Published 'F' to topic '{TOPIC}'")
    else:
        print("❌ Failed to connect, return code:", rc)

client.on_connect = on_connect

client.connect(BROKER, PORT)
client.loop_forever()
