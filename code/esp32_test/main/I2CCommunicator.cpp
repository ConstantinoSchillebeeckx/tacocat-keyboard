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

#define DATA_LENGTH 105  // we have 35 keys, and we send {key, delta} for each
#define I2C_SCL_IO GPIO_NUM_22
#define I2C_SDA_IO GPIO_NUM_21
#define I2C_FREQ_HZ    100000     /*!< I2C master clock frequency */
#define I2C_TX_BUF (2*DATA_LENGTH)
#define I2C_RX_BUF (2*DATA_LENGTH)
#define I2C_NUM I2C_NUM_0

#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

#define ESP_SLAVE_ADDR 0x28         /*!< ESP32 slave address, you can set any 7bit value */

static char tag[] = "i2c";


uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);

I2CCommunicator::I2CCommunicator(bool is_master){

	this->_is_master = is_master;

    i2c_config_t conf;
    conf.sda_io_num = I2C_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;

	if (is_master) {

		printf("Setting up I2C as a master.\n");
		// setup I2C as master
	    conf.mode = I2C_MODE_MASTER;
		conf.master.clk_speed = I2C_FREQ_HZ;
	    i2c_param_config(I2C_NUM, &conf);
	    i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);

		// check if a slave is connected
		int i;
		esp_err_t espRc;
		this->has_slave = false;
		printf("Checking for slave...\n");
		for (i=3; i< 0x78; i++) {
			i2c_cmd_handle_t cmd = i2c_cmd_link_create();
			i2c_master_start(cmd);
			i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
			i2c_master_stop(cmd);

			espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
			if (espRc == 0 && i == 40) { // TODO address 0x28 (40) is hardcoded :(
				printf("Found slave at %.2x\n", i);
				this->has_slave = true;
			}
			//ESP_LOGD(tag, "i=%d, rc=%d (0x%x)", i, espRc, espRc);
			i2c_cmd_link_delete(cmd);
		}

		if (!this->has_slave) {
			ESP_LOGI(tag, "No slave found!\n");
		}



	} else {
		printf("Setting up I2C as a slave on address %.2x.\n", ESP_SLAVE_ADDR);
		// setup I2C as slave
	    conf.mode = I2C_MODE_SLAVE;
	    conf.slave.addr_10bit_en = 0;
	    conf.slave.slave_addr = ESP_SLAVE_ADDR;
	    i2c_param_config(I2C_NUM, &conf);
	    i2c_driver_install(I2C_NUM, conf.mode,
	                       I2C_RX_BUF,
	                       I2C_TX_BUF, 0);
	}

	// initialize i2c packet
	for (int i = 0; i < DATA_LENGTH; i++) {
		data[i] = 0;
	}

}

bool I2CCommunicator::send_packet(const KeysDelta& key_changes){

	// prep data for sending
	// not sure how to send a struct over I2C
	// so we change the key_changes struct
	// into a flat object like [key, delta, key, delta]
	// however key is a 16bit number, so we split it up into two 8 bits
	// so we're actually senging [key bit1, key bit2, delta, ...]
	int count = 0;
	for (int i = 0; i < DATA_LENGTH/3; i++) {
		data[count] = key_changes.deltas[i].key >> 8; // MSB
		count++;
		data[count] = key_changes.deltas[i].key & 0XFF; // LSB
		count++;
		data[count] = key_changes.deltas[i].delta;
		count++;

		if (key_changes.deltas[i].delta != 0) {
			printf("%i %i %i %i %i %i\n", i, data[count-3], data[count-2], data[count-1], key_changes.deltas[i].key, key_changes.deltas[i].delta);
		}

	}

	size_t d_size = i2c_slave_write_buffer(I2C_NUM, data, DATA_LENGTH, 10 / portTICK_RATE_MS);
	return true;
}

//Only does anything if we're the master. Asks for a packet from the slave.
//The slave can send whatever it wants, but a properly programmed slave
//will only send key-press packets.
KeysDelta I2CCommunicator::request_packet(){
	KeysDelta result = {};


	// get slave data
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( ESP_SLAVE_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
	i2c_master_read(cmd, data, DATA_LENGTH - 1, ACK_VAL);
	i2c_master_read_byte(cmd, data + DATA_LENGTH - 1, NACK_VAL);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, 10 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if (ret == ESP_OK) {
		ESP_LOGI(tag, "OK");
	} else if (ret == ESP_ERR_INVALID_ARG){
		ESP_LOGI(tag, "ESP_ERR_INVALID_ARG");
	} else if (ret == ESP_FAIL){
		ESP_LOGI(tag, "ESP_FAIL");
	} else if (ret == ESP_ERR_INVALID_STATE){
		ESP_LOGI(tag, "ESP_ERR_INVALID_STATE");
	} else if (ret == ESP_ERR_TIMEOUT){
		ESP_LOGI(tag, "ESP_ERR_TIMEOUT");
	}

	// reformat slave data into proper format
	// incoming format is [key bit1, key bit2, delta, ...]
	int count = 0;
	int msb = 0;
	int lsb = 0;
	for (int i = 0; i < DATA_LENGTH; i++) {
		if (i%3 == 0) {
			msb = data[i];
			// printf("%i ", msb);
			// result.deltas[count].key = data[i]; // MSB
		} else if (i%3 == 1) {
			lsb = data[i];
			// printf("%i ", lsb);
			result.deltas[count].key = (msb<<8)|lsb;
			// result.deltas[count].key = (result.deltas[count].key << 8)|data[i]; // LSB
		} else {
			if (data[i] == 255) {
				result.deltas[count].delta = -1;
			} else {
				result.deltas[count].delta = data[i];
			}
			// printf("%i\n", result.deltas[count].delta);

			count++;
			// 128 1 "C"
		}
	}
	return result;

}

bool I2CCommunicator::is_master(){
    return this->_is_master;
}

bool I2CCommunicator::is_slave(){
    // return i2c_master_has_been_found;
	return !this->_is_master;
}
