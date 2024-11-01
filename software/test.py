import time

import can
from tinymovr.bus_router import init_router
from tinymovr.config import configure_logging, create_device, get_bus_config

from brush_can import BrushCan

# don't init if you already have in another program
params = get_bus_config(["canine", "slcan_disco"], bitrate=1000000)
init_router(can.Bus, params, configure_logging())

# tm = create_device(node_id=...)

# example begins here
brush1 = BrushCan(128)
brush2 = BrushCan(129)
brush3 = BrushCan(130)

# current is 0-100 and 40 seems like plenty
brush1.set_current_limit(40)
brush1.set_P(0.1)
brush1.set_direction(0)
brush1.set_position(0)

# in encoder ticks
brush1.set_target(10000)

while True:
    brush1.set_target(10000)
    time.sleep(0.001)
    #brush1.request_position()
    print(brush1.position)
