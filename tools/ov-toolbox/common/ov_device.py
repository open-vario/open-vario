# -*- coding: utf-8 -*-

import serial
import struct
from .ov_requests import *
from .ov_protocol import OvProtocol
from .ov_flight import OvDateTime, OvFlight, OvFlightEntry


class OvDevice(object):
    """ Helper class to communicate with an OpenVario device """

    def __init__(self, serial_port: serial.Serial) -> None:
        """ Constructor """

        # Protocol to communicate with the device
        self.__protocol = OvProtocol(serial_port)

    def get_device_infos(self) -> OvDeviceInfos:
        ''' Get device informations '''

        device_infos = None

        # Send request
        response = self.__protocol.send_request(OV_REQ_ID_DEVICE_INFO)
        if response:
            try:
                # Decode response
                i = 0
                device_infos = OvDeviceInfos()
                device_infos.name, i = self.__read_string(response, i)
                device_infos.hw_version, i = self.__read_string(response, i)
                device_infos.fw_version, i = self.__read_string(response, i)
            except:
                device_infos = None

        return device_infos

    def get_flight_list(self) -> [(str, int)]:
        ''' Get the list of the recorded flights '''

        flight_list = None

        # Send request
        response = self.__protocol.send_request(OV_REQ_ID_LIST_FLIGHTS)
        if response:
            try:
                # Decode response
                i = 0
                accepted, i = self.__read_bool(response, i)
                if accepted:
                    # Read list of flights
                    has_more_data = True
                    flight_list = []
                    while has_more_data:

                        # Ask for next item
                        response = self.__protocol.send_request(
                            OV_REQ_ID_LIST_FLIGHTS_DATA)
                        if response:
                            i = 0
                            has_more_data, i = self.__read_bool(response, i)
                            if has_more_data:
                                flight, i = self.__read_string(response, i)
                                size, i = self.__read_uint(response, 4, i)
                                flight_list.append((flight, size))
                        else:
                            has_more_data = False
            except:
                flight_list = None

        return flight_list

    def read_flight(self, name: str) -> OvFlight:
        ''' Read a recorded flight '''

        flight = None

        #  Prepare request
        request = bytearray()
        request.extend(self.__write_string(name))

        # Send request
        response = self.__protocol.send_request(OV_REQ_ID_READ_FLIGHT, request)
        if response:
            try:
                # Decode response
                i = 0
                accepted, i = self.__read_bool(response, i)
                if accepted:
                    # Read header
                    flight = OvFlight()
                    flight.header.timestamp, i = self.__read_datetime(
                        response, i)
                    flight.header.glider, i = self.__read_string(response, i)
                    flight.header.period, i = self.__read_uint(response, 2, i)

                    # Read entries
                    has_more_data = True
                    while has_more_data:

                        # Ask for next item
                        response = self.__protocol.send_request(
                            OV_REQ_ID_READ_FLIGHT_DATA)
                        if response:
                            i = 0
                            has_more_data, i = self.__read_bool(response, i)
                            if has_more_data:
                                entries_found = True
                                while entries_found:
                                    try:
                                        entry, i = self.__read_flight_entry(
                                            response, i)
                                        flight.entries.append(entry)
                                    except:
                                        entries_found = False
                        else:
                            has_more_data = False
            except:
                flight = None

        return flight

    def __read_bool(self, frame: bytearray, index: int) -> (bool, int):
        if frame[index] == 0:
            bool = False
        else:
            bool = True
        index += 1

        return bool, index

    def __read_int(self, frame: bytearray, size: int, index: int) -> (int, int):
        if size == 1:
            format = 'b'
        elif size == 2:
            format = 'h'
        elif size == 4:
            format = 'i'
        else:
            format = 'q'

        int_data = frame[index:(index + size)]
        int = struct.unpack(format, int_data)[0]
        index += size

        return int, index

    def __read_uint(self, frame: bytearray, size: int, index: int) -> (int, int):

        if size == 1:
            format = 'b'
        elif size == 2:
            format = 'H'
        elif size == 4:
            format = 'I'
        else:
            format = 'Q'

        uint_data = frame[index:(index + size)]
        uint = struct.unpack(format, uint_data)[0]
        index += size

        return uint, index

    def __read_float(self, frame: bytearray, index: int) -> (float, int):
        float = 0.

        float_data = frame[index:(index + 4)]
        float = struct.unpack('f', float_data)[0]
        index += 4

        return float, index

    def __read_double(self, frame: bytearray, index: int) -> (float, int):
        double = 0.

        double_data = frame[index:(index + 8)]
        double = struct.unpack('d', double_data)[0]
        index += 8

        return double, index

    def __read_string(self, frame: bytearray, index: int) -> (str, int):
        string = ""
        string_size = frame[index]
        index += 1

        if string_size > 0:
            string_data = frame[index:(index + string_size)]
            string = bytearray.decode(string_data)
            index += string_size

        return string, index

    def __read_datetime(self, frame: bytearray, index: int) -> (OvDateTime, int):
        datetime = OvDateTime()

        datetime.year, index = self.__read_uint(frame, 1, index)
        datetime.month, index = self.__read_uint(frame, 1, index)
        datetime.day, index = self.__read_uint(frame, 1, index)
        datetime.hour, index = self.__read_uint(frame, 1, index)
        datetime.minute, index = self.__read_uint(frame, 1, index)
        datetime.second, index = self.__read_uint(frame, 1, index)
        datetime.millis, index = self.__read_uint(frame, 2, index)

        return datetime, index

    def __read_flight_entry(self, frame: bytearray, index: int) -> (OvFlightEntry, int):
        entry = OvFlightEntry()

        entry.gnss.is_valid, index = self.__read_bool(frame, index)
        entry.gnss.latitude, index = self.__read_double(frame, index)
        entry.gnss.longitude, index = self.__read_double(frame, index)
        entry.gnss.altitude, index = self.__read_uint(frame, 4, index)
        entry.gnss.speed, index = self.__read_uint(frame, 4, index)

        entry.altimeter.is_valid, index = self.__read_bool(frame, index)
        entry.altimeter.altitude, index = self.__read_int(frame, 4, index)
        entry.altimeter.pressure, index = self.__read_int(frame, 4, index)
        entry.altimeter.temperature, index = self.__read_int(frame, 2, index)

        entry.accelerometer.is_valid, index = self.__read_bool(frame, index)
        entry.accelerometer.acceleration, index = self.__read_int(
            frame, 2, index)

        return entry, index

    def __write_int(self, int: int, size: int) -> bytearray:
        int_array = bytearray()

        for i in range(size):
            int_array.append((int >> (8 * i)) & 0xFF)

        return int_array

    def __write_string(self, string: str) -> bytearray:
        string_array = bytearray()
        string_array.append(len(string))
        string_array.extend(string.encode())

        return string_array
