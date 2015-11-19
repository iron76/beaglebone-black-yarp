// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 CoDyCo EU porject
 * Authors: Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <BBBiolib.h>
#include <gpio.h>
#include <map>


class bbio {
    unsigned int *m_port_clk_SPI;
    unsigned int *m_port_din_SPI;
    unsigned int *m_port_dout_SPI;
    unsigned int *m_port_cs_SPI;
private:
    void SPI_read();
public:
    void allocate();
    void open();
    void close();
    void getAxes(int *);
    void getEncoder  (int, double *);
    void getEncoders (double *);
    void setRefOutput(int, double);
    void setRefOutputs(const double *v);
    void set_m_port_clk_SPI(int, int pin);
    void set_m_port_din_SPI(int, int pin);
    void set_m_port_dout_SPI(int, int pin);
    void set_m_port_cs_SPI(int, int pin);
};