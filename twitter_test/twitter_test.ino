#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>

const char TOKEN[] = "257882187-WBB2XIkhdbzicQIrl9G9X3dkcOsUlhTXx7oRayZY";

// Ethernet Shield Settings
byte mac[] = { 0x00, 0x16, 0x3E, 0x5C, 0xF2, 0x7E };

// If you don't specify the IP address, DHCP is used(only in Arduino 1.0 or later).
byte ip[] = { 192, 168, 2, 104 };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter(TOKEN);

// Message to post
char msg[] = "IP test from Arduino";

void setup()
{
  Serial.begin(9600);
  
  Serial.print("ip: ");
  for(int i=0; i<4; i++){
    Serial.print(ip[i]);
    Serial.print(":");
  }
  Serial.println("");
  
  Serial.print("mac: ");
  for(int i=0; i<6; i++){
    Serial.print(ip[i]);
    Serial.print(":");
  }
  Serial.println("");


  delay(1000);
  
  Ethernet.begin(mac, ip);
  // or you can use DHCP for autoomatic IP address configuration.
  // Ethernet.begin(mac);    
  Serial.println("connecting ...");
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}

void loop()
{
}
