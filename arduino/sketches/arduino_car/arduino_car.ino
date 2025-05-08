#include <SoftwareSerial.h>

#define LEFT_MOTOR_PIN1 8
#define LEFT_MOTOR_PIN2 9
#define RIGHT_MOTOR_PIN1 10
#define RIGHT_MOTOR_PIN2 11
#define TRIG_PIN 4
#define ECHO_PIN 5
#define SENSOR_ID 1 // Ultrasound sensor Id

SoftwareSerial espSerial(2, 3); // RX, TX

unsigned long lastSensorTime = 0;

void setup() {
  espSerial.begin(9600);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  if (espSerial.available()) {
    char cmd = espSerial.read();
    drive(cmd);
  }

  // Send sensor data every 1s
  if (millis() - lastSensorTime > 1000) {
    long dist = readDistanceCM();
    espSerial.print("{\"");
    espSerial.print(SENSOR_ID);
    espSerial.print("\":");
    espSerial.print(dist);
    espSerial.println("}");
    lastSensorTime = millis();
  }
}
void drive(char cmd) {
  switch (cmd) {
    case 'F': // Forward
      forward();
      break;
    case 'B': // Backward
      reverse();
      break;
    case 'L': // Left
      turnLeft();
      break;
    case 'R': // Right
      turnRight();
      break;
    case 'S': // Stop
      stopMotors();
      break;
  }
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// Movement functions
void forward() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

void reverse() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW); // Left motors reverse
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);  // Right motors forward
  
  delay(500); // stop after turning
  stopMotors();
}

void turnRight() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);  // Left motors forward
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH); // Right motors reverse

  delay(500); // stop after turning
  stopMotors();
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}
