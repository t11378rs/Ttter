#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>
#include <stdio.h>    // printf系を使うのに必要
#include "ButtonHistory.h"

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>

const int BUTTON_PIN = 7;     // the number of the pushbutton pin

const char TOKEN[] = "257882187-WBB2XIkhdbzicQIrl9G9X3dkcOsUlhTXx7oRayZY";
byte MAC[] = { 0x00, 0x16, 0x3E, 0x5C, 0xF2, 0x7E };
//byte IP[] = { 192, 168, 0, 2 };
byte IP[] = { 192, 168, 2, 104 };


Twitter twitter(TOKEN);
char msg[] = "Test from Arduino";

char tmp[100];
byte buttonState = 0;

boolean button_flag=true;

ButtonHistory button_history = ButtonHistory();


void setup(){
  
  Serial.begin(9600);
  
  Serial.print("IP_ADRESS: ");
  for(int i=0; i<4; i++){
    Serial.print(IP[i]);
    Serial.print(":");
  }
  Serial.println("");
  
  Serial.print("MAC_ADRESS: ");
  for(int i=0; i<6; i++){
    Serial.print(MAC[i]);
    Serial.print(":");
  }
  Serial.println("");


  delay(1000);
  
  Ethernet.begin(MAC, IP);
  // or you can use DHCP for autoomatic IP address configuration.
  // Ethernet.begin(mac);    

}


void loop(){
  //Serial.println("");
  buttonState = digitalRead(BUTTON_PIN);
  int result = button_history.add(buttonState);
  Serial.println(result);
  delay(100); //この数字小さくし過ぎるとボタンでエラー起きる
  if(buttonState == HIGH && button_flag){
    imitate_post(msg);
  }
  buttonState = LOW;
}


char *boolean_to_string(boolean flag){
  if(flag){
    return "TRUE";
  }else{
    return "FALSE";
  }
}

boolean post(char str[]){
  Serial.println("potsing");
  delay(100);
  if (twitter.post(str)) {
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
    return true;
  } else {
    Serial.println("connection failed.");
    return false;
  }
}


boolean imitate_post(char str[]){
  button_flag = false;
  //Serial.println(boolean_to_string(button_flag));
  Serial.println("potsing ...");
  print_history();
  delay(1000);
  if (str) {
    Serial.println(str);
    Serial.println();
    button_flag=true;
    //Serial.println(boolean_to_string(button_flag));
    return true;
  } else {
    Serial.println("failed. is string ok?");
    button_flag=true;
    return false;
  }
}

void print_history(){
  byte array_size = button_history.get_size();
  byte tmp[array_size];
  for(int i=0; i<array_size; i++){
    tmp[i]=button_history.get_by_index(i);
  }
  for(int i=0; i<array_size; i++){
    Serial.print(tmp[i]);
  }
  Serial.println("");
}

  
