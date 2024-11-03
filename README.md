# Mini-Parking-Lot-Space-Tracker
Project Description

The parking lot space tracker is a device that accounts for the number of spaces available in a parking lot by tracking the number of cars that enter and exit the lot. Two infrared sensors are used for detection of cars: one for the entrance and one for the exit. Since we only had a single infrared sensor available, a photoresistor was used in its place for demonstration purposes. A seven segment display is used as a visual representation for the number of open spaces available. Additionally, an SG90 servo is used to mimic a gate for the parking lot. Whenever the seven segment display shows any number greater than zero, the servo will be pointed upward, allowing cars to enter the lot. Once a zero is displayed, the servo will turn 90 degrees, closing off access to the parking lot as no more spaces are available. Additionally, the K64F will transfer a message saying “The Parking Lot is Full.” via SPI communication to the Arduino.

System Design

<img width="469" alt="System_Design" src="https://github.com/user-attachments/assets/47121625-4d4e-4b1c-b355-6835eaf7fd58">
