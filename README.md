### YARP for BeagleBone black


A bunch of files for the installation of YARP on BeagleBone black board (http://beagleboard.org). This files have been initiated by Francesco Nori when visiting the ISI laboratoy at the Univeristy of Tokyo (October 2015). The code requires the installation of Ubuntu 14.04 on the Beagle Bone black. Since no YARP binaries exist for ARM achitectures it is necessary to compile the software by downloading it from git. 

#### Installation

```
git clone http://svc/gitbucket/git/iron/beaglebone-black-yarp.git
cd beaglebone-black-yarp
mkdir build
cmake ..
make
sudo make install
```

#### Configuring
Assuming the software was installed in the direcotry beaglebone-black-yarp_INSTALL_DIR the following command sets the proper GPIO configuration and should be exectued after every reboot of the BeagleBone board. 
```
sudo beaglebone-black-yarp_INSTALL_DIR/scripts/set_bus.sh
```

#### Launching
```
run_athletebot --from ${beaglebone-black-yarp_INSTALL_DIR}/src/modules/athleteInterface/athletebot.ini
```
