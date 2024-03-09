#ifndef DISP_HPP
#define DISP_HPP

#include <Arduino.h>
#include <SPIFFS.h>
#include <M5Unified.h>

class AppDisplay{
  public:
    void update(String time_now, String today_date, String tomorrow_date, bool today_umbrella_flag, bool tomorrow_umbrella_flag);
    void disp();

  private:
    uint8_t disp_index_ = 0;  // 0:現在時刻、1:今日の天気、2:明日の天気
    String time_now_;  // 現在時刻の文字列
    String today_date_; // 今日の日付 ("今日"付き)
    String tomorrow_date_; // 明日の日付 ("明日"付き)
    bool today_umbrella_flag_ = false;  // 今日の傘の要否。trueなら必要。
    bool tomorrow_umbrella_flag_ = false;  // 今日の傘の要否。trueなら必要。
    String path_umb_ = "/umbrella.bmp";
    String path_sun_cloud_ = "/sun_cloud.bmp";
    uint8_t icon_y_offset_ = 28;

  void disp_time_();  // 時刻表示
  void disp_weather_(String date, bool flag);
  void disp_header_date_(String date);  // 日付の表示
  void disp_umbrella_();
  void disp_sun_cloud_();
  uint16_t getColor_(uint8_t red, uint8_t green, uint8_t blue);
};

#endif