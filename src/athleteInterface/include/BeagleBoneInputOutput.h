// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 CoDyCo EU porject
 * Authors: Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <BBBiolib.h>
#include <gpio.h>


class bbio {
public:
    void open();
    void close();
    void getAxes(int *);
    void getEncoder  (int, double *);
    void getEncoders (double *);
    void setRefOutput(int, double);
    void setRefOutputs(const double *v);
};