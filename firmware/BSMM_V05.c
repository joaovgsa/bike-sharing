n/*

	BSMM 05 - Version JVTech
	
	Author: João Vitor Galdino Souto Alvares	
	Date: 10/05/2019
	
	Description: This version is own with strutcure and the code of the organization at the JVTech with the logic flow correct.
					
				-> Arduino Mega
				
					-> With Debug
					
					This program is utilizing 5378 (2%) bytes of the memory FLASH
					The maximum is of 253952 (250KB) bytes of memory FLASH
					
					This program is utilizing 709 (8%) bytes of the memory RAM
					The maximum is of 8192 (8KB) bytes of memory RAM
					
					-> Without Debug
					
					This program is utilizing 4688 (1%) bytes of the memory FLASH
					The maximum is of 253952 (250KB) bytes of memory FLASH
					
					This program is utilizing 395 (4%) bytes of the memory RAM
					The maximum is of 8192 (8KB) bytes of memory RAM
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
#define	SERIAL		115200
#define	numberMax	12
#define	numberMin	1
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

int 	readPin;



typedef struct
{
	
	int		pinsEmbedded[12] 	= {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
	int		pinsApp[12] 		= {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	int 	statusBike[12];
	int 	rxMessage;
	int 	activationRele;
	char 	flagTxBluetooth		= 0x00;
	char 	flagTxBusy			= 0x00;
	
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
	
	Serial.begin(SERIAL);
	mSerial.begin(SERIAL);
	
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
	
	delay(2000);	
	
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
	
	mSerial.write("Bikes-OK");
	
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

	receiveBluetooth();
	
} // end loop

//*****************************************************************************************************************
// Function to receive bluetooth
//*****************************************************************************************************************

void receiveBluetooth()
{
	
	String receivedString 	=	"";
	
	while(mSerial.available()) 
	{
		
		char c = mSerial.read();
		
		if(isDigit(c))
		{
			
			receivedString += c;
			
		} // end if
		
		// char c = mSerial.readBytes();
		// mSerial.readBytes((uint8_t*)&dataB.rxMessage, sizeof(dataB.rxMessage));
		

		#ifdef	DEBUG
		Serial.print("The value that was receive is: ");
		Serial.print(receivedString);
		Serial.println();
		delay(10);
		#endif
    
	} // end while
		
	dataB.rxMessage = receivedString.toInt();
	
	#ifdef	DEBUG
	Serial.print("The value that was change is: ");
	Serial.print(dataB.rxMessage);
	Serial.println();
	delay(10);
	#endif	
	
	monitoringBike();
	
	transmitterBluetooth();
	
	releActivation();
	
} // end receiveBluetooth

//*****************************************************************************************************************
// Function to monitoring
//*****************************************************************************************************************

void monitoringBike()
{
	
	if(dataB.rxMessage > numberMax || dataB.rxMessage < numberMin)
	{
		
		#ifdef	DEBUG
		Serial.println("Error. Pin dont exit ");
		Serial.println("Active the buzzer ");
		Serial.println();
		delay(10);
		#endif
		
		digitalWrite(buzzer,	HIGH);
		delay(3000);
		digitalWrite(buzzer,	LOW);
		
	} // end if	
	
	else
	{
		
		readPin 								= digitalRead(dataB.pinsEmbedded[dataB.rxMessage-1]);
		dataB.statusBike[dataB.rxMessage-1] 	= readPin;
		
		dataB.flagTxBluetooth					= 0x01;
	
		#ifdef	DEBUG
		Serial.print("Pin embedded is: ");
		Serial.println(dataB.pinsEmbedded[dataB.rxMessage-1]);
		Serial.print("Bike is: ");
		Serial.println(dataB.pinsApp[dataB.rxMessage-1]);
		Serial.print("Bike status is: ");
		Serial.println(dataB.statusBike[dataB.rxMessage-1]);			
		delay(10);
		#endif
		
	} // end else	
	
} // end monitoringBike

//*****************************************************************************************************************
// Function to transmitter bluetooth
//*****************************************************************************************************************

void transmitterBluetooth()
{
	
	if(dataB.flagTxBluetooth)
	{
		
		mSerial.print("bike");
		mSerial.println(dataB.pinsApp[dataB.rxMessage-1]);
		mSerial.print("status");
		mSerial.println(dataB.statusBike[dataB.rxMessage-1]);
		
		if(!dataB.statusBike[dataB.rxMessage-1])	
		{
			
			dataB.flagTxBusy	=	0x01;
			mSerial.println("BikeBusy");		
			
		} // end if
		
		#ifdef	DEBUG
		Serial.print("Bike is: ");
		Serial.println(dataB.pinsApp[dataB.rxMessage-1]);
		Serial.print("Bike status is: ");
		Serial.println(dataB.statusBike[dataB.rxMessage-1]);			
		delay(10);
		#endif
		
	} // end if

} // end transmitterBluetooth

//*****************************************************************************************************************
// Function to activation
//*****************************************************************************************************************

void releActivation()
{		
	
	if(dataB.flagTxBluetooth)
	{
		
		dataB.flagTxBluetooth	= 0x00;
	
		if(!dataB.flagTxBusy)
		{
			
			if(dataB.statusBike[dataB.rxMessage-1])	
			{	
			
				dataB.activationRele = dataB.pinsEmbedded[dataB.rxMessage-1];		
	
				digitalWrite(dataB.activationRele,	LOW);
				delay(timeOpen);
				digitalWrite(dataB.activationRele,	HIGH);
				
				dataB.rxMessage 	= 0;
			
				#ifdef	DEBUG
				Serial.println("Activation key ok. Bike free ");
				Serial.print("Bike free is: ");
				Serial.println(dataB.activationRele);
				Serial.print("The value of rxMessage is: ");
				Serial.println(dataB.rxMessage);	
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
			
		} // end if
		
		else	dataB.flagTxBusy	= 0x00;
		
	} // end if
	
} // end releActivation