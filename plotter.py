import serial  # import Serial Library
import csv
import numpy
from datetime import datetime
import matplotlib.pyplot as plt  # import matplotlib library
from drawnow import *


# Variable declations
Raw = []
Pressure = []
cnt = 0

# datetime object containing current date and time
now = datetime.now()
# dd/mm/YY H:M:S
dt_string = now.strftime("%d%m%Y_%H%M%S")

arduinoData = serial.Serial("com3", 9600)  # Creating our serial object

plt.ion()  # Tell matplotlib you want interactive mode to plot live data

if arduinoData.is_open == True:
    print(arduinoData, "\n")  # Printing serial configuration


def makeFig():  # Create a function that makes our desired plot
    plt.ylim(-10, 10)  # Set y min and max values
    plt.title("My Live Streaming Sensor Data")  # Plot the title
    plt.grid(True)  # Turn the grid on
    plt.ylabel("Pressure [mmHg]")  # Set ylabels
    plt.plot(Pressure, "ro-", label="mmHg")  # plot the pressure
    plt.legend(loc="upper left")  # plot the legend

    plt2 = plt.twinx()  # Create a second y axis
    # plt2.ylim(-10, 10)  # Set limits of second y axis- adjust to readings you are getting
    plt2.plot(Raw, "b^-", label="ELVH")  # plot pressure data
    plt2.set_ylabel("ELVH")  # label second y axis
    plt2.ticklabel_format(useOffset=False)  # Force matplotlib to NOT autoscale y axis
    plt2.legend(loc="upper right")  # plot the legend

    # saving to a csv file
with open ('datalog_' + dt_string + '.csv', 'w') as file:
    writer = csv.writer(file, delimiter=",")
    writer.writerows(["elvh","lpdr"])

    while True:  # While loop that loops forever
        while arduinoData.inWaiting() == 0:  # Wait here until there is data
            pass  # do nothing

        arduinoString = arduinoData.readline()  # read the line of text from the serial port
        decoded = arduinoString.decode("utf-8")
        dataArray = decoded.split(",")  # Split it into an array called dataArray
        P = float(dataArray[0])  # Convert first element to float number and put in pressure
        R = float(dataArray[1])  # Convert second element to float number and put in P
        writer.writerow([dataArray[0], dataArray[1]]) # write to csv
        Pressure.append(P)  # Build our Pressure array by appending pressure readings
        Raw.append(R)  # Building our pressure array by appending P readings
        drawnow(makeFig)  # Call drawnow to update our live graph
        plt.pause(0.000001)  # Pause Briefly. Important to keep drawnow from crashing
        cnt = cnt + 1
        if cnt > 50:  # If you have 50 or more points, delete the first one from the array
            Pressure.pop(0)  # This allows us to just see the last 50 data points
            Raw.pop(0)
