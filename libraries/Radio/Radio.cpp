/*
   Radio.cpp - Library 
   based on the code posted on rcarduino.blogspot.com
 */


#include "Radio.h"
#include "Utils.h"

// shared variables are updated by the ISR and read by loop.
// In loop we immediatley take local copies so that the ISR can keep ownership of the
// shared ones. To access these in loop
// we first turn interrupts off with noInterrupts
// we take a copy to use in loop and the turn interrupts back on
// as quickly as possible, this ensures that we are always able to receive new signals
volatile uint16_t unCh1InShared;
volatile uint16_t unCh2InShared;
volatile uint16_t unCh3InShared;
volatile uint16_t unCh4InShared;
volatile uint16_t unCh5InShared;
volatile uint16_t unCh6InShared;



extern volatile uint8_t bUpdateFlagsShared;


// These are used to record the rising edge of a pulse in the calcInput functions
uint32_t ulCh1Start;
uint32_t ulCh2Start;
uint32_t ulCh3Start;
uint32_t ulCh4Start;
uint32_t ulCh5Start;
uint32_t ulCh6Start;

uint16_t unCh1In;
uint16_t unCh2In;
uint16_t unCh3In;
uint16_t unCh4In;
uint16_t unCh5In;
uint16_t unCh6In;



void getRadio(int pChannels[])
{ 

	uint16_t ch1=0;
	uint16_t ch2=0;
	uint16_t ch3=0;
	uint16_t ch4=0;
	uint16_t ch5=0;
	uint16_t ch6=0;




	//value in [MAP_RADIO_LOW - MAP_RADIO_HIGH]
	ch1 = MAP_RADIO_HIGH - map(unCh1In, MIN_1, MAX_1, MAP_RADIO_LOW-DEADZONE*DEADZONE_ENABLE, MAP_RADIO_HIGH+DEADZONE*DEADZONE_ENABLE);
	ch2 = map(unCh2In, MIN_2, MAX_2, MAP_RADIO_LOW-DEADZONE*DEADZONE_ENABLE, MAP_RADIO_HIGH+DEADZONE*DEADZONE_ENABLE);
	ch3 = map(unCh3In, MIN_3, MAX_3, MAP_RADIO_LOW-DEADZONE*DEADZONE_ENABLE, MAP_RADIO_HIGH+DEADZONE*DEADZONE_ENABLE);
	ch4 = MAP_RADIO_HIGH - map(unCh4In, MIN_4, MAX_4, MAP_RADIO_LOW-DEADZONE*DEADZONE_ENABLE, MAP_RADIO_HIGH+DEADZONE*DEADZONE_ENABLE);

	// 0 or 1 for channels 5 or 6
	if (unCh5In >= MID_5)
	{ch5=1;}
	else
	{ch5=0;}


	if (unCh6In >= MID_6)
	{ch6=1;}
	else
	{ch6=0;}


	pChannels[0]=0;
	pChannels[1]=ch1;
	pChannels[2]=ch2;
	pChannels[3]=ch3;
	pChannels[4]=ch4;
	pChannels[5]=ch5;
	pChannels[6]=ch6;



	if(DEADZONE_ENABLE)
	{

		int i;
		for(i=2; i<=4; i++)
		{
			if(abs( pChannels[i] - MAP_RADIO_MID) < DEADZONE)
			{
				pChannels[i] = MAP_RADIO_MID;
			}
			else
			{
				pChannels[i] = pChannels[i] - sgn( pChannels[i] - MAP_RADIO_MID   )*DEADZONE;
			}
		}
		//Special case for the throttle
		if( pChannels[1] < DEADZONE)
		{
			pChannels[1] = MAP_RADIO_LOW;
		}
		else
		{
			pChannels[1] = pChannels[1] - DEADZONE;
		}
	}



	if(PRINT_ALL_CHANNELS)
	{
		int i;
		for(i=1; i<=6; i++)
		{Serial.println(pChannels[i]);}

	}




}


void updateRadio()
{
	unCh1In = unCh1InShared;
	unCh2In = unCh2InShared;
	unCh3In = unCh3InShared;
	unCh4In = unCh4InShared;
	unCh5In = unCh5InShared;
	unCh6In = unCh6InShared;

	// clear shared copy of updated flags as we have already taken the updates
	bUpdateFlagsShared = 0;
}







// simple interrupt service routine
void calcCh1()
{ 
	if(digitalRead(CH1_IN_PIN) == HIGH)
	{
		ulCh1Start = micros();
	}
	else
	{
		unCh1InShared = (uint16_t)(micros() - ulCh1Start);
		bUpdateFlagsShared |= CH1_FLAG;
	}
}



void calcCh2()
{
	if(digitalRead(CH2_IN_PIN) == HIGH)
	{
		ulCh2Start = micros();
	}
	else
	{
		unCh2InShared = (uint16_t)(micros() - ulCh2Start);
		bUpdateFlagsShared |= CH2_FLAG;
	}
}



void calcCh3()
{
	if(digitalRead(CH3_IN_PIN) == HIGH)
	{
		ulCh3Start = micros();
	}
	else
	{
		unCh3InShared = (uint16_t)(micros() - ulCh3Start);
		bUpdateFlagsShared |= CH3_FLAG;
	}
}



void calcCh4()
{
	if(digitalRead(CH4_IN_PIN) == HIGH)
	{
		ulCh4Start = micros();
	}
	else
	{
		unCh4InShared = (uint16_t)(micros() - ulCh4Start);
		bUpdateFlagsShared |= CH4_FLAG;
	}
}



void calcCh5()
{
	if(digitalRead(CH5_IN_PIN) == HIGH)
	{
		ulCh5Start = micros();
	}
	else
	{
		unCh5InShared = (uint16_t)(micros() - ulCh5Start);
		bUpdateFlagsShared |= CH5_FLAG;
	}
}



void calcCh6()
{
	if(digitalRead(CH6_IN_PIN) == HIGH)
	{
		ulCh6Start = micros();
	}
	else
	{
		unCh6InShared = (uint16_t)(micros() - ulCh6Start);
		bUpdateFlagsShared |= CH6_FLAG;
	}
}


