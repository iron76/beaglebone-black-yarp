

if [ "$1" != "" ]; then
export MY_CODE_DIR=$1
    echo "The code will be installed in the directory " $MY_CODE_DIR
    mkdir $MY_CODE_DIR
    mkdir $MY_CODE_DIR/bin
    sudo apt-get install libace-dev
    sudo apt-get install cmake

    echo 'source ~/.bash_profile'                                                  >> ~/.bashrc 
    echo '#YARP'                                                                   >> ~/.bash_profile
    echo "CODE_DIR="$1                                                            >> ~/.bash_profile
    echo 'export PATH=$PATH:$CODE_DIR/bin/bin'                                     >> ~/.bash_profile
    echo 'export YARP_DIR=$CODE_DIR/yarp'                                          >> ~/.bash_profile
    echo 'export YARP_DATA_DIRS=$CODE_DIR/bin/share/yarp:$CODE_DIR/bin/share/yarp' >> ~/.bash_profile
    echo 'source $CODE_DIR/yarp/scripts/yarp_completion'                           >> ~/.bash_profile
    source ~/.bashrc

    cd $MY_CODE_DIR
    git clone https://github.com/robotology/yarp.git
    cd yarp
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$MY_CODE_DIR/bin -DINSTALL_WITH_RPATH:STRING=ON -DCREATE_LIB_MATH:STRING=ON
    make
    make install
    cd ..
    cd ..
    git clone https://github.com/robotology/icub-main.git ./icub
    cd icub
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$MY_CODE_DIR/bin -DINSTALL_WITH_RPATH:STRING=ON
    make
    make install
else
    echo "*****************************************************************"
    echo "******Please provide a path where to install the software!*******"
    echo "Use of this script is: ./yarp_install.sh /path/where/to/install"
    echo "E.g. ./yarp_install.sh /home/user/Code"
    echo "Contact: iron@liralab.it (Francesco Nori)"
fi

