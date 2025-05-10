import paho.mqtt.client as mqtt
import ssl
import json
import os
import tkinter as tk
from tkinter import ttk

# === MQTT Configuration ===
BROKER = "3bdf4e3470b14b238c91279bf1b4c47a.s1.eu.hivemq.cloud"
PORT = 8883
USERNAME = "admin01"
PASSWORD = "hiveAdmin01"
CA_CERT = os.path.join("..", "certs", "hivemq-com-chain.pem")

COMMAND_TOPIC = "car/command"
TELEMETRY_TOPIC = "car/telemetry"

# === MQTT Callbacks ===
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        status_label.config(text="✅ Connected to HiveMQ Cloud")
        client.subscribe(TELEMETRY_TOPIC)
    else:
        status_label.config(text=f"❌ Connection failed: {rc}")

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode())
        sensor_data.set(f"📡 Distance: {payload.get('1', 'N/A')} cm")
    except json.JSONDecodeError:
        sensor_data.set("⚠️ Invalid JSON received")

# === Publish Command ===
def send_command(cmd):
    client.publish(COMMAND_TOPIC, cmd)
    last_cmd.set(f"📤 Sent: {cmd}")

# === Tkinter UI ===
root = tk.Tk()
root.title("Arduino RC Car Controller")

frame = ttk.Frame(root, padding=20)
frame.grid()

# Status and Telemetry
status_label = ttk.Label(frame, text="🔌 Connecting...", foreground="blue")
status_label.grid(column=0, row=0, columnspan=3, pady=5)

sensor_data = tk.StringVar(value="📡 Distance: -- cm")
sensor_label = ttk.Label(frame, textvariable=sensor_data)
sensor_label.grid(column=0, row=1, columnspan=3, pady=5)

last_cmd = tk.StringVar(value="📤 Sent: --")
last_cmd_label = ttk.Label(frame, textvariable=last_cmd)
last_cmd_label.grid(column=0, row=2, columnspan=3, pady=5)

# Buttons
ttk.Button(frame, text="↑ Forward", command=lambda: send_command('F')).grid(column=1, row=3, pady=10)
ttk.Button(frame, text="← Left", command=lambda: send_command('L')).grid(column=0, row=4)
ttk.Button(frame, text="Stop", command=lambda: send_command('S')).grid(column=1, row=4)
ttk.Button(frame, text="→ Right", command=lambda: send_command('R')).grid(column=2, row=4)
ttk.Button(frame, text="↓ Reverse", command=lambda: send_command('B')).grid(column=1, row=5, pady=10)

# === MQTT Setup ===
client = mqtt.Client()
client.username_pw_set(USERNAME, PASSWORD)
client.tls_set(CA_CERT, cert_reqs=ssl.CERT_REQUIRED)

client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT)
client.loop_start()

# === Start GUI Loop ===
root.mainloop()
