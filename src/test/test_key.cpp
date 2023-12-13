/**
 * @file test_key.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"

namespace TEST
{

    void TEST::key_init()
    {
        btnA.begin();
        btnB.begin();
        btnPWR.begin();
    }

    void TEST::key_test()
    {
        // while (1) {
        //     printf("%d %d %d\n", btnA.pressed(), btnB.pressed(), btnPWR.pressed());
        //     delay(10);
        // }

        display->fillScreen(TFT_BLACK);
        display->setFont(&fonts::efontCN_24);
        display->setTextSize(1);

        display->setCursor(0, 0);
        display->setTextColor(TFT_YELLOW, TFT_BLACK);
        display->printf(" [按键测试]\n 请按下各个按键3次\n");

        uint8_t key_pressed_times[3] = {0, 0, 0};
        bool pass = false;

        while (1)
        {
            if (btnA.pressed())
            {
                _tone(5000, 50);
                key_pressed_times[1]++;
            }
            if (btnB.pressed())
            {
                _tone(5500, 50);
                key_pressed_times[2]++;
            }
            if (btnPWR.pressed())
            {
                _tone(3500, 50);
                key_pressed_times[0]++;
            }

            display->setCursor(0, 60);
            for (int i = 0; i < 3; i++)
            {
                display->setTextColor((key_pressed_times[i] > 2) ? TFT_GREEN : TFT_BLUE, TFT_BLACK);
                // display->printf(" 按键%d: %d\n", i, key_pressed_times[i]);
                if (i == 0)
                {
                    display->printf(" 电源键: %d\n", key_pressed_times[i]);
                }
                else if (i == 1)
                {
                    display->printf(" 按键A: %d\n", key_pressed_times[i]);
                }
                else if (i == 2)
                {
                    display->printf(" 按键B: %d\n", key_pressed_times[i]);
                }
            }
            display->printf("\n");
            display->setTextColor(TFT_YELLOW, TFT_BLACK);
            displayUpdate();

            for (int i = 0; i < 3; i++)
            {
                if (key_pressed_times[i] > 2)
                {
                    pass = true;
                }
                else
                {
                    pass = false;
                    break;
                }
            }
            if (pass)
            {
                printf("key test passed\n");
                delay(200);
                break;
            }
        }
    }

    void TEST::checkReboot()
    {
        // if (btnPWR.pressed()) {
        //     printf("rebooting...\n");
        //     esp_restart();
        // }

        // printf("%d\n", btnPWR.read());

        // uint8_t count = 0;
        // if (!btnPWR.read()) {
        //     delay(10);
        //     if (!btnPWR.read()) {

        //         _tone(3500, 50);

        //         while (!btnPWR.read()) {
        //             delay(100);
        //             count++;
        //         }
        //         if (count > 30) {
        //             power_off();
        //         }
        //         else {
        //             printf("rebooting...\n");
        //             esp_restart();
        //         }
        //     }
        // }

        // if (btnPWR.pressed()) {
        //     _tone(3500, 50);

        //     for (int i = 0; i < 4; i++) {
        //         delay(50);
        //         if (btnPWR.pressed()) {
        //             _tone(3500, 50);
        //             printf("double\n");
        //             power_off();
        //         }
        //     }
        //     printf("not double\n");
        //     printf("rebooting...\n");
        //     esp_restart();
        // }

        /* Long press power off */
        if (!btnPWR.read())
        {
            _tone(3500, 50);

            uint32_t time_count = millis();

            while (!btnPWR.read())
            {

                display->setCursor(0, 10);
                display->setFont(&fonts::Font0);
                display->setTextSize(2);
                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf(" PWR OFF IN %d/3\n", (millis() - time_count) / 1000 + 1);
                displayUpdate();

                delay(10);
            }

            printf("rebooting...\n");

            display->fillScreen(TFT_BLACK);
            display->setCursor(0, 10);
            display->setFont(&fonts::Font0);
            display->setTextSize(2);
            display->setTextColor(TFT_RED, TFT_BLACK);
            display->drawCenterString("REBOOTING...\n", display->width() / 2, display->height() / 2 - 12);
            displayUpdate();

            delay(500);

            esp_restart();
        }
    }

    bool TEST::checkNext()
    {
        // if (btnA.pressed() || btnB.pressed()) {
        //     return true;
        // }
        if (btnA.pressed())
        {
            _tone(5000, 50);
            return true;
        }
        if (btnB.pressed())
        {
            _tone(5500, 50);
            return true;
        }

        // uint8_t count = 0;
        // if (!btnB.read()) {
        //     delay(10);
        //     if (!btnB.read()) {

        //         _tone(5500, 50);

        //         while (!btnB.read()) {
        //             delay(50);
        //             count++;
        //             if (count > 60) {
        //                 power_off();
        //             }
        //         }
        //         return true;
        //     }
        // }

        return false;
    }

    void TEST::waitNext()
    {
        while (!checkNext())
        {
            checkReboot();
            delay(10);
        }
        printf("go next\n");
    }

}
