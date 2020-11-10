//-----------------------------------------------------------------------------------------------------------//
//                                                                                                           //
//  Slave_ELEC1601_Student_2019_v3                                                                           //
//  The Instructor version of this code is identical to this version EXCEPT it also sets PIN codes           //
//  20191008 Peter Jones                                                                                     //
//                                                                                                           //
//  Bi-directional passing of serial inputs via Bluetooth                                                    //
//  Note: the void loop() contents differ from "capitalise and return" code                                  //
//                                                                                                           //
//  This version was initially based on the 2011 Steve Chang code but has been substantially revised         //
//  and heavily documented throughout.                                                                       //
//                                                                                                           //
//  20190927 Ross Hutton                                                                                     //
//  Identified that opening the Arduino IDE Serial Monitor asserts a DTR signal which resets the Arduino,    //
//  causing it to re-execute the full connection setup routine. If this reset happens on the Slave system,   //
//  re-running the setup routine appears to drop the connection. The Master is unaware of this loss and      //
//  makes no attempt to re-connect. Code has been added to check if the Bluetooth connection remains         //
//  established and, if so, the setup process is bypassed.                                                   //
//                                                                                                           //
//-----------------------------------------------------------------------------------------------------------//

#include <SoftwareSerial.h>   //Software Serial Port
#include <Servo.h>                           // Include servo library
     
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;


#define RxD 7
#define TxD 6
#define ConnStatus A1

#define DEBUG_ENABLED  1

// ##################################################################################
// ### EDIT THE LINES BELOW TO MATCH YOUR SHIELD NUMBER AND CONNECTION PIN OPTION ###
// ##################################################################################

int shieldPairNumber = 2;

// CAUTION: If ConnStatusSupported = true you MUST NOT use pin A1 otherwise "random" reboots will occur
// CAUTION: If ConnStatusSupported = true you MUST set the PIO[1] switch to A1 (not NC)

boolean ConnStatusSupported = true;   // Set to "true" when digital connection status is available on Arduino pin

// #######################################################

// The following two string variable are used to simplify adaptation of code to different shield pairs

String slaveNameCmd = "\r\n+STNA=Slave";   // This is concatenated with shieldPairNumber later

SoftwareSerial blueToothSerial(RxD,TxD);


void setup()
{
    Serial.begin(9600);
    blueToothSerial.begin(38400);                    // Set Bluetooth module to default baud rate 38400
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(ConnStatus, INPUT);
    pinMode(10, INPUT);  pinMode(9, OUTPUT);   // Left IR LED & Receiver
    pinMode(3, INPUT);  pinMode(2, OUTPUT);    // Right IR LED & Receiver

    tone(4, 3000, 1000);                       // Play tone for 1 second
    delay(1000);                               // Delay to finish tone

    servoLeft.attach(13);                      // Attach left signal to pin 13
    servoRight.attach(12);                     // Attach right signal to pin 12

    //  Check whether Master and Slave are already connected by polling the ConnStatus pin (A1 on SeeedStudio v1 shield)
    //  This prevents running the full connection setup routine if not necessary.

    if(ConnStatusSupported) Serial.println("Checking Slave-Master connection status.");

    if(ConnStatusSupported && digitalRead(ConnStatus)==1)
    {
        Serial.println("Already connected to Master - remove USB cable if reboot of Master Bluetooth required.");
    }
    else
    {
        Serial.println("Not connected to Master.");
        
        setupBlueToothConnection();   // Set up the local (slave) Bluetooth module

        delay(1000);                  // Wait one second and flush the serial buffers
        Serial.flush();
        blueToothSerial.flush();
    }
}


void loop()
{   
    char recvChar;
    

 while(1) {
        
        if(blueToothSerial.available())   // Check if there's any data sent from the remote Bluetooth shield
        {
            recvChar = blueToothSerial.read();
            Serial.print(recvChar);
            movement(recvChar);
    		
        }
        
        if(Serial.available())         // Check if there's any data sent from the local serial terminal. You can add the other applications here.
        {
            recvChar  = Serial.read();
            Serial.print(recvChar);
            blueToothSerial.print(recvChar);
            
        }
        
       
}



}

     
          //default is 510, 510, 0
          //when up, y is 1023
          //when down, y is 0
          //when left, x is 1023
          //when right, x is 0
          //when middle button not pressed, z is 1
          //when middle button pressed, z is 0
          //servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
          //servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
          //servoRight.writeMicroseconds(1300); // Right wheel clockwise
             

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  


boolean detection(){
//Colour Detection (MUST BE TUNED TO EXACT BALL COLOUR)

  if(irDetect(9, 10, BallFrequency) == 0 || irDetect(2, 3, BallFrequency) == 0){
     blueToothSerial.print("Object Found");
     captureBall();
     returnToStart(movementArray);
     return true;
  }
	
  return false;
}

void movement(char x) {

			//STATIONARY
			if (x == 'o') {
				servoLeft.writeMicroseconds(1500);  // Left wheel counterclockwise
				servoRight.writeMicroseconds(1500); // Right wheel clockwise
			}
			//REVERSE
			//STRAIGHT
			if (x == 'p') { //Straight reverse
				servoLeft.writeMicroseconds(1300); //clockwise
				servoRight.writeMicroseconds(1700); //counter clockwise

			}
			//LEFT
			if (x == 'x') { //Left Reverse
				servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
				servoRight.writeMicroseconds(1300); // Right wheel clockwise
			}
			//RIGHT
			if (x == 'q') { //Right reverse
				servoLeft.writeMicroseconds(1300); 
				servoRight.writeMicroseconds(1550);
			}
			//FORWARD
			//STRAIGHT
			if (x == 'w') { //Straight forward
				servoLeft.writeMicroseconds(1700); //counter clockwise
				servoRight.writeMicroseconds(1300); //clockwise
			}
			//LEFT
			if (x == 'g') { //forward left
				servoLeft.writeMicroseconds(1550); 
				servoRight.writeMicroseconds(1300);

			}
			//RIGHT
			if (x == 'h') { //forward right
				servoLeft.writeMicroseconds(1700); 
				servoRight.writeMicroseconds(1450);
			}
}

         

void setupBlueToothConnection()
{
    Serial.println("Setting up the local (slave) Bluetooth module.");

    slaveNameCmd += shieldPairNumber;
    slaveNameCmd += "\r\n";

    blueToothSerial.print("\r\n+STWMOD=0\r\n");      // Set the Bluetooth to work in slave mode
    blueToothSerial.print(slaveNameCmd);             // Set the Bluetooth name using slaveNameCmd
    blueToothSerial.print("\r\n+STAUTO=0\r\n");      // Auto-connection should be forbidden here
    blueToothSerial.print("\r\n+STOAUT=1\r\n");      // Permit paired device to connect me
    
    //  print() sets up a transmit/outgoing buffer for the string which is then transmitted via interrupts one character at a time.
    //  This allows the program to keep running, with the transmitting happening in the background.
    //  Serial.flush() does not empty this buffer, instead it pauses the program until all Serial.print()ing is done.
    //  This is useful if there is critical timing mixed in with Serial.print()s.
    //  To clear an "incoming" serial buffer, use while(Serial.available()){Serial.read();}

    blueToothSerial.flush();
    delay(2000);                                     // This delay is required

    blueToothSerial.print("\r\n+INQ=1\r\n");         // Make the slave Bluetooth inquirable
    
    blueToothSerial.flush();
    delay(2000);                                     // This delay is required
    
    Serial.println("The slave bluetooth is inquirable!");
}
// two options depending on if we have the ir sensors detecting the white space around the line or the line itself

boolean followTrackSpace()
{  
    int irLeft = irDetect(9, 10, 38000);       // Check for object on left
    int irRight = irDetect(2, 3, 38000);  
    // the if both sensor detect is for side branch.
    if (detection()) {
    	return false;
    }
    if ((irLeft == 0) && (irRight == 0)) {
      servoLeft.writeMicroseconds(1550);                  // turn left 
      servoRight.writeMicroseconds(1550);
      delay(100);    //delay enough time to rotate more than the width of the track
    }
    else if (irLeft == 0) {
      servoLeft.writeMicroseconds(1550);                  // turn left 
      servoRight.writeMicroseconds(1550);
    }
    else if (irRight == 0) {
      servoLeft.writeMicroseconds(1450);                  // turn right 
      servoRight.writeMicroseconds(1450);
    }
	
    else {
      servoLeft.writeMicroseconds(1400);                  // Forward 
      servoRight.writeMicroseconds(1600);
    }
    return true;
}

