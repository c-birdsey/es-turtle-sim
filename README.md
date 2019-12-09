# Turtle Simulating Robot 
### Brandon Choe, Calder Birdsey - December 2019 
## Introduction 
### Goal
Our main goal for this project was to leverage the software and hardware concepts we learned over the course of this semester to create physical simulation of the turtle library in python. We also wanted to demonstrate a physical representation of the flow from high level commands in our code, to the individual peripherals and components, to a cohesive robot that you could watch perform your inputs.   

### Motivation 
The motivation for this project stemmed from our introduction to the computer science major at Middlebury. In both of our intro classes, we used the turtle library in python to practice recursions. Now, as we are wrapping up our penultimate semester at Middlebury, we wanted to return to our humble roots as freshmen and recreate one of the most frustrating parts of our first year, but while tying together everything we learned from this past semester.    

## Methods 
### Devices and Peripherals 
### Physical Design
For this project, we opted to model and 3D print the majority of the physical robot frame. This allowed for a strong, reproducible design that was efficient and easily tweakable. The frame is fairly simple, with the breadboard and battery balancing the weight on either side. We decided to use two traditional wheels in conjunction with the caster ball bearing, as this allowed for precise turning while drawing. If was quite important for the robot to be able to turn on itself exactly, as it would severely impact the drawing if not. This also informed the location of the pen, which sits directly between the two wheels in a 20mm slot. The servo fits snugly in between the battery pack and pen slot, and like the battery pack and breadboard does not need adhesives of any sort to stay connected to the board. The entire robot frame was printed on a MakerBot Replicator 3D Printer, and the .stl files are included in the repository for this project.   

### Code Logic 

### User Interface

### Bill of Materials 
| Item            |Supplier       | Quantity | Unit Cost  | Total Cost |
|:--------------- |:------------- |:-------- |:---------- | ----------:|
| 20mm Metal Caster Bearing Wheel | Adafruit | 1  | $1.95  | $1.95 |
| 5V Stepper Motor | Adafruit | 2 | $4.95 | $9.90 |
| Skinny Wheel for DC Motor | Adafruit | 2 | $2.50 | $5.00 |
| Micro servo | Adafruit | 1 | $5.95 | $5.95 |
| 4xAA Battery Holder w/switch | Adafruit | 1 | $2.95 | $2.95 |
| Half Sized Breadboard | Sparkfun | 1 | $4.95 | $4.95 |
| 8-Channel Darlington Driver | Adafruit | 1 | $1.95 | $1.95 |
| Photo Cell - CdS Photoresistor | Adafruit | 1 | $0.95 | $0.95 |
| Diffused Red 5mm LED | Adafruit | 1 | $4.00 (25-pack) | $4.00 |
|||||**$37.60**|


## Results 
- Video

## Schedule 
- Gantt Chart 

## Issues 
- *Wiring the Darlington Driver* -
- *Throttling 6V Battery Pack* -
- *Angle Accuracy* -

## Future Work 
- *User Interface* -
- *Live Instructions* - 
- *Preset Letter Functions and Drawings* -
- *Calibration* -
- *Curves and Circles* -

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

### Notes 
