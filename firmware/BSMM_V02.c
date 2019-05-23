/*

	BSMM 02 - Version JVTech
	
	Author: João Vitor Galdino Souto Alvares	
	Date: 02/05/2019
	
	Description: This version is of the MadeiraMadeira with strutcure and the code of the organization at the JVTech.
					
				-> Arduino
				
					-> With Debug
					
					This program is utilizing xxxxxx (xx%) bytes of the memory FLASH
					The maximum is of 32256 (32KB) bytes of memory FLASH
					
					This program is utilizing xxxxxx (xx%) bytes of the memory RAM
					The maximum is of 2048 (2KB) bytes of memory RAM
					
					-> Without Debug
					
					This program is utilizing xxxxxx (xx%) bytes of the memory FLASH
					The maximum is of 32256 (32KB) bytes of memory FLASH
					
					This program is utilizing xxxxxx (xx%) bytes of the memory RAM
					The maximum is of 2048 (2KB) bytes of memory RAM
*/

//*****************************************************************************************************************
// Library(ies)
//*****************************************************************************************************************

#include <SoftwareSerial.h>

//*****************************************************************************************************************
// Contant(s)
//*****************************************************************************************************************

// Hardware mapping
#define	rele01		22	
#define	rele02		23
#define	rele03		24
#define	rele04		25
#define	rele05		26
#define	rele06		27
#define	rele07		28
#define	rele08		29
#define	rele09		30
#define	rele10		31
#define	rele11		32
#define	rele12		33
#define	buzzer		34

// Pins bluetooth
#define	rxBluetooth	10															// This is the TX of the Arduino Mega
#define	txBluetooth	11															// This is the RX of the Arduino Mega

// Macro(s) generic(s)
#define	DEBUG
#define	numberMax	1
#define	numberMin	12
#define	amountPin	(numberMax-numberMin)
#define timeOpen	10000

//*****************************************************************************************************************
// Prototype of the function(s)
//*****************************************************************************************************************

// Functions to setup
void setupSerial();
void setupPins();
void setupTestPins();
void setupBluetooth();

// Functions to bluetooth and activation
void receiveBluetooth();
void transmitterBluetooth();
void releActivation();
void monitoringBike();

//*****************************************************************************************************************
// Object(s)
//*****************************************************************************************************************
				   
SoftwareSerial mSerial(txBluetooth, rxBluetooth);				  				// RX / TX
				   
//*****************************************************************************************************************
// Global variable(s)
//*****************************************************************************************************************								

char 	readPin;

typedef struct
{
	
	char	pinsEmbedded[] 	= {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
	char	pinsApp[] 		= {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	char	statusBike[12]	= 0x00;
	char 	flagTxBluetooth	= 0x00;
	char 	rxMessage;
	char 	activationRele;
	
} dataBike;

dataBike	dataB;

//*****************************************************************************************************************
// Initial settings
//*****************************************************************************************************************

void setup()
{
	
	setupSerial();
	
	setupPins();
	
	setupTestPins();
	
	setupBluetooth();

	#ifdef	DEBUG
	Serial.print("Finished the main setup");
	Serial.println();
	delay(10);
	#endif	
	
} // end setup

//*****************************************************************************************************************
// Function for setup serials
//*****************************************************************************************************************

void setupSerial()
{
	
	Serial.begin(9600);
	mSerial.begin(9600);
	
	#ifdef	DEBUG
	Serial.print("Finished the setup serial");
	Serial.println();
	delay(10);
	#endif
	
} // end setupSerial

//*****************************************************************************************************************
// Function for setup pins
//*****************************************************************************************************************

void setupPins()
{
	
	pinMode(rele01,OUTPUT); 											
	pinMode(rele02,OUTPUT);
	pinMode(rele03,OUTPUT);
	pinMode(rele04,OUTPUT);
	pinMode(rele05,OUTPUT);
	pinMode(rele06,OUTPUT);
	pinMode(rele07,OUTPUT);
	pinMode(rele08,OUTPUT);
	pinMode(rele09,OUTPUT);
	pinMode(rele10,OUTPUT);
	pinMode(rele11,OUTPUT);
	pinMode(rele12,OUTPUT);
	pinMode(buzzer,OUTPUT);
	
	#ifdef	DEBUG
	Serial.print("Finished the pins setup");
	Serial.println();
	delay(10);
	#endif	
	
} // end setupPins

//*****************************************************************************************************************
// Function for test pins
//*****************************************************************************************************************

void setupTestPins()
{
	
	digitalWrite(rele01,	HIGH);
	digitalWrite(rele02,	HIGH);
	digitalWrite(rele03,	HIGH);
	digitalWrite(rele04,	HIGH);
	digitalWrite(rele05,	HIGH);
	digitalWrite(rele06,	HIGH);
	digitalWrite(rele07,	HIGH);
	digitalWrite(rele08,	HIGH);
	digitalWrite(rele09,	HIGH);
	digitalWrite(rele10,	HIGH);
	digitalWrite(rele11,	HIGH);
	digitalWrite(rele12,	HIGH);
	digitalWrite(buzzer,	HIGH);
	
	delay(2000);

	digitalWrite(rele01,	LOW);
	digitalWrite(rele02,	LOW);
	digitalWrite(rele03,	LOW);
	digitalWrite(rele04,	LOW);
	digitalWrite(rele05,	LOW);
	digitalWrite(rele06,	LOW);
	digitalWrite(rele07,	LOW);
	digitalWrite(rele08,	LOW);
	digitalWrite(rele09,	LOW);
	digitalWrite(rele10,	LOW);
	digitalWrite(rele11,	LOW);
	digitalWrite(rele12,	LOW);
	digitalWrite(buzzer,	LOW);
	
	#ifdef	DEBUG
	Serial.print("Finished the test pins setup");
	Serial.println();
	delay(10);
	#endif
	
} // end setupTestPins

//*****************************************************************************************************************
// Function for setup bluetooth
//*****************************************************************************************************************

void setupBluetooth()
{
	
	mSerial.write("bike-OK");
	
	#ifdef	DEBUG
	Serial.print("Finished the bluetooth setup");
	Serial.println();
	delay(10);
	#endif
	
} // end setupBluetooth

//*****************************************************************************************************************
// Loop infinite
//*****************************************************************************************************************

void loop()
{
	
	#ifdef	DEBUG
	Serial.print("Wating bike... ");
	Serial.println();
	delay(10);
	#endif

	transmitterBluetooth();

	receiveBluetooth();
	
} // end loop

//*****************************************************************************************************************
// Function to transmitter bluetooth
//*****************************************************************************************************************

void transmitterBluetooth()
{
	
	if(dataB.flagTxBluetooth)
	{
		
		dataB.flagTxBluetooth	= 0x00;
		
		monitoringBike();
		
	} // end if
	
	else
	{
		
		mSerial.write("BikeBusy");		
	
	} // end else
	
} // end transmitterBluetooth

//*****************************************************************************************************************
// Function to receive bluetooth
//*****************************************************************************************************************

void receiveBluetooth()
{
	
	if(mSerial.available()) 
	{
		
		char c = mSerial.read();
		
		dataB.rxMessage = c;
		
		#ifdef	DEBUG
		Serial.print("The value that was receive is: ");
		Serial.print(dataB.rxMessage);
		Serial.println();
		delay(10);
		#endif
		
		releActivation();
    
	} // end if	
	
} // end receiveBluetooth

//*****************************************************************************************************************
// Function to activation
//*****************************************************************************************************************

void releActivation()
{
	
	if(dataB.rxMessage > numberMax || dataB.rxMessage < numberMin)
	{
		
		#ifdef	DEBUG
		Serial.print("Error. Pin dont exit ");
		Serial.println();
		delay(10);
		#endif
		
		digitalWrite(buzzer,	HIGH);
		delay(3000);
		digitalWrite(buzzer,	LOW);
		
	} // end if
	
	else
	{
		
		for(int k = 0, k >= amountPin; k++)
		{
			
			if(dataB.rxMessage == pinsApp[i])	dataB.activationRele = dataB.pinsEmbedded[i];
			
		} // end for
		
		if(!digitalRead(dataB.activationRele))
		{
		
			digitalWrite(dataB.activationRele,	HIGH);
			delay(timeOpen);
			digitalWrite(dataB.activationRele,	LOW);
		
			#ifdef	DEBUG
			Serial.print("Activation key ok. Bike free ");
			Serial.println();
			delay(10);
			#endif
			
		} // end if	
		
		else
		{
			
			#ifdef	DEBUG
			Serial.print("Activation not key ok. Bike busy ");
			Serial.println();
			delay(10);
			#endif
			
			digitalWrite(buzzer,	HIGH);
			delay(3000);
			digitalWrite(buzzer,	LOW);
			
		} // end else
		
	} // end else
	
} // end releActivation

//*****************************************************************************************************************
// Function to monitoring
//*****************************************************************************************************************

void monitoringBike()
{
	
	for(int i = 0; i <= amountPin; i++)
	{
			
		readPin 			= digitalRead(dataB.pinsEmbedded[i])
		dataB.statusBike[i] = readPin;
		
		#ifdef	DEBUG
		Serial.print("Pin embedded is: ");
		Serial.println(dataB.pinsEmbedded[i]);
		Serial.print("Bike is: ");
		Serial.println(dataB.pinsApp[i]);
		Serial.print("Bike status is: ");
		Serial.println(dataB.statusBike[i]);			
		delay(10);
		#endif
			
	} // end for
		
	for(int j = 0; j <= amountPin; j++)
	{
			
		mSerial.write("bike");
		mSerial.write(dataB.pinsApp[i]);
		mSerial.write("status");
		mSerial.write(dataB.statusBike[i]);
		
		#ifdef	DEBUG
		Serial.print("Bike is: ");
		Serial.println(dataB.pinsApp[i]);
		Serial.print("Bike status is: ");
		Serial.println(dataB.statusBike[i]);			
		delay(10);
		#endif
			
	} // end for
	
} // end monitoringBike