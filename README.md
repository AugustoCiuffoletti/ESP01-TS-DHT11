### Using an ESP-01 to upload to ThingSpeak

The sketch uploads temperature and humidity measurements read from a low-cost DHT11 device.

The data pin of the device is connected to the GPIO2 pin of the tiny ESP-01 board. The DHT11 shares the 3.3 power with the ESP-01.

You need to fill the file *template_secret.h* using you data, and rename it as *secret.h*.

The sketch has been successfully tested using the ESP-01 development board available on github as *esp01-devboard*.