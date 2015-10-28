// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2007 RobotCub Consortium
 * Authors: Paul Fitzpatrick and Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include "AthleteBot.h"

#include <yarp/sig/all.h>
#include <yarp/sig/ImageFile.h>
#include <yarp/os/all.h>


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace yarp::dev;


void AthleteBot::init() {
    fprintf(stderr, "The AthleteBot::init function is yet to be implmented. \n");
}



bool AthleteBot::open(yarp::os::Searchable& config) {
    ConstString backFile = config.check("background",Value("textures/back.ppm"),
                                        "background image to use").asString();
//    if (backFile!="") {
//        yarp::sig::file::read(back,backFile.c_str());
//    }
//    ConstString foreFile = config.check("target",Value("textures/fore.ppm"),
//                                        "target image to use").asString();
//    if (foreFile!="") {
//        yarp::sig::file::read(fore,foreFile.c_str());
//    }
    noiseLevel = config.check("noise",Value(0.05),
                              "pixel noise level").asDouble();
    
    return true;
}


void AthleteBot::run() { }

