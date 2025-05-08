// Motor Driver pins
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
const int TRIGGER_PIN = 6;
const int ECHO_PIN = 7;

void setup() {
  // Set motor control pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {

  long getDistance(int TRIGGER_PIN, int ECHO_PIN)
  if (distance < 30) {
    stopMotors();
    
    Serial.print("Obstacle Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(1000); // Delay a little bit to improve simulation performance
 
    reverse();
    delay(1000);
    
    turnLeft();
    delay(1000);
  } else {
    forward();
    delay(2000);
  }
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

long getDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  
  // Reads the echo pin, and returns the sound wave travel time in microseconds to calculate distance
  long distance = 0.01723 * pulseIn(echoPin, HIGH);

  return distance;
}
