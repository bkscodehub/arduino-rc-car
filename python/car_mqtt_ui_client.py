import paho.mqtt.client as mqtt
import ssl
import json
import os
import tkinter as tk
from tkinter import ttk
import pygame

# === MQTT Configuration ===
BROKER = "3bdf4e3470b14b238c91279bf1b4c47a.s1.eu.hivemq.cloud"
PORT = 8883
USERNAME = "admin01"
PASSWORD = "hiveAdmin01"
CA_CERT = os.path.join("..", "certs", "hivemq-com-chain.pem")

COMMAND_TOPIC = "car/command"
TELEMETRY_TOPIC = "car/telemetry"

# === Paths to Sound Effects ===
SOUND_PATH = os.path.join("..", "sounds")
SOUNDS = {
    'F': os.path.join(SOUND_PATH, "move.wav"),
    'B': os.path.join(SOUND_PATH, "move.wav"),
    'L': os.path.join(SOUND_PATH, "turn.wav"),
    'R': os.path.join(SOUND_PATH, "turn.wav"),
    'S': os.path.join(SOUND_PATH, "brake.mp3"),
    'A': os.path.join(SOUND_PATH, "accelerate.wav"),
    'D': os.path.join(SOUND_PATH, "decelerate.wav"),
}
pygame.mixer.init()
current_channel = pygame.mixer.Channel(0)  # Use channel 0 for all sounds

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

# === Play Sound in Thread ===
def play_sound(cmd):
    path = SOUNDS.get(cmd)
    print("Sound file path:", path)
    if path and os.path.exists(path):
        try:
            current_channel.stop()  # Stop any previously playing sound
            sound = pygame.mixer.Sound(path)
            if cmd == 'S':
                current_channel.play(sound, loops=0)  # Play once
            else:
                current_channel.play(sound, loops=-1)  # Loop continuously
        except Exception as e:
            print("Sound error:", e)
    else:
        print("Sound file does not exist for command:", cmd)

        
# === Send Command & Highlight ===
def send_command(cmd, button=None):
    client.publish(COMMAND_TOPIC, cmd)
    last_cmd.set(f"📤 Sent: {cmd}")
    play_sound(cmd)
    if button:
        highlight_button(button)

# === Visual Highlight Effect ===
def highlight_button(btn):
    btn.config(style="Active.TButton")
    root.after(200, lambda: btn.config(style="TButton"))

# === Tkinter UI ===
root = tk.Tk()
root.title("Arduino RC Car Controller")

style = ttk.Style()
style.configure("TButton", padding=6, font=("Segoe UI", 10))
style.configure("Active.TButton", background="lightgreen")

frame = ttk.Frame(root, padding=20)
frame.grid()

# Status and Telemetry
status_label = ttk.Label(frame, text="🔌 Connecting...", foreground="blue")
status_label.grid(column=0, row=0, columnspan=6, pady=5)

sensor_data = tk.StringVar(value="📡 Distance: -- cm")
sensor_label = ttk.Label(frame, textvariable=sensor_data)
sensor_label.grid(column=0, row=1, columnspan=6, pady=5)

last_cmd = tk.StringVar(value="📤 Sent: --")
last_cmd_label = ttk.Label(frame, textvariable=last_cmd)
last_cmd_label.grid(column=0, row=2, columnspan=6, pady=5)

# === Buttons ===
btn_forward = ttk.Button(frame, text="↑ Forward", command=lambda: send_command('F', btn_forward))
btn_forward.grid(column=2, row=3, pady=10)

btn_left = ttk.Button(frame, text="← Left", command=lambda: send_command('L', btn_left))
btn_left.grid(column=1, row=4)

btn_stop = ttk.Button(frame, text="Stop", command=lambda: send_command('S', btn_stop))
btn_stop.grid(column=2, row=4)

btn_right = ttk.Button(frame, text="→ Right", command=lambda: send_command('R', btn_right))
btn_right.grid(column=3, row=4)

btn_accelerate = ttk.Button(frame, text="🚀 Accelerate", command=lambda: send_command('A', btn_accelerate))
btn_accelerate.grid(column=4, row=4)

btn_decelerate = ttk.Button(frame, text="🐢 Decelerate", command=lambda: send_command('D', btn_decelerate))
btn_decelerate.grid(column=5, row=4)

btn_reverse = ttk.Button(frame, text="↓ Reverse", command=lambda: send_command('B', btn_reverse))
btn_reverse.grid(column=2, row=5, pady=10)

# === Keyboard Bindings ===
def handle_key(event):
    if event.keysym == 'Up' and event.state & 0x4:
        send_command('A', btn_accelerate)
    elif event.keysym == 'Down' and event.state & 0x4:
        send_command('D', btn_decelerate)
    elif event.keysym == 'Up':
        send_command('F', btn_forward)
    elif event.keysym == 'Down':
        send_command('B', btn_reverse)
    elif event.keysym == 'Left':
        send_command('L', btn_left)
    elif event.keysym == 'Right':
        send_command('R', btn_right)

root.bind('<Key>', handle_key)

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
