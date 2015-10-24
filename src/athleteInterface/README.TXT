
# Copyright: (C) 2015 Francesco Nori
# Author: Francesco Nori
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

This program simulates a robot head looking out to sea at a boat.

On Linux type: 

   mkdir build
   cd build
   ccmake ..   [Locate the libBBBio.a library and choose installation dir]
   make
   sudo make install
   # Edit athletebot.ini and change USERNAME to something you like
   run_athletebot --file athletebot.ini
