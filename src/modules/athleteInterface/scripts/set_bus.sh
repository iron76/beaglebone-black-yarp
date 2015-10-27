# echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots
# echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots 

echo 7 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio7/direction

echo 110 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio110/direction

echo 111 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio111/direction

echo 112 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio112/direction

echo 113 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio113/direction

echo 73 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio73/direction

echo 71 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio71/direction
