#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <MCP23017.h>


void checkRecieved();
void myUdpSend(String msg);
String getValue(String data, char separator, int index);

/*---------------*/
// wifi config
/*----------------*/
char* ssid = "r2";
char* password = "qt2016c++";

/*---------------*/
// UDP packet handler
/*----------------*/
IPAddress ipBroadCast;
unsigned int udpRemotePort=45454; // port output data
unsigned int udplocalPort=45455;  // port input data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
WiFiUDP udp;

/*---------------*/
// MCP23017 handler
/*----------------*/
MCP23017 expander;


/*---------------*/
// MATRIX LED 7x5
/*----------------*/

#define rows  7
#define columns  5
//print A
int m[rows][columns] = {
  { 0, 1, 1, 1, 0 },
  { 0, 1, 0, 1, 0 },
  { 0, 1, 1, 1, 0 },
  { 0, 1, 0, 1, 0 },
  { 0, 1, 0, 1, 0 },
  { 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0 }
};

void setup()
{
  //start connecting to wifi....
  WiFi.begin(ssid,password);

  //start serial interface for debuging...
  Serial.begin(115200);

  // wait untill esp8266 connected to wifi...
  while(WiFi.status()!=WL_CONNECTED)
  {
      Serial.print(".");
      delay(500);
  }
  // debuging ...
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // todo: config ip broadcast

  ipBroadCast = WiFi.localIP() ;
  ipBroadCast[3] = 255;
  // set udp port for listen...
  udp.begin(udplocalPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  // initialize TwoWire communication
  Wire.begin();

  // set GPB0 to be an output (LED)
  expander.getPin(MCP23017_GPB0).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB1).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB2).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB3).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB4).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB5).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPB6).setPinMode(OUTPUT);

  expander.getPin(MCP23017_GPA0).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPA1).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPA2).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPA3).setPinMode(OUTPUT);
  expander.getPin(MCP23017_GPA4).setPinMode(OUTPUT);

  // // set GPB0 to be inverted (pull to ground instead of up)
  // expander.getPin(MCP23017_GPB0).setPolarityInvert(true);
  // set GPA0 to be an input (push button)
  //expander.getPin(MCP23017_GPA0).setPinMode(INPUT);

  // setup the MCP23017
  expander.setup();

}

int x,y;

void loop()
{

  for( int i = 0; i < rows ; i++)
  {
      //expander.read();
      expander.getPin(MCP23017_GPA0).setValue(HIGH);
      expander.getPin(MCP23017_GPA1).setValue(HIGH);
      expander.getPin(MCP23017_GPA2).setValue(HIGH);
      expander.getPin(MCP23017_GPA3).setValue(HIGH);
      expander.getPin(MCP23017_GPA4).setValue(HIGH);

      expander.getPin(MCP23017_GPB0).setValue(LOW);
      expander.getPin(MCP23017_GPB1).setValue(LOW);
      expander.getPin(MCP23017_GPB2).setValue(LOW);
      expander.getPin(MCP23017_GPB3).setValue(LOW);
      expander.getPin(MCP23017_GPB4).setValue(LOW);
      expander.getPin(MCP23017_GPB5).setValue(LOW);
      expander.getPin(MCP23017_GPB6).setValue(LOW);

      expander.write();

      if( i == 0 ) expander.getPin(MCP23017_GPB0).setValue(HIGH);
      if( i == 1 ) expander.getPin(MCP23017_GPB1).setValue(HIGH);
      if( i == 2 ) expander.getPin(MCP23017_GPB2).setValue(HIGH);
      if( i == 3 ) expander.getPin(MCP23017_GPB3).setValue(HIGH);
      if( i == 4 ) expander.getPin(MCP23017_GPB4).setValue(HIGH);
      if( i == 5 ) expander.getPin(MCP23017_GPB5).setValue(HIGH);
      if( i == 6 ) expander.getPin(MCP23017_GPB6).setValue(HIGH);

      if( m[i][0] == 1 )  expander.getPin(MCP23017_GPA0).setValue(LOW);
      if( m[i][1] == 1 )  expander.getPin(MCP23017_GPA1).setValue(LOW);
      if( m[i][2] == 1 )  expander.getPin(MCP23017_GPA2).setValue(LOW);
      if( m[i][3] == 1 )  expander.getPin(MCP23017_GPA3).setValue(LOW);
      if( m[i][4] == 1 )  expander.getPin(MCP23017_GPA4).setValue(LOW);

      expander.write();
      delayMicroseconds(500) ;

  }
  checkRecieved();

}

void myUdpSend(String msg)
{
    // convert string to char array
    int UDP_PACKET_SIZE = msg.length();
    char udpBuffer[UDP_PACKET_SIZE] ;
    msg.toCharArray(udpBuffer, UDP_PACKET_SIZE) ;

    // send msg broadcast to port destinie
    udp.beginPacket(ipBroadCast, udpRemotePort);
    udp.write(udpBuffer, sizeof(udpBuffer));
    udp.endPacket();
}

void checkRecieved()
{
    int packetSize = udp.parsePacket();

    if(packetSize)
    {
        //Serial.println("");
        //Serial.print("Received packet of size ");
        //Serial.println(packetSize);
        //Serial.print("From ");
        IPAddress remote = udp.remoteIP();
        // for (int i =0; i < 4; i++)
        // {
        //     //Serial.print(remote[i], DEC);
        //     if (i < 3)
        //     {
        //         //Serial.print(".");
        //     }
        // }
        //Serial.print(", port ");
        //Serial.println(udp.remotePort());

        // read the packet into packetBufffer
        udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);

        //Serial.println("Contents:");
        //for (int i =0; i < packetSize; i++)
            //Serial.print(packetBuffer[i]);

        //traitment of packet
        String myString = String(packetBuffer);
        if(myString == "<Matrix>clear")
        {
          for( int i = 0; i < rows ; i++)
          {
            for( int j = 0 ; j < columns ; j++ )
            {
              m[i][j] = 0 ;
            }
          }
        }
        else if(myString.indexOf("<Matrix>") >= 0)
        {
            myString.replace("<Matrix>","");

            //int valueI = myString.toInt();
            //Serial.println(" ");
            for(int i = 0 ; i < rows * columns ; i++)
            {
              x = i % columns ;
              y = i / columns ;
              //Serial.println("i: "+String(i)+" data: "+ String(getValue(myString, ',', i).toInt())+" | x= "+String(x)+" y= "+String(y));
              m[y][x] = getValue(myString, ',', i).toInt();
            }

        }
        for (int i =0; i < packetSize; i++)
            packetBuffer[i]= 0;
        myString = "";

    }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
