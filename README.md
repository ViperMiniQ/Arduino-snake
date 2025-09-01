Simple snake program for Arduino board (Nano was used in this specific project), utilizing some 90% of available memory, so better optimization is necessary for further additions. 

It is possible to adjust brightness and the speed of the snake (refresh rate).

Components used:
- Arduino Nano
- 1cm x 3m x 100cm wood x2
- 3cm x 3m x 50cm wood x2
- opal 0.5cm plastic (or paper) cover 50cm x 100 cm
- IR sensor IRM-56384
- WS2812B LED x200
- XPS foam

![](./images/video.gif)

# Cons:

1.
IR sensor uses interrupts to handle receiving remote signal, but communicating with WS2812B LEDs disables interrupts until the full code is transmitted to the LEDs after which IR can handle new signals.

From: https://github.com/Arduino-IRremote/Arduino-IRremote#problems-with-neopixels-fastled-etc


>IRremote will not work right when you use Neopixels (aka WS2811/WS2812/WS2812B) or other libraries blocking interrupts for a longer time (> 50 µs).
Whether you use the Adafruit Neopixel library, or FastLED, interrupts get disabled on many lower end CPUs like the basic Arduinos for longer than 50 µs. In turn, this stops the IR interrupt handler from running when it needs to. See also this video.
One workaround is to wait for the IR receiver to be idle before you send the Neopixel data with if (IrReceiver.isIdle()) { strip.show();}.
This prevents at least breaking a running IR transmission and -depending of the update rate of the Neopixel- may work quite well.
There are some other solutions to this on more powerful processors, see this page from Marc MERLIN

2.
LEDs were soldered in a way that it is easy to solder them (serpentine pattern), but that requires Arduino to do extra work doing maths to calculate the correct placement of each body segment and food.


