 @echo off
 .\esptool.exe --chip esp8266 --port COM3 --baud 921600 --before default_reset --after hard_reset write_flash 0x0 .\wemos.bin
 pause