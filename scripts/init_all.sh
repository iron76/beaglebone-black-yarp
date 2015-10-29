# Copyright: (C) 2015 CoDyCo project
# Author: Francesco Nori
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Make sure that YARP_DIR is set in your environment to wherever you
# compiled YARP.

my_dir="$(dirname "$0")"

echo "Loading the can-bus ..."
./$my_dir/set_can.sh
echo "Setting the GPIO properties  ..."
sudo ./$my_dir/set_bus.sh
echo "Adding current user to dialout (to read serial) ..."
./$my_dir/set_dialout.sh
echo "Scritp terminated!"
