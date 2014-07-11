#include <SPI.h> // needed in Arduino 0019 or later
#include <Ethernet.h>
#include <Twitter.h>
#include <String.h>

// The includion of EthernetDNS is not needed in Arduino IDE 1.0 or later.
// Please uncomment below in Arduino IDE 0022 or earlier.
//#include <EthernetDNS.h>

// Constants that has been finalized
const byte TEST = 0;
const byte PRODUCTION = 1;
//const int LED_PIN = 13;
const int BUTTON_PIN = 7;
const int SPEAKER_PIN = 9;
const char TOKEN[] = "257882187-WBB2XIkhdbzicQIrl9G9X3dkcOsUlhTXx7oRayZY";
//byte MAC[] = { 0x00, 0x16, 0x3E, 0x5C, 0xF2, 0x7E };
//byte IP[] = { 192, 168, 2, 104 };
byte MAC[] = { 0x00, 0x22, 0xCF, 0xED, 0xDB, 0x30 };
byte IP[] = { 192, 168, 2, 2 };
byte DNP[]  = {192, 168, 2, 1};
byte GATEWAY[] = {192, 168, 2, 1};
byte SUBNET[] = {255, 255, 255, 0};
/*
String A = "*-EEEEEEEE";
String B = "-***EEEEEE";
String C = "-*-*EEEEEE";
String D = "-**EEEEEEE";
String E = "*EEEEEEEEE";
String F = "**-*EEEEEE";
String G = "--*EEEEEEE";
String H = "****EEEEEE";
String I = "**EEEEEEEE";
String J = "*---EEEEEE";
String K = "-*-EEEEEEE";
String L = "*-**EEEEEE";
String M = "--EEEEEEEE";
String N = "-*EEEEEEEE";
String O = "---EEEEEEE";
String P = "*--*EEEEEE";
String Q = "--*-EEEEEE";
String R = "*-*EEEEEEE";
String S = "***EEEEEEE";
String T = "-EEEEEEEEE";
String U = "**-EEEEEEE";
String V = "***--EEEEE";
String W = "*--EEEEEEE";
String X = "-**-EEEEEE";
String Y = "-*--EEEEEE";
String Z = "--**EEEEEE";
String num_1 = "*----EEEEE";
String num_2 = "**---EEEEE";
String num_3 = "***--EEEEE";
String num_4 = "****-EEEEE";
String num_5 = "*****EEEEE";
String num_6 = "-****EEEEE";
String num_7 = "--***EEEEE";
String num_8 = "---**EEEEE";
String num_9 = "----*EEEEE";
String num_0 = "-----EEEEE";
String symbol_period = "*-*-*-EEEE";
String symbol_comma = "--**--EEEE";
String symbol_colon = "---***EEEE";
String symbol_question = "**--**EEEE";
String symbol_leftparen = "-*--*EEEEE";
String symbol_rightparen = "-*--*-EEEE";
String symbol_atsign = "*--*-*EEEE";
String symbol_underbar = "*-*-*EEEEE";//これほんとは＋なんだけどTwitter向けにアンダーバーとした
String mark_close = "***-*-EEEE";
String mark_start = "-*-*-EEEEE";
*/

String morse_codes[] = {//28こ。アルファベットとシンボル意外はメモリをケチる。Qもケチる。Vもケチる
  "*-EEEEEEEE", "-***EEEEEE", "-*-*EEEEEE", "-**EEEEEEE", "*EEEEEEEEE",
  "**-*EEEEEE", "--*EEEEEEE", "****EEEEEE", "**EEEEEEEE", "*---EEEEEE",
  "-*-EEEEEEE", "*-**EEEEEE", "--EEEEEEEE", "-*EEEEEEEE", "---EEEEEEE",
  "*--*EEEEEE", /*"--*-EEEEEE",*/ "*-*EEEEEEE", "***EEEEEEE", "-EEEEEEEEE",
  "**-EEEEEEE", /*"***--EEEEE",*/ "*--EEEEEEE", "-**-EEEEEE", "-*--EEEEEE",
  "--**EEEEEE", 
  /*"*----EEEEE", "**---EEEEE", "***--EEEEE", "****-EEEEE", "*****EEEEE",
  "-****EEEEE", "--***EEEEE", "---**EEEEE", "----*EEEEE", "-----EEEEE",
  "*-*-*-EEEE", "--**--EEEE", "---***EEEE", "**--**EEEE", "-*--*EEEEE",
  "-*--*-EEEE", "*--*-*EEEE", "*-*-*EEEEE",*/
   "***-*-EEEE", "-*-*-EEEEE"
};

/*
  A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
  num_1,num_2,num_3,num_4,num_5,num_6,num_7,num_8,num_9,num_0,
  symbol_period,symbol_comma,symbol_colon,symbol_question,symbol_leftparen,symbol_rightparen,symbol_atsign,symbol_underbar,
  mark_close,mark_start
*/

char morse_codes_dictionary[] = {//28こ。アルファベットとシンボル意外はメモリをケチる
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',/*'Q',*/'R','S','T','U',/*'V',*/'W','X','Y','Z',
  /*'1','2','3','4','5','6','7','8','9','0',
  '.',',',':','?','(',')','@','_',*/
  'x','o'
};//x is close, o is start

// Constants thas has "not" been finalized
const int SINGLE_TIME = 250;

// variables
Twitter twitter(TOKEN);
String morse_msg;
String msg;
byte button_state = 0;
byte morse_msg_cursor = 0;
byte msg_cursor = 0;

// variable to demonstrate development_phase
const byte dev_phase = TEST;

void setup()
{
  Serial.begin(9600);


  Serial.print("mac: ");
  for(int i=0; i<6; i++){
    Serial.print(MAC[i]);
    Serial.print(":");
  }
  Serial.println("");
  Serial.println(morse_msg);
  
  Serial.print("ip: ");
  for(int i=0; i<4; i++){
    Serial.print(IP[i]);
    Serial.print(":");
  }
  Serial.println("");

  Serial.print("dnp: ");
  for(int i=0; i<4; i++){
    Serial.print(DNP[i]);
    Serial.print(":");
  }
  Serial.println("");

  Serial.print("gateway: ");
  for(int i=0; i<4; i++){
    Serial.print(GATEWAY[i]);
    Serial.print(":");
  }
  Serial.println("");

  Serial.print("subnet: ");
  for(int i=0; i<4; i++){
    Serial.print(SUBNET[i]);
    Serial.print(":");
  }
  Serial.println("");

  


  delay(1000);
  
  Ethernet.begin(MAC, IP, DNP, GATEWAY, SUBNET);  

  morse_msg = "EEEEEEEEEE";
  msg = "";
  //Serial.println(morse_msg);
  //Serial.println(msg);
}


void loop()
{
  unsigned long pushed_interval = 0;//ボタンが押されている時間を入れる
  unsigned long released_interval = 0;//ボタンが離されている時間を入れる

  unsigned long just_before_pushed = millis();//ボタンが押される直前の時間
  if(digitalRead(BUTTON_PIN) == HIGH){
    tone(SPEAKER_PIN, 262);
  }
  while(digitalRead(BUTTON_PIN) == HIGH){//ボタンが押されている間
    pushed_interval = millis() - just_before_pushed; //ボタンが押されている秒数を調べる
  }
  print_s_ul_s("pushed_interval is ", pushed_interval, " sec");
  if(digitalRead(BUTTON_PIN) == LOW){//LOWになった瞬間
    if(SINGLE_TIME*3 < pushed_interval){    //長音分(single*3)押されていたら3~
      morse_msg.setCharAt(morse_msg_cursor, '-');//長音をmorse_msgに入れる
      Serial.print("add ");
      //Serial.println(morse_msg.charAt(morse_msg_cursor));
      Serial.println(morse_msg);
      morse_msg_cursor++;
    }else if(SINGLE_TIME < pushed_interval){//単音分(single)押されていたら 1~2
      morse_msg.setCharAt(morse_msg_cursor, '*');//単音をmorse_msgに入れる
      Serial.print("add ");
      //Serial.println(morse_msg.charAt(morse_msg_cursor));
      Serial.println(morse_msg);
      morse_msg_cursor++;
    }else{//単音以下の時は
      Serial.println("Unknown: too short pushed interval!");
      //何もしない
    }
    noTone(SPEAKER_PIN);
  }

  unsigned long just_before_released = millis();
  while(digitalRead(BUTTON_PIN) == LOW){//ボタンが離されている間
    released_interval = millis() - just_before_released; //ボタンが押されていない秒数を調べる
  }
  print_s_ul_s("released_interval is ", released_interval, " sec");
  if(digitalRead(BUTTON_PIN) == HIGH){//ボタンが押された瞬間
    if(SINGLE_TIME*14 < released_interval){//14拍以上離されていたら 14~
      //何もしない
      Serial.println("Unknown: too long released interval!");
    }else if(SINGLE_TIME*10 < released_interval){//7拍分離されていたら 7~13
      //morse_msg[morse_msg_cursor] = ' ';//空白をmorse_msgに入れる
      //Serial.println("add space");
      //morse_msg_cursor++;
      //何もしない
    }else if(SINGLE_TIME*3 < released_interval){//3拍分離されていたら 3~7
      char c = translation(morse_msg); //morse_msgを翻訳
      if(c=='x'){//翻訳したものが終了信号だったら、msgを投稿
        post(msg);
        reset_all();
      }else if(c=='o'){//翻訳したものが開始信号だったら、全てをリセット
        reset_all();
      }else{//開始終了文字でなければ(unknown文字も含まれることに注意)
        msg.concat(c); //msgに加えて
        msg_cursor++;
        morse_msg="EEEEEEEEEE";//morse_msgを空にする
        morse_msg_cursor=0;//morse_msg_cursorも空にする
      }
    }else if(SINGLE_TIME < released_interval){//1拍分離されていたら
      //何もしない
      Serial.println("Unknown: too short released interval!");
    }
  }
}

void reset_all(){
  msg="";
  morse_msg="EEEEEEEEEE";
  msg_cursor = 0;
  morse_msg_cursor = 0;
}

//モールス信号を文字に翻訳する
char translation(String morse){
  Serial.println("translating...");
  for(int i=0; i<28; i++){
    String morse_code = morse_codes[i];
    if(morse.equals(morse_code)){
      Serial.print("done. \"");
      Serial.print(morse);
      Serial.print("\" is ");
      Serial.println(morse_codes_dictionary[i]);
      return morse_codes_dictionary[i];
    }else{
      //do nothing
    }
  }
  //一致するものが一つもなかったら
  Serial.println(morse);
  Serial.println("no match. return unknown character \'#\'");
  return '#';
}

/*
int myCompareString(char text1[], char text2[]){
  //サイズを調べる
  if( sizeof(text1) != sizeof(text2)){
    return -1;
  }else{
    for(int i=0; i<=sizeof(text1); i++){
      if(text1[i] != text2[i]){
        return -1;
      }else{
        //do nothing
      }
      delay(100);
    }
    return 1;
  }
}
*/


void print_s_ul_s(char text1[], unsigned long digit, char text2[]){
  Serial.print(text1);
  Serial.print(digit);
  Serial.println(text2);
}

void post(String str){
  //string_to_char_array
  char post_str[str.length()];
  for(int i=0; i<str.length(); i++){
    post_str[i] = str.charAt(i);
  }

  //Serial.println("connecting ...");
  if(dev_phase == PRODUCTION){
    if (twitter.post(post_str)) {
      int status = twitter.wait(&Serial);
      if (status == 200) {
        //Serial.println("OK.");
      } else {
        //Serial.print("failed : code ");
        Serial.println(status);
      }
    } else {
      //Serial.println("connection failed.");
    }
  }else if(dev_phase == TEST){
    //Serial.print("post ");
    Serial.println(post_str);
    //Serial.println("OK.");
  }
}