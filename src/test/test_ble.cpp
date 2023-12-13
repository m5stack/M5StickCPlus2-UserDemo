/**
 * @file test_ble.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"

#define SERVICE_UUID "1bc68b2a-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_RX_UUID "1bc68da0-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_TX_UUID "1bc68efe-f3e3-11e9-81b4-2a2ae2dbcce4"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

uint8_t *data = new uint8_t[128];

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        data = pCharacteristic->getData();
    }
};

namespace TEST
{

    bool InitBLEServer()
    {
        uint64_t chipid = ESP.getEfuseMac();
        String blename = "M5-" + String((uint32_t)(chipid >> 32), HEX);

        BLEDevice::init(blename.c_str());
        // BLEDevice::setPower(ESP_PWR_LVL_N12);
        pServer = BLEDevice::createServer();

        pServer->setCallbacks(new MyServerCallbacks());
        pService = pServer->createService(SERVICE_UUID);
        pTxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_RX_UUID, BLECharacteristic::PROPERTY_NOTIFY);

        pTxCharacteristic->addDescriptor(new BLE2902());
        BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_TX_UUID, BLECharacteristic::PROPERTY_WRITE);
        pRxCharacteristic->setCallbacks(new MyCallbacks());

        return true;
    }

    bool deinit_ble_server()
    {
        BLEDevice::deinit(true);
    }

    void TEST::DisPlayBLESend()
    {
        uint8_t senddata[2] = {0};

        pService->start();
        pServer->getAdvertising()->start();

        uint64_t chipid = ESP.getEfuseMac();
        String blename = "M5-" + String((uint32_t)(chipid >> 32), HEX);

        display->setFont(&fonts::Font0);
        display->setTextSize(1);

        while (1)
        {
            Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
            if (deviceConnected)
            {
                Disbuff->pushImage(180, 16, 48, 48, (uint16_t *)icon_ble);
                Disbuff->setTextColor(Disbuff->color565(180, 180, 180));
                Disbuff->setTextSize(3);
                Disbuff->setCursor(12, 20);
                // Disbuff.printf("BLE connect!\n");
                Disbuff->printf("BLE Send\n");
                Disbuff->setTextSize(5);
                Disbuff->setCursor(12, 75);
                if (senddata[0] % 4 == 0)
                {
                    Disbuff->printf("0x%02X>  ", senddata[0]);
                }
                else if (senddata[0] % 4 == 1)
                {
                    Disbuff->printf("0x%02X>>", senddata[0]);
                }
                else if (senddata[0] % 4 == 2)
                {
                    Disbuff->printf("0x%02X >>", senddata[0]);
                }
                else if (senddata[0] % 4 == 3)
                {
                    Disbuff->printf("0x%02X  >", senddata[0]);
                }

                senddata[1]++;
                if (senddata[1] > 3)
                {
                    senddata[1] = 0;
                    senddata[0]++;
                    pTxCharacteristic->setValue(senddata, 1);
                    pTxCharacteristic->notify();
                }
            }
            else
            {
                Disbuff->setTextSize(2);
                Disbuff->setCursor(12, 20);
                Disbuff->setTextColor(TFT_RED);
                Disbuff->printf("BLE disconnect\n");
                Disbuff->setCursor(12, 45);
                Disbuff->setTextColor(Disbuff->color565(18, 150, 219));

                Disbuff->printf(String("Name:" + blename + "\n").c_str());
                Disbuff->setCursor(12, 70);
                Disbuff->printf("UUID:1bc68b2a\n");
                Disbuff->pushImage(180, 16, 48, 48, (uint16_t *)icon_ble_disconnect);
            }
            Displaybuff();

            delay(100);
            checkReboot();
            if (checkNext())
            {
                break;
            }

            // M5.update();
            // delay(100);
            // checkAXPPress();
        }
        // while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        //     M5.update();
        //     checkAXPPress();
        //     M5.Beep.tone(4000);
        //     delay(10);
        // }
        // delay(50);
        // M5.Beep.mute();

        Disbuff->setTextColor(TFT_WHITE);
        pService->stop();
        pServer->getAdvertising()->stop();
    }

    static bool is_ble_inited = false;

    void TEST::ble_init()
    {
        // InitBLEServer();
        is_ble_inited = false;
    }

    void TEST::ble_test()
    {
        printf("ble test\n");

        if (!is_ble_inited)
        {
            printf("init ble server\n");
            InitBLEServer();
            delay(100);
            is_ble_inited = true;
        }

        DisPlayBLESend();

        printf("quit ble test\n");
    }

}
