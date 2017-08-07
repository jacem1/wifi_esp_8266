#include <SoftwareSerial.h>

SoftwareSerial ESP8266(10, 11);

String NomduReseauWifi = "your_ssid"; // Garder les guillements
String MotDePasse      = "your_password"; // Garder les guillements


///****************************************************************/
///*                             INIT                             */
///****************************************************************/
int calibrationTime = 10;       
int  interval = 1000; // Wait between dumps
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int nPIR_detect;
boolean noMotion = true;
int motion = 2;
int minSecsBetweenUpdates = 300; // 5 minutes
long lastSend = -minSecsBetweenUpdates * 1000l;
boolean lockLow = true;
boolean takeLowTime; 
//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;

void setup()
{

// /// ************** Sensor calibration
  Serial.begin(115200);
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  pinMode(7,OUTPUT);
digitalWrite(7,LOW);



  /// ************** Start of wifi connection
//  Serial.begin(115200);
  ESP8266.begin(115200);  
  initESP8266();
}
/****************************************************************/
/*                        BOUCLE INFINIE                        */
/****************************************************************/
void loop()
{

if (digitalRead(pirPin) == HIGH) 
{
  ///////////////////////
       digitalWrite(7, HIGH);   //the led visualizes the sensors output pin state
        
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         
 envoieAuESP8266("AT+CIPSEND=0,26");
 recoitDuESP8266(1000);
 envoieAuESP8266("XXX Mouvement Detected XXX");
       //  }         
         takeLowTime = true;
  //////////////////////

}
else
{
digitalWrite(7, LOW);

           delay(50);
 envoieAuESP8266("AT+CIPSEND=0,14");
 recoitDuESP8266(1000);
 envoieAuESP8266("No mouvement -");

  
}


} // end loop
/****************************************************************/
/*                Fonction qui initialise l'ESP8266             */
/****************************************************************/
void initESP8266()
{  
  Serial.println("**********************************************************");  
  Serial.println("**************** DEBUT DE L'INITIALISATION ***************");
  Serial.println("**********************************************************");  
envoieAuESP8266("AT+RST");
recoitDuESP8266(2000);
  Serial.println("****************** Start Wifi connection ******************");
  envoieAuESP8266("AT+CWMODE=3");
  recoitDuESP8266(5000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CWJAP=\""+ NomduReseauWifi + "\",\"" + MotDePasse +"\"");
  recoitDuESP8266(10000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIFSR");
  recoitDuESP8266(1000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIPMUX=1");
  recoitDuESP8266(1000);
  Serial.println("**********************************************************");
  envoieAuESP8266("AT+CIPSERVER=1,80");
  recoitDuESP8266(1000);
  Serial.println("**************** Open Putty: you have 10 seconds *********************");
  delay(10000);


}

/****************************************************************/
/*        Fonction qui envoie une commande à l'ESP8266          */
/****************************************************************/
void envoieAuESP8266(String commande)
{  
  ESP8266.println(commande);
}
/****************************************************************/
/*Fonction qui lit et affiche les messages envoyés par l'ESP8266*/
/****************************************************************/
void recoitDuESP8266(const int timeout)
{
  String reponse = "";
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(ESP8266.available())
    {
      char c = ESP8266.read();
      reponse+=c;
    }
  }
  Serial.print(reponse);   
}
