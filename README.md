# Arduino RC Car with MQTT Control

This project is an Internet-controlled 4WD RC car powered by an **Arduino Uno R4 Minima**, an **ESP8266 (ESP-01)** module, and **Python**. It uses **HiveMQ Cloud MQTT** for wireless communication, allowing you to control the car and receive sensor telemetry from a desktop app.

---

## 🛠️ Features

- Wireless car control using MQTT (`Forward`, `Backward`, `Turn Left`, `Turn Right`, `Stop`, `Accelerate`, `Decelerate`)
- Live distance telemetry using an ultrasonic sensor
- Python-based command interface
- Secure connection using TLS with HiveMQ Cloud

---

## 🧰 Hardware Used

- Arduino Uno R4 Minima
- ESP8266 ESP-01 Wi-Fi module
- L298N Motor Driver
- 4WD Car Chassis with DC Motors
- HC-SR04 Ultrasonic Sensor
- External 5V Power Supply for motors

---

## 📁 Project Structure

```
arduino-rc-car/
│   .gitignore
├───arduino
│   └─── # Arduino and ESP sketches
├───certs
│   └─── hivemq-com-chain.pem
├───python
│   └─── # Python app
├── README.md
└── LICENSE
```

---

## 🔐 MQTT Configuration

Configured as environment variables directly in the shell

- **Broker**: `3bdf4...`
- **Port**: `8883`
- **Username/Password**: _(HiveMQ cloud credentials)_
- **Topics**:
  - `car/command` — commands from desktop to car
  - `car/telemetry` — sensor data from car to desktop

### Linux/macOS:

```
export MQTT_BROKER=3bdf4...
export MQTT_PORT=8883
export MQTT_USERNAME=<<HiveMQ cloud user Id>>
export MQTT_PASSWORD=<<HiveMQ cloud password>>
```

### Windows (Command Prompt):

```
set MQTT_BROKER=3bdf4...
set MQTT_PORT=8883
set MQTT_USERNAME=<<HiveMQ cloud user Id>>
set MQTT_PASSWORD=<<HiveMQ cloud password>>
```

---

## 🚀 Getting Started

### 1. Flash the Arduino & ESP

#### a) Upload the sketch from `arduino_car.ino` from `/arduino/sketches/` using Arduino IDE into the Arduino Uno R4 Minima board. Ensure `SoftwareSerial` is set correctly to receive from ESP8266.

#### b) Upload the sketch from `esp8266_relay.ino` from `/arduino/sketches/` using Arduino IDE or an ESP Flasher into the ESP 01 8266 WiFi module.

### 2. Connect the Hardware

- ESP-01 TX → Arduino D2 (SoftwareSerial RX)
- ESP-01 RX → Arduino D3 (via voltage divider)
- Motor pins → L298N
- Power motors using external 5V supply

### 3. Install Python Dependencies

```bash
pip install -r requirements.txt
```

### 4. Run the Python Controller UI

```
cd python
python car_mqtt_ui_client.py
```

Use the buttons on the UI to navigate the RC car

📡 Telemetry Example
Every second, the car sends distance data:

```
{"1": 25}
```

---

## 📜 License

This project is open-source and available under the GNU GENERAL PUBLIC LICENSE (Version 3, 29 June 2007).

---

## 🧠 Credits

Developed as a DIY robotics + IoT integration project combining Arduino Uno R4 Minima, ESP 8266, HiveMQ MQTT Broker, and Python.

---
