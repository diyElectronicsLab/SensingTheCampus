# HCSR-04 Ultrasonic Distance Sensor

## What is the HC-SR04?

The HC-SR04 is a low-cost ultrasonic distance sensor commonly used in robotics, automation, and IoT projects. It measures distance by emitting ultrasonic sound waves (inaudible to humans) and timing how long it takes for the echo to bounce back from an object.

## What it can do

- Measure distance from about 2 cm to 4 meters.
- Resolution is in the millimeter range.

Useful for:

- Obstacle detection (robots, drones).
- Level measurement (water tanks, bins).
- Simple range-finding projects
- detecting presence of moving objects in space

## How it works (basic principle)

1) The ESP32/Arduino sends a short HIGH pulse (10 µs) to the sensor’s TRIG pin = The sensor emits an ultrasonic burst at 40 kHz.

2) If the burst hits an object, the sound reflects back = The sensor outputs a HIGH signal on the ECHO pin for the duration of the sound’s round trip.

![Hcsr-04 schema](https://sites.duke.edu/memscapstone/files/2022/12/HC-SR04-Ultrasonic-Sensor-Working-Echo-reflected-from-Obstacle.gif)

3) By measuring the pulse width, the microcontroller can calculate the distance:

Distance (cm) = Pulse Duration (µs) * speed of sound (0.034) / 2

(Because sound travels about 343 m/s at room temperature.)

## Typical Wiring

- VCC → 5V
- GND → Ground
- TRIG → Digital output pin from ESP32/Arduino
- ECHO → Digital input pin (⚠️ using ESP32, we will insert a voltage divider – because ECHO outputs 5V and ESP pins are only 3.3V tolerant)

⚠️ Voltage divider:

- Resistor 10k (or 680 ohm) between Echo pin and ESP32 input pin
- Resistor 20k (or 1k) between ESP32 input pin and GND


!! USE TRIG PIN 22
!! USE ECHO PIN 23
!! Resistor between GND and echo pin 23 = 100 k = yellow stripe
!! Resistor between Echo (sensor) and echo Pin 23 = 10 k = orange stripe

![HCSR-04 voltage divider](https://docs.toit.io/static/hw_ff_ultra-divider-6d61f84710362eacf369e0098c53701f.png)

## Links
- https://docs.toit.io/tutorials/hardware/ultra
- https://randomnerdtutorials.com/esp32-hc-sr04-ultrasonic-arduino/