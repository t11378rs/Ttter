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
//char msg[] = "Test from Arduino";

int tmp_size = 20;
byte tmp[20];//状態を一時的に入れる
byte location = 0;//tmpのどこに入れるかとっておく
byte char_location = 0;
byte button_state = 0;
char msg[] = "                    ";

boolean button_flag=true;



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
}


void loop(){
  button_state = digitalRead(BUTTON_PIN);

  if(location >= tmp_size-1){//おっきくなり過ぎたら戻す
    location=0;
  }

  tmp[location] = button_state;

  //tmpの中身を見て、何の文字か判別するモジュール
  if(tmp[location]==1){
    //7連続0だったらスペースを入れる=>100000001
    //3連続0だったら次の文字へ=>10001
    //1連続0だったら次の符号へ=>101
    if(tmp[location-1]==0 && tmp[location-2]==0 && tmp[location-3]==0 && tmp[location-4]==0 && tmp[location-5]==0 && tmp[location-6]==0 && tmp[location-7]==0){
      msg[char_location] = ' ';//空白
      char_location++;//次の文字へ進める
      location=0;//tmpのカーソルをリセット
    }else if(tmp[location-1]==0 && tmp[location-2]==0 && tmp[location-3]==0){
      //次の文字へ
      location=0;//tmpのカーソルをリセット
    }else if(tmp[location-1]==0){
      //次の符号へ
      location=0;//tmpのカーソルをリセット
    }else{
      location++;//tmpのカーソルを進める
    }
  }else if(tmp[location]==0){
    //3連続1だったら長音=>01110
    //1連続1だったら単音=>010
    if(tmp[location-1]==1 && tmp[location-2]==1 && tmp[location-3]==1 && tmp[location-4]==1 && tmp[location-5]==1 && tmp[location-6]==1 && tmp[location-7]==1){
      delay(100); //この数字小さくし過ぎるとボタンでエラー起きる
      imitate_post(msg);//投稿する
    }else if(tmp[location-1]==1 && tmp[location-2]==1 && tmp[location-3]==1){
      msg[char_location] = '-';//長音
      char_location++;//次の文字へ進める
      location=0;//tmpのカーソルをリセット
    }else if(tmp[location-1]==1){
      msg[char_location] = '*';//単音
      char_location++;//次の文字へ進める
      location=0;//tmpのカーソルをリセット
    }else{
      location++;//tmpのカーソルを進める
    }
  }
  //if(buttonState == HIGH && button_flag){
  //  imitate_post(msg);
  //}

}



bool imitate_post(char str[]){
  Serial.println("potsing ...");
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




/*
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
*/
/*
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
*/
