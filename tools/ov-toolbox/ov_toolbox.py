# -*- coding: utf-8 -*-

import serial.tools.list_ports
import time
import sys

from ov_device import OvDevice
from ov_flightfile import save_flight

# USB VID/PID of OpenVario device
OV_USB_VID = 0x0483
OV_USB_PID = 0x5740

# Entry point
if __name__ == '__main__':

    exit_code = 1

    print("######################################")
    print("         OpenVario toolbox")
    print("######################################")
    print("")

    #  Wait for an available com port
    serial_port = None
    while not serial_port:
        # Listing serial ports
        serial_ports = serial.tools.list_ports.comports()
        for port in serial_ports:
            if (port.vid == OV_USB_VID) and (port.pid == OV_USB_PID):
                serial_port = port
                break
        if not serial_port:
            print(
                "No OpenVario device detected, please plug the USB cable and/or reset the device...")
            time.sleep(1)

    # Selected com port info
    print("OpenVario device found => {}".format(serial_port.device))

    # Open serial port
    print("Starting communication...")
    ser = serial.Serial(serial_port.device)
    if ser.is_open:

        #  Instanciate device and get device informations
        ov_device = OvDevice(ser)
        device_infos = ov_device.get_device_infos()
        if device_infos:
            print("")
            print("Device informations :")
            print(" - Name : {}".format(device_infos.name))
            print(" - HW version : {}".format(device_infos.hw_version))
            print(" - FW version : {}".format(device_infos.fw_version))

            print("")
            flights = ov_device.get_flight_list()
            if flights:
                print("Stored flights : ")
                flight_id = 0
                for flight in flights:
                    print(" {} - {} : {} bytes".format(flight_id,
                          flight[0], flight[1]))
                    flight_id += 1

                print("")
                flight_id = -1
                while flight_id < 0 or flight_id >= len(flights):
                    try:
                        flight_id = int(input("Select flight to retrieve : "))
                    except:
                        flight_id = -1
                flight_name = flights[flight_id][0]

                print("")
                print("Retrieving flight '{}'...".format(flight_name))
                flight = ov_device.read_flight(flight_name)
                if flight:
                    print("Saving flight '{}'...".format(flight_name))
                    if save_flight(flight_name, flight):
                        print("Done!")
                        exit_code = 0
                else:
                    print("Unable to retrieve flight")
            else:
                print("Unable to retrieve flight list")

        else:
            print("Unable to retrieve device informations")
    else:
        print("Unable to communicate with the device")

#    sys.exit(exit_code)
