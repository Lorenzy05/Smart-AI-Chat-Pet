#include <Arduino.h>
#include "i2s_manager.h"
#include <WiFi.h>
#include <RGB.h>
#include <robot_movement.h>
#include <oled_display.h>

// ---------------- Wi-Fi 配置 ----------------
const char* ssid = "STIPTE ALLES-IN-1";
const char* password = "35782211084585041641";

// Python 服务器 IP 和端口
const char* host = "192.168.178.91"; // Python TTS/STT 服务器
const uint16_t stt_port = 5005;
const uint16_t tts_port = 5006;

WiFiClient sttClient;
WiFiClient ttsClient;
I2SManager i2s;
Robot robot;

// 控制命令解析相关的变量
String controlBuffer = "";
const String CTRL_START = "!CTRL#";
const String CTRL_END = "#END";
const String PAUSE_STT_START = "!PAUSE_STT#";
bool isReceivingControl = false;

// STT 暂停功能变量
bool sttPaused = false;
unsigned long sttPauseEndTime = 0;

// 音频缓冲区
const size_t bufferSize = 512;
int16_t ttsBuffer[bufferSize];
int32_t micBuffer[bufferSize];
int16_t pcm16Buffer[bufferSize];

// 执行机器人命令的函数
void executeRobotCommand(String command) {
    Serial.println("执行命令: " + command);
    
    if (command == "FORWARD") {
        robot.Move_Forward(1);
    } else if (command == "BACKWARD") {
        robot.Move_Backward(1);
    } else if (command == "LEFT") {
        robot.Move_Right(1);
    } else if (command == "RIGHT") {
        robot.Move_Left(1);
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("Booting...");
    
    Wire.begin(8, 9);
    setup_display();
    Show_Red();

    // 初始化 I2S
    i2s.begin();

    // 连接 Wi-Fi
    WiFi.begin(ssid, password);
    Serial.printf("Connecting to Wi-Fi: %s\n", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Show_Blue();

    // 连接 STT
    while (!sttClient.connect(host, stt_port)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nSTT TCP connected!");

    // 连接 TTS
    while (!ttsClient.connect(host, tts_port)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nTTS TCP connected!");

    Show_White();

}

void loop() {
    // === 检查STT暂停状态 ===
    if (sttPaused && millis() >= sttPauseEndTime) {
        sttPaused = false;
        Serial.println("STT 暂停结束");
    }

    // === 发送麦克风数据（只有在未暂停时）===
    if (!sttPaused) {

        Show_Green();
        Face_Smile();

        size_t samplesRead = i2s.readMicrophone(micBuffer, bufferSize);
        i2s.convertMicBuffer(micBuffer, pcm16Buffer, samplesRead);

        if (sttClient.connected()) {
            sttClient.write((uint8_t*)pcm16Buffer, samplesRead * sizeof(int16_t));
        } else {
            Serial.println("STT TCP disconnected, reconnecting...");
            while (!sttClient.connect(host, stt_port)) delay(500);
        }
    }

    // === 接收 TTS 数据和控制命令 ===
    if (ttsClient.connected() && ttsClient.available()) {
        // 批量读取数据
        size_t bytesRead = ttsClient.read((uint8_t*)ttsBuffer, sizeof(ttsBuffer));
        
        // 检查是否是STT暂停命令
        if (bytesRead >= PAUSE_STT_START.length() && 
            memcmp(ttsBuffer, PAUSE_STT_START.c_str(), PAUSE_STT_START.length()) == 0) {
            
            // 查找暂停时间
            String dataStr;
            for (size_t i = 0; i < bytesRead; i++) {
                dataStr += (char)((uint8_t*)ttsBuffer)[i];
            }
            
            int startIdx = dataStr.indexOf(PAUSE_STT_START) + PAUSE_STT_START.length();
            int endIdx = dataStr.indexOf("#END", startIdx);
            
            if (endIdx != -1) {
                String pauseTimeStr = dataStr.substring(startIdx, endIdx);
                int pauseTime = pauseTimeStr.toInt();
                
                sttPaused = true;
                sttPauseEndTime = millis() + pauseTime;
                Serial.printf("STT 暂停 %d 毫秒\n", pauseTime);
            }
            return;
        }
        
        // 检查是否是控制命令
        if (bytesRead >= CTRL_START.length() && 
            memcmp(ttsBuffer, CTRL_START.c_str(), CTRL_START.length()) == 0) {
            
            // 查找控制命令结束标记
            String dataStr;
            for (size_t i = 0; i < bytesRead; i++) {
                dataStr += (char)((uint8_t*)ttsBuffer)[i];
            }
            
            int startIdx = dataStr.indexOf(CTRL_START) + CTRL_START.length();
            int endIdx = dataStr.indexOf("#END", startIdx);
            
            if (endIdx != -1) {
                String command = dataStr.substring(startIdx, endIdx);
                executeRobotCommand(command);
            }
            return;
        }
        
        // 如果是普通音频数据，直接播放
        i2s.writeDAC(ttsBuffer, bytesRead / 2);

        Show_Yellow();
        Face_Speaking();

    } else if (!ttsClient.connected()) {
        Serial.println("TTS TCP disconnected, reconnecting...");
        while (!ttsClient.connect(host, tts_port)) delay(500);
    }

    delay(5); // 避免占用 CPU 太高
}