#if defined(ARDUINO_ARCH_AMEBAD)//defined(RTL8722DM)
#ifndef RTL_BLE_UTILITIES_H
#define RTL_BLE_UTILITIES_H

#include <Arduino.h>
#include "BLEDevice.h"

#define UUID_Service  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define UUID_Receive  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define UUID_Transmit "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define DEBUG

#ifndef UNSED
#define UNUSED(expr) (void)(expr)
#endif

//#include <string>


class ameba_ble
{
public:
void begin(String a);
void write(uint8_t a);
void write(String x);
void write(int a);
void write(float a);
uint8_t available();
uint8_t read();
void stop();
};
extern bool BLE_status;        //extern BLE_status
extern ameba_ble amebaBLE;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

uint32_t ledcSetup(uint8_t chan, uint32_t freq, uint8_t bit_num);
void ledcAttachPin(uint8_t pin, uint8_t chan);
void ledcWrite(uint8_t chan, uint32_t duty);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif
#endif
