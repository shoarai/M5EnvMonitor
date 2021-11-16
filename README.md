# M5 ENV Monitor

`M5 ENV Monitor` measures and displays the environmental variables:

- Temperature
- Humidity
- Barometric pressure
- CO2

<img src="https://shoarai.com/blog/wp-content/uploads/2021/10/M5EnvMonitor-scaled.jpg" width="320px" alt="M5 ENV Monitor screen">

## Device and Sensors

- M5StickC Plus
- ENV II
- MHZ-19c

## Connections

M5StickC Plus --(Grove Cable)-- ENV II 

| M5StickC Plus Pin | MH-Z19C Pin |
| --- | --- |
| GND | GND |
| 5V→ | VIN |
| G26 | Rx |
| G36 | Tx |

## Service Account(Optional)

[Ambient](https://ambidata.io/)  
The environmental variables can be sent to Ambient of data visualization service, then displayed in charts.

## Setup

1. Import the project from PlatformIO in Visual Studio Code.

2. Create `src/config.h`.

    ```cpp:src/config.h
    #define WIFI_SSID "{WiFi SSID}"
    #define WIFI_PASSWORD "{WiFi password}"

    #define AMBIENT_CHANNEL_ID {Ambient channel ID}
    #define AMBIENT_WRITE_KEY "{Ambient write key}"
    ```

3. Upload to M5StickC Plus by PlatformIO.

## License

MIT License

## Blog

[shoarai.com](https://shoarai.com/2021/10/31/m5env-monitor/)
