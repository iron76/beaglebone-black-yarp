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
#include <assert.h>


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace yarp::dev;

#define STRING_MACRO(x) (#x)

bool AthleteBot::getNamesFromConfig(yarp::os::Searchable& config, std::vector<std::string>& names_vector, const std::string key_string)
{    
    yarp::os::Property configuration;
    configuration.fromString(config.toString().c_str());
    
    yarp::os::Bottle joint_names_bottle = configuration.findGroup(key_string);
    
    if (joint_names_bottle.isNull()) {
        std::cout << "AthleteBot::getNamesFromConfig() error: cannot find " << key_string << " parameter." << std::endl;
        return false;
    }
    
    assert(m_njoints == joint_names_bottle.size() - 1);
    
    for(unsigned int i=0; i < m_njoints; i++ )
    {
        std::string controlboard_joint_name(joint_names_bottle.get(i+1).asString().c_str());
#ifdef _ENABLE_DEBUG_
        std::cout << "AthleteBot::getNamesFromConfig(): " << controlboard_joint_name << std::endl;
#endif
        names_vector.push_back(controlboard_joint_name);
#ifdef _ENABLE_DEBUG_
        std::cout << "AthleteBot::getNamesFromConfig(): set joint name to: " << names_vector[i] << std::endl;
#endif
    }
    return true;
}


void AthleteBot::init() {
    m_referencePositions.size(m_njoints);                 /* desired reference positions */
    m_referenceVelocities.size(m_njoints);                /* desired reference speeds */
    m_trajectoryGenerationReferenceSpeed.size(m_njoints); /* desired speed for trj generation */
    m_trajectoryGenerationReferenceAcc.size(m_njoints);   /* desired acc for vel generation */
    m_positions.size(m_njoints);
    m_referenceOuputs.size(m_njoints);

    m_controlMode.resize(m_njoints,(int) VOCAB_CM_OPENLOOP);
    for (int i=0; i<m_njoints; i++) {
        m_referencePositions[i] = 0;
        m_referenceVelocities[i] = 0;
        m_trajectoryGenerationReferenceSpeed[i] = 0;
        m_trajectoryGenerationReferenceAcc[i] = 0;
        m_positions[i] = 0;
        m_referenceOuputs[i] = 0;
    }
    
    m_bbbiomap.insert ( std::pair<std::string,int>(STRING_MACRO(BBBIO_GPIO_PIN_12),BBBIO_GPIO_PIN_12) );
    m_bbbiomap.insert ( std::pair<std::string,int>(STRING_MACRO(BBBIO_GPIO_PIN_13),BBBIO_GPIO_PIN_13) );
    m_bbbiomap.insert ( std::pair<std::string,int>(STRING_MACRO(BBBIO_GPIO_PIN_16),BBBIO_GPIO_PIN_16) );
    m_bbbiomap.insert ( std::pair<std::string,int>(STRING_MACRO(BBBIO_GPIO_PIN_19),BBBIO_GPIO_PIN_19) );
    
    m_gpiomap.insert ( std::pair<std::string,PIN>(STRING_MACRO(P9_28),P9_28) );
    m_gpiomap.insert ( std::pair<std::string,PIN>(STRING_MACRO(P9_29),P9_29) );
    m_gpiomap.insert ( std::pair<std::string,PIN>(STRING_MACRO(P9_30),P9_30) );
    m_gpiomap.insert ( std::pair<std::string,PIN>(STRING_MACRO(P9_31),P9_31) );
    m_gpiomap.insert ( std::pair<std::string,PIN>(STRING_MACRO(P9_42),P9_42) );
}



bool AthleteBot::open(yarp::os::Searchable& config) {
    
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Checking the configuration file \n");
#endif
    
    m_njoints = config.check("number_of_joints",Value(0),
                             "number of joints").asInt();
    assert(m_njoints > 0);
    init();

#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Parsing the names for joints (%d) \n", m_njoints);
#endif
    bool ok = true;
    ok = ok & getNamesFromConfig(config, m_jointNames, "joint_names");
    // BBBIO pins
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Parsing the names of bbbio pins\n");
#endif
    ok = ok & getNamesFromConfig(config, m_bbbiosclk,  "sclk_bbbio");
    ok = ok & getNamesFromConfig(config, m_bbbiomosi,  "mosi_bbbio");
    ok = ok & getNamesFromConfig(config, m_bbbiomiso,  "miso_bbbio");
    ok = ok & getNamesFromConfig(config, m_bbbioss,    "ss_bbbio");
    // GPIO pins
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Parsing the names of gpio pins\n");
#endif
    ok = ok & getNamesFromConfig(config, m_gpiosclk, "sclk_gpio");
    ok = ok & getNamesFromConfig(config, m_gpiomosi, "mosi_gpio");
    ok = ok & getNamesFromConfig(config, m_gpiomiso, "miso_gpio");
    ok = ok & getNamesFromConfig(config, m_gpioss,   "ss_gpio");
    
    if( !ok )
    {
        return false;
    }

#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Double checking the name of bbbio pins\n");
#endif
    std::map<std::string,int>::iterator it_bbb;
    for (int i=0; i<m_njoints; i++)
    {
        it_bbb = m_bbbiomap.find(m_bbbiosclk[i]);
        ok &= (it_bbb != m_bbbiomap.end());
        it_bbb = m_bbbiomap.find(m_bbbiomosi[i]);
        ok &= (it_bbb != m_bbbiomap.end());
        it_bbb = m_bbbiomap.find(m_bbbiomiso[i]);
        ok &= (it_bbb != m_bbbiomap.end());
        it_bbb = m_bbbiomap.find(m_bbbioss[i]);
        ok &= (it_bbb != m_bbbiomap.end());
        if (!ok)
        {
            std::cout << "AthleteBot::open() error: cannot find supplied bbbio PIN name. " << std::endl;
            return false;
        }
        
    }

#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Double checking the name of gpio pins\n");
#endif
    std::map<std::string,PIN>::iterator it_gp;
    for (int i=0; i<m_njoints; i++)
    {
        it_gp = m_gpiomap.find(m_gpiosclk[i]);
        ok &= (it_gp != m_gpiomap.end());
        it_gp = m_gpiomap.find(m_gpiomiso[i]);
        ok &= (it_gp != m_gpiomap.end());
        it_gp = m_gpiomap.find(m_gpiomosi[i]);
        ok &= (it_gp != m_gpiomap.end());
        it_gp = m_gpiomap.find(m_gpioss[i]);
        ok &= (it_gp != m_gpiomap.end());        
        if (!ok)
        {
            std::cout << "AthleteBot::open() error: cannot find supplied gpio PIN name. " << std::endl;
            return false;
        }
    }
    
#ifndef _DISABLE_IO_
    m_bbio.open();
    m_bbio.setRefOutputs(m_referenceOuputs.data());
#endif
    
    return true;
}

void AthleteBot::run() { }

