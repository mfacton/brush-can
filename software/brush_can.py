import struct

import can
from tinymovr.bus_router import get_router


class BrushCan:
    def __init__(self, can_id):
        get_router().add_client(self.filter, self.handle)
        self.can_id = can_id

        self.request_fulfilled = False
        self.position = 0

    def filter(self, frame):
        # frame.is_remote_frame
        if (frame.arbitration_id == self.can_id):
            self.handle(frame)

    def handle(self, frame):
        self.position = struct.unpack("=i", frame.data)[0]
        self.request_fulfilled = True

    def request_position(self):
        get_router().send(can.Message(
            arbitration_id=self.can_id,
            is_extended_id=False,
            is_remote_frame=True,
        ))
        self.request_fulfilled = False

    def send_command(self, command, value):
        get_router().send(can.Message(
            arbitration_id=self.can_id,
            is_extended_id=False,
            is_remote_frame=False,
            data=struct.pack("=Bf", command, value),
        ))

    def set_current_limit(self, value):
        self.send_command(0, value)
    
    def disable(self):
        self.send_command(0, 0)

    def set_target(self, value):
        self.send_command(1, value)

    def set_P(self, value):
        self.send_command(2, value)

    def set_I(self, value):
        self.send_command(3, value)

    def set_D(self, value):
        self.send_command(4, value)

    def set_position(self, value):
        self.send_command(5, value)

    def set_direction(self, value):
        self.send_command(6, value)
