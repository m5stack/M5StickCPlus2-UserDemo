/**
 * @file test_io.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"

namespace TEST
{

    void TEST::gpio_test()
    {
        printf("hat test\n");

        display->fillScreen(TFT_BLACK);
        display->setCursor(0, 0);
        display->setFont(&fonts::efontCN_24);
        display->setTextColor(TFT_YELLOW);
        display->setTextSize(1);
        display->printf(" [IO]测试\n");
        displayUpdate();

        // gpio_reset_pin(GPIO_NUM_19);
        // pinMode(19, OUTPUT);
        // while (1) {
        //     digitalWrite(19, 0);
        //     printf("19 %d\n", digitalRead(19));
        //     delay(1000);
        //     digitalWrite(19, 1);
        //     printf("19 %d\n", digitalRead(19));
        //     delay(1000);
        // }

        std::vector<gpio_num_t> gpio_list = {GPIO_NUM_25, GPIO_NUM_19};

        /* Reset */
        for (auto i : gpio_list)
        {
            gpio_reset_pin(i);
            pinMode(i, OUTPUT);
        }

        uint32_t time_count = 0;
        auto iter = gpio_list.begin();
        while (1)
        {

            if ((millis() - time_count) > 600)
            {

                display->fillScreen(TFT_BLACK);
                display->setCursor(0, 0);
                display->setFont(&fonts::efontCN_24);
                display->setTextColor(TFT_YELLOW);
                display->setTextSize(1);
                display->printf(" [IO]测试\n");
                // displayUpdate();

                printf("set %d %d\n", *iter, !digitalRead(*iter));
                digitalWrite(*iter, !digitalRead(*iter));

                iter++;
                if (iter == gpio_list.end())
                {
                    iter = gpio_list.begin();
                }

                display->setCursor(0, 20);

                display->setTextColor(TFT_GREEN, TFT_BLACK);
                float v_bat = (float)analogReadMilliVolts(38) * 2 / 1000;
                if (v_bat < 3)
                {
                    display->setTextColor(TFT_RED, TFT_BLACK);
                }
                display->printf(" 电池: %.2fV\n", v_bat);

                display->setTextColor(TFT_GREEN, TFT_BLACK);
                float v_g36 = (float)analogReadMilliVolts(36) / 1000;
                if (digitalRead(25))
                {
                    if (v_g36 < 2.5)
                    {
                        display->setTextColor(TFT_RED, TFT_BLACK);
                    }
                }
                else
                {
                    if (v_g36 > 2)
                    {
                        display->setTextColor(TFT_RED, TFT_BLACK);
                    }
                }
                display->printf(" G36: %.2fV\n", v_g36);

                Wire.end();
                Wire.begin(32, 33, 100000);

                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf(" [Grove]:");

                byte error, address;
                uint8_t addr = 0x00;
                for (address = 0x01; address < 0x7f; address++)
                {
                    Wire.beginTransmission(address);
                    error = Wire.endTransmission();
                    if (error == 0)
                    {
                        // Serial.printf("I2C device found at address 0x%02X\n", address);

                        addr = address;
                        printf("fonud 0x%02X\n", addr);
                        break;
                    }
                }

                if (addr)
                {
                    display->setTextColor(TFT_GREEN, TFT_BLACK);
                    display->printf(" 0x%02X", addr);
                }
                else
                {
                    display->setTextColor(TFT_RED, TFT_BLACK);
                    display->printf(" 无");
                }

                Wire.end();
                Wire.begin(0, 26, 100000);

                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf("\n [Hat]:");

                // byte error, address;
                addr = 0x00;
                for (address = 0x01; address < 0x7f; address++)
                {
                    Wire.beginTransmission(address);
                    error = Wire.endTransmission();
                    if (error == 0)
                    {
                        // Serial.printf("I2C device found at address 0x%02X\n", address);

                        addr = address;
                        printf("fonud 0x%02X", addr);
                        break;
                    }
                }

                if (addr)
                {
                    display->setTextColor(TFT_GREEN, TFT_BLACK);
                    display->printf(" 0x%02X", addr);
                }
                else
                {
                    display->setTextColor(TFT_RED, TFT_BLACK);
                    display->printf(" 无");
                }

                displayUpdate();

                time_count = millis();
            }

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }

        /* Reset */
        for (auto i : gpio_list)
        {
            gpio_reset_pin(i);
        }

        display->setFont(&fonts::Font0);
        display->setTextSize(1);

        printf("quit hat test\n");
    }

}
