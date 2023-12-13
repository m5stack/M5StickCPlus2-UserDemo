/**
 * @file test_rtc.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"
// #include "../../lib/rtc/cplus_RTC.h"

namespace TEST
{

    cplus_RTC _rtc;

    void TEST::DisplayRTC()
    {

        display->setFont(&fonts::Font0);

        Disbuff->fillRect(0, 0, 240, 135, Disbuff->color565(0, 0, 0));
        // Displaybuff();
        _rtc.GetBm8563Time();
        RTC_TimeTypeDef time;
        _rtc.GetTime(&time);

        Disbuff->setTextSize(4);
        Disbuff->setCursor(6, 25);
        Disbuff->setTextColor(TFT_WHITE);

        while (1)
        {
            Disbuff->fillRect(0, 0, 240, 135, Disbuff->color565(0, 0, 0));
            _rtc.GetTime(&time);
            Disbuff->setTextSize(4);
            Disbuff->setTextColor(TFT_WHITE);
            Disbuff->setCursor(25, 50);
            Disbuff->printf("%02d:%02d:%02d", time.Hours, time.Minutes,
                            time.Seconds);
            Disbuff->fillRect(0, 0, 240, 25, Disbuff->color565(20, 20, 20));
            Disbuff->setTextSize(2);
            Disbuff->drawString("BMP8563 RTC Time", 26, 5, 1);

            if (is_test_mode)
            {
                display->setFont(&fonts::efontCN_24);
                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->setTextSize(1);
                display->setCursor(0, 80);
                display->printf("确保数值正常且变化");
                display->setFont(&fonts::Font0);
            }

            Displaybuff();

            // M5.update();
            // checkAXPPress();
            delay(100);

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }
    }

    void TEST::rtc_init()
    {
        // rtc.begin();
        _rtc.clearIRQ();
        _rtc.disableIRQ();
    }

    void TEST::rtc_test()
    {
        printf("rtc test\n");
        DisplayRTC();
        printf("quit rtc test\n");
    }

    void TEST::rtc_wakeup_test()
    {
        display->fillScreen(TFT_BLACK);
        display->setCursor(0, 10);
        display->setFont(&fonts::efontCN_24);
        display->setTextColor(TFT_YELLOW);
        display->setTextSize(1);
        display->printf(" [RTC 唤醒测试]\n - 请移除电源 -\n\n 按下按键[关机]\n 数秒后将自动启动");
        displayUpdate();

        while (1)
        {
            if (btnA.pressed())
            {
                _tone(5000, 50);
                break;
            }
            if (btnB.pressed())
            {
                _tone(5500, 50);
                break;
            }
            if (btnPWR.pressed())
            {
                _tone(3500, 50);
                break;
            }
        }

        // waitNext();
        _rtc.clearIRQ();
        _rtc.SetAlarmIRQ(4);
        display->fillScreen(TFT_BLACK);
        displayUpdate();
        delay(500);
        // power_off();

        digitalWrite(POWER_HOLD_PIN, 0);

        while (1)
        {
            printf("%d\n", btnPWR.read());
            delay(50);
        }
    }

    // void TEST::gpio_test()
    // {
    //     printf("hat test\n");

    //     display->fillScreen(TFT_BLACK);
    //     display->setCursor(0, 5);
    //     display->setFont(&fonts::efontCN_24);
    //     display->setTextColor(TFT_YELLOW);
    //     display->setTextSize(1);
    //     display->printf("[HAT接口测试]\n\n请插上治具\n观察 [流水灯]\n和 [主机LED灯]");
    //     displayUpdate();

    //     // gpio_reset_pin(GPIO_NUM_19);
    //     // pinMode(19, OUTPUT);
    //     // while (1) {
    //     //     digitalWrite(19, 0);
    //     //     printf("19 %d\n", digitalRead(19));
    //     //     delay(1000);
    //     //     digitalWrite(19, 1);
    //     //     printf("19 %d\n", digitalRead(19));
    //     //     delay(1000);
    //     // }

    //     std::vector<gpio_num_t> gpio_list = {GPIO_NUM_26, GPIO_NUM_25, GPIO_NUM_0, GPIO_NUM_19};

    //     /* Reset */
    //     for (auto i : gpio_list) {
    //         gpio_reset_pin(i);
    //         pinMode(i, OUTPUT);
    //     }

    //     uint32_t time_count = 0;
    //     auto iter = gpio_list.begin();
    //     while (1) {

    //         if ((millis() - time_count) > 600) {

    //             printf("set %d %d\n", *iter, !digitalRead(*iter));
    //             digitalWrite(*iter, !digitalRead(*iter));

    //             iter++;
    //             if (iter == gpio_list.end()) {
    //                 iter = gpio_list.begin();
    //             }

    //             time_count = millis();
    //         }

    //         checkReboot();
    //         if (checkNext()) {
    //             break;
    //         }

    //     }

    //     /* Reset */
    //     for (auto i : gpio_list) {
    //         gpio_reset_pin(i);
    //     }

    //     display->setFont(&fonts::Font0);
    //     display->setTextSize(1);

    //     printf("quit hat test\n");

    // }

}
