const int SW_PIN = 7;

// LEFT MOTOR

const int PWM1 = 10;
const int M1P1 = 5; //Motor 1 Pin 1; Input Pin 2 of L293D
const int M1P2 = 4; // Motor 1 Pin 2; Input Pin 7 of L293D

// RIGHT MOTOR
const int PWM2 = 9;
const int M2P1 = 3; // Motor 2 Pin 1; Input Pin 15 of L293D, controls output pin 14 of L293D
const int M2P2 = 2; // Motor 2 Pin 2; Input Pin 10 of L293D, controls output pin 11 of L293D

// PHOTORESISTORS
const int LEFT_LIGHT_PIN = A1;
const int RIGHT_LIGHT_PIN = A0;

// Constants
const int SEARCH_DRIVE_TIME = 100;
const int TURN_DRIVE_TIME = 200;    // Time to turn in a direction
const int MOVE_DRIVE_TIME = 300;   // Time to drive in a direction
const int STOP_DRIVE_TIME = 200;    // Time to stop after moving
const int NUM_LIGHT_LEVELS = 2;


void setup()
{// Set the switch pin as an input_pullup, this holds the state until the next push
  pinMode(SW_PIN, INPUT_PULLUP);
  // Set the motor pins as outputs ...
  pinMode(PWM1, OUTPUT);
  pinMode(M1P1, OUTPUT);
  pinMode(M1P2, OUTPUT);

  pinMode(PWM2, OUTPUT);
  pinMode(M2P1, OUTPUT);
  pinMode(M2P2, OUTPUT);
 
 
  // ... then set the sensors as inputs
  pinMode(LEFT_LIGHT_PIN, OUTPUT);
  pinMode(RIGHT_LIGHT_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Feed me photons!");

}

void loop() {

  // Store light levels as array [left, center, right]
  int light_levels[NUM_LIGHT_LEVELS];

  // If switch is flipped, search for light
  if(digitalRead(SW_PIN) == LOW){
    // Record light value to the left
    light_levels[0] = analogRead(LEFT_LIGHT_PIN);

    // Record light value to the right
    light_levels[1] = analogRead(RIGHT_LIGHT_PIN);


    // Find direction of max light
    int max_light = 0;
    int max_light_index = 0;
    for ( int i = 0; i < NUM_LIGHT_LEVELS; i++ ) {
      if ( light_levels[i] > max_light ) {
        max_light = light_levels[i];
        max_light_index = i;
      }
    }

    // Move in the direction of max light
    if ( max_light_index == 0 ) {
      drive(-100, 255);
      delay(TURN_DRIVE_TIME);
      drive(255, 255);
      delay(MOVE_DRIVE_TIME);
      drive(0, 0);
      delay(STOP_DRIVE_TIME);
    } else if ( max_light_index == 1 ) {
      drive(255, 255);
      delay(MOVE_DRIVE_TIME);
      drive(0, 0);
      delay(STOP_DRIVE_TIME);
    } else {
      drive(255, -100);
      delay(TURN_DRIVE_TIME);
      drive(255, 255);
      delay(MOVE_DRIVE_TIME);
      drive(0, 0);
      delay(STOP_DRIVE_TIME);
    }

  // If switch is not flipped, do nothing
  } else {
    drive(0, 0);
  }
}

void rightMotor(int motorSpeed)                      
{

  // If speed is positive, run the motor forward
  if (motorSpeed > 0) {
    digitalWrite(M1P1, HIGH);
    digitalWrite(M1P2, LOW);

  // If it's negative, run the motor backward
  } else if (motorSpeed < 0) {
    digitalWrite(M1P1, LOW);
    digitalWrite(M1P2, HIGH);

  // If it's 0, brake the motor
  } else {
    digitalWrite(M1P1, LOW);
    digitalWrite(M1P2, LOW);
  }
  analogWrite(PWM1, abs(motorSpeed));
}

void leftMotor(int motorSpeed)
{
  // If speed is positive, run the motor forward
  if (motorSpeed > 0) {
    digitalWrite(M2P1, HIGH);
    digitalWrite(M2P2, LOW);

  // If it's negative, run the motor backward
  } else if (motorSpeed < 0) {
    digitalWrite(M2P1, LOW);
    digitalWrite(M2P2, HIGH);

  // If it's 0, brake the motor
  } else {
    digitalWrite(M2P1, LOW);
    digitalWrite(M2P2, LOW);
  }
  analogWrite(PWM2, abs(motorSpeed));
}



void drive(int left_speed, int right_speed) {
  leftMotor(left_speed);
  rightMotor(right_speed);
}
