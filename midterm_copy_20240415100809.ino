// Libraries
#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Define pin numbers for sensors
const int ultrasonicTriggerPin = 9;  // Ultrasonic sensor trigger pin
const int ultrasonicEchoPin = 10;    // Ultrasonic sensor echo pin
const int inductiveSensorPin = 7;     // Inductive proximity sensor pin
const int ldrSensorPin = A0;          // LDR sensor pin

// Define thresholds for sensor readings
const int ultrasonicThreshold = 15;   // Threshold for ultrasonic sensor (in cm)
const int ldrThreshold = 600;         // Threshold for LDR sensor

// Define constants for trash types
const int METAL_TRASH = 1;
const int PLASTIC_TRASH = 2;
const int PAPER_TRASH = 3;
const int GLASS_TRASH = 4;
const int UNKNOWN_TRASH = 0;

// Initialize MLX90614 sensor for temperature
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Function to read ultrasonic sensor
float readUltrasonicSensor() {
  digitalWrite(ultrasonicTriggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTriggerPin, LOW);
  return pulseIn(ultrasonicEchoPin, HIGH) / 58.0; // Convert pulse duration to distance (cm)
}

// Function to read inductive proximity sensor
bool readInductiveSensor() {
  return digitalRead(inductiveSensorPin);
}

// Function to read LDR sensor
int readLDRSensor() {
  return analogRead(ldrSensorPin);
}

// Function to determine trash type
int determineTrashType() {
  float distance = readUltrasonicSensor();
  bool isMetal = readInductiveSensor();
  int lightIntensity = readLDRSensor();
  float temperature = mlx.readObjectTempC();

  if (distance <= ultrasonicThreshold) {
    if (isMetal) {
      return METAL_TRASH;
    } else {
      if (lightIntensity < ldrThreshold) {
        return PAPER_TRASH;
      } else {
        return PLASTIC_TRASH;
      }
    }
  } else {
    return GLASS_TRASH;
  }
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize ultrasonic sensor pins
  pinMode(ultrasonicTriggerPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  // Initialize inductive proximity sensor pin
  pinMode(inductiveSensorPin, INPUT);

  // Initialize MLX90614 sensor
  mlx.begin();
}

void loop() {
  int trashType = determineTrashType();

  // Print trash type for debugging
  switch (trashType) {
    case METAL_TRASH:
      Serial.println("Metal Trash");
      break;
    case PLASTIC_TRASH:
      Serial.println("Plastic Trash");
      break;
    case PAPER_TRASH:
      Serial.println("Paper Trash");
      break;
    case GLASS_TRASH:
      Serial.println("Glass Trash");
      break;
    default:
      Serial.println("Unknown Trash");
      break;
  }

  delay(1000); // Delay for stability
}
