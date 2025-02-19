import time
import os
import math
import board
from trackball import TrackBall
import busio


print("Trackball mouse started...")

trackball = TrackBall(interrupt_pin=board.D3)

def clear_trackball():
    trackball.set_rgbw(0, 0, 0, 0)
    
x = 0
y = 50.0

toggled = False

def hsv_to_rgb(h, s, v):
    """Convert HSV to RGB.
    
    Args:
        h (float): Hue, between 0.0 and 1.0
        s (float): Saturation, between 0.0 and 1.0
        v (float): Value (brightness), between 0.0 and 1.0
    
    Returns:
        tuple: (r, g, b) values as integers between 0 and 255
    """
    i = int(h * 6)  # Determine which segment of the color wheel
    f = (h * 6) - i  # Fractional part for blending
    p = int(255 * v * (1 - s))
    q = int(255 * v * (1 - f * s))
    t = int(255 * v * (1 - (1 - f) * s))
    v = int(255 * v)

    i = i % 6
    if i == 0:
        return v, t, p
    elif i == 1:
        return q, v, p
    elif i == 2:
        return p, v, t
    elif i == 3:
        return p, q, v
    elif i == 4:
        return t, p, v
    elif i == 5:
        return v, p, q

while True:
    up, down, left, right, switch, state = trackball.read()
    
    # Update x and y values based on movement
    y += up
    y -+ down
    x += right / 10.0
    x-= left / 10.0
    
    # Clamp to min of 0 and max of 100
    x %= 100
    y = max(0, min(y,100))
    
    # Calculate hue and brightness
    h = x / 100.0
    v = y / 100.0
    
    # Prevents button from retriggering
    debounce = 0.5
    
    # Change toggled state if switch is pressed
    if state and not toggled:
        toggled = True
        time.sleep(debounce)
    elif state and toggled:
        toggled = False
        time.sleep(debounce)
        
    # Set brightness to zero if switch toggled
    if toggled:
        v = 0
        
    # Calculate RGB value
    w = 0
    r, g, b = hsv_to_rgb(h, 1.0, v)
    
    # Set LEDs
    trackball.set_rgbw(r, g, b, w)
    
    time.sleep(0.01)