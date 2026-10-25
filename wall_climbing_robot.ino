#include <Servo.h> // Required for ESC control

#define TRIG_PIN 4
#define ECHO_PIN 5
#define GREEN_LED A0
#define RED_LED A1
#define BUZZER_PIN 11 // PWM-capable digital pin

// Rear motor pins
#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9

Servo bldc; // BLDC ESC control

float duration_us, distance_cm;
String command = "";
int currentPWM = 1000; // Track current speed for smooth ramping

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  bldc.attach(3); // ESC PWM signal pin
  bldc.writeMicroseconds(1000); // Arm ESC
  delay(3000);
  Serial.println("Ready with 1 Motor Driver + BLDC control");
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void playBeep(int delayGap) {
  tone(BUZZER_PIN, 1000); delay(50);
  noTone(BUZZER_PIN); delay(delayGap);
}

void rampToSpeed(int targetPWM) {
  Serial.print("Ramping to ");
  Serial.print(targetPWM);
  Serial.println(" µs");
  if (targetPWM > currentPWM) {
    for (int i = currentPWM; i <= targetPWM; i++) {
      bldc.writeMicroseconds(i);
      delay(5); // Adjust smoothness
    }
  } else if (targetPWM < currentPWM) {
    for (int i = currentPWM; i >= targetPWM; i--) {
      bldc.writeMicroseconds(i);
      delay(5); // Adjust smoothness
    }
  }
  currentPWM = targetPWM;
}

void loop() {
  // Distance measurement using HC-SR04
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  // Visual distance indicators
  if (distance_cm > 15) {
    digitalWrite(GREEN_LED, HIGH);
    analogWrite(RED_LED, 0);
  } else {
    digitalWrite(GREEN_LED, LOW);
    int redIntensity = map(distance_cm, 15, 0, 0, 255);
    analogWrite(RED_LED, constrain(redIntensity, 0, 255));
  }

  // Audio obstacle alert
  if (distance_cm < 15) {
    int delayGap = 0;
    if (distance_cm > 10) delayGap = 300;
    else if (distance_cm > 7) delayGap = 150;
    else if (distance_cm > 4) delayGap = 80;
    else delayGap = 30;
    playBeep(delayGap);
  }

  // Bluetooth control commands
  while (Serial.available()) {
    char ch = Serial.read();
    ch = tolower(ch);
    if (ch == 'f') {
      forward(); Serial.println("Forward");
    } else if (ch == 'b') {
      backward(); Serial.println("Backward");
    } else if (ch == 's') {
      stopMotors(); Serial.println("Stop");
    } else if (ch == 'l') {
      turnLeft(); Serial.println("Turning Left (Tank Turn)");
    } else if (ch == 'r') {
      turnRight(); Serial.println("Turning Right (Tank Turn)");
    } else if (ch == 'y') {
      Serial.println("Horn Honk!");
      tone(BUZZER_PIN, 1000); delay(100);
      noTone(BUZZER_PIN); delay(100);
      tone(BUZZER_PIN, 1000); delay(150);
      noTone(BUZZER_PIN);
    }
    // Suction motor PWM speed stepping
    else if (ch == '1') {
      rampToSpeed(1300); Serial.println("BLDC Speed: LOW");
    } else if (ch == '2') {
      rampToSpeed(1350); Serial.println("BLDC Speed: MEDIUM");
    } else if (ch == '3') {
      rampToSpeed(1400); Serial.println("BLDC Speed: HIGH");
    } else if (ch == '4') {
      rampToSpeed(1500); Serial.println("BLDC Speed: FULL");
    } else if (ch == 'z') {
      rampToSpeed(900); Serial.println("BLDC STOPPED");
    }
  }
}
