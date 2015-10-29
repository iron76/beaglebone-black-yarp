### YARP for BeagleBone black

A bunch of files for the installation of YARP on BeagleBone black board (http://beagleboard.org). This files have been initiated by Francesco Nori when visiting the ISI laboratoy at the Univeristy of Tokyo (October 2015). The code requires the installation of Ubuntu 14.04 on the Beagle Bone black. Since no YARP binaries exist for ARM achitectures it is necessary to compile the software by downloading it from git. 

#### Installation

The beaglebone-black-yarp has three dependecies. The first one is yarp which is configured with a script. The second and third one are submodules in the repostory.

##### Step 0 of installation: create a new user bbb-user
```
adduser -m bbb-user sudo
sudo passwd bbb-user
```

##### Step 1 of installation: create a new user bbb-user
```
su bbb-user
```

##### Step 2 of installation: decide where to put the code
```
export BBB_YARP_DIR=/home/${USER}/Code
```

##### Step 3 of installation: cloning the repo
```
mkdir $BBB_YARP_DIR
cd $BBB_YARP_DIR
git clone http://svc/gitbucket/git/iron/beaglebone-black-yarp.git 
```

##### Step 4 of installation: yarp
```
./$BBB_YARP_DIR/beaglebone-black-yarp/scripts/yarp_install.sh $BBB_YARP_DIR
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
sudo $BBB_YARP_DIR/beaglebone-black-yarp/scripts/init_all.sh 
echo "export BBB_YARP_DIR=$BBB_YARP_DIR/yarp" >> ~/.bashrc
```

#### Launching
From a console PC on the same network with YARP installed (instructions here http://www.yarp.it/install.html) launch the server.

```
user@console:~$ yarpserver

```

From the BBB launch the yarprun:
```
bbb-user@arm:~$ yarprun --server /bbb
```

From the console PC launch the yarpmanager

```
user@console:~$ yarpmanager --application ./$BBB_YARP_DIR/beaglebone-black-yarp/conf/app.xml

```

