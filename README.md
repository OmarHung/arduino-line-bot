# arduino-line-bot
使用NodeMCU 開發板 ESP8266 WiFi ESP-12E 連到指定網頁去觸發LINE Massaging API  
 
使用材料：   
1.NodeMCU 開發板 ESP8266 WiFi ESP-12E   
2.KY-021 迷你磁簧開關模組   

原理 
利用NodeMCU的ESP8266 Wifi晶片連線到家中網路，當磁簧開關被斷開後，ESP8266連線到內含調控LINE Massaging API功能的網頁   
  
方法
1.LINE Massaging API部分：   
  1.1.申請LINE Massaging API  
  1.2.搭建網頁：將web/index.php設為調控LINE Massaging API頁面   
  
2.NodeMCU開發板設定部分：   
  2.1.打開Arduino IDE選擇檔案->偏好設定   
  2.2.在「額外的開發板管理員網址」輸入http://arduino.esp8266.com/stable/package_esp8266com_index.json ，並按下確定   
  2.3.Arduino IDE選擇工具->開發版：你的板子型號->開發板管理員...  
  2.4.搜尋「esp8266」，選擇「ESP8266 Community」安裝   
  2.5.Arduino IDE選擇工具->開發版：你的板子型號，選擇「NodeMCU 1.0 (ESP-12E Module)」  
  2.6.將Arduino/HttpsRequestForLine.ino的程式碼上傳到NodeMCU開發板   
  
3.接線部分：KY-021 迷你磁簧開關模組   
    KY-021      NodeMCU
    GND     ->  GND
    +5v     ->  3V3
    signal  ->  4D

