// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(3, 2); //RX&TX

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 12;

void setup() {
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default the relay is off
  digitalWrite(relay, HIGH);
  
  // Initializing serial commmunication
  Serial.begin(115200); 
  SIM900.begin(115200);

  // Give time to your GSM shield log on to network
  delay(5000);
  Serial.println("SIM800L start...");
 /*
  SIM900.println("AT"); //Handshaking with SIM900
  delay(100);
  SIM900.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  delay(100);
  SIM900.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  delay(100);
 SIM900.println("AT+CREG?"); //Check whether it has registered in the network
  delay(100);
*/
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT"); //Handshaking with SIM900
  delay(100);
  SIM900.println("AT+CMGF=1"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.println("AT+CNMI=2,2,0,0,0");
  delay(100);
}

void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  if(textMessage.indexOf("ON")>=0){
    // Turn on relay and save current state
    digitalWrite(relay, LOW);
    lampState = "on";
    Serial.println("Relay set to ON");  
    textMessage = "";   
  }
  if(textMessage.indexOf("OFF")>=0){
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "off"; 
    Serial.println("Relay set to OFF");
    textMessage = ""; 
  }
  if(textMessage.indexOf("STATE")>=0){
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"+84868475996\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
