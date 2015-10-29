### YARP for BeagleBone black

A bunch of files for the installation of YARP on BeagleBone black board (http://beagleboard.org). These files have been initiated by Francesco Nori when visiting the ISI laboratoy at the Univeristy of Tokyo (October 2015). The code requires the installation of Ubuntu 14.04 on the Beagle Bone black. Since no YARP binaries exist for ARM achitectures it is necessary to compile the software by downloading it from git. 

#### Features

 - Compiles both YARP and iCub git repositories on the beagleBone (ARM) hardware.
 - Configure the system to use GPIO pins.
 - Includes a module for creating a yarp::dev::ControlBoardWrapper to export data from SPI connections.
 - Configure the BeagleBone hardware for using the CAN-USB2 from ESD (https://esd.eu/en/products/can-usb2) to read a CAN-bus.
 - Uses the socketcan to read force/torque sensors and export them as yarp::dev::AnalogSensors.
 - Configure the hardware to read an Mtx IMU from Xsens (https://www.xsens.com/products/mtx/) via the USB to CAN (directly reading from serial port not currrently available for 3.3 vs 5 volts incompatibility).

#### Installation

The beaglebone-black-yarp has three dependecies. The first one is yarp which is configured with a script. The second and third one are submodules in the repostory.

##### Step 0 of installation: create a new user bbb-user
```
sudo useradd -m bbb-user
sudo adduser bbb-user sudo
sudo passwd bbb-user 
*** create password for bbb-user, recommended 'bbb' ***
```

##### Step 1 of installation: switch user to bbb-user
```
su bbb-user 
*** insert bbb-user password ***
```

##### Step 2 of installation: decide where to put the code
```
export BBB_YARP_DIR=/home/${USER}/Code
```

##### Step 3 of installation: cloning the repo
```
mkdir $BBB_YARP_DIR
cd $BBB_YARP_DIR
git clone https://github.com/iron76/beaglebone-black-yarp.git 
```

##### Step 4 of installation: yarp
```
$BBB_YARP_DIR/beaglebone-black-yarp/scripts/yarp_install.sh $BBB_YARP_DIR
```

##### Step 5 of installation: beaglebone-black-yarp
```
cd $BBB_YARP_DIR/beaglebone-black-yarp
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$BBB_YARP_DIR/bin
make
sudo make install
```

#### Configuring
```
$BBB_YARP_DIR/beaglebone-black-yarp/scripts/init_all.sh 
echo "export BBB_YARP_DIR=$BBB_YARP_DIR/yarp" >> ~/.bashrc
source ~/.bashrc
```

#### Launching
From a console PC on the same network with YARP installed (instructions here http://www.yarp.it/install.html) launch the server.

```
user@console:~$ yarp namespace /athletebot
user@console:~$ yarpserver --write

```

From the BBB launch the yarprun:

```
bbb-user@arm:~$ yarp namespace /athletebot
bbb-user@arm:~$ yarp detect --write
bbb-user@arm:~$ yarprun --server /bbb
```

From the console PC launch the yarpmanager

```
user@console:~$ export BBB_YARP_DIR=$HOME/Code
user@console:~$ mkdir -p $BBB_YARP_DIR
user@console:~$ cd $BBB_YARP_DIR
user@console:~$ git clone https://github.com/iron76/beaglebone-black-yarp.git 
user@console:~$ yarpmanager --application $BBB_YARP_DIR/beaglebone-black-yarp/conf/app.xml
```

Among applications find AthleteRobotInterface and launch the different modules.

#### Tips and tricks

The kernel version shoudl be 4.1 and should be updated as suggested here: http://elinux.org/BeagleBoardUbuntu#Install_Latest_Kernel_Image
```
cd /opt/scripts/tools
git pull
./update_kernel.sh --beta-kernel
```
