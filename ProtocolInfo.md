# Introduction #

All Xbox 360 controllers use a slightly broken form of the HID protocol.  The missing pieces have been reverse-engineered and [documented](http://www.free60.org/wiki/Gamepad), and drivers exist to work around these flaws for Windows, Linux, and Mac OS X.  The Ion Drum Rocker behaves like an Xbox 360 controller, but the mapping of drum inputs to HID input report fields is non-intuitive.  This page documents those mappings.

# Background #

_The following information is speculative._

The original protocol for Rock Band Xbox 360 drums maps drum hits to face button presses on a traditional controller (full details coming soon).  The Drum Rocker is intended to be backwards-compatible, so it provides those mappings; however, with the inclusion of cymbals and velocity-sensitive drum pads, much more information needs to be provided.  Cymbal hits need to be treated as identical to pad hits for Rock Band 1's sake, so each pad still maps to a face button as before, but cymbal hits need to be differentiated from pad hits for Rock Band 2, so a mask is set up using buttons that went unused in the Rock Band 1 drum protocol to identify which type of item is being hit.  This limits the Rock Band 2 protocol such that it can only generally support at most two unambiguous hits per input report, but since most drummers only have two arms, this is not a crippling limitation.

# The protocol #

Refer to the reverse-engineered documentation on normal 360 controllers [here](http://www.free60.org/wiki/Gamepad).

```
Byte  Mask    Function
00    00    # Protocol ID. Does not change.
01    14    # Packet size. Does not change.
02    01    # D-Pad up / Resolve to yellow cymbal[1].
02    02    # D-Pad down / Resolve to blue cymbal.
02    04    # D-Pad left.
02    08    # D-Pad right.
02    10    # Start button.
02    20    # Back button.
02    40    # Pedal connected.
02    80    # At least one drum pad hit.
03    01    # Pedal depressed.
03    02    # At least one cymbal hit.
03    10    # Green pad / Green cymbal / 'A' button.
03    20    # Red pad / 'B' button.
03    40    # Blue pad / Blue cymbal / 'X' button.
03    80    # Yellow pad / Yellow cymbal / 'Y' button.
04    00    # Left trigger. Unused.
05    00    # Right trigger. Unused.
06    ??    # Noise[2].
07    ff    # Red pad / extra slot[3].  Inverted[4].
08    ??    # Noise.
09    ff    # Yellow pad / Yellow cymbal.
0a    ??    # Noise.
0b    ff    # Blue pad / Blue cymbal. Inverted.
0c    ??    # Noise.
0d    ff    # Green pad / Green cymbal.
0e-13 00    # Unused.
```

## Notes ##

  1. **Resolve:**  When this is set and there is an ambiguity about which color is the cymbal, resolve as above.  If unset, resolve to green cymbal.
  1. **Noise:**  These are the least-significant bytes of their respective axes, and as such could just be reporting noise from the long analog cable runs to the drum pads.  However, the bytes usually take on only certain values which differ for each position, and remain fixed throughout much of a given note's duration.  I'm still investigating these.
  1. **Extra slot:** When both cymbal and pad of a given color are hit simultaneously, this value stores the velocity information of the cymbal.  I think.
  1. **Inverted:** Value is subtractive: at rest, it's 0xff, and maximum intensity is 0x00.

# A dump #

This is quick, noisy, and not necessarily correct. I think some of the samples are just flat-out wrong, but I can't tell if it's human error or noise yet. I'll update it soon with more detail as I work out all the kinks in the driver's noise filters.

```
 Kick
20 00144001 0000b9ff 8c00b9ff 8c000000 00000000
KICK
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 0

 Red pad
20 0014c020 00005232 8c00b9ff d2000000 00000000
PAD
RED
RED_VEL: 205
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 0

 Yellow pad
20 0014c080 0000b9ff 4ca6b9ff 8c000000 00000000
PAD
YEL
RED_VEL: 0
YEL_VEL: 166
BLU_VEL: 0
GRN_VEL: 0

 Blue pad
20 0014c040 0000b9ff 8c005232 46000000 00000000
PAD
BLU
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 205
GRN_VEL: 0

 Green pad
20 0014c010 0000b9ff 8c000000 73bc0000 00000000
PAD
GRN
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 255
GRN_VEL: 188

 Red and yellow pad
20 0014c0a0 00005232 a4e1b9ff 8c000000 00000000
PAD
RED
YEL
RED_VEL: 205
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 0

 Red and blue pad
20 0014c060 0000f959 4600a11e 8c000000 00000000
PAD
RED
BLU
RED_VEL: 166
YEL_VEL: 0
BLU_VEL: 225
GRN_VEL: 0

 Red and green
20 0014c030 00005232 8c00b9ff a4e10000 00000000
PAD
GRN
RED
RED_VEL: 205
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 225

 Yellow and blue
20 0014c080 0000b9ff 008073ff 8c000000 00000000
PAD
YEL
RED_VEL: 0
YEL_VEL: 128
BLU_VEL: 0
GRN_VEL: 0

 Yellow and green
20 0014c080 0000b9ff 008073ff 8c000000 00000000
PAD
YEL
RED_VEL: 0
YEL_VEL: 128
BLU_VEL: 0
GRN_VEL: 0

 Yellow and green
20 0014c080 00000000 008073ff 8c000000 00000000
PAD
YEL
RED_VEL: 255
YEL_VEL: 128
BLU_VEL: 0
GRN_VEL: 0

 Yellow and green
20 0014c090 000073ff a4e1b9ff f3cd0000 00000000
PAD
GRN
YEL
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 205

 Blue and green
20 0014c050 000073ff d2005232 2dbc0000 00000000
PAD
GRN
BLU
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 205
GRN_VEL: 188

Yellow cym and... Yellow cym and
Yellow cym and 0
20 00144182 000073ff a4e1b9ff 46000000 00000000
CYM
YEL
YEL_RESOLV
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 0

Yellow cym and red pad
20 0014c1a2 00009832 eae1b9ff 8c000000 00000000
PAD
CYM
RED
YEL
YEL_RESOLV
RED_VEL: 205
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 0

Yellow cym and yellow pad
20 0014c182 00009832 a4e1b9ff 8c000000 00000000
PAD
CYM
YEL
YEL_RESOLV
RED_VEL: 205
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 0

Yellow cym and blue pad
20 0014c1c2 00000000 eae15232 8c000000 00000000
PAD
CYM
BLU
YEL
YEL_RESOLV
RED_VEL: 255
YEL_VEL: 225
BLU_VEL: 205
GRN_VEL: 0

Yellow cym and green pad
20 0014c192 0000b9ff eae1b9ff eae10000 00000000
PAD
CYM
GRN
YEL
YEL_RESOLV
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 225

Yellow cym and blue cym
20 001441c2 000073ff a4e1a11e d2000000 00000000
CYM
BLU
YEL
YEL_RESOLV
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 225
GRN_VEL: 0

Yellow cym and green cym
20 00144192 000073ff a4e173ff eae10000 00000000
CYM
GRN
YEL
YEL_RESOLV
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 225

Blue cym and... Blue cym and
Blue cym and nothing else
20 00144242 0000b9ff d2005b1e d2000000 00000000
CYM
BLU
BLU_RESOLV
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 225
GRN_VEL: 0

Blue cym and red pad
20 0014c262 00001844 8c00a11e d2000000 00000000
PAD
CYM
RED
BLU
BLU_RESOLV
RED_VEL: 187
YEL_VEL: 0
BLU_VEL: 225
GRN_VEL: 0

Blue cym and yellow pad
20 0014c2c2 0000b9ff eae15b1e 8c000000 00000000
PAD
CYM
BLU
YEL
BLU_RESOLV
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 225
GRN_VEL: 0

Blue cym and yellow pad
20 0014c2c2 00000000 eae1a11e 8c000000 00000000
PAD
CYM
BLU
YEL
BLU_RESOLV
RED_VEL: 255
YEL_VEL: 225
BLU_VEL: 225
GRN_VEL: 0

Blue cym and blue pad
20 0014c242 0000a11e 8c00f959 8c000000 00000000
PAD
CYM
BLU
BLU_RESOLV
RED_VEL: 225
YEL_VEL: 0
BLU_VEL: 166
GRN_VEL: 0

Blue cym and green pad
20 0014c252 000073ff 8c00a11e a4e10000 00000000
PAD
CYM
GRN
BLU
BLU_RESOLV
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 225
GRN_VEL: 225

Green cym and... Green cym and
Green cym and nothing
20 00144012 0000b9ff 8c00b9ff eae10000 00000000
CYM
GRN
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 225

Green cym and red pad
20 0014c032 00005b1e 8c00b9ff eae10000 00000000
PAD
CYM
GRN
RED
RED_VEL: 225
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 225

Green cym and yellow pad
20 0014c092 0000b9ff a4e1b9ff a4e10000 00000000
PAD
CYM
GRN
YEL
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 225

Green cym and yellow pad
20 0014c092 0000b9ff a4e1b9ff a4e10000 00000000
PAD
CYM
GRN
YEL
RED_VEL: 0
YEL_VEL: 225
BLU_VEL: 0
GRN_VEL: 225

Green cym and blue pad
20 0014c052 00000000 8c001844 eae10000 00000000
PAD
CYM
GRN
BLU
RED_VEL: 255
YEL_VEL: 0
BLU_VEL: 187
GRN_VEL: 225

Green cym and green pad
20 0014c012 0000f959 8c00b9ff eae10000 00000000
PAD
CYM
GRN
RED_VEL: 166
YEL_VEL: 0
BLU_VEL: 0
GRN_VEL: 225

Green cym and blue cym
20 00144052 0000b9ff 46005b1e a4e10000 00000000
CYM
GRN
BLU
RED_VEL: 0
YEL_VEL: 0
BLU_VEL: 225
GRN_VEL: 225
```