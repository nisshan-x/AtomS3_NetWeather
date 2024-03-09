#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define JSON_URL "https://www.jma.go.jp/bosai/forecast/data/forecast/130000.json"  // 東京都 130000

class ForecastData{
  public:
  ForecastData(){};

  String fc_date_time;  // 例 "2024-03-01T05:00:00+09:00"
  String forecast;  // 例 "晴れ　時々　くもり"
  String weather_code_str; // 例 "102"

  time_t fc_date_time_t;
  uint16_t weather_code; // 例 102
  String fc_date_mmdd;  // "03-01"

  bool umbrella_flag = false;  // 雨か雪ならtrue
  int diff_days;  // 今と天気予報の日付の差(日数) 正なら未来、負なら過去。
  String day_info;  // 昨日、今日、明日、明後日
};

class WeatherJson{
  public:
    WeatherJson(){};

    bool getWeatherJson();
    void analyze_data(time_t current_time_info_t);

    String getTodayDateStr();  // 今日の日付 ("今日"付き)
    String getTomorrowDateStr();  // 明日の日付 ("明日"付き)
    bool getTodayUmbrella();
    bool getTomorrowUmbrella();

  private:
    String payload_;
    uint8_t weather_data_size_;  // jsonに入っている、天気予報の個数 (=日数)
    uint8_t weather_index_ = 0;  // 表示する天気予報を指すインデックス
    JsonDocument doc_;
    static const uint8_t FC_NUM_ = 3;  // 最大3個
    ForecastData fc_data[FC_NUM_];  // jsonの天気予報を分析して格納する箱
    unsigned long updatedMillis_ = millis() - (1000 * 60 * 60 + 100);
    bool disp_toggle_ = false;
    uint8_t today_index_;
    uint8_t tomorrow_index_;

    static const uint8_t UC_NUM_ = 105;  // umbrella_codeのデータの個数
    // ローカルであらかじめ分析した、雨か雪を表すwether code
    uint16_t umbrella_code_[UC_NUM_] = {102, 103, 104, 105, 106, 107, 108, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 140, 160, 170, 181, 202, 203, 204, 205, 206, 207, 208, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 224, 225, 226, 228, 229, 230, 231, 240, 250, 260, 270, 281, 300, 301, 302, 303, 304, 306, 308, 309, 311, 313, 314, 315, 316, 317, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 340, 350, 361, 371, 400, 401, 402, 403, 405, 406, 407, 409, 411, 413, 414, 420, 421, 422, 423, 425, 426, 427, 450};

    void analyze_data_func_(const uint8_t index, const time_t current_time_info_t);
    bool umbrella_check_weather_code_(const String weather_code_str);
    tm stringToTm_(const String dateString); // 指定された日付文字列をtm構造体に変換する関数
};

#endif