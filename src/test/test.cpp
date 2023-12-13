/**
 * @file test.cpp
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

    void TEST::hardware_init()
    {
        /* Hold pwr pin */
        gpio_reset_pin((gpio_num_t)POWER_HOLD_PIN);
        pinMode(POWER_HOLD_PIN, OUTPUT);
        digitalWrite(POWER_HOLD_PIN, HIGH);

        Wire1.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    }

    void TEST::power_off()
    {
        rtc_init();
        display->fillScreen(TFT_BLACK);
        displayUpdate();

        while ((!btnPWR.read()))
        {
            delay(100);
        }

        delay(200);

        printf("power off\n");
        digitalWrite(POWER_HOLD_PIN, 0);
        delay(10000);

        while (1)
        {
            delay(1000);
        }
    }

    void TEST::setup()
    {
        hardware_init();
        key_init();
        lcd_init();

        // lcd.fillScreen(TFT_BLUE);
        // delay(1000);

        // lcd.fillScreen(TFT_RED);
        // while (1)
        // {
        //     delay(1000);
        // }

        // gpio_test();

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

        /* Check test mode */
        is_test_mode = false;
        if (!btnB.read())
        {
            delay(20);
            if (!btnB.read())
            {
                printf("test mode\n");
                is_test_mode = true;

                display->fillScreen(TFT_BLACK);
                display->setFont(&fonts::efontCN_24);
                display->printf("松开按键进入测试模式");
                display->setCursor(0, 10);
                displayUpdate();

                /* Start wifi test */
                // wifi_init();

                while (!btnB.read())
                {
                    delay(10);
                }
            }
        }
        else
        {
            printf("normal mode\n");
        }

        // lcd_init();
        rtc_init();
        imu_init();
        mic_init();
        ble_init();

        if (!is_test_mode)
        {
            ColorBar();
        }
    }

    void TEST::loop()
    {
        // rtc_wakeup_test();

        if (is_test_mode)
        {
            lcd_test();
        }

        imu_test();
        rtc_test();

        // mic_test();
        // new_mic_test();
        new_mic_test_fft();

        ir_test();

        if (is_test_mode)
        {
            gpio_test();
            key_test();
            // rtc_wakeup_test();
        }

        wifi_test();

        display->setFont(&fonts::Font0);
        display->setTextSize(1);
        displayUpdate();
        ble_test();

        if (is_test_mode)
        {
            // gpio_test();
            // key_test();
            rtc_wakeup_test();
        }
    }

}
