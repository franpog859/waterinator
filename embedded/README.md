# Development

To set up Arduino IDE install the [Arduino ESP32 driver](https://github.com/espressif/arduino-esp32).

Make sure that you changed the WiFi SSID and password! You should aso adjust the service URL to be sure it points to the right service. To do it change the consts defined at the beginning of the embedded code:

```c
const char* wifiSsid = "{WIFI_NAME};
const char* wifiPassword = "{WIFI_PASSWORD}";
...
const char* serviceURL = "{SERVICE_URL}";
```

To rebuild the solution and upload it to the board press the `Upload` button on Arduino IDE and press the BOOT/FLASH button on the board while connecting. It's a solution to the problem documented in [this article](https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/).

# Hardware

- Board: NODEMCU ESP-32S

