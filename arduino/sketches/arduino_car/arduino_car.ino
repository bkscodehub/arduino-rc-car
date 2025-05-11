#include <SoftwareSerial.h>

// === Pin Configuration ===
#define LEFT_MOTOR_PIN1 8
#define LEFT_MOTOR_PIN2 9
#define LEFT_MOTOR_ENA_PIN 5 // ENA on L298N
#define RIGHT_MOTOR_PIN1 10
#define RIGHT_MOTOR_PIN2 11
#define RIGHT_MOTOR_ENB_PIN 6 //ENB on L298N
#define TRIG_PIN 4
#define ECHO_PIN 7
#define SENSOR_ID 1 // Ultrasound sensor Id

SoftwareSerial espSerial(2, 3); // RX, TX

// === Motor Speed Variables ===
byte leftMotorSpeed = 127;
byte rightMotorSpeed = 127;

// === Time Tracking ===
unsigned long lastSensorTime = 0;
const unsigned long SENSOR_INTERVAL = 1000;

// === State Tracking ===
enum CarState { STOPPED, FORWARD, BACKWARD, LEFT, RIGHT } carState = STOPPED;

// === Setup ===
void setup() {
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_ENA_PIN, OUTPUT); // ENA on L298N for speed control
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_ENB_PIN, OUTPUT); // ENB on L298N for speed control
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
  espSerial.begin(9600);

  stopMotors();
  applyMotorSpeeds();
}

// === Main Loop ===
void loop() {
  if (espSerial.available()) {
    String line = espSerial.readStringUntil('\n');
    Serial.println(line);
    if (line.startsWith(">")) {
      char cmd = line.charAt(1); // Get the character after '>'
      handleCommand(cmd);
    }
  }

  if (millis() - lastSensorTime > SENSOR_INTERVAL) {
    processSensorData();
    lastSensorTime = millis();
  }
}

// === Command Handler ===
void handleCommand(char cmd) {
  Serial.println(cmd);
  switch (cmd) {
    case 'F': forward(); break; // Forward
    case 'B': reverse(); break; // Backward
    case 'L': turnLeft(); break; // Left
    case 'R': turnRight(); break; // Right
    case 'S': stopMotors(); break; // Stop
    case 'A': accelerate(); break; // Accelerate
    case 'D': decelerate(); break; // Decelerate
    default: stopMotors(); break;
  }
}

// === Sensor Handling ===
void processSensorData() {
  long dist = readDistanceCM();
  espSerial.print("{\"");
  espSerial.print(SENSOR_ID);
  espSerial.print("\":");
  espSerial.print(dist);
  espSerial.println("}");
}

// === Motor Control Functions ===
void forward() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  applyMotorSpeeds();
  carState = FORWARD;
}

void reverse() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
  applyMotorSpeeds();
  carState = BACKWARD;
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  applyMotorSpeeds();
  delay(750);
  stopMotors();
  carState = LEFT;
}

void turnRight() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
  applyMotorSpeeds();
  delay(750);
  stopMotors();
  carState = RIGHT;
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  carState = STOPPED;
}

void applyMotorSpeeds() {
  analogWrite(LEFT_MOTOR_ENA_PIN, leftMotorSpeed);
  analogWrite(RIGHT_MOTOR_ENB_PIN, rightMotorSpeed);
}

// === Speed Control ===
void accelerate() {
  leftMotorSpeed = min(leftMotorSpeed + 10, 255);
  rightMotorSpeed = min(rightMotorSpeed + 10, 255);
  applyMotorSpeeds();
  reportSpeed();
}

void decelerate() {
  leftMotorSpeed = max(leftMotorSpeed - 10, 0);
  rightMotorSpeed = max(rightMotorSpeed - 10, 0);
  applyMotorSpeeds();
  reportSpeed();
}

void reportSpeed() {
  espSerial.print("{\"speedL\":");
  espSerial.print(leftMotorSpeed);
  espSerial.print(",\"speedR\":");
  espSerial.print(rightMotorSpeed);
  espSerial.println("}");
}

// === Ultrasonic Distance ===
long readDistanceCM() {
  long sum = 0;
  for (int i = 0; i < 3; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    sum += duration * 0.034 / 2;
    delay(10);
  }
  return sum / 3;
}
