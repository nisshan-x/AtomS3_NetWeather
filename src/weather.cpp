#include "weather.hpp"

bool WeatherJson::getWeatherJson(){
  unsigned long currentMillis = millis();
  if(currentMillis < updatedMillis_ + 1000 * 60 * 60){
    return true;  // 1時間に1回だけjsonを取得する
  }
  updatedMillis_ = currentMillis;

  HTTPClient http;
  http.begin(JSON_URL);  // 気象庁
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      payload_ = http.getString();
      http.end();

      // デバッグ用
      // while(true){
      //   USBSerial.println("Get json");
      // };

      return true;
    }
  }
  else if (httpCode <= 0)
  {
    http.end();
    // Error : Can't get Weather json"
    return false;
  }

  http.end();
  return false;
}

void WeatherJson::analyze_data(const time_t current_time_info_t){
  deserializeJson(doc_, payload_);

  weather_data_size_ = doc_[0]["timeSeries"][0]["timeDefines"].size();  // json内のデータの個数を取得 = 3

  // デバッグ用
  // USBSerial.print("    weather_data_size_ ");
  // USBSerial.println(weather_data_size_);

  for(uint8_t i=0; i<weather_data_size_; i++){
    analyze_data_func_(i, current_time_info_t);
  }
}

void WeatherJson::analyze_data_func_(const uint8_t index, const time_t current_time_info_t){
  // date
  String date_tmp = doc_[0]["timeSeries"][0]["timeDefines"][index];  // 例 "2024-03-01T05:00:00+09:00"
  fc_data[index].fc_date_time = date_tmp;
  String date_tmp2 = fc_data[index].fc_date_time;
  String date_ymd = date_tmp2.substring(0, 10);  // 年と月と日だけ "2024-03-01"

  tm date_tm = stringToTm_(date_ymd);  // time形式に変換
  fc_data[index].fc_date_time_t = mktime(&date_tm);  // time_t形式に変換
  double diff_sec = difftime(fc_data[index].fc_date_time_t, current_time_info_t);  // 現在の日付との差を出す。出力はsec
  fc_data[index].diff_days = diff_sec / (60 * 60 * 24); // 日数に変換

  // 昨日、今日、明日、明後日
  if(fc_data[index].diff_days == -1) fc_data[index].day_info = String(" 昨日");
  if(fc_data[index].diff_days == 0){
    fc_data[index].day_info = String(" 今日");
    today_index_ = index;

    // デバッグ用
    // USBSerial.print("    today_index_ ");
    // USBSerial.println(today_index_);
  }
  if(fc_data[index].diff_days == 1){
    fc_data[index].day_info = String(" 明日");
    tomorrow_index_ = index;
  }
  if(fc_data[index].diff_days == 2) fc_data[index].day_info = String(" 明後日");

  String date_temp2 = fc_data[index].fc_date_time;
  fc_data[index].fc_date_mmdd = date_temp2.substring(5, 10);  // 月と日だけ "03-01"
  fc_data[index].fc_date_mmdd.concat(fc_data[index].day_info);  // "03-04 明日"

  // forecast
  String forecast_tmp = doc_[0]["timeSeries"][0]["areas"][0]["weathers"][index];  // 例 "晴れ　時々　くもり"
  forecast_tmp.replace("　", " ");  // 例 "晴れ　時々　くもり" -> "晴れ 時々 くもり"
  fc_data[index].forecast = forecast_tmp;

  // debug
  USBSerial.print("    index ");
  USBSerial.println(index);
  USBSerial.print("    fc_data[index].forecast ");
  USBSerial.println(fc_data[index].forecast);

  //weather code
  String weather_code_tmp = doc_[0]["timeSeries"][0]["areas"][0]["weatherCodes"][index];  // 例 "102"
  fc_data[index].weather_code_str = weather_code_tmp;
  fc_data[index].umbrella_flag = umbrella_check_weather_code_(fc_data[index].weather_code_str);

  // デバッグ用
  // int weather_code_size = doc_[0]["timeSeries"][0]["areas"][0]["weatherCodes"].size();
  // USBSerial.print("    weather_code_size ");
  // USBSerial.println(weather_code_size);

  // USBSerial.print("    weather_code_tmp ");
  // USBSerial.println(weather_code_tmp.c_str());
  USBSerial.print("    fc_data[index].weather_code_str ");
  USBSerial.println(fc_data[index].weather_code_str.c_str());
  // USBSerial.print("    fc_data[index].umbrella_flag ");
  // USBSerial.println(fc_data[index].umbrella_flag);
  USBSerial.println("");
}

String WeatherJson::getTodayDateStr(){  // 今日の日付 ("今日"付き)
  return fc_data[today_index_].fc_date_mmdd;
}

String WeatherJson::getTomorrowDateStr(){  // 明日の日付 ("明日"付き)
  return fc_data[tomorrow_index_].fc_date_mmdd;
}

bool WeatherJson::getTodayUmbrella(){
  return fc_data[today_index_].umbrella_flag;
}

bool WeatherJson::getTomorrowUmbrella(){
  return fc_data[tomorrow_index_].umbrella_flag;
}

bool WeatherJson::umbrella_check_weather_code_(const String weather_code_str){
  int weather_code_int = weather_code_str.toInt();
  for(uint8_t i = 0; i < UC_NUM_; i++){
    if(weather_code_int == umbrella_code_[i])

// デバッグ用
      // USBSerial.print("    weather_code_int ");
      // USBSerial.println(weather_code_int);

      return true;
  }
  return false;
}

// 指定された日付文字列をtm構造体に変換する関数
tm WeatherJson::stringToTm_(const String dateString) {
  tm tmDate;
  sscanf(dateString.c_str(), "%d-%d-%d", &tmDate.tm_year, &tmDate.tm_mon, &tmDate.tm_mday);
  tmDate.tm_year -= 1900; // tm構造体では年を1900からの差で表す
  tmDate.tm_mon -= 1; // tm構造体では月を0から11で表す
  tmDate.tm_hour = 0;
  tmDate.tm_min = 0;
  tmDate.tm_sec = 0;
  tmDate.tm_isdst = -1; // 夏時間は未定義
  return tmDate;
}