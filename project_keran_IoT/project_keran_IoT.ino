#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//----------------------------------------Include the Servo Library
#include <Servo.h>
//
//const char* ssid     = "dragneel";
//const char* password = "Fire Dragneel";
//;
//const char* host = "192.168.1.8"; //


const char* ssid     = "killu";
const char* password = "12345678";
//const char* host = "192.168.43.200"; // host website
const char* host = "langsattanjung.com"; // host website

Servo myservo;
#define ServoPort D1   //--> Defining Servo Port

WiFiClient client;
const int httpPort = 80; //
String url; //


unsigned long timeout; //
int servo;
int status_keran;

void setup() {
  Serial.begin(115200);
  delay(10);
  myservo.attach(ServoPort); //--> attaches the servo on D1 to the servo object

  //  pinMode(D0, OUTPUT);
  //  digitalWrite(D0, HIGH);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); //
  Serial.println(ssid); //

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  
  int sensorWater = analogRead(A0);
  Serial.print("connecting to "); 
  Serial.println(host); // Use WiFiClient class to create TCP connections 
  WiFiClient client; 
  const int httpPort = 80; 
  if (!client.connect(host, httpPort)) 
  { 
  Serial.println("connection failed"); 
  return; 
  } 


  //  water sensor
  if (sensorWater <= 300) {
    Serial.println("empty");

    // We now create a URI for the request
    url = "http://langsattanjung.com/project_keran/arduino_get.php?id_pelanggan=1";

    Serial.print("Requesting URL: ");
    Serial.println(url);
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      if (client.find("ON")) {
        Serial.println("keran ON");
        //        servo = 180;
        myservo.write(90);
        return;
      } else {
        Serial.println("keran OFF");
        //      servo = 0;
        myservo.write(0);
        return;
      }
    }

  } else if (sensorWater > 300) {
    Serial.println("medium");
    HTTPClient http;    //Declare object of class HTTPClient

    // We now create a URI for the request
    url = "http://langsattanjung.com/project_keran/arduino_get.php?id_pelanggan=1";

    Serial.print("Requesting URL: ");
    Serial.println(url);
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
//    timeout = millis();
//    while (client.available() == 0) {
//      if (millis() - timeout > 5000) {
//        Serial.println(">>> Client Timeout !");
//        client.stop();
//        return;
//      }
//    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      if (client.find("ON")) {
        status_keran = 0;
        return;
      } else {
        status_keran = 1;
        return;
      }
    }

    int id_pelanggan = 1;
    
     //Post Data
    String postData = "id_pelanggan=";
    postData += id_pelanggan;
    postData += "&keran_status=";
    postData += status_keran;
 
    http.begin("http://langsattanjung.com/project_keran/updateStatus.php"); 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    auto httpCode = http.POST(postData); 
    Serial.println(httpCode); //Print HTTP return code 
    String payload = http.getString(); 
    Serial.println(payload); //Print request response payload 
    http.end(); //Close connection Serial.println();

    if(httpCode == 200) {
          // We now create a URI for the request
        url = "http://langsattanjung.com/project_keran/arduino_get.php?id_pelanggan=1";
    
        Serial.print("Requesting URL: ");
        Serial.println(url);
        // This will send the request to the server
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }
    
        // Read all the lines of the reply from server and print them to Serial
        while (client.available()) {
          if (client.find("ON")) {
            Serial.println("keran ON");
            //        servo = 180;
            myservo.write(90);
            return;
          } else {
            Serial.println("keran OFF");
            //      servo = 0;
            myservo.write(0);
            return;
          }
        }
    }

//    delay(5000);  //Post Data at every 5 seconds
  }



  Serial.println();
  Serial.println("closing connection");
  Serial.println();
  //  delay(20000);
}
