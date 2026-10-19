#include <WiFi.h>
#include <WebServer.h>
#include <stdio.h>

const char* ssid_AP = "ESP32_LED";
const char* password_AP = "12345678";

WebServer server(80);

float currentTemp = 0.0;
float currentHumi = 0.0;

void setup() {
  Serial.begin(115200);
  
  
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 
  Serial2.setTimeout(50);

  delay(1000);
  Serial.println("==== ESP32 启动 ====");

  WiFi.softAP(ssid_AP, password_AP);
  Serial.print("ESP32 自己产生的 WiFi 名字: ");
  Serial.println(ssid_AP);
  Serial.print("管理后台 IP 地址: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    String ledState = server.arg("LED");

    if (ledState == "ON") {
      Serial2.print('1');
      Serial.println("Sent '1' to STM32");
      server.send(200, "text/html", "<meta charset=\"UTF-8\"><h1>LED 已开启</h1><a href=\"/\">返回</a>");
    } 
    else if (ledState == "OFF") {
      Serial2.print('0');
      Serial.println("Sent '0' to STM32");
      server.send(200, "text/html", "<meta charset=\"UTF-8\"><h1>LED 已关闭</h1><a href=\"/\">返回</a>");
    } 
    else {
      String html = "<html><head><meta charset=\"UTF-8\">";
      html += "<meta http-equiv=\"refresh\" content=\"5\">";
      html += "<style>body { text-align: center; font-family: sans-serif; margin-top: 50px; }</style>";
      html += "</head><body>";
      html += "<h1>STM32 环境监测站</h1>";
      html += "<hr>";
      html += "<h2>当前温度: " + String(currentTemp) + " °C</h2>";
      html += "<h2>当前湿度: " + String(currentHumi) + " %</h2>";
      html += "<hr>";
      html += "<h3>远程控制</h3>";
      html += "<a href=\"/?LED=ON\"><button style=\"font-size:20px; margin:10px; padding:10px 20px;\">开灯</button></a>";
      html += "<a href=\"/?LED=OFF\"><button style=\"font-size:20px; margin:10px; padding:10px 20px;\">关灯</button></a>";
      html += "</body></html>";
      server.send(200, "text/html", html);
    }
  });

  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();

  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();

    Serial.print("收到原始数据: [");
    Serial.print(data);
    Serial.print("]  长度: ");
    Serial.println(data.length());

    
    int tIndex = data.indexOf("T:");
    int cIndex = data.indexOf(" C");
    int hIndex = data.indexOf("H:");
    int pIndex = data.indexOf(" %");

    if (tIndex != -1 && cIndex != -1 && hIndex != -1 && pIndex != -1) {
      String tStr = data.substring(tIndex + 2, cIndex);
      String hStr = data.substring(hIndex + 2, pIndex);

      currentTemp = tStr.toFloat();
      currentHumi = hStr.toFloat();

      Serial.print("解析成功 -> 温度: ");
      Serial.print(currentTemp);
      Serial.print(" C, 湿度: ");
      Serial.println(currentHumi);
    } else {
      Serial.println("⚠️ 格式不匹配，解析失败");
    }
  }
}
