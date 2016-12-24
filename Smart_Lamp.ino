#include<SoftwareSerial.h>
SoftwareSerial mySerial(10,11);//RX, TX
#include <TimeLib.h>  // 타임 라이브러리를 불러온다.
#include <TimeAlarms.h>// 타임알람 라이브러리를 불러온다.
String input = "";
void light();
void a();
void color();
void onoff();
void mp3();
void processSyncMessage();
void MoningAlarm();
void PauseAlarm();
int audioCommand (void);
int bluepin = 5; // select the pin for the blue LED
int greenpin = 4; // select the pin for the green LED
int redpin = 3; // select the pi
String mp3Cmd = "";
void MoningAlarm();
void PauseAlarm();
void setclock();
void digitalClockDisplay();
void printDigits(int digits);
void bulblight();

void setup() // 프로그램을 시작할때 한번만 실행되는 변수를 선언합니다.
{
  Serial.begin (9600);
  Serial1.begin(9600);
  mySerial.begin(9600);
  mySerial.println("Hello, android?");
  pinMode(13, OUTPUT);  
  pinMode (redpin, OUTPUT);
  pinMode (bluepin, OUTPUT);
  pinMode (greenpin, OUTPUT); 
   setTime(20,26,0,12,21,16);
//  Alarm.alarmRepeat(20,26,10,MoningAlarm);
//  Alarm.alarmRepeat(20,26,10,PauseAlarm);
}

void loop() // 프로그램이 끝날때까지 영원히 이 안의 명령을 반복합니다.
{
  
   digitalClockDisplay(); //디지탈 시계를 표시하라.
//   Alarm.delay(1000); // wait one second between clock display 1초 기다렸다가 표시하라.
   char Temp[100];
  if(mySerial.available()){
       char inChar = mySerial.read();
      input += inChar;
     if (inChar == 'z') {    
        input.substring(0,1).toCharArray(Temp,100);
        
        int _case = atoi(Temp);

         switch(_case){
         case 1:
              light();
              break;
         case 2:
              color();
              break;
         case 3:
              mp3();
              audioCommand();
              break;
         case 4:
              processSyncMessage();
              break;
         case 5:
              mp3();
              audioCommand();
              break;
         case 6:
              onoff();
              break;
         }
           input="";
      }
   }

}

void a(){
  while (true) {
     char inChar = mySerial.read();
      input += inChar;
     if (inChar == 'z') {
        break;
      }
   }
}

void light()
{
  char Temp[10];
  int len=input.indexOf('z',0);
  input.substring(2,len).toCharArray(Temp,len);
  int bright=atoi(Temp);
  Serial.println(bright);
  analogWrite(13,bright);
 }

//2,     0,28,255z
 void color()
 { 
   char ra[10];
   char ga[10];
   char ba[10];
  
   int len=input.indexOf('z',0);
   String Temp = input.substring(2,len);
   String t1 = Temp;
   String t2 = Temp;

   //Temp = 0,28,255
   int first = Temp.indexOf(',');
   int sec = Temp.indexOf(',', first + 1 );
  
   Temp.substring(0,first).toCharArray(ra,10);
   int r = atoi(ra);

   t1.substring(first+1,sec).toCharArray(ga,10);
   int g = atoi(ga);

   t2.substring(sec+1,len-2).toCharArray(ba,10);
   int b = atoi(ba);

   analogWrite (redpin, r );   
   analogWrite (greenpin, g );
   analogWrite (bluepin, b );
 }
 
 void onoff()//6,0    /1
 {  
  char Temp[10];
  int len=input.indexOf('z',0);
  input.substring(2,3).toCharArray(Temp,2);
  int bright=atoi(Temp);
  if(bright == 1){
   bright = 255;   
  }
  analogWrite(13,bright);
  analogWrite (redpin, bright);   
  analogWrite (greenpin, bright );
  analogWrite (bluepin, bright);
 }
 
 void mp3()
{
  int len=input.indexOf('z',0);
  mp3Cmd = input.substring(2,len);

}

int audioCommand () 
{
  if (mp3Cmd.equals ("p")) {
    Serial.println("play");
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x01);
  } 
  else if (mp3Cmd.equals ("s")) {
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x02);
  } 
  else if (mp3Cmd.equals ("n")) {
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x03);
  } else if (mp3Cmd.equals ("b")) {
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x04);
  } else if (mp3Cmd.equals ("u")) {
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x05);
  } else if (mp3Cmd.equals ("d")) {
    Serial1.write(0x7E);
    Serial1.write(0x02);
    Serial1.write(0x06);
  }
  else 
    return -1;
  Serial1.write(0xEF);
  return 0;
}

void processSyncMessage() {
    char Temp[10];
  // if time sync available from serial port, update time and return true
        char c_y[2];
        char c_m[2];
        char c_d[2];
        char c_h[2];
        char c_min[2];
      
   int len=input.indexOf('z',0);
  input.substring(2,3).toCharArray(Temp,2);
  int bright=atoi(Temp);
  input.substring(4,6).toCharArray(c_y,2);
  int _cy=atoi(c_y);
  input.substring(7,9).toCharArray(c_m,2);
  int _cm=atoi(c_m); 
  input.substring(10,12).toCharArray(c_d,2);
  int _cd=atoi(c_d);
  input.substring(13,15).toCharArray(c_h,2);
  int _ch=atoi(c_h);
  input.substring(16,18).toCharArray(c_min,2);
  int _cmin=atoi(c_min);  
}  

void bulblight(){
  int i;
  for(i=0;i<255;i++){
  analogWrite(13,i);
  delay(100);
  }
  analogWrite(13,0);
  analogWrite (redpin, 255 );
  analogWrite (bluepin, 255 );
  analogWrite (greenpin, 255 );
}


void PauseAlarm(){
  Serial.println("Alarm end");
  mp3Cmd = "s";
  audioCommand(); 
}

void MoningAlarm(){   //알람
  Serial.println("Alarm ring");
  mp3Cmd = "p";
  audioCommand(); 
  bulblight();
}

void digitalClockDisplay() //디지털 시계 표시
{
  // digital clock display of the time 시간의 디지털 시계 표시
  Serial.print(hour()); // 시간표시
  printDigits(minute()); // 분 표시
  printDigits(second());// 초 표시
  Serial.println();  // 시리얼 모니터에 표시하라.
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
