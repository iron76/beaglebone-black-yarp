// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 CoDyCo EU porject
 * Authors: Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <BBBiolib.h>

class bbio {
public:
    void init();
    void close();
    void getAxes(int *);
    void getEncoder  (int, double *);
    void getEncoders (double *);
};