#include <WiFi.h>
#include <WebServer.h>

const char* ssid_AP = "ESP32_LED";     // ESP32 自己发出的 WiFi 名字
const char* password_AP = "12345678";  // ESP32 自己的 WiFi 密码

WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // 与STM32通信

  delay(1000);
  Serial.println();

  // ⚠️ 开启 AP 模式（自己变热点）
  WiFi.softAP(ssid_AP, password_AP);
  Serial.print("ESP32 自己产生的 WiFi 名字: ");
  Serial.println(ssid_AP);
  Serial.print("管理后台 IP 地址: ");
  Serial.println(WiFi.softAPIP()); // 默认必定是 192.168.4.1

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
      String html = "<html><head><meta charset=\"UTF-8\"></head><body><h1>STM32 LED Control</h1>";
      html += "<a href=\"/?LED=ON\"><button style=\"font-size:20px; margin:10px;\">开灯</button></a>";
      html += "<a href=\"/?LED=OFF\"><button style=\"font-size:20px; margin:10px;\">关灯</button></a>";
      html += "</body></html>";
      server.send(200, "text/html", html);
    }
  });

  server.begin();
  Serial.println("Web服务器已启动");
}

void loop() {
  server.handleClient();
}