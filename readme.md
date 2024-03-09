# M5StackシリーズのAtomS3で、天気予報をシンプルにアイコンで表示する


https://github.com/nisshan-x/AtomS3_NetWeather/assets/67353276/ce9daa78-b3eb-42b6-bd8d-64e56355e6d4


## 仕様

- 気象庁の天気予報(東京都)を取得。
- 現在時刻　→　今日の天気　→　明日の天気　の順で表示
- 表示する天気はシンプルに「傘」か「晴れ/曇り」の2種類だけ
- 今日の天気が「傘」なら、背景を赤く塗る。

## 環境

### ハードウェア

M5Stack AtomS3

### ソフトウェア開発環境

Platform IO

#### Library:

- 	m5stack/M5Unified@^0.1.12
-	bblanchon/ArduinoJson@^7.0.3

