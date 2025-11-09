#ifndef robot_movement
#define robot_movement

#include <Arduino.h>

class Robot {
private:
    #define GPIO_IN1 4
    #define GPIO_IN2 10
    #define GPIO_IN3 11
    #define GPIO_IN4 12

    // 停止所有电机
    void Stop_Motors() {
        digitalWrite(GPIO_IN1, LOW);
        digitalWrite(GPIO_IN2, LOW);
        digitalWrite(GPIO_IN3, LOW);
        digitalWrite(GPIO_IN4, LOW);
    }

public:
    Robot() {
        // 构造函数 - 设置引脚模式
        pinMode(GPIO_IN1, OUTPUT);
        pinMode(GPIO_IN2, OUTPUT);
        pinMode(GPIO_IN3, OUTPUT);
        pinMode(GPIO_IN4, OUTPUT);
        
        // 初始化时停止所有电机
        Stop_Motors();
    }

    void Move_Forward(float duration) {
        Serial.print("前进 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机前进
        digitalWrite(GPIO_IN1, HIGH);
        digitalWrite(GPIO_IN2, LOW);
        
        // 右电机前进
        digitalWrite(GPIO_IN3, HIGH);
        digitalWrite(GPIO_IN4, LOW);
        
        // 保持运动指定时间
        delay(duration * 300);
        
        // 停止电机
        Stop_Motors();
    }

    void Move_Backward(float duration) {
        Serial.print("后退 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机后退
        digitalWrite(GPIO_IN1, LOW);
        digitalWrite(GPIO_IN2, HIGH);
        
        // 右电机后退
        digitalWrite(GPIO_IN3, LOW);
        digitalWrite(GPIO_IN4, HIGH);
        
        // 保持运动指定时间
        delay(duration * 300);
        
        // 停止电机
        Stop_Motors();
    }

    void Move_Left(float duration) {
        Serial.print("左转 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机停止（或低速）
        digitalWrite(GPIO_IN1, LOW);
        digitalWrite(GPIO_IN2, LOW);
        
        // 右电机前进
        digitalWrite(GPIO_IN3, HIGH);
        digitalWrite(GPIO_IN4, LOW);
        
        // 保持转向指定时间
        delay(duration * 200);
        
        // 停止电机
        Stop_Motors();
    }

    void Move_Right(float duration) {
        Serial.print("右转 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机前进
        digitalWrite(GPIO_IN1, HIGH);
        digitalWrite(GPIO_IN2, LOW);
        
        // 右电机停止（或低速）
        digitalWrite(GPIO_IN3, LOW);
        digitalWrite(GPIO_IN4, LOW);
        
        // 保持转向指定时间
        delay(duration * 200);
        
        // 停止电机
        Stop_Motors();
    }

    // 可选：原地左转（两个轮子反向）
    void Turn_Left_InPlace(int duration) {
        Serial.print("原地左转 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机后退
        digitalWrite(GPIO_IN1, LOW);
        digitalWrite(GPIO_IN2, HIGH);
        
        // 右电机前进
        digitalWrite(GPIO_IN3, HIGH);
        digitalWrite(GPIO_IN4, LOW);
        
        delay(duration * 200);
        Stop_Motors();
    }

    // 可选：原地右转（两个轮子反向）
    void Turn_Right_InPlace(int duration) {
        Serial.print("原地右转 ");
        Serial.print(duration);
        Serial.println(" 秒");
        
        // 左电机前进
        digitalWrite(GPIO_IN1, HIGH);
        digitalWrite(GPIO_IN2, LOW);
        
        // 右电机后退
        digitalWrite(GPIO_IN3, LOW);
        digitalWrite(GPIO_IN4, HIGH);

        delay(duration * 200);
        Stop_Motors();
    }
};

#endif