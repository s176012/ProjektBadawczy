# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

# Simple demo of the TCS34725 color sensor.
# Will detect the color from the sensor and print it out every second.
import time
import board
import adafruit_tcs34725


# Create sensor object, communicating over the board's default I2C bus
i2c = board.I2C()  # uses board.SCL and board.SDA
# i2c = board.STEMMA_I2C()  # For using the built-in STEMMA QT connector on a microcontroller
sensor = adafruit_tcs34725.TCS34725(i2c)

# Change sensor integration time to values between 2.4 and 614.4 milliseconds
# sensor.integration_time = 150

# Change sensor gain to 1, 4, 16, or 60
# sensor.gain = 4

# Main loop reading color and printing it every second.
def read_color_values():
    # Raw data from the sensor in a 4-tuple of red, green, blue, clear light component values
    # print(sensor.color_raw)

    color = sensor.color
    color_rgb = sensor.color_rgb_bytes
    result_string = "{}|{}|{}".format(color_rgb[0], color_rgb[1], color_rgb[2])
    # Read the color temperature and lux of the sensor too.
    temp = sensor.color_temperature
    lux = sensor.lux
    print(result_string)
    return result_string

