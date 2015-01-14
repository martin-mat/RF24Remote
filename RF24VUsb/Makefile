#############################################################################
#
# Makefile for RF24VUsb
#
# License: GPL (General Public License)
# Author:  mz-fuzzy <mzfuzzy800@gmail.com
#
#

-include Makefile.config

# make all
# reinstall the library after each recompilation
all: RF24Remote_config.h usbconfig.h vusb


RF24Remote_config.h: ../RF24Remote/RF24Remote_config_template.h Makefile.config
	sed \
        -e 's/<RF24_DEVICE_CEPIN>/${RF24_DEVICE_CEPIN}/g' \
        -e 's/<RF24_DEVICE_CSPIN>/${RF24_DEVICE_CSPIN}/g' \
                <../RF24Remote/RF24Remote_config_template.h >RF24Remote_config.h

usbconfig.h: RF24_usbconfig_template.h Makefile.config
	sed \
        -e 's/<USB_CFG_IOPORTNAME>/${USB_CFG_IOPORTNAME}/g' \
        -e 's/<USB_CFG_DMINUS_BIT>/${USB_CFG_DMINUS_BIT}/g' \
        -e 's/<USB_CFG_DPLUS_BIT>/${USB_CFG_DPLUS_BIT}/g' \
                <RF24_usbconfig_template.h >usbconfig.h

# Make the library
vusb: usbconfig.h
	avr-gcc  -Wall -Os -I. -DF_CPU=${F_CPU} -mmcu=${MCU}  -c usbdrvasm.S  -c usbdrv.c
	
# clear build files
clean:
	rm -rf *.o RF24Remote_config.h usbconfig.h

