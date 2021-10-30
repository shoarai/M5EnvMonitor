# M5 ENV Monitor

`M5 ENV Monitor` measures and displays the environmental parameters:

- Temperature
- Humidity
- Pressure
- CO2

These parameters can be sent to Ambient of data visualization service, then displayed in charts.

## Device and Sensors

- M5StickC Plus
- ENV II
- MHZ-19c

## Cloud Service

- Ambient

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
