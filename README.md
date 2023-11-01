# Smart Agriculture System

## Abstract

Smart Agriculture System is an innovative project that leverages IoT sensors, Arduino, and a WEB APPLICATION to monitor and automate various aspects of farming. By combining cutting-edge technology and agriculture, this project aims to improve crop yield and overall farm management.

## Features

- **Smart Irrigation**: The system monitors soil moisture and atmospheric conditions. It activates the irrigation system when the soil is dry, and there is no rain, optimizing water usage.

- **Environmental Monitoring**: It includes motion detection using a PIR sensor and air quality monitoring with an MQ2 gas sensor.

- **Web Application**: A user-friendly web application is provided for remote monitoring and control, offering mobility and convenience to farmers and agricultural professionals.

- **Data Logging**: Sensor data is stored in a MySQL database, enabling historical analysis and decision-making.
  

## Software Components

- **Arduino IDE**: The core control system is programmed and executed on an Arduino Nano. The code continuously collects sensor data and controls irrigation.

- **Flask Web Application**: A Flask-based web application provides a user-friendly interface for real-time sensor data display. It retrieves data from the Arduino and displays it for remote monitoring.

- **MySQL Database**: Sensor data is stored in a MySQL database for historical records and analysis.
  
## Understanding the Flask Application

The Flask application plays a crucial role in this project, serving as the user interface for real-time sensor data display and interaction with the Arduino-based system. Here are the key components and functionalities of the Flask application:

- **Web Interface**: The Flask application provides a user-friendly web interface where users can access real-time data from the sensors and control the irrigation system remotely.

- **Database Integration**: The application connects to a MySQL database to store and retrieve sensor data. This database is crucial for maintaining historical records and enabling data analysis.

- **Real-time Data Display**: Users can view real-time data related to soil moisture, rain status, relay status, motion detection, and gas levels on the web interface.

- **Data Synchronization**: The application continuously updates sensor data from the Arduino and stores it in the database for future reference.

- **JSON API**: The Flask application exposes a JSON API endpoint `/updatedata`, which allows external applications to fetch the latest sensor data in JSON format.

## APP INTERFACE

![image](https://github.com/shreyakhale/Smart-Agriculture-System/assets/86523832/616b307d-9a22-4830-b01f-ed0e47b8ead4)

## Algorithm and Process Design

![image](https://github.com/shreyakhale/Smart-Agriculture-System/assets/86523832/ab24606e-37fa-43b3-a62f-a9c88264018b)

The heart of the project is an Arduino-based system equipped with various sensors, including a moisture sensor, PIR sensor, MQ2 gas sensor, and a rain sensor. The system continuously monitors the soil's moisture level and atmospheric conditions. Here's a brief overview of the system's operations:

1. **Moisture Sensor**: Constantly measures soil moisture to detect dry soil conditions.

2. **Rain Sensor**: Detects a clear atmosphere, indicating the absence of rain.

3. **PIR Sensor**: Detects motion in the field, useful for security and monitoring.

4. **MQ2 Gas Sensor**: Monitors gases in the farm environment, providing insights into air quality and safety.

5. **Smart Irrigation**: When dry soil conditions and a clear atmosphere are confirmed, the system autonomously activates a motor for field irrigation.

## Hardware Requirements

1. Relay Module
2. Proximity (PIR) Sensor
3. Wires
4. 9V Battery
5. Raindrop Sensor
6. Soil Moisture Sensor
7. Arduino Nano
8. Water Pump
9. Breadboard

## Getting Started

1. **Clone the Repository**: Clone this GitHub repository to your local machine.

2. **Arduino Setup**: Connect the Arduino Nano to your system and upload the code through the Arduino IDE.

3. **Web Application**: Set up the Flask web application and configure it to connect with the Arduino for data retrieval.

4. **Database Configuration**: Create a MySQL database and configure the Flask application to store and retrieve sensor data.

5. **Hardware Setup**: Assemble the hardware components as per the project's requirements.

6. **Run the System**: Power up the Arduino, run the Flask application, and start monitoring and controlling your agricultural environment.

