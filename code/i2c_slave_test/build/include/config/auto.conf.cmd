deps_config := \
	/Users/constantino/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/constantino/esp/esp-idf/components/bt/Kconfig \
	/Users/constantino/esp/esp-idf/components/esp32/Kconfig \
	/Users/constantino/esp/esp-idf/components/ethernet/Kconfig \
	/Users/constantino/esp/esp-idf/components/fatfs/Kconfig \
	/Users/constantino/esp/esp-idf/components/freertos/Kconfig \
	/Users/constantino/esp/esp-idf/components/log/Kconfig \
	/Users/constantino/esp/esp-idf/components/lwip/Kconfig \
	/Users/constantino/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/constantino/esp/esp-idf/components/openssl/Kconfig \
	/Users/constantino/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/constantino/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/constantino/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/constantino/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/constantino/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
