# Sensor Actuator Systems

Now it's time to go one step further and connect what we learned about sensors with what we learned about actuators to create our first sensor-actuator system! 

![images](https://github.com/user-attachments/assets/5f7f1624-7aae-4e93-96e4-cb38c51478f2)

# Getting Started

In order to do so, you should first recreate the basic circuit for the HCSR-04 ultrasonic distance sensor as well as the basic circuit for the servo motor. You can find a summary of both in the respective folders on this repository.

Once you have both circuits set up again and you made sure that everything is connected the right way, we can combine both codes in order to make the sensor (HCSR-04) and the actuator (Servo) work at the same time.
<br><br>

## The Idea

With this simple program, I want to demostrate how a sensor and an actuator can interact with each other. The sensor values are directly translated into a reaction by the actuator. 

In this little setup, our servo motor will move further to one side (towards 180 degree) while we are moving an object closer to the sensor. If the sensor is not measuring any readable distance (nothing in front), the servo motor will remain at the start position (0 degree).

> sensor-actuator relationship: the closer we come, the more the motor turns away from the start position.

We will have to define a range of detection in which the servo motor should react. For example, I want it to react only between a distance of 30 cm (0 degree) and 0 cm (180 degree). Everything else should be ignored and result in the servo start position.

## The Code

We basically combine the code for servo and sensor. The only new things are the Arduino constrain(value, min, max) and map(value, from min, from max, to min, to max) functions.

We first define two constant variables in which we store the minimum and maximum cm of our desired range.

`#define MIN_CM 0` <br>
`#define MAX_CM 30` 

(You can choose your own range.)

Then in `loop()`, we tell the program to not go beyond our MAX_CM value, when saving the meaasured distance in our `distance` variable:

  `float constrainedDistance = constrain(distance, MIN_CM, MAX_CM);`

The `constrain()` function makes sure, that a value never exceeds the given min and max values.

With the `map()` function, we project the constrained value on the action range of our servo motor (0-180 degree). As we want it to be at 0 degree, when we are over or at 30 cm, and at 180 degree when we are at 0 cm, we have to flip 0 and 180.

The conversion goes like: <br>
map( our_value, from minimum, from maximum, to minimum, to maximum)

  `float mappedDistance = map(constrainedDistance, MIN_CM, MAX_CM, 180, 0);`


## Links

constrain() function <br>
https://docs.arduino.cc/language-reference/en/functions/math/constrain/

map() function<br>
https://docs.arduino.cc/language-reference/en/functions/math/map/

