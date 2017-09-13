AVR Emulator of IC AY-8910, AY-3-8910, AY-3-8912 for Atmega8/48/88/168/328
Version 24.7 (08.07.2017) 2 & 3 channel versions

- Improved noise generator and fixed other generators.

New serial protocol from version 14.0
- Baud Rate: 57600
- Data Bits: 8
- Stop Bits: 1
- Parity: NONE
- Registers sent as a pair of values 1) Register number 0-13, 2) Register value
- To synchronize, just send 0xFF at start of sending

Features:

- Complete chip emulation
- Parallel mode support (Read mode also supported)
- Serial mode support using pin 2 (RX) at speed 57600
- Very fast INT interrupts for parallel mode

Uploading/Programming:

	Use avrdude and USBAsp programmer
ATMEGA8 =====================================================
	avrdude -p m8 -c USBasp -U flash:w:AY_Emul_XXX_Nch_KK_MM.hex -U eeprom:w:Conf_XXX_YYMHz_ZZMhz.hex -U lfuse:w:0xCE:m -U hfuse:w:0xCF:m
Example:
	avrdude -p m8 -c USBasp -U flash:w:AY_Emul_245_2ch_m8_ay.hex -U eeprom:w:Conf_standard_27MHz_1_75Mhz.hex -U lfuse:w:0xCE:m -U hfuse:w:0xCF:m
=============================================================

ATMEGA48 ====================================================
	avrdude -p m48 -c USBasp -U flash:w:AY_Emul_XXX_Nch_KK_MM.hex -U eeprom:w:Conf_XXX_YYMHz_ZZMhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
Example:
	avrdude -p m48 -c USBasp -U flash:w:AY_Emul_245_2ch_m48_ay.hex -U eeprom:w:Conf_standard_27MHz_1_75Mhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
=============================================================

ATMEGA88 ====================================================
	avrdude -p m88 -c USBasp -U flash:w:AY_Emul_XXX_Nch_KK_MM.hex -U eeprom:w:Conf_XXX_YYMHz_ZZMhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
Example:
	avrdude -p m88 -c USBasp -U flash:w:AY_Emul_245_2ch_m88_ay.hex -U eeprom:w:Conf_standard_27MHz_1_75Mhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
=============================================================

ATMEGA168 ===================================================
	avrdude -p m168 -c USBasp -U flash:w:AY_Emul_XXX_Nch_KK_MM.hex -U eeprom:w:Conf_XXX_YYMHz_ZZMhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
Example:
	avrdude -p m168 -c USBasp -U flash:w:AY_Emul_245_2ch_m168_ay.hex -U eeprom:w:Conf_standard_27MHz_1_75Mhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m
=============================================================

ATMEGA328 ===================================================
	avrdude -p m328p -c USBasp -U flash:w:AY_Emul_XXX_Nch_KK_MM.hex -U eeprom:w:Conf_XXX_YYMHz_ZZMhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdf:m -U efuse:w:0xfd:m
Example:
	avrdude -p m328p -c USBasp -U flash:w:AY_Emul_245_2ch_m328_ay.hex -U eeprom:w:Conf_standard_27MHz_1_75Mhz.hex -U lfuse:w:0xee:m -U hfuse:w:0xdf:m -U efuse:w:0xfd:m
=============================================================

Files:
>> Emulator -------
	AY_Emul_XXX_Nch_KK_MM.hex
		XXX - version number
		N - number of channels 2 or 3
		KK - MCU type (m8 - Atmega8, m48 - Atmega48, etc...)
		MM - volume table type ay or ym
	Examples:
	AY_Emul_XXX_2ch_m8_ay.hex 	- emulator HEX file to flash (2 channel version with AY volume table for Atmega8)
	AY_Emul_XXX_3ch_m328_ym.hex 	- emulator HEX file to flash (3 channel version with YM volume table for Atmega328)
	AY_Emul_XXX_2ch_m88_ay_speaker.hex 	- emulator HEX file to flash (2 channel version with AY volume table for Atmega88 with speaker port input on PD1)
	
>> Config --------
	Conf_XXX_YYMHz_ZZMhz.hex	- configuration file for emulator to write to EEPROM
		where XXX - standard, parallel or serial
			standard - use both types of connection parallel and serial
			parallel - use only parallel mode connection for using in real hardware (e.g. ZX-Spectrum)
			serial - use only serial mode connection (use RX pin on atmega8 with USB->TTL converter and AYPlayer or other software)
		YY - Atmega oscillator frequency
		ZZ - AY/YM chip oscillator frequency emulation 1_75 = 1.75MHz, 2 - 2MHz

ORIGIN: http://www.avray.ru