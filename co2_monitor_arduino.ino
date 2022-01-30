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
#define LED_ORDER GRB     // LED RGBの並び
CRGB leds[NUM_LEDS];  // LEDの色を指定する変数

// eCO2濃度とLED発光色の設定
#define BLUE_ANGLE  240   // NORMAL_PPM時のhue angle 
#define RED_ANGLE 0       // ALERT_PPM時のhue angle
#define NORMAL_PPM  400   // NORMAL_PPM値
#define ALERT_PPM 1000    // ALERT_PPM値

// eCO2値をLEDのCRGB値に変換する
// (参考にした情報)
//   http://blog.eldhrimnir.com/?p=611
//   https://www.petitmonte.com/javascript/rgb_hsv_convert.html
CRGB toCRGB( int eco2 ) {
  CRGB rgbcolor;
  int h_angle;

  // eco2値　から　hue angle　へ
  h_angle = map(eco2, NORMAL_PPM, ALERT_PPM, BLUE_ANGLE, RED_ANGLE);
  // 上限・下限値でクリップ
  if (h_angle < RED_ANGLE) {
    h_angle = RED_ANGLE;
  } else if (h_angle >= BLUE_ANGLE) {
    h_angle = BLUE_ANGLE;
  }
  Serial.print("eCO2: ");
  Serial.print(eco2);
  Serial.print("ppm, hue angle: ");
  Serial.print(h_angle);
  
  if(h_angle <= 0){
    rgbcolor.r=255;
    rgbcolor.g=0;
    rgbcolor.b=0;
  }else if (h_angle <= 120) {
    /* H値(0-120) 赤–黄–緑     */
    rgbcolor.r = map(h_angle,0,120,255,0);     // 赤LED R←→G
    rgbcolor.g = map(h_angle,0,120,0,255);     // 緑LED G←→R
    rgbcolor.b = 0;
  } else if (h_angle <= 240) {
    /* H値(120-240) 緑–水色–青 */
    rgbcolor.r = 0;
    rgbcolor.g = map(h_angle,120,240,255,0);   // 緑LED G←→B
    rgbcolor.b = map(h_angle,120,240,0,255);   // 青LED B←→G
  } else {
    /* H値(240-360) 青–紫–赤   */
    rgbcolor.r = map(h_angle,240,360,0,255);   // 青LED R←→B
    rgbcolor.g = 0;
    rgbcolor.b = map(h_angle,240,360,255,0);   // 青LED B←→R
  }

  return rgbcolor;
}

void setup() {
  // LEDの準備
  FastLED.addLeds<LED_TYPE, DATA_PIN, LED_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(9600);
  Serial.println("CCS811 x WS2812 by Code for SUSONO");
  Serial.print("LED Brightness:");
  Serial.println(FastLED.getBrightness());

  // LED発光テスト
  Serial.println("LED test!");
  leds[0] = toCRGB(400);  // 400pppmの色（青）
  leds[1] = toCRGB(600);  // 600ppmの色（青緑）
  leds[2] = toCRGB(800);  // 800ppmの色（黄緑）
  leds[3] = toCRGB(1000); // 1000ppmの色（赤）
  FastLED.show();
  Serial.println("LED test done.");
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

void loop() {
  int cVal;
  CRGB rgb = CRGB::WhiteSmoke;
  int err_id;
   
  if(ccs.available()){
    if(!ccs.readData()){
      // センサーのeCO2値をCRGB値に変換
      rgb = toCRGB( ccs.geteCO2() );
      Serial.print(", TVOC: ");
      Serial.println( ccs.getTVOC() );
      err_id = 0;
    }
    else{
      Serial.println("ERROR: ccs can't read data !");
      err_id = 1;
    }
  }
  else {
    Serial.println("ERROR: ccs data isn't ready !");
    err_id = 2;
  }
  
  // テープライトの発光
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