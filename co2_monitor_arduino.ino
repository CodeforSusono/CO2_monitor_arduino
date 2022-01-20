/*  
 * CO2_monitor_arduino
 *  target : arduino nano
 *  sensor : CCS811 Air Quality GAS sensor
 *  led : WS2812B 
 *  
 *  Copyright (c) 2021-  Code for SUSONO
 *  Released under MIT license.
 *  see https://github.com/CodeforSusono/CO2_monitor_arduino/blob/main/LICENSE
 */
 // CCS811のライブラリはAdafruit_CCS811を使用
 // ライセンスはMIT License
 // https://github.com/adafruit/Adafruit_CCS811
#include <Adafruit_CCS811.h>
Adafruit_CCS811 ccs;  // ccsセンサーオブジェクト

// WS2812BのライブラリはFastLEDを使用
// ライセンスはMIT License
// https://github.com/FastLED/FastLED
#include <FastLED.h>
#define NUM_LEDS  4   // LEDの数
#define DATA_PIN  13  // D13番PINに接続
#define LED_TYPE  WS2812B // LEDチップセット
#define BRIGHTNESS  128   // LEDの明るさ
CRGB leds[NUM_LEDS];  // LEDの色を指定する変数

void setup() {
  // LEDの準備
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(9600);
  Serial.println("CCS811 x WS2812 by Code for SUSONO");
  Serial.print("LED Brightness:");
  Serial.println(FastLED.getBrightness());

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

// eCO2値をLEDのCRGB値に変換する
// (参考にした情報)
//   http://blog.eldhrimnir.com/?p=611
//   https://www.petitmonte.com/javascript/rgb_hsv_convert.html
CRGB toCRGB( int val ) {

  CRGB rgbcolor;
  
  if(val <= 0){
    rgbcolor.r=255;
    rgbcolor.g=0;
    rgbcolor.b=0;
  }else if (val <= 120) {
    /* H値(0-120) 赤–黄–緑     */
    rgbcolor.r = map(val,0,120,255,0);     // 赤LED R←→G
    rgbcolor.g = map(val,0,120,0,255);     // 緑LED G←→R
    rgbcolor.b = 0;
  } else if (val <= 240) {
    /* H値(120-240) 緑–水色–青 */
    rgbcolor.r = 0;
    rgbcolor.g = map(val,120,240,255,0);   // 緑LED G←→B
    rgbcolor.b = map(val,120,240,0,255);   // 青LED B←→G
  } else {
    /* H値(240-360) 青–紫–赤   */
    rgbcolor.r = map(val,240,360,0,255);   // 青LED R←→B
    rgbcolor.g = 0;
    rgbcolor.b = map(val,240,360,255,0);   // 青LED B←→R
  }

  return rgbcolor;
}

void loop() {
  int cVal;
  CRGB rgb;
  int err_id;
   
  if(ccs.available()){
    if(!ccs.readData()){
      cVal = map(ccs.geteCO2(), 400, 1500, 240, 0); // 色空間の中での位置を設定。CO2の値400から1500の間で260から0の値を返す
      Serial.print("cVal: ");
      Serial.print(cVal);
      Serial.print(", CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
      err_id = 0;
    }
    else{
      Serial.println("ERROR: ccs can't read data !");
      err_id = 1;
      //while(1);
    }
  }
  else {
    Serial.println("ERROR: ccs data isn't ready !");
    err_id = 2;
    //while(1);
  }
  
  //CRGB値に変換
  rgb = toCRGB( cVal );
  
//ここからシリアルテープライトの発光
  leds[0] = rgb;
  leds[1] = leds[0];
  leds[2] = leds[1];
  leds[3] = leds[2];
  FastLED.show();

  if (err_id == 1) {
    // ccs.readData()エラー時はLEDを0.5秒間隔で点滅
    delay( 500 );
    leds[0] = CRGB::Black;
    leds[1] = leds[0];
    leds[2] = leds[1];
    leds[3] = leds[2];
    FastLED.show();
    delay( 500 );
  } else if (err_id == 2) {
    // ccs.available()エラー時はLEDを0.25秒点灯、0.75秒消灯
    delay( 250 );
    leds[0] = CRGB::Black;
    leds[1] = leds[0];
    leds[2] = leds[1];
    leds[3] = leds[2];
    FastLED.show();
    delay( 750 );
  } else {
   // エラーなし
    delay(1000);
  }

  //RGB値シリアル表示
  Serial.print("R");
  Serial.print(rgb.r);
  Serial.print(",G");
  Serial.print(rgb.g);
  Serial.print(",B");
  Serial.println(rgb.b);
  Serial.println("----------");

}
