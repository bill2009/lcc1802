::modified 16-12-18 to use more recent avrdude
:: 20-05-09 change to com4
@mode com4: rts=on >NUL
@"C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avrdude" -C"C:\Program Files (x86)\Arduino\hardware\tools\avr/etc/avrdude.conf" -c avrispv2 -P COM8 -p m328 -v 
rem -F -u -V  -D -patmega644p -cstk500v1 -P\\.\COM4 -b57600 -U flash:w:%1.hex:i %2 %3
rem avrdude -p m328p -P COM2 -c avrispv2 -U flash:w:BlinkLED.hex 
