import time
import struct
import board
import busio
import digitalio

__version__ = '0.0.1'

I2C_ADDRESS = 0x0A
CHIP_ID = 0xBA11

REG_LED_RED = 0x00
REG_LED_GRN = 0x01
REG_LED_BLU = 0x02
REG_LED_WHT = 0x03

REG_LEFT = 0x04
REG_RIGHT = 0x05
REG_UP = 0x06
REG_DOWN = 0x07
REG_SWITCH = 0x08
MSK_SWITCH_STATE = 0b10000000

REG_INT = 0xF9
MSK_INT_TRIGGERED = 0b00000001
MSK_INT_OUT_EN = 0b00000010
REG_CHIP_ID_L = 0xFA

class TrackBall:
    def __init__(self, address=I2C_ADDRESS, interrupt_pin=None, timeout=5):
        self._i2c_address = address
        self._timeout = timeout

        # Initialize I2C using CircuitPython
        self._i2c = busio.I2C(board.SCL, board.SDA)

        # Initialize interrupt pin if provided
        if interrupt_pin:
            self._interrupt_pin = digitalio.DigitalInOut(interrupt_pin)
            self._interrupt_pin.direction = digitalio.Direction.INPUT
            self._interrupt_pin.pull = digitalio.Pull.UP
        else:
            self._interrupt_pin = None

        # Verify the chip ID
        chip_id = struct.unpack("<H", bytes(self.i2c_rdwr([REG_CHIP_ID_L], 2)))[0]
        if chip_id != CHIP_ID:
            raise RuntimeError(f"Invalid chip ID: 0x{chip_id:04X}, expected 0x{CHIP_ID:04X}")

        self.enable_interrupt()

    def i2c_rdwr(self, data, length=0):
        """Write and optionally read I2C data."""
        while not self._i2c.try_lock():
            pass
        try:
            self._i2c.writeto(self._i2c_address, bytes(data))
            if length > 0:
                time.sleep(0.02)  # Small delay before reading
                result = bytearray(length)
                self._i2c.readfrom_into(self._i2c_address, result)
                return list(result)
        finally:
            self._i2c.unlock()

        return []

    def enable_interrupt(self, interrupt=True):
        """Enable/disable GPIO interrupt pin."""
        value = self.i2c_rdwr([REG_INT], 1)[0]
        value = value & ~MSK_INT_OUT_EN
        if interrupt:
            value |= MSK_INT_OUT_EN
        self.i2c_rdwr([REG_INT, value])

    def get_interrupt(self):
        """Get the trackball interrupt status."""
        if self._interrupt_pin:
            return not self._interrupt_pin.value
        else:
            value = self.i2c_rdwr([REG_INT], 1)[0]
            return value & MSK_INT_TRIGGERED

    def set_rgbw(self, r, g, b, w):
        """Set all LED brightness as RGBW."""
        self.i2c_rdwr([REG_LED_RED, r, g, b, w])

    def read(self):
        """Read up, down, left, right and switch data from trackball."""
        left, right, up, down, switch = self.i2c_rdwr([REG_LEFT], 5)
        switch, switch_state = switch & ~MSK_SWITCH_STATE, (switch & MSK_SWITCH_STATE) > 0
        return up, down, left, right, switch, switch_state