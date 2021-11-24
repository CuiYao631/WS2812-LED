/*
 * @Author: CuiYao 
 * @Date: 2021-08-27 16:58:36 
 * @Last Modified by: CuiYao
 * @Last Modified time: 2021-08-27 17:00:37
 */

#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>
#include <Adafruit_NeoPixel.h>

/***************此处要修改******************/
char auth[] = "bad6082c1224";/****秘钥****/
char ssid[] = "infiprimes";  /*WiFi名称***/
char pswd[] = "zhongmao15ZZ"; /***WiFi密码*/

#define PIN 3  //  DIN PIN (GPIO15, D8)
#define NUMPIXELS 6 // Number of you led
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

BlinkerRGB RGB1("RGB");
BlinkerButton Button1("btn-on");
BlinkerButton Button2("btn-off");
BlinkerNumber Number1("num-abc");

/***************配网函数**************/

int counter = 0;
int LED_R=0,LED_G=0,LED_B=0,LED_Bright=255;// RGB和亮度
void button1_callback(const String & state)
{
   //需要执行两遍，不知道什么情况
   SET_RGB(255,255,255,255); 
   SET_RGB(255,255,255,255);   
   BLINKER_LOG("get state: ", "ON");
}
void button2_callback(const String & state)
{
     SET_RGB(0,0,0,0);  
     BLINKER_LOG("get state: ", "OFF");
}


void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter=counter;
    Number1.print(counter);
}


void SET_RGB(int R,int G,int B,int bright)
{
    BLINKER_LOG("R:",R,",G:",G,",B:",B,",bright:",bright);
    for (uint16_t i = 0; i < NUMPIXELS; i++) //把灯条变色
    {
        pixels.setPixelColor(i,R,G,B);
    }
    pixels.setBrightness(bright);//亮度
    pixels.show();    //送出显示
}
//APP RGB颜色设置回调
void rgb1_callback(uint8_t r_value, uint8_t g_value, 
                    uint8_t b_value, uint8_t bright_value)
{
    
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);
    LED_Bright = bright_value;
    SET_RGB(r_value,g_value,b_value,LED_Bright);
}


//小爱电源类回调
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        //digitalWrite(LED_BUILTIN, LOW);
        SET_RGB(255,255,255,255);        
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_OFF) {
        //digitalWrite(LED_BUILTIN, HIGH);
        SET_RGB(255,255,255,0);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}
//小爱设置颜色回调
void miotColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);
    int colorR,colorG,colorB;
    colorR = color >> 16 & 0xFF;
    colorG = color >>  8 & 0xFF;
    colorB = color       & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);
    SET_RGB(colorR,colorG,colorB,LED_Bright);
    //pixelShow();

    BlinkerMIOT.color(color);//反馈小爱控制状态
    BlinkerMIOT.print();
}
//小爱调亮度回调
void miotBright(const String & bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    int colorW = bright.toInt();

    BLINKER_LOG("now set brightness: ", colorW);
    LED_Bright = colorW;
    SET_RGB(LED_R,LED_G,LED_B,LED_Bright);
    Serial.printf("亮度调节中...%d",colorW);
    
    BlinkerMIOT.brightness(colorW);//反馈小爱控制状态
    BlinkerMIOT.print();
}












void setup() {
    // 初始化串口
    Serial.begin(115200);
    pixels.begin();//WS2812初始化
    pixels.show();
    pinMode(LED_BUILTIN, OUTPUT);
    #if defined(BLINKER_PRINT)
    BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif

    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);//连接WiFi和设备上线

    
    Blinker.attachData(dataRead);//如果没有被绑定的组件被触发就执行这个回调
    Button1.attach(button1_callback);//Button1这个组件被触发就执行这个回调
    Button2.attach(button2_callback);//Button1这个组件被触发就执行这个回调
    RGB1.attach(rgb1_callback);//注册调节颜色的回调函数
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);//小爱调节颜色
    BlinkerMIOT.attachBrightness(miotBright);//小爱调节RGB亮度
}

void loop() 
{
    Blinker.run();
}
