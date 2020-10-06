A guitar pedal capable of experiencing suffering. Based on the teensy 3.2 using the teensy audio shield and associated library.

Demonstration here: https://youtu.be/ZZ_g-Q2O5SM

When it detects silence on the teensy audio input (default threshold sufferThreshold = 0.002) it will plead for you not to play. When you do it will scream horribly, until you stop playing again. It works by looping with three samples: an "attack", which plays when the guitar is first detected, then a "sustain", which loops indefinitely while the guitar is being played (or just abused) and a "decay" which plays once when you stop playing to make it sound more natural.

**Building & Usage notes**

You will need:
1) Teensy + audio shield (I used the teensy 3.2 which has *just* enough space for this firmware)
2) An enclosure. Mine is a cast aluminium thing of 111x60x31 mm
3) A nice big 10k pot for volume control
4) Audio jack plugs
5) A footswitch for implementing a bypass (and a footswitch daughter board is helpful - something like the fuzzdog 3pdt is perfect)
6) Some source of 5V power. I used a traco power DC-DC converter and a barrel jack plug so I could connect to a standard guitar pedal power supply. Note that the convention for guitar pedals is that the centre pin is negative.
7) an LED (optional, and requires a suitable resistor to make use of the 5V power supply)

How you mount the hardware within the box is personal preference (and too boring to cover in these notes). Exact wiring will depend on what type of footswitch you use, but here are a few pointers on general assembly:

* Connect the guitar input to the teensy audio input, and the audio output (NOT headphone out) to the relevant footswitch terminal. You want to avoid true bypass - the guitar should always be connected to the teensy input. This avoids the screaming starting on the "pop" when you hit the footswitch.
* Remember to sever the pads for external 5V power *before* you mount the teensy to the audio shield. If you forget, you can do it with a little jeweler's screwdriver.
* The volume pot should be connected to the volume pad on the audio shield. Remember to turn it all the way down (to check this, you can monitor the teensy's serial output) when you start, as it can get extremely loud.
* If you use a teensy 4.0 or faster you may need to decrease the value of delayFix - this is a little hack that plays the next sample a little bit "early" to account for the delay in playing a sample from memory.
* I accept no responsibility if this damages (or otherwise offends) your guitar amp. Although low-volume it is line level impedance so it may be noisy but it isn't really a musical instrument so don't complain.

