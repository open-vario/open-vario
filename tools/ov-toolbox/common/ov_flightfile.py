# -*- coding: utf-8 -*-

import os
import configparser

from .ov_flight import OvFlight, OvFlightEntry


def save_flight(name: str, flight: OvFlight) -> bool:
    ''' Save a flight into the Open Vario format '''

    # Get home directory
    home_dir = os.path.expanduser("~")

    # Create file
    filepath = os.path.join(home_dir, name)
    try:
        flight_file = open(filepath, "w")

        # Write header
        flight_file.write("[header]\n")
        flight_file.write("Date={}\n".format(str(flight.header.timestamp)))
        flight_file.write("Glider={}\n".format(flight.header.glider))
        flight_file.write("Period={}\n".format(flight.header.period))

        # Write flight data
        flight_file.write("[data]\n")
        entry_id = 0
        for flight_data in flight.entries:
            flight_file.write("{}={},{},{},{},{},{},{},{},{},{},{},{},{}\n".format(entry_id,
                                                                                   flight_data.gnss.is_valid,
                                                                                   flight_data.gnss.latitude,
                                                                                   flight_data.gnss.longitude,
                                                                                   flight_data.gnss.speed,
                                                                                   flight_data.gnss.altitude,
                                                                                   flight_data.altimeter.is_valid,
                                                                                   flight_data.altimeter.pressure,
                                                                                   flight_data.altimeter.altitude,
                                                                                   flight_data.altimeter.temperature,
                                                                                   flight_data.accelerometer.is_valid,
                                                                                   flight_data.accelerometer.acceleration,
                                                                                   flight_data.computed.sink_rate,
                                                                                   flight_data.computed.glide_ratio))
            entry_id += 1

        # Close file

        ret = True

    except IOError as ex:
        print("Unable to write to file '{}' => {}".format(filepath, str(ex)))
        ret = False

    return ret


def load_flight(filename: str) -> OvFlight:
    ''' Load a flight from the Open Vario format '''

    flight = None

    # Open file
    flight_file = configparser.ConfigParser()
    if not (len(flight_file.read(filename)) == 0):

        # Read header
        is_header_valid = False
        try:
            date = flight_file.get("header", "Date")
            glider = flight_file.get("header", "Glider")
            period = flight_file.getint("header", "Period")
            is_header_valid = True
        except configparser.NoOptionError as e:
            print("Missing '{}' value in header".format(e.option))
        if is_header_valid:

            # Instanciate flight
            flight = OvFlight()
            flight.header.timestamp = date
            flight.header.glider = glider
            flight.header.period = period

            # Read values
            for i in range(len(flight_file["data"])):
                value = flight_file.get("data", "{}".format(i))
                values = value.split(",")
                if len(values) == 13:

                    entry = OvFlightEntry()

                    # Extract GNSS data
                    entry.gnss.is_valid = eval(values[0])
                    entry.gnss.latitude = float(values[1])
                    entry.gnss.longitude = float(values[2])
                    entry.gnss.speed = int(values[3])
                    entry.gnss.altitude = int(values[4])

                    # Extract altimeter data
                    entry.altimeter.is_valid = eval(values[5])
                    entry.altimeter.altitude = int(values[6])
                    entry.altimeter.pressure = int(values[7])
                    entry.altimeter.temperature = int(values[8])

                    # Extract accelerometer data
                    entry.accelerometer.is_valid = eval(values[9])
                    entry.accelerometer.acceleration = int(values[10])

                    # Extract computed data
                    entry.computed.sink_rate = int(values[11])
                    entry.computed.glide_ratio = int(values[12])

                    flight.entries.append(entry)

                else:
                    print("Invalid value '{}' => {}".format(i, value))

    else:
        print("Unable to open input file : {}".format(filename))

    return flight
