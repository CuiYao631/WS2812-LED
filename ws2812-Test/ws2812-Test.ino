/*
 * @Author: CuiYao 
 * @Date: 2021-09-18 14:18:01 
 * @Last Modified by: CuiYao
 * @Last Modified time: 2021-09-18 15:03:44
 */

/*主函数只保留 WiFi 和 按钮 基础功能,其他功能扩展加入 */

#include <Adafruit_NeoPixel.h>
#include "ButtonDebounce.h"
#include "ButtonHandler.h"
#include <ESP8266WiFi.h>  

#define PL(s) Serial.println(s)
#define P(s) Serial.print(s)

/*引脚参数*/
#define PIN_ButtonDebounce 0 //按钮控制引脚
#define PIN 3  //  DIN PIN (GPIO15, D8)
#define NUMPIXELS 29 // LED个数


ButtonDebounce btn(PIN_ButtonDebounce, INPUT_PULLUP, LOW);
ButtonHandler btnHandler;
int count = 0;     //时间计数
bool WIFI_Status = true;   //WIFI状态标志位

int LED_R=0,LED_G=0,LED_B=0,LED_Bright=255;// RGB和亮度
bool LED_Flag = false;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const uint8_t LED_0[6][29] = { 
{255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,},
{0,0,0,0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,},
{0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,},
{0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,},
{255,255,255,255,0,0,0,0,0,0,0,255,255,255,255,255,255,255,0,0,0,0,0,0,255,255,255,255,255,},
{255,255,255,0,0,0,0,255,255,255,255,0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,},
};
int i = 0;

 void IRAM_ATTR btnInterrupt()
{
  btn.update();
}

void SET_RGB(int R,int G,int B,int bright)
{
    for (uint16_t i = 0; i < NUMPIXELS; i++) //把灯条变色
    {
        pixels.setPixelColor(i,R,G,B);
    }
    pixels.setBrightness(bright);//亮度
    pixels.show();    //送出显示
}

void ButtonInit(){
btn.setCallback([](const bool down)
                  { btnHandler.handleChange(down); });
  btn.setInterrupt(btnInterrupt);

  btnHandler.setIsDownFunction([](void)
                               { return btn.checkIsDown(); });
  btnHandler.setCallback([](const button_event e)
                         {
                           P(F("Button Event: "));
                           switch (e)
                           {
                           case BUTTON_EVENT_SINGLECLICK:
                             PL(F("SINGLECLICK"));
                             LED_Flag = !LED_Flag;
    
                              if(LED_Flag){
                                SET_RGB(255,255,255,255);
                                SET_RGB(255,255,255,255);
                              }
                              else{
                                SET_RGB(0,0,0,0);
                              }
                             break;
                           case BUTTON_EVENT_DOUBLECLICK:
                             PL(F("DOUBLECLICK"));
                            
                             break;
                           case BUTTON_EVENT_LONGCLICK:
                             PL(F("LONGCLICK"));
                           
                             
                             break;
                           }
                         });

}
void setup() {
  
  Serial.begin(115200);

  pixels.begin();//WS2812初始化
  pixels.show();
  ButtonInit();  
  }
void loop() {
  
   btnHandler.loop();
   if (i>5){
    i=0;
   }
   pixels.clear();
     //********LED*************
  for (uint8_t j = 0; j < NUMPIXELS; j++) {
   
    pixels.setPixelColor(j, LED_0[i][j],LED_0[i][j],LED_0[i][j]);

    pixels.show();

   }
   
    delay(1000);
    i++;
}
