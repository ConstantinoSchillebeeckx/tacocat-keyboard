#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"

#define SDA_PIN 21
#define SCL_PIN 22

static char tag[] = "i2cscanner";

/*
Trying to get the slave keyboard to send
some data to the master.

Slave is setup on address 0x28
*/

/**
 * @brief i2c master initialization
 */
static void i2c_example_master_init()
{
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = SDA_PIN;
	conf.scl_io_num = SCL_PIN;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	i2c_param_config(I2C_NUM_0, &conf);

	i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

/*
not sure what this does
*/
esp_err_t check_slave(int i) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
	i2c_master_stop(cmd);
	esp_err_t espRc;

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);

	i2c_cmd_link_delete(cmd);

	return espRc;
}


void task_i2cscanner(void *ignore) {
	i2c_example_master_init();

	ESP_LOGD(tag, ">> i2cScanner");

	int i;
	esp_err_t espRc;
	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	printf("00:         ");
	for (i=3; i< 0x78; i++) {
		espRc = check_slave(i);
		if (i%16 == 0) {
			printf("\n%.2x:", i);
		}
		if (espRc == 0) {
			printf(" %.2x", i);
		} else {
			printf(" --");
		}

	}
	printf("\n");
	vTaskDelete(NULL);
}


void app_main()
{

    xTaskCreate(task_i2cscanner, "task_i2cscanner", 1024 * 2, NULL, 10, NULL);
}
