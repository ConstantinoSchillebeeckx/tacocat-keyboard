/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"


#include "ButtonDebouncer.h"
#include "ButtonDeltaDetector.h"
#include "HardwareController.h"
#include "MasterNotifier.h"
#include "SlaveNotifier.h"
#include "I2CCommunicator.h"
#include "USBCommunicator.h"
#include "KeyMapper.h"


// Also change KeyMapper.cpp key layout
// bool is_master = false; // True if connected to host PC; otherwise false
bool is_master = true; // True if connected to host PC; otherwise false

void tacocat(void *pvParameter)
{
	//Manages hardware functions, including scanning keys. Constructor configures hardware.
	HardwareController      hardware;

	I2CCommunicator i2c_communicator(is_master); //Start I2C
	USBCommunicator usb_communicator(is_master); //Start USB hardware

	//Holds information from the master about the state of the indicator LEDs
	uint8_t led_status = 0;

	//Manages debouncing buttons. The argument is the number of samples needed to detect a change in state.
	ButtonDebouncer         debouncer(1); // TODO was 3, that seems too high
	//Detects changes in button states.
	ButtonDeltaDetector     push_detector;
	//Maps physical button deltas to USB key deltas. This is where to look if you want to change the key layout.
	KeyMapper               key_mapper;
	//Talks to the computer over USB or the masterboard over I2C. Constructor hangs until a master is found (either USB or I2C)
	MasterNotifier          master(usb_communicator, i2c_communicator);
	//Represents the slave, if one exists. Acts as a dummy slave otherwise. Tries to connect to slave during construction.
	SlaveNotifier           slave(i2c_communicator);

	// i2c_communicator.is_slave_connected();

	while(1) {
		ButtonsState raw_state = hardware.update();
		ButtonsState debounced_state = debouncer.update(raw_state);
		ButtonsDelta button_changes = push_detector.update(debounced_state);
		KeysDelta key_changes = key_mapper.resolve(button_changes);

		//If we don't have a slave, this returns a KeysDelta full of zeros
		KeysDelta slave_key_changes = slave.update(led_status);

		// KeysDelta slave_key_changes;
		if (is_master && false) {
			for(uint8_t i = 0; i < 35; i++) {
				// if (button_changes.deltas[i] != 0) {
					printf("%i, %i, %i, %i, %i\n",
					i, key_changes.deltas[i].delta, key_changes.deltas[i].key, slave_key_changes.deltas[i].delta, slave_key_changes.deltas[i].key
					// i, button_changes.deltas[i], button_changes.deltas[i]
					);
				// }
			}
			printf("\n");
			vTaskDelay(( 1000 ) / portTICK_RATE_MS);
		}

		//Sends all key press/release events to the USB or I2C master and
		//returns the state of the keyboard LEDs, as reported by the master
		led_status = master.notify(key_changes, slave_key_changes);
	}
	esp_restart();
}


extern "C" void app_main()
{

	nvs_flash_init();
	if (!is_master) {
		// hoping this will help connect the two boards
		vTaskDelay(( 1000 ) / portTICK_RATE_MS);
	}
    xTaskCreate(&tacocat, "tacocat", 2048, NULL, 5, NULL);


}
