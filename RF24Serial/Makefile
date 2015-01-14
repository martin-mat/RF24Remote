#############################################################################
#
# Makefile for RF24Serial
#
# License: GPL (General Public License)
# Author:  mz-fuzzy <mzfuzzy800@gmail.com
#
#

-include Makefile.config

# make all
# reinstall the library after each recompilation
all: RF24Remote_config.h


RF24Remote_config.h: ../RF24Remote/RF24Remote_config_template.h Makefile.config
	sed \
        -e 's/<RF24_DEVICE_CEPIN>/${RF24_DEVICE_CEPIN}/g' \
        -e 's/<RF24_DEVICE_CSPIN>/${RF24_DEVICE_CSPIN}/g' \
                <../RF24Remote/RF24Remote_config_template.h >RF24Remote_config.h

# clear build files
clean:
	rm -rf *.o RF24Remote_config.h

