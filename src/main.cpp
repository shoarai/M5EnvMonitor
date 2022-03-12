#include <M5StickCPlus.h>

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

#include <MHZ19_uart.h>

#include "Ambient.h"

#include "config.h"

const int rxPin = 36;
const int txPin = 26;

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

const unsigned int channelId = AMBIENT_CHANNEL_ID;
const char *writeKey = AMBIENT_WRITE_KEY;

const int DISPLAY_INTERVAL_MS = 5 * 1000;
const int SEND_INTERVAL_MS = 30 * 1000;

SHT3X sht30;
Adafruit_BMP280 bme;

MHZ19_uart mhz19;

WiFiClient client;
Ambient ambient;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;
bool sendResult = false;
int count = 0;

void initEnv()
{
  Serial.println(F("ENV Unit(SHT30 and BMP280) test..."));
  M5.Lcd.printf("ENV Unit test...");

  while (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    if (M5.BtnA.wasPressed())
    {
      M5.update();
      break;
    }
    M5.update();
    delay(1000);
    M5.Lcd.printf(".");
  }
  M5.Lcd.println();
}

void setup()
{
  // Init M5StickC Plus
  M5.begin();
  M5.IMU.Init();
  pinMode(36, INPUT);
  gpio_pulldown_dis(GPIO_NUM_25);
  gpio_pullup_dis(GPIO_NUM_25);

  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);

  initEnv();

  Serial.begin(9600);
  mhz19.begin(rxPin, txPin);
  mhz19.setAutoCalibration(false);

  // Connect to WiFi
  M5.Lcd.printf("WiFi connect...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    if (M5.BtnA.wasPressed())
    {
      M5.update();
      break;
    }
    M5.update();
    delay(1000);
    M5.Lcd.printf(".");
  }
  M5.Lcd.println();
  M5.Lcd.println("WiFi connected.");

  ambient.begin(channelId, writeKey, &client);
  M5.Lcd.println("MH-Z19 is warming up now.");

  setCpuFrequencyMhz(80);
}

// uint8_t wasBtnAPressedWithUpdate()
// {
//   uint8_t wasPressed = M5.BtnA.wasPressed();
//   M5.update();
//   return wasPressed;
// }

const float MAX_BATTERY_VOLTAGE = 4.2f;
const float MIN_BATTERY_VOLTAGE = 3.0f;

int calcBatteryPercent()
{
  float vbat = M5.Axp.GetBatVoltage();
  float percent = (vbat - MIN_BATTERY_VOLTAGE) / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE);
  return roundf(percent * 100.0f);
}

void setLcdRotationByAttitude()
{
  float pitch, roll, yaw;
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  int rotation = (pitch < 0) ? 1 : 3;

  M5.Lcd.setRotation(rotation);
}

void setLcdRotationVerticalByAttitude()
{
  float pitch, roll, yaw;
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  int rotation = (roll < 0) ? 2 : 4;

  M5.Lcd.setRotation(rotation);
}

void loop()
{
  pressure = bme.readPressure() / 100;
  if (sht30.get() == 0)
  {
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("ENVII Temperature:%2.2f*C, Humidity:%0.2f%%, Pressure:%0.2fPa\r\n", tmp, hum, pressure);

  float ibat = M5.Axp.GetBatCurrent();
  int battery = calcBatteryPercent();
  int co2ppm = mhz19.getCO2PPM();
  Serial.printf("MH-Z19C CO2:%4dppm\r\n", co2ppm);

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  setLcdRotationByAttitude();

  // Display header
  if (ibat == 0)
  {
    M5.Lcd.printf("Full");
  }
  else if (ibat > 0)
  {
    M5.Lcd.printf("USB ");
  }
  else
  {
    M5.Lcd.printf("    ");
  }
  M5.Lcd.printf("            ");
  M5.Lcd.printf("%3d%%\r\n", battery);

  // Display contents
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("    %4.1f 'C\r\n", tmp);
  M5.Lcd.printf("    %4.1f %%\r\n", hum);
  M5.Lcd.printf("  %6.1f hPa\r\n", pressure);
  if (co2ppm == -1)
  {
    M5.Lcd.printf("    %4s ppm\r\n", "N/A");
  }
  else
  {
    M5.Lcd.printf("  %4d   ppm\r\n", co2ppm);
  }
  // Alert
  if (tmp <= 17 || tmp >= 28 ||
      hum <= 40 || hum >= 60)
  {
    M5.Lcd.fillRect(0, 17, 8, 92, RED);
  }
  else
  {
    M5.Lcd.fillRect(0, 17, 8, 92, GREEN);
  }

  // Send data to ambient
  if (count >= SEND_INTERVAL_MS / DISPLAY_INTERVAL_MS)
  {
    count = 0;
    ambient.set(1, co2ppm);
    ambient.set(2, tmp);
    ambient.set(3, hum);
    ambient.set(4, pressure);
    sendResult = ambient.send();
  }

  // Display footer
  if (!sendResult)
  {
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Send NG");
  }

  count++;
  delay(DISPLAY_INTERVAL_MS);
}