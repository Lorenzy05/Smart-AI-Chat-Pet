#ifndef RGB_H
#define RGB_H

#include <Adafruit_NeoPixel.h>

#define RGB_PIN 48
#define NUMPIXELS  1


Adafruit_NeoPixel rgbStrip(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup_RGB(){
    rgbStrip.begin();
    rgbStrip.setBrightness(50);
}

void Show_Red(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(255, 0, 0));
    rgbStrip.show();
}

void Show_Green(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(0, 255, 0));
    rgbStrip.show();
}

void Show_Blue(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(0, 0, 255));
    rgbStrip.show();
}

void Show_Yellow(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(255, 255, 0));
    rgbStrip.show();
}

void Show_Black(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(0, 0, 0));
    rgbStrip.show();
}

void Show_White(){
    rgbStrip.setPixelColor(0, rgbStrip.Color(255, 255, 255));
    rgbStrip.show();
}

#endif

