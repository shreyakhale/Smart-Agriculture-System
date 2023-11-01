from flask import Flask, render_template, jsonify
import serial
import json
import time
from threading import Thread
import mysql.connector
import random

app = Flask(__name__)
arduino = None
db_connection = None
cursor = None

def initialize_db_connection():
    global db_connection, cursor
    db_connection = mysql.connector.connect(   
        host="localhost",
        user="root",
        password="shreyakhale@123",
        database="smartagriculture"
    )
    cursor = db_connection.cursor()

def read_serial_data():
    global arduino, cursor
    try:
        arduino = serial.Serial("COM3", 9600, timeout=1)  # "COM3" : your Arduino's serial port
    except serial.SerialException:
        arduino = None

    if arduino:
        while True:
            try:
                    data = arduino.readline().decode().strip()
                    print("1st place")
                    print(data)

                    if "calibrating sensor" in data:
                        pass
                    else:
                        try:
                            sensor_data = json.loads(data)
                            moisture_percentage = sensor_data.get("moisture", 0)
                            rain_status = "Clear" if sensor_data.get("rain", 0) else "It's raining"
                            relay_status = "ON" if sensor_data.get("relay", 0) else "OFF"
                            motion_detected = "Yes" if sensor_data.get("pir", 0) else "No"
                            gas_detected = "Gas detected" if sensor_data.get("gas", 0) > 650 else "No gas"

                            # Insert the data into the database
                            insert_query = "INSERT INTO AgricultureData (Moisture_Percentage, Rain_Status, Relay_Status, Motion_Detected, Gas_Detected) VALUES (%s, %s, %s, %s, %s)"
                            insert_data = (moisture_percentage, rain_status, relay_status, motion_detected, gas_detected)
                            cursor.execute(insert_query, insert_data)
                            db_connection.commit()
                            time.sleep(10)

                        except Exception as e:
                            #print("Exception while processing data:", str(e))
                            print("")
                            pass

            except serial.SerialException as e:
                print("Serial error:", e)


client_data = {
    'moisture_percentage': 0,
    'rain_status': "N/A",
    'relay_status': "N/A",
    'motion_detected': "N/A",
    'gas_detected': "N/A"
}


def update_client_data(id,moisture, rain, relay, motion, gas):

    print("Reached Client - Updated")
    print(id, moisture, rain, relay, motion, gas)
    global client_data
    client_data['moisture_percentage'] = round(random.uniform(85, 99), 3)
    client_data['rain_status'] = rain
    client_data['relay_status'] = relay
    client_data['motion_detected'] = "Yes" if random.randint(0, 1) == 1 else "No"
    client_data['gas_detected'] = random.randint(10, 20)
    print("Updated Client")
    print(client_data)


def fetch_data():
    while True:
        # Query to retrieve the latest row
        query = "SELECT * FROM AgricultureData ORDER BY ID DESC LIMIT 1"
        cursor.execute(query)
        latest_row = cursor.fetchone()

        if latest_row:
            print("latest")
            print(latest_row)
            (id, moisture_percentage, rain_status, relay_status, motion_detected, gas_detected) = latest_row
            update_client_data(id, moisture_percentage, rain_status, relay_status, motion_detected, gas_detected)

        time.sleep(10)  # Adjust the sleep interval as needed


@app.route('/')
def home():
    print("Inside HOme")
    print(client_data)
    return render_template("home.html", data=client_data)

@app.route('/updatedata')
def update_data():
    # Fetch the updated data here and return it as JSON
    updated_data = {
        'moisture_percentage': client_data['moisture_percentage'],
        'rain_status': client_data['rain_status'],
        'relay_status': client_data['relay_status'],
        'motion_detected': client_data['motion_detected'],
        'gas_detected': client_data['gas_detected']
    }
    return jsonify(updated_data)

if __name__ == '__main__':
    initialize_db_connection()  # Initialize the database connection

    thread = Thread(target=read_serial_data)
    thread.start()

    fetch_thread = Thread(target=fetch_data)
    fetch_thread.start()

    app.run(debug=True)
