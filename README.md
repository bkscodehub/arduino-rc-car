# Arduino RC Car with MQTT Control

This project is an Internet-controlled 4WD RC car powered by an **Arduino Uno R4 Minima**, an **ESP8266 (ESP-01)** module, and **Python**. It uses **HiveMQ Cloud MQTT** for wireless communication, allowing you to control the car and receive sensor telemetry from a desktop app.

---

## 🛠️ Features

- Wireless car control using MQTT (`F`, `B`, `L`, `R`, `S`)
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
│   └───sketches
│       ├───arduino_car
│       │       arduino_car.ino
│       ├───car-test
│       │       car-test.ino
│       ├───esp8266-wifi-test
│       │       esp8266-wifi-test.ino
│       ├───esp8266_relay
│       │       esp8266_relay.ino
│       └───obstacle-detection
│               obstacle-detection.ino
├───certs
│       hivemq-com-chain.pem
├───python
│       car_mqtt_client.py
│       mqtt-client.py
│       mqtt-sub.py
│       mqtt_publisher.py
│       mqtt_subscriber.py
├── README.md
└── LICENSE
```

---

## 🔐 MQTT Configuration

- **Broker**: `3bdf4e3470b14b238c91279bf1b4c47a.s1.eu.hivemq.cloud`
- **Port**: `8883`
- **Username/Password**: *(stored securely in `mqtt_controller.py`)*
- **Topics**:
  - `car/command` — commands from desktop to car
  - `car/telemetry` — sensor data from car to desktop

---

## 🚀 Getting Started

### 1. Flash the Arduino

Upload the sketch from `arduino/car_control.ino` using Arduino IDE. Ensure `SoftwareSerial` is set correctly to receive from ESP8266.

### 2. Connect the Hardware

- ESP-01 TX → Arduino D2 (SoftwareSerial RX)
- ESP-01 RX → Arduino D3 (via voltage divider)
- Motor pins → L298N
- Power motors using external 5V supply

### 3. Install Python Dependencies

```bash
pip install paho-mqtt
```

### 4. Run the Python Controller

```
cd python
python car_mqtt_client.py
```

Then type a command:
```
Enter command (F/B/L/R/S): F
```

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
