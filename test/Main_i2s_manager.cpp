/*
功能：初始化 I2S 麦克风和 DAC，处理 PCM 流播放/采集
核心函数：

void Init_INMP441(); —— 初始化 INMP441 麦克风

void Init_MAX98357A(); —— 初始化 MAX98357A

size_t readMicPCM(uint8_t* buffer, size_t maxLen); —— 从麦克风读取 PCM 数据

void writeDACPCM(uint8_t* buffer, size_t len); —— 播放 PCM 数据到 MAX98357A
*/

#pragma once

#include <Arduino.h>
#include "driver/i2s.h"

// I2S 配置
#define I2S_PORT_MIC     I2S_NUM_0
#define I2S_PORT_DAC     I2S_NUM_1

// 麦克风引脚配置 (INMP441)
#define MIC_DATA_PIN     16 //SD
#define MIC_BCLK_PIN     18 //SCK
#define MIC_LRCK_PIN     17 //WS

// DAC引脚配置 (MAX98357A)
#define DAC_BCLK_PIN     5
#define DAC_LRCK_PIN     6
#define DAC_DATA_PIN     7

// 音频参数
#define SAMPLE_RATE      16000
#define DMA_BUF_LEN      256
#define MIC_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_32BIT
#define DAC_BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT

class I2SManager {
public:
    void begin() {
        initMicrophone();
        initDAC();
    }

    size_t readMicrophone(int32_t* buffer, size_t maxSamples) {
        size_t bytesRead = 0;
        i2s_read(I2S_PORT_MIC, buffer, maxSamples * sizeof(int32_t), &bytesRead, portMAX_DELAY);
        return bytesRead / sizeof(int32_t);
    }

    size_t writeDAC(const int16_t* buffer, size_t samples) {
        size_t bytesWritten = 0;
        i2s_write(I2S_PORT_DAC, buffer, samples * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
        return bytesWritten / sizeof(int16_t);
    }

    void printAudioStats(const int32_t* samples, size_t count) {
        int32_t minVal = INT32_MAX;
        int32_t maxVal = INT32_MIN;
        int64_t sum = 0;
        
        for(size_t i = 0; i < count; i++) {
            int32_t val = processMicrophoneSample(samples[i]);
            minVal = min(minVal, val);
            maxVal = max(maxVal, val);
            sum += val;
            
            // 可选：打印原始样本值
            Serial.println(val);
        }
        
        Serial.printf("Samples: %d, Min: %d, Max: %d, Avg: %d\n", 
                     count, minVal, maxVal, (int32_t)(sum/count));
    }

    int16_t processMicrophoneSample(int32_t sample) {
        // 提取24位有效数据并转换为16位
        sample >>= 8;  // 丢弃低8位填充数据
        sample &= 0x00FFFFFF; // 确保只保留24位
        
        // 处理符号位（24位有符号数）
        if (sample & 0x00800000) {
            sample |= 0xFF000000; // 符号扩展
        }

        const float volume_gain = 2.0f; 

        return (int16_t)(sample / 256 * volume_gain);
    }

    void convertMicBuffer(const int32_t* micBuf, int16_t* dacBuf, size_t samples) {
        for (size_t i = 0; i < samples; i++) {
            dacBuf[i] = processMicrophoneSample(micBuf[i]);
        }
    }

    void loopbackOnce() {
        int32_t micBuf[DMA_BUF_LEN];
        int16_t dacBuf[DMA_BUF_LEN];

        size_t samples = readMicrophone(micBuf, DMA_BUF_LEN);
        convertMicBuffer(micBuf, dacBuf, samples);
        writeDAC(dacBuf, samples);
    }



private:
    void initMicrophone() {
        i2s_config_t cfg = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = SAMPLE_RATE,
            .bits_per_sample = MIC_BITS_PER_SAMPLE,
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
            .communication_format = I2S_COMM_FORMAT_STAND_I2S,
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 4,
            .dma_buf_len = DMA_BUF_LEN,
            .use_apll = false,
            .tx_desc_auto_clear = false,
            .fixed_mclk = 0
        };

        i2s_pin_config_t pins = {
            .bck_io_num = MIC_BCLK_PIN,
            .ws_io_num = MIC_LRCK_PIN,
            .data_out_num = I2S_PIN_NO_CHANGE,
            .data_in_num = MIC_DATA_PIN
        };

        i2s_driver_install(I2S_PORT_MIC, &cfg, 0, NULL);
        i2s_set_pin(I2S_PORT_MIC, &pins);
        i2s_zero_dma_buffer(I2S_PORT_MIC);
        Serial.println("Microphone initialized");
    }

    void initDAC() {
        i2s_config_t cfg = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
            .sample_rate = SAMPLE_RATE,
            .bits_per_sample = DAC_BITS_PER_SAMPLE,
            .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT, // 单声道左
            .communication_format = I2S_COMM_FORMAT_STAND_I2S, // 与麦克风一致
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 8,
            .dma_buf_len = DMA_BUF_LEN,
            .use_apll = false,
            .tx_desc_auto_clear = true,
            .fixed_mclk = 0
        };

        i2s_pin_config_t pins = {
            .bck_io_num = DAC_BCLK_PIN,
            .ws_io_num = DAC_LRCK_PIN,
            .data_out_num = DAC_DATA_PIN,
            .data_in_num = I2S_PIN_NO_CHANGE
        };

        i2s_driver_install(I2S_PORT_DAC, &cfg, 0, NULL);
        i2s_set_pin(I2S_PORT_DAC, &pins);
        i2s_zero_dma_buffer(I2S_PORT_DAC);
        Serial.println("DAC initialized");
    }
};