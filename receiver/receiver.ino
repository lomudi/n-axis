#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
char ssid[] = "Milab";     // your network SSID (name)
char pass[] = "Milab2016"; // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(10, 40, 10, 105); // remote IP (not needed for receive)
const unsigned int outPort = 9999;      // remote port (not needed for receive)
const unsigned int localPort = 8888;    // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

void setup()
{
  //pinMode(BUILTIN_LED, OUTPUT);
  ///digitalWrite(BUILTIN_LED, ledState);    // turn *on* led
  Serial.begin(115200);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif
}

void imuquat(OSCMessage &msg)
{
  float roatationAnd = msg.getFloat(0); // when battry is up - left +, right -
  float sideAng = msg.getFloat(1); // when battry is up - left +, right -
  float pitchAng = msg.getFloat(2); // when battry is up - back -, fornt +
  Serial.println(String(msg.getFloat(0)) + " , " + msg.getFloat(1) + " , " + msg.getFloat(2));

  
}

void loop()
{
  handleMsg();
}

void handleMsg()
{
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0)
  {
    //-- try puting in to an arry
    //change the wihle to be until the size of arry
    while (size--)
    {

      msg.fill(Udp.read());
    }
    if (!msg.hasError())
    {
      msg.dispatch("/imuquat", imuquat);
    }
    else
    {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
