/*
Application to read the voltage on an analog pin and send enable/disable messages to other nodes.  
*/

#include <SPI.h>
#include <RH_RF95.h>

// Messages for enabling and disabling the relay on remote nodes
const uint8_t RELAY_ENABLE_MESSAGE[6] = {0x0C, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t RELAY_DISABLE_MESSAGE[6] = {0x0C, 0x00, 0x00, 0x00, 0x00, 0x00};

const int VOLTAGE_TRIGGER_LEVEL = 500;  // 1024 = 3.3v

// We need to provide the RFM95 module's chip select and interrupt pins to the
// rf95 instance below.On the SparkFun ProRF those pins are 12 and 6 respectively.
RH_RF95 rf95(12, 6);

int LED = 13; //Status LED is on pin 13
int packetCounter = 0; //Counts the number of packets sent
long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received

// The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// anywhere in the range of 902-928MHz in the Americas.
// Europe operates in the frequencies 863-870, center frequency at 868MHz.
// This works but it is unknown how well the radio configures to this frequency:
//float frequency = 864.1; 
float frequency = 921.2; //Broadcast frequency

// Just flashes the onboard LED to show activity
void flashLeds() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

void sendEnableMessage() {
  SerialUSB.println("Sending enable message");
  flashLeds();

  // send the message thrice for robustness
  for (int i=0; i<3; i++) {
    rf95.send(RELAY_ENABLE_MESSAGE, sizeof(RELAY_ENABLE_MESSAGE));
    rf95.waitPacketSent();
    delay(500);
  }
}

void sendDisableMessage() {
  SerialUSB.println("Sending disable message");
  flashLeds();

  // send the message thrice for robustness
  for (int i=0; i<3; i++) {
    rf95.send(RELAY_DISABLE_MESSAGE, sizeof(RELAY_DISABLE_MESSAGE));
    rf95.waitPacketSent();
    delay(500);
  }
}

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(9600);
  // It may be difficult to read serial messages on startup. The following line
  // will wait for serial to be ready before continuing. Comment out if not needed.
  // while(!SerialUSB); 
  SerialUSB.println("RFM Client!"); 

  //Initialize the Radio.
  if (rf95.init() == false){
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  }
  else{
    //An LED inidicator to let us know radio initialization has completed. 
    SerialUSB.println("Transmitter up!"); 
    flashLeds();
  }

  // Set frequency
  rf95.setFrequency(frequency);

   // The default transmitter power is 13dBm, using PA_BOOST.
   // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
   // you can set transmitter powers from 5 to 23 dBm:
   // Transmitter power can range from 14-20dbm.
   rf95.setTxPower(14, false);

   // Send a disable message so we know the current state of the system
   sendDisableMessage();
}


void loop()
{
  // static variable to hold the last known state of the system
  // since we sent a disable message in setup(), it should be off initially
  static bool lastState = false;

  // read the input on analog pin 2
  int sensorValue = analogRead(A2);

  bool newState = false;
  newState = sensorValue > VOLTAGE_TRIGGER_LEVEL ? true : false;

  if (newState != lastState) {
    SerialUSB.println("State change!"); 
    
    if (newState == true)
      sendEnableMessage();
    else
      sendDisableMessage();

    lastState = newState;
  }

  delay(2000);
}
