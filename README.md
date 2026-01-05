# REPEATER
ESP32 based echo repeater for HAM RADIO


Version V.3

Attention: It is necessary to use ESP32 chips with PSRAM

Features: 
    Audio signal detection
        rising and falling edges
        noise suppression
        minimal record length

    PTT control
    bluetooth classic signal level and status monitoring
Features of V.3: 
    Recording into ESP PSRAM
    Playing via external I2S amplifier


ПО для использования:
VS-Code                                     ver >= 1.117.1
ESP-IDF                 VS-Code plugin      ver >= 1.10.2
ESPTool                 VS-Code plugin      ver >= 1.2.0
C/C++ Extension Pack    VS-Code plugin
    C/C++ IntelliSense  VS-Code plugin      ver >= 1.3.1
    C/C++ Themes        VS-Code plugin      ver >= 2.0.0
    CMake Tools         VS-Code plugin      ver >= 1.21.36
Pyton                                       ver >=3.12

ESP-IDF Framework       Framework           ver = 5.5.1
ESP-IDF Toolchain

Директории 
    ./.vscode
    ./build
не включены в репазиторий.
./.vscode - настройки среды. для создания директории - в меню плагина ESP-IDF 'Comands'->'Advanced'->'Add .vscode subdirectory files'
./build формируется при компиляции проекта
