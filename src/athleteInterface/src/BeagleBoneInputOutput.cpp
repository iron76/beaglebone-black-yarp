// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 CoDyCo EU porject
 * Authors: Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <BeagleBoneInputOutput.h>

#define NUM_ADC 1
#define NUM_ADC_PORT 8

/* set gpio pin number */
unsigned int port_clk_SPI[]={BBBIO_GPIO_PIN_16};
unsigned int port_din_SPI[]={BBBIO_GPIO_PIN_13};
unsigned int port_dout_SPI[]={BBBIO_GPIO_PIN_12};
unsigned int port_cs_SPI[]={BBBIO_GPIO_PIN_19};

/* set gpio pin number */
unsigned long SPI_sensor_value[NUM_ADC][NUM_ADC_PORT];

void SPI_read()
{
    unsigned long pin_num=0x00;
    unsigned long sVal[NUM_ADC];
    unsigned long commandout=0x00;
    
    /* SPI */
    for(pin_num=0;pin_num<NUM_ADC_PORT;pin_num++){
        for(int j=0;j<NUM_ADC;j++){
            sVal[j]=0x00;
        }
        
        // set CS
        BBBIO_GPIO_high(BBBIO_GPIO1 ,port_cs_SPI[0]);
        BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0] | port_din_SPI[0]);
        BBBIO_GPIO_low(BBBIO_GPIO1 ,port_cs_SPI[0]);
        
        commandout=pin_num;
        commandout|=0x18;
        commandout<<=3;
        
        //wait_SPI();
        
        for(int i=0;i<5;i++){
            if(commandout&0x80){
                BBBIO_GPIO_high(BBBIO_GPIO1 ,port_din_SPI[0]);
            }
            else{
                BBBIO_GPIO_low(BBBIO_GPIO1 ,port_din_SPI[0]);
            }
            commandout<<=1;
            BBBIO_GPIO_high(BBBIO_GPIO1 ,port_clk_SPI[0]);
            BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0]);
        }
        
        //sleep(1);
        
        for(int i=0;i<2;i++){
            BBBIO_GPIO_high(BBBIO_GPIO1 ,port_clk_SPI[0]);
            BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0]);
        }
        for(int i=0;i<12;i++){
            BBBIO_GPIO_high(BBBIO_GPIO1 ,port_clk_SPI[0]);
            for(int j=0;j<NUM_ADC;j++){
                sVal[j]<<=1;
            }
            for(int j=0;j<NUM_ADC;j++){
                if(BBBIO_GPIO_get(BBBIO_GPIO1 ,port_dout_SPI[j])){
                    sVal[j]|=0x01;
                }
            }
            BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0]);
        }
        for(int j=0;j<NUM_ADC;j++){
            SPI_sensor_value[j][pin_num]=sVal[j];
        }
    }
}

void bbio::init(){
    iolib_init();
    BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);
    BBBIO_GPIO_set_dir(BBBIO_GPIO1 ,
                       port_dout_SPI[0] ,
                       port_din_SPI[0] | port_clk_SPI[0] | port_cs_SPI[0]);
    
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_din_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_cs_SPI[0]);
}

void bbio::close(){
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_din_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_clk_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,port_cs_SPI[0]);
    iolib_free();
}

void bbio::getAxes(int *ax) {
    *ax = NUM_ADC_PORT;
}

void bbio::getEncoder(int j, double *v){
    if (j < NUM_ADC_PORT) {
        (*v) = SPI_sensor_value[0][j];
    }
}

void bbio::getEncoders(double *v) {
    for (int i=0; i<NUM_ADC_PORT; i++) {
        v[i] = SPI_sensor_value[0][i];
    }
}



