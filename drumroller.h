/* drumroller.h: headery things
 *
 * This file is part of Drumroller, a userspace MIDI driver for the
 * ION Drum Rocker (and similar game peripherals).
 * 
 * Copyright (c) 2008 Steven Robertson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301 USA.
 */

/* This is, uh, messy. */

typedef struct {
    const int n;
    const char** names;
} dr_update_info;

typedef struct {
    double kick;
    double red_pad;
    double yel_pad;
    double blu_pad;
    double grn_pad;
    double yel_cym;
    double blu_cym;
    double grn_cym;
} dr_update__ion;

const char* DR_UPDATE_INFO_NAMES__ION[] = {
    "Kick",
    "Red pad",
    "Yellow pad",
    "Blue pad",
    "Green pad",
    "Yellow cymbal",
    "Blue cymbal",
    "Green cymbal"
};


const static dr_update_info DR_UPDATE_INFO__ION = {
    8,
    DR_UPDATE_INFO_NAMES__ION
};

typedef struct {
    struct usb_device *usbdev;
    usb_dev_handle *usbdevh;
} dr_dev;

