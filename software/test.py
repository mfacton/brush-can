import time

import can
from brush_can import BrushCan
from tinymovr.bus_router import init_router
from tinymovr.config import configure_logging, create_device, get_bus_config

# don't init if you already have in another program
params = get_bus_config(["canine", "slcan_disco"], bitrate=1000000)
init_router(can.Bus, params, configure_logging())

# tm = create_device(node_id=...)

# example begins here
brush1 = BrushCan(69)
brush2 = BrushCan(70)
brush3 = BrushCan(71)

brush1.send_command(0, 20)
brush1.send_command(2, 0.1)

brush1.send_command(1, 10000)

while True:
    brush1.request_position()
    time.sleep(0.1)
    print(brush1.position)
