/**
 * @file test.h
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <Arduino.h>
#include <M5GFX.h>
#include <lgfx/v1/panel/Panel_ST7789.hpp>
#include <Button.h>
#include <Wire.h>
#include <MPU6886.h>

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <cplus_RTC.h>

#include <esp_wifi.h>

#include "arduinoFFT.h"

#define display Disbuff
#define displayUpdate Displaybuff

#define LCD_MOSI_PIN 15
#define LCD_MISO_PIN -1
#define LCD_SCLK_PIN 13
#define LCD_DC_PIN 14
#define LCD_CS_PIN 5
#define LCD_RST_PIN 12
#define LCD_BUSY_PIN -1
#define LCD_BL_PIN 27

#define POWER_HOLD_PIN 4
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

#define I2S_DATA_PIN 34
#define I2S_SCLK_PIN 0

#define BUZZ_PIN 2

class CLite_GFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

public:
    CLite_GFX(void)
    {
        {
            auto cfg = _bus_instance.config();

            cfg.pin_mosi = LCD_MOSI_PIN;
            cfg.pin_miso = LCD_MISO_PIN;
            cfg.pin_sclk = LCD_SCLK_PIN;
            cfg.pin_dc = LCD_DC_PIN;
            cfg.freq_write = 40000000;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        {
            auto cfg = _panel_instance.config();

            cfg.invert = true;
            cfg.pin_cs = LCD_CS_PIN;
            cfg.pin_rst = LCD_RST_PIN;
            cfg.pin_busy = LCD_BUSY_PIN;
            cfg.panel_width = 135;
            cfg.panel_height = 240;
            cfg.offset_x = 52;
            cfg.offset_y = 40;
            // cfg.offset_x     = 0;
            // cfg.offset_y     = 0;

            _panel_instance.config(cfg);
        }
        {                                        // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

            cfg.pin_bl = 27;    // バックライトが接続されているピン番号
            cfg.invert = false; // バックライトの輝度を反転させる場合 true
            // cfg.freq   = 44100;           // バックライトのPWM周波数
            cfg.freq = 200;      // バックライトのPWM周波数
            cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }

        setPanel(&_panel_instance);
    }
};

extern const unsigned char ImageData[768];
extern const unsigned char error_48[4608];
extern const unsigned char icon_ir[4608];
extern const unsigned char icon_ble[4608];
extern const unsigned char icon_wifi[4608];
extern const unsigned char icon_ble_disconnect[4608];

namespace TEST
{

    class TEST
    {
    private:
        bool is_test_mode;

        void hardware_init();
        void power_off();

        inline void _tone(unsigned int frequency, unsigned long duration = 0UL) { tone(BUZZ_PIN, frequency, duration); }
        inline void _noTone() { noTone(BUZZ_PIN); }

        /* Display */
        CLite_GFX lcd;
        LGFX_Sprite *Disbuff;

        void Displaybuff();
        void ColorBar();

        void frame_test();
        void color_test();
        void lcd_init();
        void lcd_test();

        /* Keys */
        void key_init();
        void key_test();
        Button btnA = Button(37, 20);
        Button btnB = Button(39, 20);
        Button btnPWR = Button(35, 20);
        void checkReboot();
        bool checkNext();
        void waitNext();

        /* IMU */
        MPU6886 imu;
        void imu_init();
        void imu_test();

        /* Mic */
        void mic_init();
        void mic_test();
        void mic_test_one_task();

        void DisplayMicro();

        void new_mic_test();
        void new_mic_test_fft();

        /* IR */
        void ir_init();
        void ir_test();

        /* BLE */
        void ble_init();
        void ble_test();

        void DisPlayBLESend();

        /* RTC */
        void rtc_init();
        void rtc_test();
        void rtc_wakeup_test();

        void DisplayRTC();

        /* GPIO */
        void gpio_test();

        /* Wifi */
        void wifi_init();
        void wifi_test();
        void wifi_display_test_mode();
        void wifi_display_normal_mode();

    public:
        TEST() : is_test_mode(false) {}
        ~TEST() {}

        inline void setTestMode(bool testMode) { is_test_mode = testMode; }

        void setup();
        void loop();
    };

}
