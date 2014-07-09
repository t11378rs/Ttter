#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>

// Constants that has been finalized
const byte TEST = 0;
const byte PRODUCTION = 1;
const int LED_PIN = 13;
const int BUTTON_PIN = 7;
const int SPEAKER_PIN = 9;
const char TOKEN[] = "257882187-WBB2XIkhdbzicQIrl9G9X3dkcOsUlhTXx7oRayZY";
byte MAC[] = { 0x00, 0x16, 0x3E, 0x5C, 0xF2, 0x7E };
byte IP[] = { 192, 168, 2, 104 };

// Constants thas has "not" been finalized
const int SINGLE_TIME = 400;

// variables
Twitter twitter(TOKEN);
char morse_msg[140];
char msg[] = "Kounaien mo itai death";
byte button_state = 0;
byte morse_msg_cursor = 0;

// variable to demonstrate development_phase
byte dev_phase = TEST;


void setup()
{
  Serial.begin(9600);
  
  Serial.print("ip: ");
  for(int i=0; i<4; i++){
    Serial.print(IP[i]);
    Serial.print(":");
  }
  Serial.println("");
  
  Serial.print("mac: ");
  for(int i=0; i<6; i++){
    Serial.print(MAC[i]);
    Serial.print(":");
  }
  Serial.println("");


  delay(1000);
  
  Ethernet.begin(MAC, IP);  
}

void loop()
{
  unsigned long pushed_interval = 0;//ボタンが押されている時間を入れる
  unsigned long released_interval = 0;//ボタンが離されている時間を入れる

  unsigned long just_before_pushed = millis();//ボタンが押される直前の時間
  if(digitalRead(BUTTON_PIN) == HIGH){
    //digitalWrite(LED_PIN, HIGH); //LEDをON
    tone(SPEAKER_PIN, 262);
  }
  while(digitalRead(BUTTON_PIN) == HIGH){//ボタンが押されている間
    pushed_interval = millis() - just_before_pushed; //ボタンが押されている秒数を調べる
  }
  print_s_ul_s("pushed_interval is ", pushed_interval, " sec");
  if(digitalRead(BUTTON_PIN) == LOW){//LOWになった瞬間
    if(SINGLE_TIME*3 < pushed_interval){    //長音分(single*3)押されていたら3~
      morse_msg[morse_msg_cursor] = '-';//長音をmorse_msgに入れる
      Serial.println("add -");
      morse_msg_cursor++;
    }else if(SINGLE_TIME < pushed_interval){//単音分(single)押されていたら 1~2
      morse_msg[morse_msg_cursor] = '*';//単音をmorse_msgに入れる
      Serial.println("add *");
      morse_msg_cursor++;
    }else{//単音以下の時は
      //何もしない
    }
    noTone(SPEAKER_PIN);
    //digitalWrite(LED_PIN, LOW); //LEDをOFF
  }

  unsigned long just_before_released = millis();
  while(digitalRead(BUTTON_PIN) == LOW){//ボタンが離されている間
    released_interval = millis() - just_before_released; //ボタンが押されていない秒数を調べる
  }
  print_s_ul_s("released_interval is ", released_interval, " sec");
  if(digitalRead(BUTTON_PIN) == HIGH){//ボタンが押された瞬間
    if(SINGLE_TIME*14 < released_interval){//14拍以上離されていたら 14~
      //何もしない
    }else if(SINGLE_TIME*7 < released_interval){//7拍分離されていたら 7~13
      morse_msg[morse_msg_cursor] = ' ';//空白をmorse_msgに入れる
      Serial.println("add space");
      morse_msg_cursor++;
    }else if(SINGLE_TIME*3 < released_interval){//3拍分離されていたら 3~7
      //morse_msgを翻訳して、msgに加え
      //morse_msgを空にする
      //morse_msg_cursorも空にする
      //空白文字のため時間はリセットしない
      //翻訳したものが開始信号だったら、全てをリセット
      morse_msg[morse_msg_cursor] = 'E';
      Serial.println("add EOE");
      if(true){//翻訳したものが終了信号だったら、msgを投稿
        post(morse_msg);
      }
    }else if(SINGLE_TIME < released_interval){//1拍分離されていたら
      //何もしない
    }
  }
}

char translation(char morse[]){
  //モールス信号を文字に翻訳する
}

void print_s_ul_s(char text1[], unsigned long digit, char text2[]){
  Serial.print(text1);
  Serial.print(digit);
  Serial.println(text2);
}

void post(char string[]){
  Serial.println("connecting ...");
  if(dev_phase == PRODUCTION){
    if (twitter.post(string)) {
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
  }else if(dev_phase == TEST){
    Serial.print("post ");
    Serial.println(string);
    Serial.println("OK.")
  }
}