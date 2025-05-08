import paho.mqtt.client as mqtt
import ssl
import json

BROKER = "<HiveMQ MQTT BROKER HOST URL>"
PORT = 8883
USERNAME = "<HiveMQ USERID>"
PASSWORD = "<HiveMQ PASSWORD>"
CA_CERT = "../certs/hivemq-com-chain.pem"

COMMAND_TOPIC = "car/command"
TELEMETRY_TOPIC = "car/telemetry"

# Callback when connected
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Connected to HiveMQ Cloud")
#        client.subscribe(TELEMETRY_TOPIC)
#        print(f"📥 Subscribed to {TELEMETRY_TOPIC}")
    else:
        print("❌ Failed to connect, return code:", rc)

# Callback when message is received
def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        print(f"📨 Sensor Data: {payload}")
    except json.JSONDecodeError:
        print(f"⚠️ Received malformed JSON: {msg.payload.decode()}")

client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.tls_set(CA_CERT, cert_reqs=ssl.CERT_REQUIRED)

client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT)
client.loop_start()

# === Simulate UI commands ===
def send_command(command):
    client.publish(COMMAND_TOPIC, command)
    print(f"📤 Sent command: {command}")

# === Test Commands ===
while True:
    cmd = input("Enter command (F/B/L/R/S): ").strip().upper()
    if cmd in ['F', 'B', 'L', 'R', 'S']:
        send_command(cmd)
    else:
        print("❌ Invalid command.")
