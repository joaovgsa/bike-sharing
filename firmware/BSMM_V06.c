/*

	BSMM 06 - Version JVTech
	
	Author: João Vitor Galdino Souto Alvares	
	Date: 21/05/2019
	
	Description: This version is own with strutcure and the code of the organization at the JVTech with the logic flow correct.
					
				-> Arduino Mega
				
					-> With DEBUG01
					
					This program is utilizing 5378 (2%) bytes of the memory FLASH
					The maximum is of 253952 (250KB) bytes of memory FLASH
					
					This program is utilizing 709 (8%) bytes of the memory RAM
					The maximum is of 8192 (8KB) bytes of memory RAM
					
					-> Without DEBUG01
					
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
#define	rele01			22	
#define	rele02			23
#define	rele03			24
#define	rele04			25
#define	rele05			26
#define	rele06			27
#define	rele07			28
#define	rele08			29
#define	rele09			30
#define	rele10			31
#define	rele11			32
#define	rele12			33
#define	buzzer			34

#define key01			42
#define key02			43
#define key03			44
#define key04			45
#define key05			46
#define key06			47
#define key07			48
#define key08			49
#define key09			50
#define key10			51
#define key11			52
#define key12			53

// Pins bluetooth
#define	rxBluetooth		10														// This is the TX of the Arduino Mega
#define	txBluetooth		11														// This is the RX of the Arduino Mega

// Macro(s) generic(s)
#define	DEBUG01	
//#define	DEBUG02	
#define	SERIAL			115200
#define	numberMax		12
#define	numberMin		1
#define	amountPin		(numberMax-numberMin)
#define timeOpen		10000

// Message that received
#define messageDefault	"BSMM"

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

int status01;
int status02;
int status03;
int status04;
int status05;
int status06;
int status07;
int status08;
int status09;
int status10;
int status11;
int status12;

typedef struct
{
	
	// Pins to monitoring
	int 	pinsLock[12]		= {key01, key02, key03, key04, key05, key06, key07, key08, key09, key10, key11, key12};
	int 	pinsLockStatus[12]	= {status01, status02, status03, status04, status05, status06, status07, status08, status09, status10, status11, status12};
	// Pins to action bike
	int		pinsEmbedded[12] 	= {rele01, rele02, rele03, rele04, rele05, rele06, rele07, rele08, rele09, rele10, rele11, rele12};
	// Number Bike
	int		pinsApp[12] 		= {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	int 	statusBike[12];

} dataBike;

typedef struct
{
	
	// Flags
	char	flagMonitoringBike	= 0x00;
	char 	flagTxBluetooth		= 0x00;
	char 	flagUnlock 			= 0x01;
	
} dataFlag;

typedef struct
{
	
	// Messages that received
	String 	rxMessage;
	
	// Messa for action
	int 	activationRele;
	int 	readPin;
	
	// Message to define
	int		defineBike;
	
} dataVariables;

dataBike		dataB;
dataFlag		dataF;
dataVariables	dataV;

//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
// Initial settings
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************

void setup()
{
	
	setupSerial();
	
	setupPins();
	
	setupTestPins();
	
	setupBluetooth();

	#ifdef	DEBUG01
	Serial.print("Finished the main setup");
	Serial.println();
	delay(10);
	#endif	
	
	dataF.flagMonitoringBike = 0x01;
	
} // end setup

//*****************************************************************************************************************
// Function for setup serials
//*****************************************************************************************************************

void setupSerial()
{
	
	Serial.begin(SERIAL);
	mSerial.begin(SERIAL);
	
	#ifdef	DEBUG01
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
	
	pinMode(rele01,	OUTPUT); 											
	pinMode(rele02,	OUTPUT);
	pinMode(rele03,	OUTPUT);
	pinMode(rele04,	OUTPUT);
	pinMode(rele05,	OUTPUT);
	pinMode(rele06,	OUTPUT);
	pinMode(rele07,	OUTPUT);
	pinMode(rele08,	OUTPUT);
	pinMode(rele09,	OUTPUT);
	pinMode(rele10,	OUTPUT);
	pinMode(rele11,	OUTPUT);
	pinMode(rele12,	OUTPUT);
	pinMode(buzzer,	OUTPUT);
	
	pinMode(key01,	INPUT); 											
	pinMode(key02,	INPUT);
	pinMode(key03,	INPUT);
	pinMode(key04,	INPUT);
	pinMode(key05,	INPUT);
	pinMode(key06,	INPUT);
	pinMode(key07,	INPUT);
	pinMode(key08,	INPUT);
	pinMode(key09,	INPUT);
	pinMode(key10,	INPUT);
	pinMode(key11,	INPUT);
	pinMode(key12,	INPUT);
	
	#ifdef	DEBUG01
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
	
	#ifdef	DEBUG01
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
	
	mSerial.write("bikes-ok");
	
	#ifdef	DEBUG01
	Serial.print("Finished the bluetooth setup");
	Serial.println();
	delay(10);
	#endif
	
} // end setupBluetooth

//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
// End setup
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************
//*****************************************************************************************************************

//*****************************************************************************************************************
// Loop infinite
//*****************************************************************************************************************

void loop()
{
	
	#ifdef	DEBUG01
	Serial.print("Wating bike... ");
	Serial.println();
	delay(10);
	#endif

	monitoringBike();
	
	receiveBluetooth();
	
} // end loop

//*****************************************************************************************************************
// Function to monitoring
//*****************************************************************************************************************

void monitoringBike()
{
	
	if(dataF.flagMonitoringBike)
	{
		
		for(int i = 0; i < numberMax; i++)
		{
			
			dataB.pinsLockStatus[i] = 0;
			
		}		
		
		for(int k = 0; k < numberMax; k++)
		{
			
			dataV.readPin			= digitalRead(dataB.pinsLock[k]);
			dataB.pinsLockStatus[k] = dataV.readPin;
			
			#ifdef	DEBUG01
			Serial.print("Pin of the lock is: ");
			Serial.print(dataB.pinsLock[k]);
			Serial.print(" -- ");
			Serial.print("Bike is: ");
			Serial.print(dataB.pinsApp[k]);
			Serial.print(" -- ");
			Serial.print("Bike status is: ");
			Serial.println(dataB.pinsLockStatus[k]);			
			delay(10);
			#endif		
			
		} // end for
	
	} // end if	
	
} // end monitoringBike

//*****************************************************************************************************************
// Function to receive bluetooth
//*****************************************************************************************************************

void receiveBluetooth()
{
	
	String receivedString 	=	"";
	
	while(mSerial.available()) 
	{
				
		char c	= mSerial.read();
		
		if(isDigit(c))
		{
			
			receivedString += c;
			
		} // end if
		
		#ifdef	DEBUG01
		Serial.print("The value that was receive is: ");
		Serial.print(receivedString);
		Serial.println();
		delay(10);
		#endif
    
	} // end while
		
	dataV.rxMessage = receivedString.toInt();
	
	#ifdef	DEBUG01
	Serial.print("The value that was change is: ");
	Serial.print(dataV.rxMessage);
	Serial.println();
	delay(10);
	#endif	
	
	if(dataV.rxMessage == messageDefault)
	{
		
		dataF.flagTxBluetooth = 0x01;
		
		transmitterBluetooth();
		
		releActivation();
		
	} // end if
	
} // end receiveBluetooth

//*****************************************************************************************************************
// Function to transmitter bluetooth
//*****************************************************************************************************************

void transmitterBluetooth()
{
	
	if(dataF.flagTxBluetooth)
	{
	
		if(dataV.rxMessage = messageDefault)
		{
			
			for(int j = numberMax; j > 0; j--)
			{
				
				if(!dataB.pinsLockStatus[j])
				{
					
					dataF.flagUnlock 	= 0x01;
					dataV.defineBike	= j;
				
				} // end if
				
			} // end for
			
			mSerial.print("bike|");
			mSerial.print(dataB.pinsApp[dataV.defineBike]);
			
		} // end if		
		
		#ifdef	DEBUG01
		Serial.print("Bike in the App is: ");
		Serial.print(dataB.pinsApp[dataV.defineBike]);
		Serial.print(" -- ");
		Serial.print("Bike status is: ");
		Serial.print(dataB.pinsLockStatus[dataV.defineBike]);	
		Serial.print(" -- ");
		Serial.print("Bike status is: ");
		Serial.print(dataB.pinsLock[dataV.defineBike]);			
		Serial.print(" -- ");
		Serial.print("Bike embedded is: ");
		Serial.println(dataB.pinsEmbedded[dataV.defineBike]);			
		delay(10);
		#endif
		
	} // end if

} // end transmitterBluetooth

//*****************************************************************************************************************
// Function to activation
//*****************************************************************************************************************

void releActivation()
{		
	
	if(dataF.flagTxBluetooth)
	{
		
		dataF.flagTxBluetooth = 0x00;
	
		if(dataB.pinsLockStatus[dataV.defineBike])
		{	
			
			dataV.activationRele = dataB.pinsEmbedded[dataV.defineBike];		
		
			digitalWrite(dataV.activationRele,	LOW);
			delay(timeOpen);
			digitalWrite(dataV.activationRele,	HIGH);
			
			dataV.defineBike	= 0;
			
			#ifdef	DEBUG01
			Serial.println("Activation key ok. Bike free ");
			Serial.print("Bike free is: ");
			Serial.println(dataV.activationRele);
			Serial.print(" -- ");
			Serial.print("The value of rxMessage is: ");
			Serial.println(dataV.defineBike);	
			Serial.print(" -- ");
			Serial.print("Bike in the App is: ");
			Serial.print(dataB.pinsApp[dataV.defineBike]);
			Serial.println();
			delay(10);
			#endif
				
		} // end if	
			
		else
		{
				
			#ifdef	DEBUG01
			Serial.print("Activation not key ok. Bike busy ");
			Serial.println();
			delay(10);
			#endif
			
			digitalWrite(buzzer,	HIGH);
			delay(3000);
			digitalWrite(buzzer,	LOW);
				
		} // end else
		
	} // end if
	
} // end releActivation