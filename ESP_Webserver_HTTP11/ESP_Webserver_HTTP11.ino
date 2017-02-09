/*--------------------------------------------------
  HTTP 1.1 Webserver for ESP8266
  for ESP8266 adapted Arduino IDE

  Stefan Thesen 04/2015

  Running stable for days
  (in difference to all samples I tried)

  Does HTTP 1.1 with defined connection closing.
  Reconnects in case of lost WiFi.
  Handles empty requests in a defined manner.
  Handle requests for non-exisiting pages correctly.

  Serial output can e.g. be used to steer an attached
  Arduino, Raspberry etc.
  --------------------------------------------------*/

#include <ESP8266WiFi.h>

const String COLOR_BLACK      = "#000000";
const String COLOR_WHITE      = "#FFFFFF";
const String COlOR_GREY       = "#808080";
const String COLOR_RED        = "#FF0000";
const String COLOR_GREEN      = "#32CD32";
const String COLOR_BLUE       = "#FFFF00";
const String COLOR_LIGHT_BLUE = "#B2DFEE";


String font_color;
String background_color;
String button_color_function1;
String button_color_function2;

boolean module_start;
boolean function1;
boolean function2;

const char* ssid = "DD8ZJ";
const char* password = "";

//unsigned long ulReqcount;
//unsigned long ulReconncount;


// Create an instance of the server on Port 80
WiFiServer server(80);

void setup()
{
  // setup globals
  module_start = true;
  function1 = false;
  function2 = false;
  button_color_function1 = COLOR_RED;
  button_color_function2 = COLOR_RED;
  background_color = COLOR_LIGHT_BLUE;
  font_color = COLOR_BLACK;

  //ulReqcount=0;
  //ulReconncount=0;

  // prepare GPIO1
  pinMode(1, OUTPUT);
  digitalWrite(1, 0);

  // prepare GPIO2
  pinMode(2, INPUT);
 // digitalWrite(2, 0);

  // start serial
  Serial.begin(9600);
  delay(1);

  // inital connect
  WiFi.mode(WIFI_STA);
  WiFiStart();

}
//--------------------------------------------------------------------------------------------------------------------------------------
void WiFiStart()
{
  // ulReconncount++;

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

//---------------------------------------------------------------------------------------------------------------------------------------
void loop()
{
  if (WiFi.status() != WL_CONNECTED)                                //when not connected to a WiFi
  {
    WiFiStart();
  }

  WiFiClient client = server.available();                           //a Client object, if no client is connected ---> NULL
  if (!client)
  {
    return;
  }

  // Wait until the client sends some data
  //Serial.println("new client");
  unsigned long ultimeout = millis() + 250;
  while (!client.available() && (millis() < ultimeout) )            //returns the amount of data that has been written to the client by the server it is connected to && wait 250ms
  {
    delay(1);
  }
  if (millis() > ultimeout)
  {
    Serial.println("client connection time-out!");
    return;
  }

  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');                   //Read the next String received from the server the client is connected to
  // Serial.println("Client_Request:" + sRequest);
  client.flush();                                                   //Discard any bytes that have been written to the client but not yet read

  // stop client, if request is empty
  if (sRequest == "")
  {
    // Serial.println("empty request! - stopping client");
    client.stop();                                                  //Disconnect from the server
    return;
  }

  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sPath = "", sParam = "", sCmd = "";
  String sGetstart = "GET ";
  int iStart, iEndSpace, iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart >= 0)
  {
    iStart += +sGetstart.length();
    iEndSpace = sRequest.indexOf(" ", iStart);
    iEndQuest = sRequest.indexOf("?", iStart);

    // are there parameters?
    if (iEndSpace > 0)
    {
      if (iEndQuest > 0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart, iEndQuest);
        sParam = sRequest.substring(iEndQuest, iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart, iEndSpace);
      }
    }
  }

  ///////////////////////////////////////////////////////////////////////////////
  // output parameters to serial, you may connect e.g. an Arduino and react on it
  ///////////////////////////////////////////////////////////////////////////////
  if (sParam.length() > 0)
  {
    int iEqu = sParam.indexOf("=");
    if (iEqu >= 0)
    {
      sCmd = sParam.substring(iEqu + 1, sParam.length());
      Serial.println(sCmd);
      Serial.flush();

      while(digitalRead(2) == LOW);

    }
  }

  //-------------------------------------------------------------------------------------------------------------------------------------------------
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse, sHeader;

  ////////////////////////////
  // 404 for non-matching path
  ////////////////////////////
  if (sPath != "/")
  {
    sResponse = "<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";

    sHeader  = "HTTP/1.1 404 Not found\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  ///////////////////////
  // format the html page
  ///////////////////////
  else
  {
    //ulReqcount++;
    sResponse  = "<html><head><title>ESP8266 Steuerung</title></head><body>\r\n";
    sResponse += "<font color=\"" + font_color + "\"><body bgcolor=\"" + background_color + "\">\r\n";
    sResponse += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">\r\n";
    sResponse += "<meta http-equiv=\"refresh\" content=\"5\">\r\n";
    sResponse += "<h1>ESP8266 Steuerung</h1>\r\n";
    sResponse += "<h4>Funktion 1 schaltet GPIO30 </h4>\r\n";
    sResponse += "<h4>Funktion 2 schaltet GPIO40 </h4>\r\n";

    //////////////////////
    // react on parameters
    //////////////////////

    if (sCmd.length() > 0)
    {
      // write received command to html page
      sResponse += "Letztes Kommando: " + sCmd + "<BR><BR>";

      // switch GPIO
      if (sCmd.indexOf("FUNCTION1ON") >= 0)
      {
        function1 = true;
        button_color_function1 = COLOR_GREEN;
      }
      else if ((sCmd.indexOf("FUNCTION1OFF") >= 0))
      {
        function1 = false;
        button_color_function1 = COLOR_RED;
      }
      else if ((sCmd.indexOf("FUNCTION2ON") >= 0))
      {
        function2 = true;
        button_color_function2 = COLOR_GREEN;
      }
      else if ((sCmd.indexOf("FUNCTION2OFF") >= 0))
      {
        function2 = false;
        button_color_function2 = COLOR_RED;
      }
    }
    else sResponse += "<BR><BR>";

    if (function1 == false) {
      sResponse += "Funktion 1:&nbsp;&nbsp;&nbsp;&nbsp;<input type = button onClick = \"location.href='?pin=FUNCTION1ON'\" value = 'Einschalten&nbsp;' style = \"background-color:" + button_color_function1 + ";font-family:Arial;font-size:15px;\"><BR>\r\n";
    }
    else if (function1 == true) {
      sResponse += "Funktion 1:&nbsp;&nbsp;&nbsp;&nbsp;<input type = button onClick = \"location.href='?pin=FUNCTION1OFF'\" value = 'Ausschalten' style = \"background-color:" + button_color_function1 + ";font-family:Arial;font-size:15px;\"><BR>\r\n";
    }
    if (function2 == false) {
      sResponse += "Funktion 2:&nbsp;&nbsp;&nbsp;&nbsp;<input type = button onClick = \"location.href='?pin=FUNCTION2ON'\" value = 'Einschalten&nbsp;' style = \"background-color:" + button_color_function2 + ";font-family:Arial;font-size:15px;\"><BR>\r\n";
    }
    else if (function2 == true) {
      sResponse += "Funktion 2:&nbsp;&nbsp;&nbsp;&nbsp;<input type = button onClick = \"location.href='?pin=FUNCTION2OFF'\" value = 'Ausschalten' style = \"background-color:" + button_color_function2 + ";font-family:Arial;font-size:15px;\"><BR>\r\n";
    }

    /* sResponse += "<FONT SIZE=-2>";
       sResponse += "<BR>Aufrufz&auml;hler=";
       sResponse += ulReqcount;
       sResponse += " - Verbindungsz&auml;hler=";
       sResponse += ulReconncount;
       sResponse += "<BR>";
       sResponse += "</body></html>"; */

    sHeader  = "HTTP/1.1 200 OK\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }

  client.print(sHeader);                                              //Print data to the server that a client is connected to
  client.print(sResponse);                                            //Print data to the server that a client is connected to

  // and stop the client
  client.stop();                                                      //Disconnect from the server
  // Serial.println("Client stopped");
}
