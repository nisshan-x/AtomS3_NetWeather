#include "current_date_time.hpp"

void CurrentDateTime::setup(){
  // NTPサーバとローカルのタイムゾーンを設定
  configTime(JST_, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" Time received");

  updatedMillis_ = millis();
}

void CurrentDateTime::update(){
  unsigned long currentMillis = millis();
  if(currentMillis > updatedMillis_ + 1000 * 60 * 60 * 24){
    setup();  // 1日に1回、時計を合わせる。
  }

  if (!getLocalTime(&current_time_info_)) {
    // Failed to obtain time.
    while(true);
  }

  createTimeInfoForDiff_();

  // 現在時刻をtime_t形式に変換
  current_time_info_t = mktime(&current_time_info_for_diff_);
}

String CurrentDateTime::tmDateToStr(){
  // Jsonの日付表記に合わせた今の日付のString "2024-03-01"
  char current_year_c[4];
  sprintf(current_year_c, "%04d", current_time_info_.tm_year+1900);
  String current_year = String(current_year_c);

  char current_month_c[2];
  sprintf(current_month_c, "%02d", current_time_info_.tm_mon+1);
  String current_month = String(current_month_c);

  char current_day_c[2];
  sprintf(current_day_c, "%02d", current_time_info_.tm_mday);
  String current_day = String(current_day_c);

  String hyphen = String("-");
  current_year.concat(hyphen);
  current_year.concat(current_month);
  current_year.concat(hyphen);
  current_year.concat(current_day);

  current_date_str = current_year;
  return current_date_str;
}

String CurrentDateTime::tmTimeToStr(){
  // Jsonの時刻表記に合わせた今の時刻のString "12:43"
  int hour_small;
  if(current_time_info_.tm_hour > 12){
    hour_small = current_time_info_.tm_hour - 12;
  }
  else{
    hour_small = current_time_info_.tm_hour;
  }
  char current_hour_c[4];
  sprintf(current_hour_c, "%02d", hour_small);
  String current_hour = String(current_hour_c);

  char current_min_c[2];
  sprintf(current_min_c, "%02d", current_time_info_.tm_min);
  String current_min = String(current_min_c);

  String colon = String(":");
  current_hour.concat(colon);
  current_hour.concat(current_min);

  current_time_str = current_hour;
  return current_time_str;
}

void CurrentDateTime::createTimeInfoForDiff_(){
  current_time_info_for_diff_.tm_year = current_time_info_.tm_year;
  current_time_info_for_diff_.tm_mon = current_time_info_.tm_mon;
  current_time_info_for_diff_.tm_mday = current_time_info_.tm_mday;
  current_time_info_for_diff_.tm_hour = 0;
  current_time_info_for_diff_.tm_min = 0;
  current_time_info_for_diff_.tm_sec = 0;
}