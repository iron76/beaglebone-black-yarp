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
    
    PIN **m_pin_spi_sclk;
    PIN **m_pin_spi_miso;
    PIN **m_pin_spi_mosi;
    PIN **m_pin_spi_cs;
    
private:
    void SPI_read();
    void init_DAConvAD5328();
    void init_pins();
    void setDARegister(unsigned char, unsigned short);
    unsigned char transmit8bit(unsigned char, int);
    unsigned short transmit16bit(unsigned short, int);
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
    
    void set_m_pin_spi_sclk(int, PIN* pin);
    void set_m_pin_spi_miso(int, PIN* pin);
    void set_m_pin_spi_mosi(int, PIN* pin);
    void set_m_pin_spi_cs(int, PIN* pin);
    
};