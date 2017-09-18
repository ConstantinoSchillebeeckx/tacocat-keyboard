//  I2CCommunicator.cpp
//  Created by William Yager on 10/10/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Communicates over I2C

#include <stdio.h>
#include "I2CCommunicator.h"
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"

#define DATA_LENGTH 64  /*!<Data buffer length for test buffer*/
#define I2C_SCL_IO GPIO_NUM_22
#define I2C_SDA_IO GPIO_NUM_21
#define I2C_FREQ_HZ    100000     /*!< I2C master clock frequency */
#define I2C_TX_BUF (2*DATA_LENGTH)
#define I2C_RX_BUF (2*DATA_LENGTH)
#define I2C_NUM I2C_NUM_0

#define ESP_SLAVE_ADDR 0x28         /*!< ESP32 slave address, you can set any 7bit value */


uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);

I2CCommunicator::I2CCommunicator(){
	// setup I2C
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config(I2C_NUM, &conf_slave);
    i2c_driver_install(I2C_NUM, conf_slave.mode,
                       I2C_RX_BUF,
                       I2C_TX_BUF, 0);


}

bool I2CCommunicator::send_packet(const KeysDelta& key_changes){

	// prep data for sending
	for (int i = 0; i < DATA_LENGTH; i++) {
		printf("%i, %i\n",i, key_changes.deltas[i].delta);
		// if (key_changes.deltas[i].delta != 0) {
		// 	printf("%i, %i, %i\n",
		// 	i, key_changes.deltas[i].delta, key_changes.deltas[i].key);
		// }
	}

	// size_t d_size = i2c_slave_write_buffer(I2C_NUM, packet, DATA_LENGTH, 1000 / portTICK_RATE_MS);
	return true;
}

//Only does anything if we're the master. Asks for a packet from the slave.
//The slave can send whatever it wants, but a properly programmed slave
//will only send key-press packets.
Packet I2CCommunicator::request_packet(){
    Packet result;
	// TODO I2C to request data from slave
    //Slave address, buffer pointer, length, send stop
    // twi_readFrom(1, result.data, 16, true);
    return result;
}

bool I2CCommunicator::is_master(){
    return this->_is_master;
}

bool I2CCommunicator::is_slave(){
    // return i2c_master_has_been_found;
	return !this->_is_master;
}


bool Packet::is_KeyPacket(){return this->data[0] <= 5;}


Packet::Packet(){
    for (uint8_t i=0; i<16; i++) {
        this->data[i] = 0;
    }
}
Packet::Packet(const uint8_t* data){
    for (uint8_t i=0; i<16; i++) {
        this->data[i] = data[i];
    }
}

KeyPacket::KeyPacket(){
    this->data[0] = 0; //Length
}
KeyPacket::KeyPacket(const Packet& packet){
    for (uint8_t i=0; i<16; i++) {
        this->data[i] = packet.data[i];
    }
}
bool KeyPacket::is_full(){return (this->count() >= 5);}
bool KeyPacket::is_empty(){return (this->count() == 0);}
uint8_t KeyPacket::count(){return this->data[0];}

//Returns true if there's still space.
bool KeyPacket::add_KeyDelta(KeyDelta delta){
    if (!this->is_full()) {
        uint8_t index = this->data[0]*3 + 1;
        this->data[index]   = delta.key >> 8;
        this->data[index+1] = delta.key &  0xFF;
        this->data[index+2] = delta.delta;
        this->data[0]++;
    }
    return !this->is_full();
}
//Removes and returns the last KeyDelta in the packet.
KeyDelta KeyPacket::pop_KeyDelta(){
    KeyDelta result;
    if(!this->is_empty()){
        // uint8_t index = this->data[0]*3 + -2;
        // result.key  |= this->data[index] << 8;
        // result.key  |= this->data[index+1];
        // result.delta = this->data[index+2];
        // this->data[0]--;
    }
    return result;
}
