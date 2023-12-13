/**
 * @file test_ir.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"
#define DECODE_NEC
// #define DECODE_DENON
#include <IRremote.hpp>
#include <TinyIRSender.hpp>
// #include "../../lib/IRremote/src/IRremote.hpp"
// #include "../../lib/IRremote/src/TinyIRSender.hpp"

// extern const unsigned char icon_ir[4608];

namespace TEST
{

    static void ir_test_task(void *param)
    {
        printf("ir task\n");

        uint8_t IR_SEND_PIN = 19;
        uint8_t cmd = 0x00;
        while (1)
        {
            sendNEC(IR_SEND_PIN, 0x10, cmd, 0);
            printf("ir sended: addr: %0x02X cmd:0x%02X\n", 0x10, cmd);
            cmd++;
            delay(1000);
        }

        vTaskDelete(NULL);
    }

    void TEST::ir_init()
    {
        xTaskCreate(ir_test_task, "ir", 2048, NULL, 5, NULL);
    }

    void TEST::ir_test()
    {
        printf("ir test\n");

        uint8_t IR_SEND_PIN = 19;
        // while (1) {

        //     sendNEC(IR_SEND_PIN, 0x10, 0xFF, 0);
        //     printf("sended\n");
        //     delay(1000);

        // }

        display->setFont(&fonts::Font0);

        uint8_t cmd = 0x00;
        unsigned long count = millis();
        unsigned long anim_count = millis();
        uint8_t anim_pos = 0;

        sendNEC(IR_SEND_PIN, 0x10, cmd, 0);
        printf("ir sended: addr: 0x%02X cmd:0x%02X\n", 0x10, cmd);

        while (1)
        {

            if ((millis() - count) > 800)
            {
                cmd++;
                sendNEC(IR_SEND_PIN, 0x10, cmd, 0);
                printf("ir sended: addr: 0x%02X cmd:0x%02X\n", 0x10, cmd);
                count = millis();
            }

            if ((millis() - anim_count) > 400)
            {

                anim_pos++;
                if (anim_pos > 3)
                {
                    anim_pos = 0;
                }

                Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
                // Disbuff->setSwapBytes(true);
                Disbuff->pushImage(180, 16, 48, 48, (uint16_t *)icon_ir);
                Disbuff->setTextColor(Disbuff->color565(180, 180, 180));
                Disbuff->setTextSize(3);
                Disbuff->setCursor(12, 20);
                Disbuff->printf("IR Send ");
                Disbuff->setTextSize(5);
                Disbuff->setCursor(12, 75);

                switch (anim_pos)
                {
                case 0:
                    Disbuff->printf("0x%02X> ", cmd);
                    break;
                case 1:
                    Disbuff->printf("0x%02X>> ", cmd);
                    break;
                case 2:
                    Disbuff->printf("0x%02X >>", cmd);
                    break;
                case 3:
                    Disbuff->printf("0x%02X  >", cmd);
                    break;
                default:
                    break;
                }

                if (is_test_mode)
                {
                    display->setFont(&fonts::efontCN_24);
                    display->setTextColor(TFT_YELLOW, TFT_BLACK);
                    display->setTextSize(1);
                    display->setCursor(0, 0);
                    display->printf("距离[2米]测试");
                    display->setFont(&fonts::Font0);
                }

                displayUpdate();

                anim_count = millis();
            }

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }

        printf("quit ir test\n");
    }

}
