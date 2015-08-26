**Please note, this project is no longer under development (and hasn't been for some time).**

This project seeks to provide drivers for the Ion Drum Rocker peripheral, a prosumer-grade electronic drum kit designed to work with the Rock Band series of games.  The focus will be on the Xbox 360 version of the device because, well, it's the only one out right now.

Unlike previous Xbox 360 controller driver projects, this one will be targeted only at the Drum Rocker and compatible peripherals, and will provide MIDI events rather than the typical HID events.  Because the Drum Rocker abuses the HID protocol in a somewhat unique way, it also seems likely that the driver will sit entirely in userspace rather than attempt to use the existing input infrastructure in each OS.  This has the added advantage of making the driver more portable.

The Guitar Hero: World Tour drums are also apparently velocity-sensitive, although it would be surprising if they used the same protocol perversions that the Drum Rocker does.  If the author can get his hands on a set, GH:WT support will be added.