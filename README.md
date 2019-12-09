# Turtle Simulating Robot
### Brandon Choe, Calder Birdsey - December 2019 
## Introduction 
### Goal
Our main goal for this project was to leverage the software and hardware concepts we learned over the course of this semester to create physical simulation of the [Turtle graphics library](https://docs.python.org/3.3/library/turtle.html?highlight=turtle)  in Python. We also wanted to demonstrate a physical representation of the flow from high level commands in our code, to the individual peripherals and components, to a cohesive robot that you could watch perform your inputs in the real world. This connection between hardware and software was a key part of the course, and something we really wanted to explore in our final project.    

### Motivation 
The motivation for this project stemmed from our introduction to the computer science major at Middlebury. In both of our intro classes (CSCI101 and CSCI150), we used the Turtle library in Python to practice recursions. Now, as we are wrapping up our penultimate semester at Middlebury, we wanted to return to our humble roots as freshmen and recreate one of the most frustrating parts of our first year, but while tying together everything we learned from this past semester and implementing the relatively simply graphics library in far more complex and challenging manner.   

## Methods 
### Devices and Peripherals   

**Adafruit HUZZAH32 – ESP32 Feather Board**     
This was the microcontroller we used for this project. We initially planned on using the dual cores for multithreading (in order to implement the light sensor) and this informed our decision to use the HUZZAH32. Although our final iteration of the code did not utilize multithreading, the ESP32 Feather was still a great choice because of how simple it was to incorporate it into the breadboard, and the substantial number of GPIO pins.   

**5V Stepper Motor**   
The 5V Stepper Motors are arguably the most important peripherals we used to create our robot, as they offered very precise control over the distance we wanted our robot to travel or turn. The specific motors we used have 513 steps per revolution. Combined with our wheels that had a circumference of 18.78 centimeters, each step was 0.037 centimeters, giving our robot a significant level of precision, which leads to precise drawings.   

**8-Channel Darlington Driver**    
The 8-Channel Darlington Driver was a necessity for our project, because without it, the stepper motors would not have been able to draw enough power to drive our robot. The driver allows the motors to still receive instructions from the ESP32 microcontroller, but supplies the motors with a seperate voltage that is suitable for unipolar stepper motors.       
 
**20mm Metal Caster Bearing Wheel**   
This bearing wheel was essential for giving the robot the ability to reliably change its angle without changing the location of the pen. This feature is obviously very important when designing a robot for drawings, as a typical drawing has dozens of angles, and even a small error would compound into a very visible error if the angles are not precise.    

**Micro Servo**   
We used this peripheral to control the pen_up() and pen_down() commands. The servo simply lifts a small metal hook attached to the pen, lifting the pen off or lowering it onto the canvas. This function is also very important to a drawing robot, as most drawings will not be one continuous line, but will have discontinuities. This feature allows for that possibility.      

**Diffused RED 5mm LED**    
This LED is one of two parts needed for the feature of our robot that checks if the robot is going to drive off the canvas. The LED, using a small paper cone,shines a light underneath the body of the robot, about three centimeters from the front and supplies a continuous and consistent light source for our photoresistor to interact with.   

**Photoresistor**  
The photoresistor reads how much light from the LED is bouncing back from the surface of the canvas onto the photoresistor. Thus, if the robot is moving off the white canvas onto a darker surface, such as a wooden desk or carpeted floor, less light will be measured by the sensor. If the drop in measurement is significant enough (approximately 33% drop), the robot immediately stops its forward movement and waits for a hard reset.   

<img src="https://c-birdsey.github.io/es-turtle-sim/img/led.png">

### Physical Design
For this project, we opted to model and 3D print the majority of the physical robot frame. This allowed for a strong, reproducible design that was efficient and easily tweakable. The frame is fairly simple, with the breadboard and battery balancing the weight on either side. We decided to use two traditional wheels in conjunction with the caster ball bearing, as this allowed for precise turning while drawing. If was quite important for the robot to be able to turn on itself exactly, as it would severely impact the drawing if not. This also informed the location of the pen, which sits directly between the two wheels in a 20mm slot. The servo fits snugly in between the battery pack and pen slot, and like the battery pack and breadboard does not need adhesives of any sort to stay connected to the board. The entire robot frame was printed on a MakerBot Replicator 3D Printer, and the .stl files are included in the repository for this project.   

### Logic and Code Structure
Essentially, our robot relies on a small set of basic commands to execute its drawings. We set out to replicate three simple tasks within the Turtle functionality - movement forward and backward, turning a set number of degrees, and pen position (either up or down). We then broke that into six main functions: forward(dist), backward(dist), right(angle), left(angle), pen_up(), pen_down(). These are what the user can interact with and call to create their drawing. Turning the robot utilizes a helper function to translate the angle given, in degrees, to the number of steps for each wheel, and movement forward and back uses a similar function that translates distance to steps. Both use simple algorithms based on the wheel circumference and steps per rotation of our stepper motors.    

We are also relying on Arduino’s Stepper.h library to create stepper instances, as well as ESP32Servo.h to control and initialize the micro servo (see code snippets below). These two libraries take care of much of the low level initialization, and we could simply interact with the abstracted C++/Arduino syntax. This is all done within the setup() function in Arduino.    

```cpp
// init stepper instances  
Stepper L_wheel(STEPS_PER_REV, 14, 32, 15, 33);     
Stepper R_wheel(STEPS_PER_REV, 27, 12, 13, 21);
``` 

```cpp
//set servo params with standard 50 Hz  
PEN_SERVO.setPeriodHertz(50);  
PEN_SERVO.attach(SERVO_PIN, 1000, 2000);
```

This is also where we are taking an initial reading with the photoresistor to establish a baseline for the canvas color. Within the forward function, we calculate a new canvas reading and compare it to the global baseline - this is how we are implementing the off-canvas detection functionality.   

### User Interface
With this project, the addition of a user interface was initially a stretch goal. As we expected, we weren’t able to develop a UI within our timeline so our “interface” is essentially the Arduino interface. Behavior of the robot can be altered via hard coded instructions in the parent for loop within the Arduino source file and the highest level of abstraction available to a user is simply the Turtle-esque functions we’ve developed. In its current state, our robot executes its drawing directions and then enters into an infinite while loop until powered off, or a new sketch is uploaded (i.e. the ESP32 HUZZAH is reset). As mentioned in the Future Work section, the development of a user interface would be the primary goal if moving forward with this project.   

### Bill of Materials

| Item            |Supplier       | Quantity | Unit Cost  | Total Cost |
|:--------------- |:------------- |:-------- |:---------- | ----------:|
| HUZZAH32 – ESP32 Feather Board | Adafruit | 1 | $21.95 | $21.95 |
| 20mm Metal Caster Bearing Wheel | Adafruit | 1  | $1.95  | $1.95 |
| 5V Stepper Motor | Adafruit | 2 | $4.95 | $9.90 |
| Skinny Wheel for DC Motor | Adafruit | 2 | $2.50 | $5.00 |
| Micro servo | Adafruit | 1 | $5.95 | $5.95 |
| 4xAA Battery Holder w/switch | Adafruit | 1 | $2.95 | $2.95 |
| Half Sized Breadboard | Sparkfun | 1 | $4.95 | $4.95 |
| 8-Channel Darlington Driver | Adafruit | 1 | $1.95 | $1.95 |
| Photo Cell - CdS Photoresistor | Adafruit | 1 | $0.95 | $0.95 |
| Diffused Red 5mm LED | Adafruit | 1 | $4.00 (25-pack) | $4.00 |
|||||**$59.55**|


## Results   
The end result of our project was very similar to what we envisioned when we were initially ideating what we wanted to create. The two major changes from our earliest designs was a switch from four omnidirectional wheels to two regular wheels and a ball bearing wheel, and the inclusion of the photoresistor and LED.    

This first video shows the precise turns the robot is able to make, the consistency between lines that are meant to be the same length, and the pen_up and pen_down commands. Because this video is from an iteration of the robot without the photoresistor, it runs off the canvas. You can also see the jittery movement of the micro servo, as we were initially having trouble correctly configuring it via the ESP32Servo.h library.     

[<img src="https://c-birdsey.github.io/es-turtle-sim/img/draw-img.jpg">](https://drive.google.com/file/d/1qrff_zdK6xWhZOJm2RL2-cdg02QwqyaU/view?usp=sharing)

The second video highlights the addition of the photoresistor and LED. In the first run of the robot, it stops once the photoresistor comes off the canvas. Once reset and repositioned, it continues to run, makes a turn, and stops again when going of the canvas. You can also see that we fixed the servo’s jittery behavior.   

[<img src="https://c-birdsey.github.io/es-turtle-sim/img/photoresistor-img.jpg">](https://drive.google.com/file/d/1iHSxn1WZR-QDcf7Hkx0Y760qUZxlPzAe/view?usp=sharing)

## Schedule   
<img src="https://c-birdsey.github.io/es-turtle-sim/img/schedule.png">

For the most part, we stayed on track with our original schedule. We originally weren’t planning on including the photoresistor and LED to track the robots position, but we were able to get ahead of schedule to create time for the photoresistor and LED. The reason we were able to squeeze more goals into the given timeline of the project is that we overestimated how long it would take to implement basic movement and pen control. These were phases that we thought would be more time consuming and hands-on, and for the most part we were able to assemble the robot smoothly.   

## Issues 
**Wiring the Darlington Driver**  
Initially, we struggled significantly with the wiring of the Darlington Driver and correctly driving the two stepper motors. Because we opted for a more hands-on build (hardwiring the driver rather than using a pre built motor driver breakout board), there was limited documentation online or in datasheets about the correct wiring from our microcontroller output pins to the driver. Eventually, we worked through this process by digging into the inner mechanics of the particular stepper motors we used, and significant trial and error.    

**Throttling 6V Battery Pack**   
This wasn’t a major issue during the project, but it was an aspect of assembly we didn’t fully consider when designing the process. We are using our 6V battery pack to power our microcontroller, LED, photoresistor, two stepper motors, and the micro servo. These peripherals have different ranges of voltage, and so we had to make sure that we were providing correct and safe amounts of power to each.    

**Angle Accuracy**  
Like many of the other projects we saw, we were limited to a certain level of accuracy based on the quality of the hardware we were able to work with, as well as by the fact that this build was indeed a prototype and not assembled with optimum precision. While our stepper motors are fairly accurate for their price, we were not able to get consistent and precise turns from our robot. The turning was also impacted back the surface on which the robot was running on. If on a table or desk with a smooth surface, we often overshoot the programmed turn, and when on carpet or a surface with greater resistance, we were often several degrees below the expected angle.     

## Future Work 
**User Interface and Live Instructions**   
The user interface of the robot, or lack thereof, is what we consider to be the biggest shortcoming of the project. As it is now, to change the instructions for the robot, the user must go into the source code and change the directions in the loop. While we have some layers of abstraction so users don’t have to understand how to use the different libraries we rely on, it is still not very user friendly and assumes a certain level of competence and access to certain resources. It would be far more ideal if we were able to set up a site hosted by the ESP32,  and allow users to provide live instructions to the robot over WiFi. This could be built out to any degree, perhaps even including a Python script and simulation using the Turtle graphics library to see what image you were programming the robot to draw.    

**Preset Letter Functions and Drawings**   
We also think it would be very useful to offer users a preset library of functions for specific drawings or shapes, and perhaps even letters. Figuring out how to draw characters can often be a painful trial and error process, so providing preset function for the letters of the alphabet, and numbers zero through nine could make drawing and writing far easier. It would expand the usability of the robot, and lead to a far better user experience.    

**Calibration**    
A big drawback of the stepper motors we chose for this project is that they can become uncalibrated over time. This means that the distances that users want the robot to move, or the angles they request, will slowly become less and less accurate. Recalibrating the motor can also be a very slow trial and error process, so we think it would be worthwhile to also incorporate an automated process for recalibrating the motors. This would be in the form of some specific, designed drawing pattern that can illuminate the shortcomings of the physical build and provide quicker insight into what needs to be tweaked to regain precision and accuracy.    

**Curves and Circles**    
We chose not to incorporate circles and curves into this iteration of the project because we figured it would be too time consuming to code, as well as outside the range of our (relatively) inexpensive hardware. Technically, it wouldn’t be too difficult of a process (having the two wheels rotate at different speeds) but it would be rather unpredictable and would certainly compound any errors present in the hardware already. That being said, we do acknowledge that circles and curves are important parts of drawing, so we do believe they should be incorporated into future work.     

## References 
- https://www.vivaxsolutions.com/web/python-turtle.aspx
- https://randomnerdtutorials.com/esp32-pir-motion-sensor-interrupts-timers/
- https://pimylifeup.com/arduino-light-sensor/
- https://wolfpaulus.com/micro-python-esp32/
- https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf
- https://www.arduino.cc/reference/en/#functions 
- https://github.com/espressif/arduino-esp32 
- https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/system/intr_alloc.html
- https://create.arduino.cc/projecthub/reanimationxp/how-to-multithread-an-arduino-protothreading-tutorial-dd2c37
- https://learn.adafruit.com/adafruit-arduino-lesson-16-stepper-motors/breadboard-layout
- https://www.arduino.cc/en/Reference/Stepper
- https://www.arduinolibraries.info/libraries/esp32-servo
- https://docs.python.org/3.3/library/turtle.html?highlight=turtle
