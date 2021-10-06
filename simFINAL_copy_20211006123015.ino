#include <SoftwareSerial.h>

SoftwareSerial mySerial(14,12);  // (Rx,Tx  > Tx,Rx) 

char incomingByte; 
String inputString;
int relay = 5; // Выход для управления реле


void setup() 
{
      pinMode(relay, OUTPUT);
      digitalWrite(relay, LOW); // Исходное состояние реле
      Serial.begin(9600);
      mySerial.begin(9600);      
      while(!mySerial.available())
      {
        mySerial.println("AT");
        delay(1000); 
        Serial.println("Connecting...");
      }
      Serial.println("Connected!");  
      mySerial.println("AT+CMGF=1");  // Установка SMS в текстовый режим
      delay(1000);  
      mySerial.println("AT+CNMI=1,2,0,0,0");  // Процедура обработки вновь полученных сообщений 
      delay(1000);
      mySerial.println("AT+CMGL=\"REC UNREAD\""); // Прочитать непрочитанные сообщения
      delay(1000);   
 }
void loop()
{  
  if(mySerial.available()){
      delay(100);

      // Последовательный буфер
      while(mySerial.available()){
        incomingByte = mySerial.read();
        inputString += incomingByte; 
        }

        delay(10);      

        Serial.println(inputString);
        inputString.toUpperCase();

        // включить или выключить реле
        if (inputString.indexOf("ON") > -1){
          digitalWrite(relay, HIGH);
          }
         if (inputString.indexOf("OFF") > -1){
          digitalWrite(relay, LOW);
          }
          if (inputString.indexOf("TEST") > -1){ 
            mySerial.println("AT+CMGS=\"+79967958097\"");
            delay(500);
            mySerial.print(digitalRead(5));
            delay(500);
            mySerial.print((char)26);
            delay(500);
            }     

        delay(50);

        // Удалить сообщения и сохранить память
        if (inputString.indexOf("OK") == -1){
        mySerial.println("AT+CMGDA=\"DEL ALL\"");

        delay(1000);}

        inputString = "";
  }
}