#include "disp.hpp"


void AppDisplay::update(const String time_now, const String today_date, const String tomorrow_date, const bool today_umbrella_flag, const bool tomorrow_umbrella_flag){
  time_now_ = time_now;  // 現在時刻の文字列
  today_date_ = today_date; // 今日の日付 ("今日"付き)
  tomorrow_date_ = tomorrow_date; // 明日の日付 ("明日"付き)
  today_umbrella_flag_ = today_umbrella_flag;  // 今日の傘の要否。trueなら必要。
  tomorrow_umbrella_flag_ = tomorrow_umbrella_flag;  // 今日の傘の要否。trueなら必要。

  // デバッグ用
  // USBSerial.print("time_now_ ");
  // USBSerial.println(time_now_);
  // USBSerial.print("today_date_ ");
  // USBSerial.println(today_date_);
  // USBSerial.print("tomorrow_date_ ");
  // USBSerial.println(tomorrow_date_);
  // USBSerial.print("today_umbrella_flag_ ");
  // USBSerial.println(today_umbrella_flag_);
  // USBSerial.print("tomorrow_umbrella_flag_ ");
  // USBSerial.println(tomorrow_umbrella_flag_);
}

void AppDisplay::disp(){
  M5.Display.startWrite();

  // 背景塗りつぶし
  if(today_umbrella_flag_){
    M5.Display.fillScreen(getColor_(255, 10, 10));  // RED。今日が雨なら赤。
  }
  else{
    M5.Display.fillScreen(getColor_(0, 0, 0));  // Black
  }

  // デバッグ用
  // USBSerial.print("disp_index_ ");
  // USBSerial.println(disp_index_);

  switch(disp_index_){
    case 0:
      disp_time_();
      break;
    case 1:
      disp_weather_(today_date_, today_umbrella_flag_);
      break;
    case 2:
      disp_weather_(tomorrow_date_, tomorrow_umbrella_flag_);
      break;
    default:
      USBSerial.println("Error!!!!");
      break;
  }
  M5.Display.endWrite();

  if(disp_index_ == 2){
    disp_index_ = 0;
  }
  else{
    disp_index_++;
  }
}

void AppDisplay::disp_time_(){  // 時刻表示
  M5.Display.setFont(&fonts::lgfxJapanGothic_24);
  M5.Display.setCursor(5, 40);
  M5.Display.print(time_now_);
}

void AppDisplay::disp_weather_(const String date, const bool flag){
  disp_header_date_(date);    // 日付の表示

  // デバッグ用
  // fc_data[weather_index_].forecast.replace("　", " ");  // 例 "晴れ　時々　くもり" -> "晴れ 時々 くもり"
  // USBSerial.println(fc_data[weather_index_].forecast.c_str());

  // アイコン表示
  if(flag)
    disp_umbrella_();
  else
    disp_sun_cloud_();
}

void AppDisplay::disp_header_date_(const String date){  // 日付の表示
  M5.Display.setFont(&fonts::lgfxJapanGothic_12);
  M5.Display.setCursor(0, 0);
  M5.Display.print(date);

  USBSerial.println(date.c_str());  // デバッグ用
}

void AppDisplay::disp_umbrella_(){
  M5.Display.drawBmpFile(SPIFFS, path_umb_.c_str(), 0, icon_y_offset_);  // 傘
}

void AppDisplay::disp_sun_cloud_(){
  M5.Display.drawBmpFile(SPIFFS, path_sun_cloud_.c_str(), 0, icon_y_offset_);  // 太陽と雲
}

uint16_t AppDisplay::getColor_(uint8_t red, uint8_t green, uint8_t blue){
  return ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);
}