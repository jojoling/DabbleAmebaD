#if defined(ARDUINO_ARCH_AMEBAD)//defined(RTL8722DM)
#include "ameba_BLEUtilities.h"
bool BLE_status=0;

bool isDeviceConnected=false;
bool prevDeviceConnected = false;
uint8_t tx_Value = 0;
uint8_t rxdatalength=0;
uint8_t bytesremaining=0;
uint8_t* rxdataBuffer=NULL;
bool newDataReceived = 0;


#define STRING_BUF_SIZE 100

BLEService UartService(UUID_Service);
BLECharacteristic Rx(UUID_Receive);
BLECharacteristic Tx(UUID_Transmit);
BLEAdvertData advdata;
BLEAdvertData scndata;
bool notify = false;


//void readCB (BLECharacteristic* chr, uint8_t connID) {
//#ifdef DEBUG
//    printf("**** Characteristic %s read by connection %d \n", chr->getUUID().str(), connID);
//#endif
//
//}

void writeCB (BLECharacteristic* chr, uint8_t connID) {

    UNUSED(connID);

#if 0//#ifdef DEBUG
    printf("**** Characteristic %s write by connection %d :\n", chr->getUUID().str(), connID);
#endif

    if (chr->getDataLen() > 0) {
        int count = chr->getDataLen();
        //Serial.print("Received string: ");
        //Serial.print(chr->readString());
        //Serial.println();
#ifdef DEBUG
        printf("len = %d\n\r",count);
#endif
        //std::string rx_Value(chr->readString().c_str());
        if(newDataReceived == 1)delete [] rxdataBuffer;
        newDataReceived = 1;

        if (count > 0) {
            rxdataBuffer = new uint8_t[count];
            chr->getData(rxdataBuffer, count);
            for (int i = 0;i< count; i++)
            {
                //rxdataBuffer[i] = rx_Value[i];

        #ifdef DEBUG
            Serial.print(rxdataBuffer[i]);
            Serial.print(" ");
        #endif
            }
        #ifdef DEBUG
            Serial.println();
        #endif
            rxdatalength= count;//rx_Value.length();
            bytesremaining= count;//rx_Value.length();

        }
    }
}

void notifCB (BLECharacteristic* chr, uint8_t connID, uint16_t cccd) {

#ifdef DEBUG
            printf("**** cccd = %d\n\r",cccd);
#endif

    if (cccd & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {

        if(BLE.connected())
            isDeviceConnected = true;
        else
          isDeviceConnected = false;

#ifdef DEBUG
        printf("**** isDeviceConnected = %d\n\r",isDeviceConnected);
        printf("**** Notifications enabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        notify = true;
#endif
    } else {
#ifdef DEBUG
        printf("**** Notifications disabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        notify = false;
#endif
    }
}

void ameba_ble::begin(String a)
{

  advdata.addFlags(GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);
  advdata.addCompleteName(a.c_str());
  scndata.addCompleteServices(BLEUUID(UUID_Service));

  Rx.setWriteProperty(true);
  Rx.setWritePermissions(GATT_PERM_WRITE);
  Rx.setWriteCallback(writeCB);
  Rx.setBufferLen(STRING_BUF_SIZE);

  //Tx.setReadProperty(true);
  //Tx.setReadPermissions(GATT_PERM_READ);
  //Tx.setReadCallback(readCB);

  Tx.setNotifyProperty(true);
  //Tx.setCCCDCallback(notifCB);
  Tx.setBufferLen(STRING_BUF_SIZE);

  UartService.addCharacteristic(Tx);
  UartService.addCharacteristic(Rx);

  BLE.init();
  BLE.setDeviceName(String(a.c_str()));

  BLE.configAdvert()->setAdvData(advdata);
  BLE.configAdvert()->setScanRspData(scndata);
  BLE.configServer(1);
  BLE.addService(UartService);

  BLE.beginPeripheral();

}

void ameba_ble::write(uint8_t a)
{

	 if (isDeviceConnected) {

        Tx.writeData8(a);
        Tx.notify(0);
	    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!isDeviceConnected && prevDeviceConnected) {


#ifdef DEBUG
        Serial.println("**** bleServer->startAdvertising()");
#endif

        delay(500); // give the bluetooth stack the chance to get things ready


        // <TODO> bleServer->startAdvertising(); // restart advertising
		#ifdef DEBUG
        Serial.println("start advertising");
		#endif
        prevDeviceConnected = isDeviceConnected;
    }

}

void ameba_ble::write(String x)
{

	 if (isDeviceConnected) {

        Tx.writeString(x);
        Tx.notify(0);
	    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!isDeviceConnected && prevDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        //<TODO> bleServer->startAdvertising(); // restart advertising
		#ifdef DEBUG
        Serial.println("start advertising");
		#endif
        prevDeviceConnected = isDeviceConnected;
    }

}

void ameba_ble::write(int a)
{

	 if (isDeviceConnected) {

        Tx.writeData32(a);
        Tx.notify(0);
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!isDeviceConnected && prevDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        //<TODO> bleServer->startAdvertising(); // restart advertising
		#ifdef DEBUG
        Serial.println("start advertising");
		#endif
        prevDeviceConnected = isDeviceConnected;
    }

}

void ameba_ble::write(float a)
{

	 if (isDeviceConnected) {

        Tx.writeData32((uint32_t)a);
        Tx.notify(0);
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

    // disconnecting
    if (!isDeviceConnected && prevDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        //<TODO>bleServer->startAdvertising(); // restart advertising
		#ifdef DEBUG
        Serial.println("start advertising");
		#endif
        prevDeviceConnected = isDeviceConnected;
    }

}

uint8_t ameba_ble::available()
{
  return bytesremaining;
}
uint8_t ameba_ble::read()
{
	if(bytesremaining > 0)
	{
		uint8_t a = rxdataBuffer[rxdatalength-bytesremaining];
		bytesremaining--;
		return a;
	}
	else
	{
		return 0;
	}
}

void ameba_ble::stop()
{
	//btStop();
	BLE.deinit();
}

//Esp32ble esp32ble;
ameba_ble amebaBLE;

#endif
