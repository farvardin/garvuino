avrdude -p atmega8 -c USBasp -U flash:w:avray/AY_Emul_247_2ch_m8_ay.hex -U eeprom:w:avray/Conf_serial_24MHz_2Mhz.hex -U lfuse:w:0xCE:m -U hfuse:w:0xCF:m 
