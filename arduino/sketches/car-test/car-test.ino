// Motor Driver pins
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

void setup() {
  // Set motor control pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  forward();
  delay(2000);

  stopMotors();
  delay(1000);

  reverse();
  delay(2000);

  stopMotors();
  delay(1000);

  turnLeft();
  delay(500);

  stopMotors();
  delay(1000);

  turnRight();
  delay(500);

  stopMotors();
  delay(2000);
}

// Movement functions
void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void reverse() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); // Left motors reverse
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Right motors forward
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Left motors forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Right motors reverse
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
