# -*- coding: utf-8 -*-


# OpenVario maintenance requests IDs
OV_REQ_ID_DEVICE_INFO = 0x01
OV_REQ_ID_LIST_FLIGHTS = 0x02
OV_REQ_ID_LIST_FLIGHTS_DATA = 0x03
OV_REQ_ID_READ_FLIGHT = 0x04
OV_REQ_ID_READ_FLIGHT_DATA = 0x05


class OvDeviceInfos:
    """ Device informations """

    def __init__(self) -> None:
        """ Constructor """

        #  Device name
        self.name = ""
        # Hardware version
        self.hw_version = ""
        # Firmware version
        self.fw_version = ""
