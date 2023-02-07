#include <Arduino.h>
#include <Stepper.h>

const byte sevenSegmtDisplay[11][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4 
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}, // 9
  {0,0,0,0,0,0,1}  // "-" : indicates transition}
};

const int displayPin1 = 2;
const int displayPin2 = 3;
const int displayPin3 = 4;
const int displayPin4 = 5;
const int displayPin5 = 6;
const int displayPin6 = 7;
const int displayPin7 = 8;

const int segments = 7;
const int transition = 10;

const int stepsPerRevolution = 65;
const int stepperPin1 = 10;
const int stepperPin2 = 11;
const int stepperPin3 = 12;
const int stepperPin4 = 13;
const int floorStep = 4000;
const int adjustStep = 200;

const int button1Pin = 30;
const int button2Pin = 31;

const int ledPin1 = 40;
const int ledPin2 = 41;


int button1State = 0;
int button2State = 0; 
int actualFloor = 1;
char command;

Stepper stepper(stepsPerRevolution, stepperPin1, stepperPin3, stepperPin2, stepperPin4);


void display(int num) {

    for (int j = 0, pin = displayPin1; j < segments; j++, pin++)
      digitalWrite(pin, sevenSegmtDisplay[num][j]);

}

void up()  {

  display(transition);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  stepper.step(floorStep);
  
  actualFloor++;
}

void down() {

  display(transition);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

  stepper.step(-floorStep);

  actualFloor--;
}

void setup() {

  Serial.begin(9600);
  Serial.println("Beginning...");

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  pinMode(displayPin1, OUTPUT);
  pinMode(displayPin2, OUTPUT);
  pinMode(displayPin3, OUTPUT);
  pinMode(displayPin4, OUTPUT);
  pinMode(displayPin5, OUTPUT);
  pinMode(displayPin6, OUTPUT);
  pinMode(displayPin7, OUTPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  stepper.setSpeed(500);
}



void loop() {

  display(actualFloor);

  
  if (actualFloor == 1)
    digitalWrite(ledPin1, HIGH);
  else
    digitalWrite(ledPin2, HIGH);


  if (Serial.available()) {
    command = Serial.read();

    switch (command)
    {
    case '+':
      Serial.println("Positive adjust");
      stepper.step(adjustStep);
      break;

    case '-':
      Serial.println("Negative adjust");
      stepper.step(-adjustStep);
      break;

    default:
      Serial.println("Invalid command!");
      break;
    }
  }


  button1State = digitalRead(button1Pin);

  if (button1State == HIGH) {
    if (actualFloor == 2)
      down();
  }

  button2State = digitalRead(button2Pin);
  
  if (button2State == HIGH) {
    if (actualFloor == 1)
      up();      
  }

}
