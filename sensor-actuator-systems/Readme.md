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



