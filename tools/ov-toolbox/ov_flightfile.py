# -*- coding: utf-8 -*-

import os
from ov_requests import OvFlight


def save_flight(name: str, flight: OvFlight) -> bool:

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
            flight_file.write("{}={},{},{},{},{},{},{},{},{},{}\n".format(entry_id,
                                                                          flight_data.gnss.is_valid,
                                                                          flight_data.gnss.latitude,
                                                                          flight_data.gnss.longitude,
                                                                          flight_data.gnss.speed,
                                                                          flight_data.gnss.altitude,
                                                                          flight_data.gnss.track_angle,
                                                                          flight_data.altimeter.is_valid,
                                                                          flight_data.altimeter.pressure,
                                                                          flight_data.altimeter.altitude,
                                                                          flight_data.altimeter.temperature))
            entry_id += 1

        # Close file

        ret = True

    except IOError as ex:
        print("Unable to write to file '{}' => {}".format(filepath, str(ex)))
        ret = False

    return ret
