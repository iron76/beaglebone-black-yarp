// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2015 CoDyCo EU porject
 * Authors: Francesco Nori
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <BeagleBoneInputOutput.h>

#define pin_spi_cs1  P9_28 // 3_17=113
#define pin_spi_other P9_29 // 3_16=111
#define pin_spi_mosi P9_30 // 3_15=112
#define pin_spi_sclk P9_31 // 3_14=110
#define pin_spi_cs2  P9_42 // 0_7 =7

#define NUM_ADC 1
#define NUM_ADC_PORT 8
#define NUM_OF_CHANNELS 8


/* set gpio pin number */
unsigned int port_clk_SPI[]={BBBIO_GPIO_PIN_16};
unsigned int port_din_SPI[]={BBBIO_GPIO_PIN_13};
unsigned int port_dout_SPI[]={BBBIO_GPIO_PIN_12};
unsigned int port_cs_SPI[]={BBBIO_GPIO_PIN_19};

/* set gpio pin number */
unsigned long SPI_sensor_value[NUM_ADC][NUM_ADC_PORT];

// SPI
bool clock_edge = false;
unsigned short resolution = 0x0FFF;
void set_SCLK(bool value) { digitalWrite(pin_spi_sclk, value); }
void set_OTHER(bool value) { digitalWrite(pin_spi_other, value); }
void set_MOSI(bool value) { digitalWrite(pin_spi_mosi, value); }
void setCS1(bool value){ digitalWrite(pin_spi_cs1, value); }
void setCS2(bool value){ digitalWrite(pin_spi_cs2, value); }
void set_clock_edge(bool value){ clock_edge = value; }
bool get_MISO(void) { return false; } // dummy
void wait_SPI(void){}
void chipSelect1(bool value){ setCS1(!value); wait_SPI(); wait_SPI(); }
void chipSelect2(bool value){ setCS2(!value); wait_SPI(); wait_SPI(); }
void init_pins()
{
    set_SCLK(LOW);
    set_MOSI(LOW);
    set_OTHER(LOW);
    setCS1(HIGH);
    setCS2(HIGH);
}

unsigned char transmit8bit(unsigned char output_data){
    unsigned char input_data = 0;
    int i;
    for(i = 7; i >= 0; i--){
        // MOSI - Master : write with down trigger
        //        Slave  : read with up trigger
        // MISO - Master : read before down trigger
        //        Slave  : write after down trigger
        set_SCLK(!clock_edge);
        set_MOSI( (bool)((output_data>>i)&0x01) );
        input_data <<= 1;
        wait_SPI();
        set_SCLK(clock_edge);
        input_data |= get_MISO() & 0x01;
        wait_SPI();
    }
    return input_data;
}

unsigned short transmit16bit(unsigned short output_data){
    unsigned char input_data_H, input_data_L;
    unsigned short input_data;
    input_data_H = transmit8bit( (unsigned char)(output_data>>8) );
    input_data_L = transmit8bit( (unsigned char)(output_data) );
    input_data = (((unsigned short)input_data_H << 8)&0xff00) | (unsigned short)input_data_L;
    return input_data;
}


void setDARegister(unsigned char ch, unsigned short dac_data){
    unsigned short register_data;
    
    if (ch < 8) {
        register_data = (((unsigned short)ch << 12) & 0x7000) | (dac_data & 0x0fff);
        chipSelect1(true);
        transmit16bit(register_data);
        chipSelect1(false);
    }
    else if (ch >= 8) {
        register_data = (((unsigned short)(ch & 0x0007) << 12) & 0x7000) | (dac_data & 0x0fff);
        chipSelect2(true);
        transmit16bit(register_data);
        chipSelect2(false);
    }
}

void init_DAConvAD5328(void) {
    set_clock_edge(false);// negative clock (use falling-edge)
    
    // initialize chip 1
    chipSelect1(true);
    transmit16bit(0xa000);// synchronized mode
    chipSelect1(false);
    
    chipSelect1(true);
    transmit16bit(0x8003);// Vdd as reference
    chipSelect1(false);
    
    // initialize chip 2
    chipSelect2(true);
    transmit16bit(0xa000);// synchronized mode
    chipSelect2(false);
    
    chipSelect2(true);
    transmit16bit(0x8003);// Vdd as reference
    chipSelect2(false);
}

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

void bbio::open(){
    
    // GPIO init
    init();
    init_pins(); // ALL 5 pins are HIGH except for GND
    init_DAConvAD5328();

    //BBBIO init
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
    SPI_read();
    if (j < NUM_ADC_PORT) {
        (*v) = SPI_sensor_value[0][j];
    }
}

void bbio::getEncoders(double *v) {
    SPI_read();
    for (int i=0; i<NUM_ADC_PORT; i++) {
        v[i] = SPI_sensor_value[0][i];
    }
}

void bbio::setRefOutput(int j, double v)
{
    unsigned short resolution = 0x0FFF;
    double pressure_coeff = v/100.0;
    if(pressure_coeff >= 0.0 && pressure_coeff <= 0.9)
        if(j >= 0 && j < NUM_OF_CHANNELS)
        {
            fprintf(stderr, "Sending %f\n", pressure_coeff);
            setDARegister(j, (unsigned short)(pressure_coeff * resolution));
        }
}

void bbio::setRefOutputs(const double *v)
{
    unsigned short resolution = 0x0FFF;
        for(int j = 0; j < NUM_OF_CHANNELS; j++)
            if(v[j]/100.0 >= 0.0 && v[j]/100.0 <= 0.9)
            {
                fprintf(stderr, "Sending %f\n", v[j]/100.0);
                setDARegister(j, (unsigned short)(v[j]/100.0 * resolution));
            }
}



