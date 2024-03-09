#ifndef CURRENT_DATE_TIME_HPP
#define CURRENT_DATE_TIME_HPP

#include <Arduino.h>

class CurrentDateTime{
  public:
    CurrentDateTime(){};

    String current_date_str;  // 現在日付の文字列  "03-04"
    String current_time_str;  // 現在時刻の文字列  "04-49"  (12時間表記)
    time_t current_time_info_t;  // 現在日時のtime_t形式

    void setup();
    void update();
    String tmDateToStr();
    String tmTimeToStr();

  private:
    const int JST_ = 3600 * 9;        // グリニッジ標準時と日本標準時の差
    struct tm current_time_info_;  // 現在の日時
    struct tm current_time_info_for_diff_;  // 差分日数の計算用の、現在の日時
    unsigned long updatedMillis_ = 0;

  void createTimeInfoForDiff_();
};

#endif