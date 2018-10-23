#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "ssid"; //你家Wifi名稱
const char* password = "password"; //你家Wifi密碼

const char* host = "serverDomain"; //控制LINE Massaging API的網址
const int httpsPort = 443; //server走的port

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "35 85 74 EF 67 35 A7 CE 40 69 50 F3 C0 F6 80 CF 80 3B 2E 19";

int ledPin = 13; //設定第13 pin給LED燈
int inPin = 2; //設定第2（NodeMCU的4D） pin為輸入pin (為了讀取按鈕狀態)
int val = 0; //讀取按鈕狀態
int lastVal = 0; //上一個按鈕狀態

unsigned long previous_time; //上一次磁簧開關觸發的時間

void setup() {
  pinMode(ledPin, OUTPUT); //設定LED pin腳狀態為輸出
  pinMode(inPin, INPUT); //設定第2 pin狀態為輸入
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
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
  notifyLunch(); //發出LINE訊息通知系統已啟動
}

void loop() {
  val = digitalRead(inPin); //讀取輸入pin腳的值
  if (val == HIGH) { //確認是否輸入為高電壓 (按鈕壓下)
    digitalWrite(ledPin, LOW); //給pin腳低電壓 (關掉LED燈)
    if(lastVal==0 && millis()-previous_time>=10000) { //當上一次輸入pin腳的值為0（門是闔上）且 上一次起的時間跟現在開啟的時間差大於10秒
      notifyOpen(); //發出LINE通知有人開門
      previous_time = millis(); //紀錄當下時間
    }
    lastVal = 1; //更新lastVal
  } else {
    digitalWrite(ledPin, HIGH); //給pin腳高電壓 (開啟LED燈)
    if(lastVal==1) {
      lastVal = 0; //更新lastVal
    }
  }
}

void notifyLunch() {
  httpsRequest("/index.php?type=0");
}

void notifyOpen() {
  httpsRequest("/index.php?type=1");
}

void httpsRequest(String url) {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }

  //String url = "/index.php";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}
