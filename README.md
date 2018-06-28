# SHT31

Not see a port?

`ls -lha /dev/ttyUSB*`

should return something like, remember to try different USB cables

`crw-rw---- 1 root dialout 188, 0 Jun 28 09:12 /dev/ttyUSB0`

http://docs.platformio.org/en/latest/faq.html#platformio-udev-rules

#Dependencies
  NTPClient ` platformio lib install 551`
  `pio lib install "ThingSpeak"`
  `pio lib install "Adafruit SHT31 Library"`
