#include "main.hpp"

#if __has_include("config.hpp")
#include "config.hpp"
#else
#define SSID "ssid"
#define PASSWORD "password"
#endif

const char *ssid = SSID;       //接続先wifiのssid
const char *password = PASSWORD;   //接続先wifiのパスワード
unsigned long startTime = millis();
CurrentDateTime cdt;
WeatherJson wj;
AppDisplay ad;

// function declarations:
void setup_wifi();

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)  // 接続されるまで待つ
  {
    delay(500);
    if(millis() - startTime > 10000)     // タイムアウト(10秒)
    {
      M5.Display.print("Wifi Error!!");
      USBSerial.printf("Wifi Error!!");
      while(true);
    }
  }
  // USBSerial.printf("Wifi connected");
}

void setup() {
  auto cfg = M5.config();       // M5Stack初期設定用の構造体を代入
  M5.begin(cfg);                           // M5デバイスの初期化
  USBSerial.begin(115200);                 // シリアルモニターの初期化

  if (!SPIFFS.begin()) {  // 画像ファイルの内部ストレージを有効化
    // SPIFFSが未フォーマットの場合
    USBSerial.print("SPIFFS Format ... (please wait)");
    delay(100);
    SPIFFS.format();
    USBSerial.println("Down");
    ESP.restart();
  }

  M5.Display.setBrightness(128);
  M5.Display.setTextSize(2);               // テキストサイズを変更
  M5.Display.setTextColor(TFT_WHITE);

  setup_wifi();
  cdt.setup();
}

void loop() {
  M5.update();

  cdt.update();   // 現在時刻（現地時間）をネット時刻で更新
  cdt.tmTimeToStr();

  if(wj.getWeatherJson()){  // 天気予報を取得
    wj.analyze_data(cdt.current_time_info_t);
  }

  ad.update(cdt.current_time_str, wj.getTodayDateStr(), wj.getTomorrowDateStr(), wj.getTodayUmbrella(), wj.getTomorrowUmbrella());
  ad.disp();

  delay(1500);  // 1.5秒で表示を切り替え
}