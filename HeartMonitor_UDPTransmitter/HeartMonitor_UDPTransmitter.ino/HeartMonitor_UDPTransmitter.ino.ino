#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//const char* ssid     = "ARPANET";
//const char* password = "internetinternet";

const char* ssid     = "MOVISTAR_927E";
const char* password = "Urn60gBG5mYpcgvtIypJ";

IPAddress broadcast;
IPAddress MyIp;
unsigned int sendPort = 50002;

WiFiUDP Udp;

const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(115200);

  digitalWrite(BUILTIN_LED, HIGH);
 
  /* Script file name MUST be included in every script */
  Serial.println();
  Serial.println("HeartMonitor_UDPTransmitter");
  Serial.println("V 2.00");
  Serial.println();
  
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect to ");
  
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  MyIp = WiFi.localIP();
  broadcast = MyIp;
  broadcast[3] = 255;

//set up the UDP sender
 Udp.begin(sendPort);

 digitalWrite(BUILTIN_LED, LOW);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
  if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
  }
 }

void transmitNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        newData = false;
        Udp.beginPacket(broadcast,sendPort);
        Udp.write(receivedChars); 
        Udp.endPacket();   
    }
}

void testTransmit() {
      receivedChars[0]='H'; 
      receivedChars[1]='e'; 
      receivedChars[2]='l'; 
      receivedChars[3]='l'; 
      receivedChars[4]='o';
      receivedChars[5]=' ';
      int randomNumber = random(0, 10); 

      switch (randomNumber) 
     {
         case 0:
          receivedChars[6]='x';
          break;
        case 1:
          receivedChars[6]='r';
          break;
        case 2:
          receivedChars[6]='j';
          break;
        case 3:
          receivedChars[6]='w';
          break;
        case 4:
          receivedChars[6]='p';
          break;
        case 5:
          receivedChars[6]='t';
          break;
        case 6:
          receivedChars[6]='q';
          break;
        case 7:
          receivedChars[6]='m';
          break;
        case 8:
          receivedChars[6]='z';
          break;
        case 9:
          receivedChars[6]='f';
          break;
        case 10:
          receivedChars[6]='c';
          break;
    }
     
      newData = true;       
}




void loop() {
    //recvWithStartEndMarkers();
    testTransmit();
    delay(1000);
    transmitNewData();
}
