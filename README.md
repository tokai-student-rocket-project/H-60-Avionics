# H-60-Avionics
H-60搭載計器 すばる1.2 | 2024年度 冬季 大樹

すばる1.1の機能を持ちつつ[第6世代GSE](https://github.com/tokai-student-rocket-project/Gen6-GSE-Elec) に対応しました．


## 回路図・ボード図

- [Flight Module](https://github.com/tokai-student-rocket-project/H-60-Avionics/tree/main/KiCad/FlightModule)
- [Power Module](https://github.com/tokai-student-rocket-project/H-60-Avionics/tree/main/KiCad/PowerModule)
- [Sensing Module](https://github.com/tokai-student-rocket-project/H-60-Avionics/tree/main/KiCad/SensingModule)
- [基板外形](https://github.com/tokai-student-rocket-project/H-60-Avionics/tree/main/KiCad/StandardModuleBoardShape)


## 報告書
- [H-60　報告書](https://github.com/tokai-student-rocket-project/H-60-Avionics/blob/main/Analysis/H-60_%E5%A0%B1%E5%91%8A%E6%9B%B8.pdf)


## ディレクトリ構成図

PlatformIO により自動生成されるフォルダを含めたディレクトリ構成です．

<pre>
├─.pio                    // 自動生成
├─include                 // 特に使用しない
├─lib                     // 自作ライブラリの保存先
│  ├─B3MSC1170A
│  ├─IcsBaseClass
│  ├─IcsHardSerialClass
│  ├─Lib_ADXL375
│  ├─Lib_Altimeter
│  ├─Lib_BNO055
│  ├─Lib_Buzzer
│  ├─Lib_CAN
│  ├─Lib_CountDetector
│  ├─Lib_FlightMode
│  ├─Lib_FlightPin
│  ├─Lib_FlightTime
│  ├─Lib_FRAM
│  ├─Lib_GNSS
│  ├─Lib_GseSignal
│  ├─Lib_Logger1
│  ├─Lib_Logger2
│  ├─Lib_Logger4
│  ├─Lib_OutputPin
│  ├─Lib_PowerMonitor
│  ├─Lib_RateMonitor
│  ├─Lib_Shiranui
│  ├─Lib_Telemeter
│  ├─Lib_Thermistor
│  ├─Lib_Var
│  └─RS405CB
├─src                     // ソースコードの保存先
│  ├─Flight              // 各モジュールたち
│  │  ├─FlightModule
│  │  ├─MissionModule
│  │  ├─SensingModule
│  │  ├─SutegomaModule
│  │  └─ValveControlModule
│  ├─Ground              // 地上局
│  │  ├─CSV
│  │  │  ├─FlightModule
│  │  │  └─SensingModule
│  │  └─JSON
│  │      ├─FlightModule
│  │      └─SensingModule
│  └─LogDumper            // ログを読み出すとき
│      ├─FlightModule
│      ├─MissionModule
│      └─SensingModule
│
└─platformio.ini           // PlatformIOの設定ファイル
└─README.md                // このファイル
</pre>

---


## ライブラリでエラーが出るとき

MsgPacketizer.h, Packetizer.h, MsgPack.h は自身の環境に直接クローンする

**場所**

- Flight Module[Flihgt]

  - .pio > libdeps > FlightModule

- Mission Module[Flight]

 - .pio > libdeps > MissionModule

- Sensing Module[Flight]

  - .pio > libdeps > Sensing Module

- Valve Control Module[Flight]

  - .pio > libdeps > Valve Control Module

- Flight Module[Ground]

  - .pio > libdeps > GroundFlightModule

- Sensing Module[Ground]

  - .pio > libdeps > GroundSensingModule
