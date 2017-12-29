#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include "sdkconfig.h"


#define DATA_LENGTH          512  /*!<Data buffer length for test buffer*/

#define I2C_EXAMPLE_SLAVE_SCL_IO     22    /*!<gpio number for i2c slave clock  */
#define I2C_EXAMPLE_SLAVE_SDA_IO     21    /*!<gpio number for i2c slave data */
#define I2C_EXAMPLE_SLAVE_NUM I2C_NUM_0    /*!<I2C port number for slave dev */
#define I2C_EXAMPLE_SLAVE_TX_BUF_LEN  (2*DATA_LENGTH) /*!<I2C slave tx buffer size */
#define I2C_EXAMPLE_SLAVE_RX_BUF_LEN  (2*DATA_LENGTH) /*!<I2C slave rx buffer size */

#define ESP_SLAVE_ADDR 0x28         /*!< ESP32 slave address, you can set any 7bit value */


/**
 * @brief i2c slave initialization
 */
// static void i2c_example_slave_init()
// {
//     int i2c_slave_port = I2C_EXAMPLE_SLAVE_NUM;
//     i2c_config_t conf_slave;
//     conf_slave.sda_io_num = I2C_EXAMPLE_SLAVE_SDA_IO;
//     conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
//     conf_slave.scl_io_num = I2C_EXAMPLE_SLAVE_SCL_IO;
//     conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
//     conf_slave.mode = I2C_MODE_SLAVE;
//     conf_slave.slave.addr_10bit_en = 0;
//     conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
//     i2c_param_config(i2c_slave_port, &conf_slave);
//     i2c_driver_install(i2c_slave_port, conf_slave.mode,
//                        I2C_EXAMPLE_SLAVE_RX_BUF_LEN,
//                        I2C_EXAMPLE_SLAVE_TX_BUF_LEN, 0);
// }
/**
 * @brief i2c slave initialization
 */
static void i2c_example_slave_init()
{
    i2c_config_t conf_slave;
    conf_slave.sda_io_num = I2C_EXAMPLE_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_EXAMPLE_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
	
    i2c_param_config(I2C_EXAMPLE_SLAVE_NUM, &conf_slave);
    i2c_driver_install(I2C_EXAMPLE_SLAVE_NUM, conf_slave.mode,
                       I2C_EXAMPLE_SLAVE_RX_BUF_LEN,
                       I2C_EXAMPLE_SLAVE_TX_BUF_LEN, 0);
}




void task(void *ignore) {

	uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);

	// generate data
	for (int i = 0; i < DATA_LENGTH; i++) {
		data[i] = i%2;
	}

	while(1) {

		size_t d_size = i2c_slave_write_buffer(I2C_EXAMPLE_SLAVE_NUM, data, DATA_LENGTH, 1000 / portTICK_RATE_MS);
		printf("Sent %i bytes to I2C\n", d_size);

	}

}


void app_main()
{
	i2c_example_slave_init();
    xTaskCreate(task, "task", 1024 * 2, NULL, 10, NULL);
}
