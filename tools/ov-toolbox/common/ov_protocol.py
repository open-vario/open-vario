# -*- coding: utf-8 -*-

import serial

# Start of frame
OV_MAINT_SOF_1 = 0x0D
OV_MAINT_SOF_2 = 0xF0
OV_MAINT_SOF_3 = 0xAD
OV_MAINT_SOF_4 = 0x8B
OV_MAINT_SOF = [OV_MAINT_SOF_1, OV_MAINT_SOF_2, OV_MAINT_SOF_3, OV_MAINT_SOF_4]

# Header of a frame size in bytes (SOF + ID + LEN)
OV_FRAME_HEADER_SIZE = 4 + 1 + 2


class OvProtocol(object):
    """ Implement OpenVario maintenance protocol """

    def __init__(self, serial_port: serial.Serial) -> None:
        """ Constructor """

        # Serial port to communicate with the device
        self.__serial_port = serial_port
        self.__serial_port.reset_input_buffer()

        # Configure default timeout
        self.__serial_port.timeout = 2
        self.__serial_port.write_timeout = 2
        self.__serial_port.inter_byte_timeout = None

        # Length of the currently receiving frame
        self.__rx_frame_length = 0
        # Currently receiving frame
        self.__rx_frame = []
        # CRC of the currently receiving frame
        self.__rx_frame_crc = 0

    def send_request(self, req_id: int, payload=bytearray(0)) -> bytearray:
        """ Send a request to the device and wait for its response """

        resp = None

        # Send request
        if self.__send(req_id, payload):
            # Wait for response
            resp = self.__wait_response(req_id)

        return resp

    def __send(self, req_id: int, payload=bytearray(0)) -> bool:
        """ Send a request to the device """

        # Discard previously received data
        self.__serial_port.reset_input_buffer()

        # Prepare frame :
        # Start of frame => 4 bytes
        # Id             => 1 byte
        # Size           => 2 bytes (LE)
        # Payload        => 0 to 1000 bytes
        # Checksum       => 2 bytes (LE)

        frame = bytearray(9 + len(payload))
        frame[0] = OV_MAINT_SOF_1
        frame[1] = OV_MAINT_SOF_2
        frame[2] = OV_MAINT_SOF_3
        frame[3] = OV_MAINT_SOF_4
        frame[4] = req_id
        frame[5] = len(payload) & 0xFF
        frame[6] = (len(payload) >> 8) & 0xFF
        index = 7
        for i in range(len(payload)):
            frame[index] = payload[i]
            index += 1
        crc = self.__compute_crc(frame)
        frame[index] = crc & 0xFF
        frame[index + 1] = (crc >> 8) & 0xFF

        # Send frame
        ret = True
        try:
            written = self.__serial_port.write(frame)
            if not (written == len(frame)):
                print("Unable to send full request => 0x{:02x}".format(req_id))
                ret = False
        except serial.SerialTimeoutException:
            print("Unable to send request => 0x{:02x}".format(req_id))
            ret = False

        return True

    def __wait_response(self, req_id: int) -> bytearray:
        """ Wait for the response from the device for a specific request """

        resp = None

        # State machine
        states = {0: self.__wait_sof, 1: self.__wait_sof, 2: self.__wait_sof,
                  3: self.__wait_sof, 4: self.__wait_req_id, 5: self.__wait_len,
                  6: self.__wait_len, 7: self.__wait_payload, 8: self.__wait_crc,
                  9: self.__wait_crc}

        # Wait loop
        end = False
        state = 0
        while not end:
            # Reset state
            if state == 0:
                self.__rx_frame_length = 0
                self.__rx_frame = []
                self.__rx_frame_crc = 0

            #  Read a byte from the device
            try:
                byte = self.__serial_port.read(1)
                if len(byte) == 0:
                    print(
                        "Unable to receive response for request => 0x{:02x}".format(req_id))
                    end = True
                else:
                    # Store received byte
                    self.__rx_frame.append(byte[0])
            except serial.SerialTimeoutException:
                print(
                    "Unable to receive response for request => 0x{:02x}".format(req_id))
                end = True
            if not end:
                # Handle reception state
                ret = states[state](state, req_id, byte[0])
                if ret is None:
                    # State was not successfull, reset state
                    state = 0
                elif ret:
                    # State was successfull, go to next state
                    state += 1
                else:
                    # State is not finished yet
                    pass

                # Check end of frame
                if state == len(states):
                    resp = bytearray(
                        self.__rx_frame[OV_FRAME_HEADER_SIZE:(OV_FRAME_HEADER_SIZE + self.__rx_frame_length)])
                    end = True

        return resp

    def __wait_sof(self, state: int, req_id: int, byte: int) -> bool:
        """ Check the reception of the start of frame char """
        return (byte == OV_MAINT_SOF[state])

    def __wait_req_id(self, state: int, req_id: int, byte: int) -> bool:
        """ Check the reception of the request id """
        return (byte == req_id)

    def __wait_len(self, state: int, req_id: int, byte: int) -> bool:
        """ Check the reception of the length of the frame """
        ret = True
        if state == 5:
            self.__rx_frame_length = byte
        else:
            self.__rx_frame_length += (byte << 8)
            if (self.__rx_frame_length == 0) or (self.__rx_frame_length > 1000):
                print("Invalid received frame length => {}".format(
                    self.__rx_frame_length))
                ret = None
        return ret

    def __wait_payload(self, state: int, req_id: int, byte: int) -> bool:
        """ Check the reception of the payload """
        ret = False
        if len(self.__rx_frame) == (self.__rx_frame_length + OV_FRAME_HEADER_SIZE):
            ret = True
        return ret

    def __wait_crc(self, state: int, req_id: int, byte: int) -> bool:
        """ Check the reception of the CRC of the frame """
        ret = True
        if state == 8:
            self.__rx_frame_crc = byte
        else:
            self.__rx_frame_crc += (byte << 8)

            # Check CRC
            crc = self.__compute_crc(self.__rx_frame) & 0x0000FFFF
            if not (crc == self.__rx_frame_crc):
                print("Invalid received frame CRC : {}, expected CRC {}".format(crc,
                                                                                self.__rx_frame_crc))
                ret = None
        return ret

    def __compute_crc(self, frame: bytearray) -> int:
        """ Compute the CRC of a request """
        crc = 0
        for i in range(len(frame) - 2):
            crc = crc + frame[i]
            crc = crc << 1
        return crc
