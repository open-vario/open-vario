# -*- coding: utf-8 -*-

class OvDateTime:
    """ Date time information """

    def __init__(self) -> None:
        """ Constructor """

        # Year (since 2000)
        self.year = 0
        # Month (1 - 12)
        self.month = 0
        # Day (1 - 31)
        self.day = 0
        # Hour (0 - 24)
        self.hour = 0
        # Minute (0 - 59)
        self.minute = 0
        # Second (0 - 59)
        self.second = 0
        # Milliseconds (0 - 999)
        self.millis = 0

    def __str__(self) -> str:
        return "{:04}-{:02}-{:02}T{:02}-{:02}-{:02}.{:03}".format(self.year + 2000, self.month, self.day, self.hour, self.minute, self.second, self.millis)


class OvFlightHeader:
    """ Recorded flight header """

    def __init__(self) -> None:
        """ Constructor """

        # Timestamp
        self.timestamp = OvDateTime()
        # Glider
        self.glider = ""
        # Entry period in milliseconds
        self.period = 0


class OvFlightGnssData:
    """ Recorded flight GNSS data """

    def __init__(self) -> None:
        """ Constructor """

        # Indicate if the data is valid
        self.is_valid = False
        # Latitude (1 = 1°)
        self.latitude = 0.0
        # Longitude (1 = 1°)
        self.longitude = 0.0
        #  Speed (1 = 0.1 m/s)
        self.speed = 0
        # Altitude (1 = 0.1 m)
        self.altitude = 0
        # Track angle (1 = 0.1°)
        self.track_angle = 0


class OvFlightAltimeterData:
    """ Recorded flight altimeter data """

    def __init__(self) -> None:
        """ Constructor """

        # Indicate if the data is valid
        self.is_valid = False
        # Pressure (1 = 0.01mbar)
        self.pressure = 0
        # Altitude (1 = 0.1m) */
        self.altitude = 0
        # Temperature (1 = 0.1°C) */
        self.temperature = 0


class OvFlightAccelerometerData:
    """ Recorded flight accelerometer data """

    def __init__(self) -> None:
        """ Constructor """

        # Indicate if the data is valid
        self.is_valid = False
        # Total acceleration (1000 = 1g)
        self.acceleration = 0


class OvFlightComputedData:
    """ Recorded flight computed data """

    def __init__(self) -> None:
        """ Constructor """

        # Sink rate (1 = 0.1m/s)
        self.sink_rate = 0
        # Glide ratio (1 = 0.1)
        self.glide_ratio = 0


class OvFlightEntry:
    """ Recorded flight entry """

    def __init__(self) -> None:
        """ Constructor """

        # GNSS data
        self.gnss = OvFlightGnssData()
        # Altimeter data
        self.altimeter = OvFlightAltimeterData()
        # Accelerometer data
        self.accelerometer = OvFlightAccelerometerData()
        # Computed data
        self.computed = OvFlightComputedData()


class OvFlight:
    """ Recorded flight """

    def __init__(self) -> None:
        """ Constructor """

        # Header
        self.header = OvFlightHeader()
        # Entries
        self.entries = []
