# Implemented #

The USB backend, based on libusb, is working, but it's still got a few hardcoded values that I would like to unbake.  The MIDI backends for ALSA, JACK and Core Audio will be done shortly.  Code will be in SVN very soon.

# Planned #

### Guitar Hero: World Tour drum support ###

This requires hardware which I do not have.

### LED Control ###

Depending on the speed of the "Ring of Light" controller, it may be possible to give it a MIDI channel and use it as a metronome or whatever.  In any case, it'd be nice to have it stop blinking incessantly.

### Input remapping ###

The current version fixes drum events to MIDI events (in a sensible way, but still).  While it's possible to remap the instruments in most software, it's easier to remap the controller.

### Velocity curves ###

The hardware-provided velocity curve is rather useless. I'll try and map it to something more sensible, but it doesn't look like there's a lot of variability to work with.

### Windows? ###

Depends entirely on people letting me know they're interested.