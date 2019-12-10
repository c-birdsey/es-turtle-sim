/* TURTLE-SIMULATING DRAWING ROBOT

   CSCI435 Embedded Systems
   Middlebury College - Fall 2019
   Authors: Brandon Choe, Calder Birdsey

   Description: Drawing robot simulating Turtle graphics
   library in Python. See full documentation and report at 
   https://c-birdsey.github.io/es-turtle-sim/
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
void left(int angle);
void right(int angle);
int getSteps(float dist);
int translateAngle(float angle);

/* GLOBALS */
const int STEPS_PER_REV = 513;    //steps per rotation
const int RPM = 50;               //stepper RPMs
const float STEP_ANGLE = 0.689;   //degree per step - technically 0.7 (360 / 513) but tweaked from testing
const float WHEEL_CIRC = 18.787;  //wheel circumference - 18.787cm (7.396in)

/* globals for light sensor reading */
int volatile CANVAS_READING;
int volatile ADJUSTED_READING; 

/* init stepper and servo instances */
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

  // initialize the serial port for debugging 
  Serial.begin(9600);

  //get initial canvas reading
  int vals[20];
  int valSum = 0;
  for (int i = 0; i < 20; i++) {
    vals[i] = analogRead(lightSensorPin);
    valSum += vals[i];
  }
  /* Store initial canvas reading in CANVAS_READING variable and 
   * store adjusted benchmark in ADJUSTED_READING - this is 66% 
   * less than the initial, a threshold determined via trial and 
   * error testing. Only detects movement from light to dark 
   * surfaces (assuming white or near-white background for drawing)
   */
  CANVAS_READING = (valSum / 20);
  ADJUSTED_READING = (CANVAS_READING / 3) * 2; 

 
}

/* parent loop */
void loop() {
  /* basic directions for a 10cm square */
  pen_down();
  forward(10);
  right(90);
  forward(10);
  right(90);
  forward(10);
  right(90);
  forward(10);
  pen_up();
  
  /* entering infinite while after all directions executed */
  while(1){}
}

/*----------------------- HELPER FUNCTIONS ------------------------*/

/* pen_up() - function to move pen off paper via servo control*/
void pen_up() {
  PEN_SERVO.write(80);
  delay(1000);
}

/* pen_down() - function to move pen onto paper via servo control */
void pen_down() {
  PEN_SERVO.write(175);
  delay(1000);
}

/* backward(dist) - function to move robot backward, input distance in cm */
void backward(float dist) {
  int steps = getSteps(dist);
  /* move both wheels a single step at a time */
  for (steps; steps > 0; steps --) {
    L_wheel.step(-1);
    R_wheel.step(1);
    delay(10);
  }
}

/* forward(dist) - function to move robot forward, input distance in cm */
void forward(float dist) {
  int steps = getSteps(dist);
  /* move both wheels a single step at a time */
  for (steps; steps > 0; steps --) {
    /* get new reading from light sensor */
    float measure = analogRead(lightSensorPin); 

    /* compare measurement to adjusted reading var ADJUSTED_READING */
    if((measure - ADJUSTED_READING) > 100) {
      L_wheel.step(1);
      R_wheel.step(-1);
      delay(10);
    } else {
      /* if detected canvas change, enter infinite while loop and wait for restart */
      while(1); 
    }
  }
}

/* right(angle) - function to turn robot to the right, input angle in degrees */
void right(int angle) {
  /* get translation of angle to steps */
  int steps = translateAngle(angle);
  for (steps; steps > 0; steps --) {
    L_wheel.step(-1);
    R_wheel.step(-1);
    delay(10);
  }
}

/* left(angle) - function to turn robot to the left, input angle in degrees */
void left(int angle) {
  /* get translation of angle to steps */
  int steps = translateAngle(angle);
  for (steps; steps > 0; steps --) {
    L_wheel.step(1);
    R_wheel.step(1);
    delay(10);
  }
}

/* getSteps(dist) - function to convert distance to steps using the measured 
 * circumference of wheels and steps per revolution of steppers, returns 
 * step count as integer
 */
int getSteps(float dist) {
  float step_dist = WHEEL_CIRC / STEPS_PER_REV;
  int steps = dist / step_dist;
  return steps;
}

/* translateAngle(angle) - function to convert angle to steps using global
 * STEP_ANGLE which stores steps per degree, returns step count as integer
 */
int translateAngle(float angle) {
  int steps = angle / STEP_ANGLE;
  return steps;
}
