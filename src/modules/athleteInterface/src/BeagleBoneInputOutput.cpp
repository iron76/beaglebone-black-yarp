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
#define NUM_OF_CHANNELS 8
#define NUM_DAC 2

// #define pin_spi_mosi P9_30 // 3_15=112
// #define pin_spi_sclk P9_31 // 3_14=110
// PIN pin_spi_cs[]={P9_28,P9_42}; // 3_17=113, 0_7 =7

//sclk_bbbio         BBBIO_GPIO_PIN_16
//mosi_bbbio         BBBIO_GPIO_PIN_13
//miso_bbbio         BBBIO_GPIO_PIN_12
//ss_bbbio           BBBIO_GPIO_PIN_19

/* set gpio pin number */

/* set gpio pin number */
unsigned long SPI_sensor_value[NUM_ADC][NUM_ADC_PORT];

// SPI
void wait_SPI(void){}
bool clock_edge = false;
unsigned short resolution = 0x0FFF;
void setCS(bool value, PIN pin){ digitalWrite(pin, !value); wait_SPI(); wait_SPI();}
void set_clock_edge(bool value){ clock_edge = value; }
bool get_MISO(void) { return false; } // dummy

void bbio::init_pins()
{
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Running init_pins\n");
#endif
    //set_SCLK(LOW);
    digitalWrite((PIN) *m_pin_spi_sclk[0], !clock_edge);
    
    //set_MOSI(LOW);
    digitalWrite((PIN) *m_pin_spi_mosi[0], LOW);
    setCS(LOW, (PIN) *m_pin_spi_cs[0]);
    setCS(LOW, (PIN) *m_pin_spi_cs[1]);
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "init_pins terminated\n");
#endif
    
}

unsigned char bbio::transmit8bit(unsigned char output_data, int j){
    unsigned char input_data = 0;
    int i;
    for(i = 7; i >= 0; i--){
        // MOSI - Master : write with down trigger
        //        Slave  : read with up trigger
        // MISO - Master : read before down trigger
        //        Slave  : write after down trigger
        
        //set_SCLK(!clock_edge);
        digitalWrite((PIN) *m_pin_spi_sclk[j], !clock_edge);
        
        //set_MOSI( (bool)((output_data>>i)&0x01) );
        digitalWrite((PIN) *m_pin_spi_mosi[j], (bool)((output_data>>i)&0x01));
        
        input_data <<= 1;
        wait_SPI();
        
        //set_SCLK(clock_edge);
        digitalWrite((PIN) *m_pin_spi_sclk[j], !clock_edge);
        
        input_data |= get_MISO() & 0x01;
        wait_SPI();
    }
    return input_data;
}

unsigned short bbio::transmit16bit(unsigned short output_data, int j){
    unsigned char input_data_H, input_data_L;
    unsigned short input_data;
    input_data_H = transmit8bit( (unsigned char)(output_data>>8) , j);
    input_data_L = transmit8bit( (unsigned char)(output_data) , j);
    input_data = (((unsigned short)input_data_H << 8)&0xff00) | (unsigned short)input_data_L;
    return input_data;
}

void bbio::setDARegister(unsigned char j, unsigned short dac_data){
    unsigned short register_data;
    
    
    register_data = (((unsigned short)j << 12) & 0x7000) | (dac_data & 0x0fff);
    setCS(true, (PIN) *m_pin_spi_cs[j]);
    transmit16bit(register_data, j);
    setCS(false, (PIN) *m_pin_spi_cs[j]);
}

void bbio::init_DAConvAD5328(void) {
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Running init_DAConvAD5328\n");
#endif
    set_clock_edge(false);// negative clock (use falling-edge)
    
    for(int i = 0; i < NUM_DAC; i++)
    {
        // initialize chip i
        setCS(true, (PIN) *m_pin_spi_cs[i]);
        transmit16bit(0xa000, i);// synchronized mode
        setCS(false, (PIN) *m_pin_spi_cs[i]);
        
        setCS(true, (PIN) *m_pin_spi_cs[i]);
        transmit16bit(0x8003, i);// Vdd as reference
        setCS(false, (PIN) *m_pin_spi_cs[i]);
    }
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "init_DAConvAD5328 terminated\n");
#endif
}

void bbio::SPI_read()
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
        BBBIO_GPIO_high(BBBIO_GPIO1 ,m_port_cs_SPI[0]);
        BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0] | m_port_din_SPI[0]);
        BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_cs_SPI[0]);
        
        commandout=pin_num;
        commandout|=0x18;
        commandout<<=3;
        
        //wait_SPI();
        
        for(int i=0;i<5;i++){
            if(commandout&0x80){
                BBBIO_GPIO_high(BBBIO_GPIO1 ,m_port_din_SPI[0]);
            }
            else{
                BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_din_SPI[0]);
            }
            commandout<<=1;
            BBBIO_GPIO_high(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
            BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
        }
        
        //sleep(1);
        
        for(int i=0;i<2;i++){
            BBBIO_GPIO_high(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
            BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
        }
        for(int i=0;i<12;i++){
            BBBIO_GPIO_high(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
            for(int j=0;j<NUM_ADC;j++){
                sVal[j]<<=1;
            }
            for(int j=0;j<NUM_ADC;j++){
                if(BBBIO_GPIO_get(BBBIO_GPIO1 ,m_port_dout_SPI[j])){
                    sVal[j]|=0x01;
                }
            }
            BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
        }
        for(int j=0;j<NUM_ADC;j++){
            SPI_sensor_value[j][pin_num]=sVal[j];
        }
    }
}

void bbio::allocate(){
    m_port_clk_SPI  = new unsigned int[NUM_ADC];
    m_port_din_SPI  = new unsigned int[NUM_ADC];
    m_port_dout_SPI = new unsigned int[NUM_ADC];
    m_port_cs_SPI   = new unsigned int[NUM_ADC];
    
    m_pin_spi_sclk  = new PIN*[NUM_DAC];
    m_pin_spi_miso  = new PIN*[NUM_DAC];
    m_pin_spi_mosi  = new PIN*[NUM_DAC];
    m_pin_spi_cs    = new PIN*[NUM_DAC];
    
}

void bbio::open(){
    
    // GPIO init
#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Initializing the DAC converter \n");
#endif
    init();
    init_pins(); // ALL 5 pins are HIGH except for GND
    init_DAConvAD5328();

#ifdef _ENABLE_DEBUG_
    fprintf(stderr, "Initializing the PINS \n");
#endif
    
    //BBBIO init
    iolib_init();
    BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);
    BBBIO_GPIO_set_dir(BBBIO_GPIO1 ,
                       m_port_dout_SPI[0] ,
                       m_port_din_SPI[0] | m_port_clk_SPI[0] | m_port_cs_SPI[0]);
    
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_din_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_cs_SPI[0]);
    
}

void bbio::close(){
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_din_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_clk_SPI[0]);
    BBBIO_GPIO_low(BBBIO_GPIO1 ,m_port_cs_SPI[0]);
    iolib_free();
    
    delete m_port_clk_SPI;
    delete m_port_din_SPI;
    delete m_port_dout_SPI;
    delete m_port_cs_SPI;
    
    delete m_pin_spi_sclk;
    delete m_pin_spi_miso;
    delete m_pin_spi_mosi;
    delete m_pin_spi_cs;
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

//BBBIO
void bbio::set_m_port_clk_SPI(int i, int pin)
{
    if (i >=0 && i< NUM_ADC)
        m_port_clk_SPI[i]  = pin;
}

void bbio::set_m_port_din_SPI(int i, int pin)
{
    if (i >=0 && i< NUM_ADC)
        m_port_din_SPI[i]  = pin;
}

void bbio::set_m_port_dout_SPI(int i, int pin)
{
    if (i >=0 && i< NUM_ADC)
        m_port_dout_SPI[i]  = pin;
}

void bbio::set_m_port_cs_SPI(int i, int pin)
{
    if (i >=0 && i< NUM_ADC)
        m_port_cs_SPI[i]  = pin;
}

//GPIO
void bbio::set_m_pin_spi_sclk(int i, PIN *pin)
{
    if (i >=0 && i< NUM_ADC)
        m_pin_spi_sclk[i]  = pin;
}

void bbio::set_m_pin_spi_miso(int i, PIN *pin)
{
    if (i >=0 && i< NUM_ADC)
        m_pin_spi_miso[i]  = pin;
}

void bbio::set_m_pin_spi_mosi(int i, PIN *pin)
{
    if (i >=0 && i< NUM_ADC)
        m_pin_spi_mosi[i]  = pin;
}

void bbio::set_m_pin_spi_cs(int i, PIN *pin)
{
    if (i >=0 && i< NUM_ADC)
        m_pin_spi_cs[i]  = pin;
}

