import serial  # import Serial Library
import csv  # import csv library
import numpy
from datetime import datetime
import matplotlib.pyplot  # import matplotlib library
from drawnow import *


# Variable declations
temperature = []
elvh_pressure = []
lpdr_pressure = []

# datetime object containing current date and time
now = datetime.now()
dt_string = now.strftime("%d%m%Y_%H%M%S")

# Creating our serial object
arduinoData = serial.Serial("com3", 9600)

# Tell matplotlib you want interactive mode to plot live data
plt.ion()

# Printing serial configuration
if arduinoData.is_open == True:
    print(arduinoData, "\n")

fig, (ax1, ax2, ax3) = plt.subplots(3)
# Create a function that configures our desired plot

def make_fig():

    plt.subplot(311)
    plt.plot(temperature, "r", label="C")
    plt.ylabel("temperature")
    plt.ticklabel_format(useOffset=False)
    plt.tight_layout()

    plt.subplot(312)
    plt.plot(elvh_pressure, "b", label="mmHg")
    plt.ylabel("ELVH pressure")
    plt.ticklabel_format(useOffset=False)
    plt.tight_layout()

    plt.subplot(313)
    plt.plot(lpdr_pressure, "g", label="mmHg")
    plt.ylabel("LPDR pressure")
    plt.ticklabel_format(useOffset=False)
    plt.tight_layout()

def main():
    # saving to a csv file and launching the plot GUI
    with open('datalog_' + dt_string + '.csv', 'w',  encoding='UTF8', newline='') as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerow(["time","elvh_temperature", "elvh_pressure", "lpdr_pressure"])

        while True:  # While loop that loops forever
            while arduinoData.inWaiting() == 0:  # Wait here until there is data
                pass  # do nothing

            # read the line of text from the serial port
            arduinoString = arduinoData.readline()
            # decode the string received on the serial port
            decoded = arduinoString.strip().decode("utf-8")

            # getting the time
            now_update = datetime.now()
            timer = now_update.strftime("%H:%M:%S")

            print(timer +" "+ decoded)  # Printing the decoded data to the terminal (for debugging)

            # Split it into an array called dataArray
            dataArray = decoded.split(",")

            ELVH_Temperature = float(dataArray[0])
            ELVH_Pressure = float(dataArray[1])
            LPDR_Pressure = float(dataArray[2])

            if (ELVH_Pressure < 0):ELVH_Pressure = 0
            if (LPDR_Pressure < 0):LPDR_Pressure = 0

            # write to csv
            writer.writerow([timer, ELVH_Temperature, ELVH_Pressure, LPDR_Pressure])
            temperature.append(ELVH_Temperature)
            elvh_pressure.append(ELVH_Pressure)
            lpdr_pressure.append(LPDR_Pressure)

            drawnow(make_fig)  # Call drawnow to update our live graph
            # Pause Briefly. Important to keep drawnow from crashing
            plt.pause(0.000001)
            cnt = 0
            cnt =+ 1
            if cnt > 500:  # If you have 50 or more points, delete the first one from the array
                # This allows us to just see the last 50 data points
                temperature.pop(0)
                elvh_pressure.pop(0)
                lpdr_pressure.pop(0)

if __name__ == "__main__":
    main()
