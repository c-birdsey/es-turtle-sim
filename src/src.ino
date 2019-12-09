/* TURTLE-SIMULATING DRAWING ROBOT

   CSCI435 Embedded Systems
   Middlebury College - Fall 2019
   Authors: Brandon Choe, Calder Birdsey

   Description:
   Usage:
*/

/*---------------------------------------------------------------------*/

/* includes */
#include <Stepper.h>
#include <ESP32Servo.h>
#include <Arduino.h>

/* prototypes */
void setup(void);
void loop(void);
void pen_up(void);
void pen_down(void);
void forward(float dist);
void backward(float dist);
int getSteps(float dist);
void left(int angle);
void right(int angle);
int translateAngle(float angle);

/* GLOBALS */
const int STEPS_PER_REV = 513;
const int RPM = 50;
const float STEP_ANGLE = 0.689;
const float WHEEL_CIRC = 18.787;
int lightSensorPin = A1;

TaskHandle_t CanvasTask;
TaskHandle_t DriveTask;

bool volatile OFF_CANVAS_FLAG = false;
int volatile CANVAS_READING;
int volatile ADJ; 

/* init stepper and sehow trvo instances */
Stepper L_wheel(STEPS_PER_REV, 14, 32, 15, 33);
Stepper R_wheel(STEPS_PER_REV, 27, 12, 13, 21);
Servo PEN_SERVO;
int SERVO_PIN = 26;

/* setup function */
void setup() {
  // set speeds for wheels
  L_wheel.setSpeed(RPM);
  R_wheel.setSpeed(RPM);

  //set servo params with standard 50 Hz
  PEN_SERVO.setPeriodHertz(50);
  PEN_SERVO.attach(SERVO_PIN, 1000, 2000);

  // initialize the serial port:
  Serial.begin(9600);

  //get initial canvas reading
  int vals[20];
  int valSum = 0;
  for (int i = 0; i < 20; i++) {
    vals[i] = analogRead(lightSensorPin);
    valSum += vals[i];
  }
  CANVAS_READING = (valSum / 20);
  ADJ = (CANVAS_READING / 3) * 2; 

  //config tasks 
  xTaskCreatePinnedToCore(checkCanvas, "CanvasTask", 10000, NULL, 1, &CanvasTask, 0); 
  delay(500); 
  xTaskCreatePinnedToCore(execInstruc, "DriveTask", 10000, NULL, 1, &DriveTask, 1); 
  delay(500); 
}

/* parent loop */
void loop() {
  pen_up();
  pen_down();
  forward(10);
  right(90);
  forward(10);
  right(90);
  forward(10);
  pen_up();
  forward(20);
  while(1){}
}

/*----------------------- HELPER FUNCTIONS ------------------------*/

void pen_up() {
  //;Serial.println("up");
  PEN_SERVO.write(80);
//  for (int pos = 175; pos >= 80; pos--) {
//    PEN_SERVO.write(pos);
//    delay(10);
//  }
  delay(1000);
}

void pen_down() {
  //Serial.println("down");
  PEN_SERVO.write(175);
//  for (int pos = 80; pos <= 175; pos++) {
//    PEN_SERVO.write(pos);
//    delay(10);
//  }
  delay(1000);
}

void backward(float dist) {
  int steps = getSteps(dist);
  for (steps; steps > 0; steps --) {
 
    float measure = analogRead(lightSensorPin) - CANVAS_READING; 
    Serial.print("measure: "); 
    
    Serial.println(measure); 
    if(measure < 1500) {
      L_wheel.step(-1);
      R_wheel.step(1);
      delay(10);
    } else {
      OFF_CANVAS_FLAG = false;
      while(1); 
    }
  }
}

void forward(float dist) {
  Serial.println("drive"); 
  int steps = getSteps(dist);
  for (steps; steps > 0; steps --) {
    float measure = analogRead(lightSensorPin); 
    Serial.print("measure: "); 
    Serial.println(measure); 
    
    if((measure - ADJ) > 100) {
      L_wheel.step(1);
      R_wheel.step(-1);
      delay(10);
    } else {
      OFF_CANVAS_FLAG = false;
      while(1); 
    }
  }
}

void right(int angle) {
  int steps = translateAngle(angle);
  for (steps; steps > 0; steps --) {
    L_wheel.step(-1);
    R_wheel.step(-1);
    delay(10);
  }
}

void left(int angle) {
  int steps = translateAngle(angle);
  for (steps; steps > 0; steps --) {
    L_wheel.step(1);
    R_wheel.step(1);
    delay(10);
  }
}

int getSteps(float dist) {
  // one rotation is 18.787cm (7.396in), each step is 0.037cm (0.0144in)
  float step_dist = WHEEL_CIRC / STEPS_PER_REV;
  int steps = dist / step_dist;
  return steps;
}

int translateAngle(float angle) {
//  Serial.print("ANGLE: ");
//  Serial.println(angle);
//  Serial.print("STEP PER DEGREE: ");
//  Serial.println(STEP_ANGLE);
  int steps = 2 * (angle / STEP_ANGLE);
//  Serial.print("STEPS: ");
//  Serial.println(steps);
  return steps;
}

//ISR(PCINT0_vect) {
//  //read new vals from light sensor
//  int vals[20];
//  int valSum = 0;
//  for(int i = 0; i < 20; i++) {
//     vals[i] = analogRead(lightSensorPin);
//     valSum += vals[i];
//  }
//  //get new average and compare to initial
//  int avg = (valSum / 20);
//  if((avg / CANVAS_READING) < 0.66) {
//    end();
//  }
//}
