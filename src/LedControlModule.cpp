#define FROM_DABBLE_LIBRARY
#include "DabbleESP32.h"
#include "LedControlModule.h"

#ifdef ESP32
#define MAX_PWM_CHANNELS	8
uint8_t freeChannel =0,prevPin=0,currentChannel=0;
bool pinType=0;   // flag to differentiate pin behaviour means whether pin will be traeted as PWM or digital
#elif defined(ARDUINO_ARCH_AMEBAD)//defined(RTL8722DM) // AMEBA PORTING
#define MAX_PWM_CHANNELS	4 // for RTL8720DN
uint8_t freeChannel =0,prevPin=0,currentChannel=0;
bool pinType=0;   // flag to differentiate pin behaviour means whether pin will be traeted as PWM or digital
#endif

LedControlModule::LedControlModule(): ModuleParent(LEDCONTROL_ID)
{
}

static uint8_t pwm_list[] = {7,8,11,12}; // for RTL8720DN only. AMEBA PORTING


void LedControlModule::processData()
{
	byte   functionId =getDabbleInstance().getFunctionId();

	if(functionId == 0x01)
	{
		pin = getDabbleInstance().getArgumentData(0)[0];

		if(prevPin != pin)
		{
		    pinType = 0;
			if(1) //freeChannel < MAX_PWM_CHANNELS) // check pin as pwm functinal.
			{
				#ifdef DEBUG
				Serial.println("Pin is treated as pwm pin");
				#endif
				/*#ifdef DEBUG
				Serial.print("Channels: ");
				Serial.print(freeChannel);
				Serial.print(" ");
				Serial.println(currentChannel);
				#endif*/
				//currentChannel = freeChannel;
				//ledcAttachPin(pin,currentChannel);
				//ledcSetup(currentChannel,100,8);
				//freeChannel++;
				/*#ifdef DEBUG
				Serial.print("freeChannels: ");
				Serial.println(freeChannel);
				#endif*/

                for(int i=0;i < (int) sizeof(pwm_list); i++) // AMEBA PORTING
                {
                    if(pwm_list[i] == pin)
                    {

#ifdef DEBUG
       printf("pwm pin %d init.\n\r",pin);
#endif
                        pinMode(pin,OUTPUT);
                        analogWrite(pin,map(brightness,0,100,0,255));
				        pinType =1;     // pin is a PWM type
				        break;
                    }
                }
			}

			if(pinType == 0)
			{
				#ifdef DEBUG
				Serial.println("Pin is treated as digital pin");
				#endif
				pinMode(pin,OUTPUT);
				//pinType = 0;      // pin is a digital type
			}
		}
		prevPin = pin;
	}
	else if (functionId == 0x03)    //ON as per Brightness value
	{
	    brightness = getDabbleInstance().getArgumentData(0)[0];
	    #ifdef DEBUG
			Serial.println(brightness);
	    #endif
	    value  = map(brightness,0,100,0,255);
	    #ifdef DEBUG
			Serial.println(value);
	    #endif
	}
	else if(functionId == 0x02)         //OFF
	{
	   brightness =0;
	   value=0;
	}

	if(pinType == 1 && pin!= 0)      //for PWM type pin
	{

#ifdef DEBUG
        printf("analogWrite(%d,%d)\n\r",pin,value);
#endif

		//Serial.println("Value Assigned");
		analogWrite(pin, value); //ledcWrite(currentChannel,value);
	}
	else                             //for digital type pin
	{
		//Serial.println("Value Assigned");
		if(value == 255)
		{
#ifdef DEBUG
            printf("digitalWrite(%d,%d)\n\r",pin,HIGH);
#endif
			digitalWrite(pin,HIGH);
		}
		else
		{
#ifdef DEBUG
            printf("digitalWrite(%d,%d)\n\r",pin,LOW);
#endif
			digitalWrite(pin,LOW);
		}
	}
}

uint8_t LedControlModule::readBrightness()
{
return brightness;
}

bool LedControlModule::getpinState()
{
 if(brightness>0)
	return 1;
 else
	return 0;
}

uint8_t  LedControlModule::getpinNumber()
{
 return pin;
}

