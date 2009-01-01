/* usb.c: libusb interface code (the cross-platform bits)
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



#include <stdio.h>
#include <usb.h>
#include <unistd.h>

#include "drumroller.h"

// take only what we need
#define HID_REPORT_GET        0x01

#define X360_REPORT_ID_CNTRLR 0x00
#define X360_REPORT_ID_LEDS   0x01

#define HID_REPORT_TYPE_INPUT 0x01

#define X360_MAX_PKT_LEN      0x20
#define REASONABLE_TIMEOUT    0x4000


double* get_latest_update__ion(dr_dev *d) {
    /* Returns the most recent update, as normalized ([0, 1]) doubles.
     * The length of the return array must be known by the configuration,
     * but the intermediate layers don't need to bother with it. */
    
    /* This code can be used for any wired 360 peripheral */
    unsigned char buf[X360_MAX_PKT_LEN];
    usb_control_msg(d->usbdevh, 
		USB_ENDPOINT_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE, 
		HID_REPORT_GET, 
		(HID_REPORT_TYPE_INPUT << 8) | X360_REPORT_ID_CNTRLR, 
		0, // interface. TODO: Allow for wireless drums (GH:WT et al)
		(char*) buf, 
		X360_MAX_PKT_LEN, 
		REASONABLE_TIMEOUT);
    
    /* See http://code.google.com/p/drumroller/wiki/ProtocolInfo for more */
    dr_update__ion *upd = calloc(sizeof(dr_update__ion), 1);
    
    
    /* For clarity only */
    #define KICK_FLAG	(buf[3] & 0x01)

    #define PAD_FLAG	(buf[2] & 0x80)
    #define CYM_FLAG	(buf[3] & 0x02)

    #define GRN_FLAG	(buf[3] & 0x10)
    #define RED_FLAG	(buf[3] & 0x20)
    #define BLU_FLAG	(buf[3] & 0x40)
    #define YEL_FLAG	(buf[3] & 0x80)

    #define YEL_RESOLV	(buf[2] & 0x01)
    #define BLU_RESOLV	(buf[2] & 0x02)
    #define GRN_RESOLV	(!YEL_RESOLV && !BLU_RESOLV)

    #define RED_VEL	((255 - buf[7]) / 255.)
    #define YEL_VEL	((buf[9]) / 255.)
    #define BLU_VEL	((255 - buf[11]) / 255.)
    #define GRN_VEL	((buf[13]) / 255.)

    /* The drum kit can accurately report velocity data for a pad and a cymbal 
     * of the same color at the same time, as long as the red pad isn't also
     * being played, by dumping the velocity data into the red pad's slot. 
     *
     * TODO: Investigate noise bits. For now, though, we just use the MSB. */

    upd->kick = KICK_FLAG ? 1.0 : 0.0;

    int num_colors = 0;
    if (RED_FLAG) num_colors++;
    if (YEL_FLAG) num_colors++;
    if (BLU_FLAG) num_colors++;
    if (GRN_FLAG) num_colors++;

    if (num_colors == 0) return (double*) upd;
    
    if (RED_FLAG)
	upd->red_pad = RED_VEL;
    
    if (YEL_FLAG) {
	if (num_colors == 1 && CYM_FLAG && PAD_FLAG) {
	    upd->yel_pad = YEL_VEL;
	    upd->yel_cym = RED_VEL;
	} else {
	    if (YEL_RESOLV) 
		upd->yel_cym = YEL_VEL;
	    else
		upd->yel_pad = YEL_VEL;
	}
    }

    if (BLU_FLAG) {
	if (num_colors == 1 && CYM_FLAG && PAD_FLAG) {
	    upd->blu_pad = BLU_VEL;
	    upd->blu_cym = RED_VEL;
	} else if (CYM_FLAG) {
	    if (PAD_FLAG && !BLU_RESOLV) 
		upd->blu_pad = BLU_VEL;
	    else
		upd->blu_cym = BLU_VEL;
	} else
	    upd->blu_pad = BLU_VEL;
    }

    if (GRN_FLAG) {
	if (num_colors == 1 && CYM_FLAG && PAD_FLAG) {
	    upd->grn_pad = GRN_VEL;
	    upd->grn_cym = RED_VEL;
	} else if (CYM_FLAG) {
	    if (PAD_FLAG && !GRN_RESOLV) {
		upd->grn_pad = GRN_VEL;
	    } else
		upd->grn_cym = GRN_VEL;
	} else
	    upd->grn_pad = GRN_VEL;
    }

    return (double*) upd;
}

void print_ion(struct usb_device *usbdev) {
    
    int i, j;
    dr_dev d;
    double* u;
    const dr_update_info* info = &DR_UPDATE_INFO__ION;

    d.usbdev = usbdev;
    d.usbdevh = usb_open(usbdev);
    u = get_latest_update__ion(&d);

    while (!usleep(5000)) {
	double* nu = get_latest_update__ion(&d);

	for (i = 0; i < info->n; i++) 
	    if (u[i] != nu[i])
		break;
	
	if (i < 8) {
	    for (i = 0; i < info->n; i++) {
		if (nu[i] > 0.) {
		    printf("%16s ", info->names[i]);
		    for (j = 0; j < 32 * nu[i]; j++)
			printf("*");
		    for (; j < 32; j++) 
			printf(" ");
		    printf("|\n");
		}
	    }
	    printf("\n");
	}
	free(u);
	u = nu;
    }
}
		


int main() {
   
    struct usb_bus *b;

    usb_init();
    usb_find_busses();

    if (usb_find_devices() > 0) {
	for (b = usb_busses; b; b = b->next) {
	    struct usb_device *d;
	    for (d = b->devices; d; d = d->next) {
		/* TODO: Add other drum peripherals using Rock Band's protocol */
		if (d->descriptor.idVendor == 0x1bad && 
		    d->descriptor.idProduct == 0x0130) 
		    print_ion(d);
	    }
	}
    }

    return 0;
}

