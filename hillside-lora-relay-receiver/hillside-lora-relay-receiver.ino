 /*
Application to listen for specific messages and then enable/disable a relay based on the message
*/

#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>

#define COMMAND_RELAY_OFF     0x00
#define COMMAND_RELAY_ON      0x01

const uint8_t RELAY_ENABLE_MESSAGE[6] = {0x0C, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t RELAY_DISABLE_MESSAGE[6] = {0x0C, 0x00, 0x00, 0x00, 0x00, 0x00};

const byte qwiicRelayAddress = 0x18;  // Default Relay Address

// We need to provide the RFM95 module's chip select and interrupt pins to the 
// rf95 instance below.On the SparkFun ProRF those pins are 12 and 6 respectively.
RH_RF95 rf95(12, 6);

int LED = 13; //Status LED on pin 13

int packetCounter = 0; //Counts the number of packets sent
long timeSinceLastPacket = 0; //Tracks the time stamp of last packet received
// The broadcast frequency is set to 921.2, but the SADM21 ProRf operates
// anywhere in the range of 902-928MHz in the Americas.
// Europe operates in the frequencies 863-870, center frequency at 
// 868MHz.This works but it is unknown how well the radio configures to this frequency:
//float frequency = 864.1;
float frequency = 921.2;

// RelayOn() turns on the relay at the qwiicRelayAddress
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(qwiicRelayAddress);
  Wire.write(COMMAND_RELAY_ON);
  if (Wire.endTransmission() != 0) {
    Serial.println("Check Connections. No slave attached.");
  }
}

// RelayOff() turns off the relay at the qwiicRelayAddress
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(qwiicRelayAddress);
  Wire.write(COMMAND_RELAY_OFF);
  if (Wire.endTransmission() != 0) {
    Serial.println("Check Connections. No slave attached.");
  }
}

// Cheap comparison function to evaluate the first six elements in an array
// message: the message received from the radio
// reference: the reference message to compare against
bool compareMessages(uint8_t *message, const uint8_t *reference) {
  for (int i=0; i<6; i++) {
    if (message[i] != reference[i])
      return false;
  }
  return true;
}

void setup()
{
  pinMode(LED, OUTPUT);

  SerialUSB.begin(9600);
  // It may be difficult to read serial messages on startup. The following
  // line will wait for serial to be ready before continuing. Comment out if not needed.
  //while(!SerialUSB);
  SerialUSB.println("RFM Server!");

  // Join the I2C bus
  Wire.begin();

  //Initialize the Radio. 
  if (rf95.init() == false){
    SerialUSB.println("Radio Init Failed - Freezing");
    while (1);
  }
  else{
  // An LED indicator to let us know radio initialization has completed.
    SerialUSB.println("Receiver up!");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }

  rf95.setFrequency(frequency); 

   // The default transmitter power is 13dBm, using PA_BOOST.
   // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
   // you can set transmitter powers from 5 to 23 dBm:
   // Transmitter power can range from 14-20dbm.
   rf95.setTxPower(14, false);

   // Since we just came up, let's make sure the relay is off so we know the state of the system
   relayOff();
}

void loop()
{
  if (rf95.available()){
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)){
      digitalWrite(LED, HIGH); //Turn on status LED to indicate we have a message
      timeSinceLastPacket = millis(); //Timestamp this packet

      //SerialUSB.print("Got message: ");
      //SerialUSB.print((char*)buf);

      if (compareMessages(buf, RELAY_ENABLE_MESSAGE)) {
        SerialUSB.print("Enabling relay");
        relayOn();
      }
      else if (compareMessages(buf, RELAY_DISABLE_MESSAGE)) {
        SerialUSB.print("Disabling relay");
        relayOff();
      }

      // Send a reply
      // uint8_t toSend[] = "Hello Back!"; 
      // rf95.send(toSend, sizeof(toSend));
      // rf95.waitPacketSent();
      // SerialUSB.println("Sent a reply");

      digitalWrite(LED, LOW); //Turn off status LED
    }
    else
      SerialUSB.println("Recieve failed");
  }
  
  //Turn off status LED if we haven't received a packet after 1s
  // if(millis() - timeSinceLastPacket > 1000){
  //   digitalWrite(LED, LOW); //Turn off status LED
  //   timeSinceLastPacket = millis(); //Don't write LED but every 1s
  // }
}
