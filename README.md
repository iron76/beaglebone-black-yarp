### YARP for BeagleBone black

A bunch of files for the installation of YARP on BeagleBone black board (http://beagleboard.org). This files have been initiated by Francesco Nori when visiting the ISI laboratoy at the Univeristy of Tokyo (October 2015). The code requires the installation of Ubuntu 14.04 on the Beagle Bone black. Since no YARP binaries exist for ARM achitectures it is necessary to compile the software by downloading it from git. 

#### Installation

The beaglebone-black-yarp has three dependecies. The first one is yarp which is configured with a script. The second and third one are submodules in the repostory.

##### Step 0 of installation: decide where to put the code
```
export INSTALL_DIR=/path/where/to/install
```
e.g.

```
export INSTALL_DIR=/home/${USER}/Code
```

##### Step 1 of installation: cloning the repo
```
git clone http://svc/gitbucket/git/iron/beaglebone-black-yarp.git 
```

##### Step 2 of installation: yarp
```
./${beaglebone-black-yarp_DIR}/scripts/yarp_install.sh $INSTALL_DIR
```

##### Step 3 of installation: beaglebone-black-yarp
```
cd ${beaglebone-black-yarp_DIR}
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR/bin
make
sudo make install
```

#### Configuring
Assuming the software was installed in the directory beaglebone-black-yarp_DIR the following command sets the proper GPIO configuration and should be exectued after every reboot of the BeagleBone board.

```
sudo ${beaglebone-black-yarp_DIR}/scripts/set_bus.sh
```

#### Launching

```
run_athletebot --from ${beaglebone-black-yarp_DIR}/src/modules/athleteInterface/athletebot.ini
```
