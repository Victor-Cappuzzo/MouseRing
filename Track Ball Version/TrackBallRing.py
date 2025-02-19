import time
import os
import math
import board
from trackball import TrackBall
import busio
import digitalio as dio
from adafruit_ble import BLERadio
from adafruit_ble.services.standard.hid import HIDService
from adafruit_hid.mouse import Mouse
from adafruit_ble.advertising.standard import ProvideServicesAdvertisement


# Button 1 setup
BUTTON_1_PIN = board.D10
button1 = dio.DigitalInOut(BUTTON_1_PIN)
button1.direction = dio.Direction.INPUT
button1.pull = dio.Pull.DOWN

# Button 2 setup
BUTTON_2_PIN = board.D0
button2 = dio.DigitalInOut(BUTTON_2_PIN)
button2.direction = dio.Direction.INPUT
button2.pull = dio.Pull.DOWN

# Switch setup


# Trackball setup
INTERRUPT_PIN = board.D3
trackball = TrackBall(interrupt_pin=INTERRUPT_PIN)

# Bluetooth setup
ble = BLERadio()
hid = HIDService()
ble.name = "TrackballMouse"
advertisement = ProvideServicesAdvertisement(hid)
mouse = Mouse(hid.devices)
ble.start_advertising(advertisement) # Start advertising

connected = False

sensitivity = 50

#def clear_trackball():
#    trackball.set_rgbw(0, 0, 0, 0)

while True:
    
    # If bluetooth is connected
    while ble.connected:
        
        # Print bluetooth connected message
        if not connected:
            connected = True
            print("Bluetooth connected!")
            
        # Read in trackball inputs
        up, down, left, right, switch, state = trackball.read()
        
        # Translate trackball movements into mouse movements
        if up:
            mouse.move(y=-sensitivity)
        if down:
            mouse.move(y=sensitivity)
        if left:
            mouse.move(x=-sensitivity)
        if right:
            mouse.move(x=sensitivity)
            
        # Translate trackball click to LMB click
        if state: # Trackball button pressed
            mouse.press(Mouse.LEFT_BUTTON)
        else: # Trackball button released
            mouse.release_all()
        
        # Detect button presses (pin goes HIGH)
        if button1.value: # True if button is pressed
            print("Button #1 Pressed!")
            trackball.set_rgbw(0, 255, 0, 0)
            time.sleep(0.2) # Debounce delay to avoid repeated detections
        
        elif button2.value:
            print("Button #2 Pressed!")
            trackball.set_rgbw(0, 0, 255, 0)
            time.sleep(0.2)
            
        else:
            trackball.set_rgbw(255, 0, 0, 0)
            
        time.sleep(0.01) # For smoother movement
            
    # If bluetooth is not connected
    while not ble.connected:
        
        connected = False
        print("Waiting for Bluetooth connection...")
        time.sleep(5)
        
        
