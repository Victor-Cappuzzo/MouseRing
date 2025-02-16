import time
import board
import digitalio
from digitalio import DigitalInOut, Direction

# Set up LED's

# Red LED
red_led = DigitalInOut(board.LED_RED)
red_led.direction = Direction.OUTPUT
red_led.value = True # Set LED to OFF

# Green LED
green_led = DigitalInOut(board.LED_GREEN)
green_led.direction = Direction.OUTPUT
green_led.value = True # Set LED to OFF

# Blue LED
blue_led = DigitalInOut(board.LED_BLUE)
blue_led.direction = Direction.OUTPUT
blue_led.value = True # Set LED to OFF

sleep_time = 1

# Cycle through LED colors
while True:
    # Set LED to red
    red_led.value = False
    time.sleep(sleep_time)
    red_led.value = True
    
    # Set LED to green
    green_led.value = False
    time.sleep(sleep_time)
    green_led.value = True
    
    # Set LED to blue
    blue_led.value = False
    time.sleep(sleep_time)
    blue_led.value = True
    
    
    
    
