#ifndef llm_client
#define llm_client

/*
功能：向 LLM 发送问题并获取回答
核心函数：

void llmInit(const char* host, uint16_t port); —— 初始化网络/连接参数

String llmSendPrompt(const String& prompt); —— 获取完整回答

void llmSendPromptStream(const String& prompt, void (*callback)(String)); —— 流式输出，每接收到一段调用回调
*/

#pragma once
#include <WiFi.h>
#include <WiFiClient.h>

// 你的 WiFi 信息
const char* ssid = "STIPTE ALLES-IN-1";
const char* password = "35782211084585041641";

const char* ollamaHost = "192.168.178.91"; // 运行 Ollama 的电脑 IP
const uint16_t ollamaPort = 11434;

String sendPromptStreamToOllama(const char* host, uint16_t port, const char* model, const String& prompt) {
    WiFiClient client;
    if (!client.connect(host, port)) {
        Serial.println("连接 Ollama 失败");
        return ""; // 修正：返回空字符串
    }

    // 准备 JSON body
    String body = "{\"model\":\"" + String(model) + "\",\"prompt\":\"" + prompt + "\",\"stream\":true}";
    int contentLength = body.length();

    // HTTP 请求头（必须有 Content-Length 和空行）
    String request =
        String("POST /api/generate HTTP/1.1\r\n") +
        "Host: " + host + "\r\n" +
        "Content-Type: application/json\r\n" +
        "Content-Length: " + String(contentLength) + "\r\n" +
        "Connection: close\r\n\r\n" +
        body; // 注意这里是紧接着 body

    // 发送请求
    client.print(request);

    // 跳过 HTTP 头，进入正文
    bool headerEnded = false;
    String fullResponse = "";
    while (client.connected() || client.available()) {
        String line = client.readStringUntil('\n');

        if (!headerEnded) {
            if (line == "\r") { // 空行，HTTP header 结束
                headerEnded = true;
            }
            continue;
        }

        // 流式解析 "response" 字段
        int start = line.indexOf("\"response\":\"");
        if (start != -1) {
            start += 12;
            int end = line.indexOf("\"", start);
            if (end != -1) {
                String token = line.substring(start, end);
                token.replace("\\n", "\n");
                token.replace("\\\"", "\"");
                Serial.print(token); // 边收到边输出
                fullResponse += token;
            }
        }
    }

    client.stop();
    return fullResponse; // 修正：返回完整响应
}




class OllamaClient {
public:
    OllamaClient(const char* host, uint16_t port) : host(host), port(port) {}

    bool connectServer() {
        if (!client.connect(host, port)) {
            Serial.println("连接 Ollama 失败");
            return false;
        }
        return true;
    }

    void disconnect() {
        client.stop();
    }

    String sendPromptStreamToOllama(const char* host, uint16_t port, const char* model, const String& prompt) {
        WiFiClient client;
        if (!client.connect(host, port)) {
            Serial.println("连接 Ollama 失败");
            return "";
        }

        String body = "{\"model\":\"" + String(model) + "\",\"prompt\":\"" + prompt + "\",\"stream\":true}";
        String request =
            String("POST /api/generate HTTP/1.1\r\n") +
            "Host: " + host + "\r\n" +
            "Content-Type: application/json\r\n" +
            "Content-Length: " + String(body.length()) + "\r\n" +
            "Connection: close\r\n\r\n" +
            body;

        client.print(request);

        bool headerEnded = false;
        String fullResponse = ""; // 缓存完整文本

        while (client.connected() || client.available()) {
            String line = client.readStringUntil('\n');

            if (!headerEnded) {
                if (line == "\r") headerEnded = true;
                continue;
            }

            int start = line.indexOf("\"response\":\"");
            if (start != -1) {
                start += 12;
                int end = line.indexOf("\"", start);
                if (end != -1) {
                    String token = line.substring(start, end);
                    token.replace("\\n", "\n");
                    token.replace("\\\"", "\"");

                    Serial.print(token);   // 实时输出
                    fullResponse += token; // 缓存
                }
            }
        }

        client.stop();
        return fullResponse; // 最终返回完整句子
    }


private:
    const char* host;
    uint16_t port;
    WiFiClient client;
};


#endif
