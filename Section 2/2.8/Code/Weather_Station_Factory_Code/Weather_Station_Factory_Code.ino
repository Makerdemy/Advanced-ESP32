
#include <FS.h>          
#include <WiFiManager.h>


#include <WiFiClient.h>
#include <Update.h>
#include <ESPmDNS.h>
#include <WebServer.h>


#define DHTPIN 27    
#define DHTTYPE DHT11   

#include "DHT.h"
#include <Wire.h>
#include "SSD1306.h" 

const char* hostname = "esp32";
SSD1306  display(0x3c, 21, 22);
DHT dht(DHTPIN, DHTTYPE);


WebServer server(80);


/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>"
"<h1>OTA Updater Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='pass')" //You can change the login credentials 
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

void setup() {
 
  Serial.begin(115200);
  display.init();
  dht.begin();
  WiFiManager wifiManager;

  display.clear();
  display.drawString(0, 0, "Booting");
  display.display();
  delay(1000);
  display.clear();
  
  wifiManager.autoConnect("ESPCONFIGAP");
  String ssid = wifiManager.getWiFiSSID();
 
  display.clear();
  display.drawString(0, 0, "Connected to this WiFi:");
  display.drawString(0, 20, ssid);
  display.display();
  delay(5000);
  
   /*use mdns for host name resolution*/
     MDNS.begin(hostname);
      Serial.println("mDNS responder started");
      Serial.print("Access the webserver at http://");
      Serial.print(hostname);
      Serial.print(".local/");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
       
  
        /*return index page which is stored in serverIndex */
        server.on("/", HTTP_GET, []() {
          server.sendHeader("Connection", "close");
          server.send(200, "text/html", loginIndex);
        });
        
        server.on("/serverIndex", HTTP_GET, []() {
          server.sendHeader("Connection", "close");
          server.send(200, "text/html", serverIndex);
        });
        
        /*handling uploading firmware file */
        server.on("/update", HTTP_POST, []() {
          server.sendHeader("Connection", "close");
          server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
          ESP.restart();
        }, []() {
          HTTPUpload& upload = server.upload();
          
          if (upload.status == UPLOAD_FILE_START) 
            {
              Serial.printf("Update: %s\n", upload.filename.c_str());
              if (!Update.begin(UPDATE_SIZE_UNKNOWN)) 
                { //start with max available size
                 Update.printError(Serial);
                }
            } 
          else if (upload.status == UPLOAD_FILE_WRITE) 
            {
         /* flashing firmware to ESP*/
              if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) 
                {
                 Update.printError(Serial);
                 }
             } 
          else if (upload.status == UPLOAD_FILE_END) {
               if (Update.end(true)) 
                  { //true to set the size to the current progress
                  Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                   } 
          else 
            {
              Update.printError(Serial);
            }
            
          }
        });
  server.begin();
    
  
}

void loop() {
  server.handleClient();

  float h = dht.readHumidity();
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

 
  static char hum[15];
  static char temp[15];

  dtostrf(h,7, 0, hum);
  dtostrf(t,7, 0, temp);
  
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C "));
  Serial.println();

  display.clear();
  display.drawString(30, 0, "Weather Station");
 
  
  display.drawString(0, 12,"Humidity: ");
  display.drawString(65,12, hum);
  display.drawString(95,12,"%");

  display.drawString(0, 24,"Temperature: ");
  display.drawString(65,24, temp);
  display.drawString(95, 24,"*C");
 
  display.display();
  delay(1000);
  display.clear();

  }
 
