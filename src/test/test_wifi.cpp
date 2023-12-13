/**
 * @file test_wifi.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"

namespace TEST
{

    struct WiFiList_t
    {
        String ssid;
        int8_t rssi = 0;
    };
    static WiFiList_t _wifi_list[13];
    static bool _wifi_tested = false;

    void TEST::wifi_display_normal_mode()
    {
        printf("wifi scan\n");

        display->fillScreen(TFT_BLACK);
        display->setFont(&fonts::Font0);
        display->setTextSize(2);
        display->setTextColor(TFT_WHITE);
        display->setCursor(0, 6);

        display->fillRect(0, 0, 240, 25, Disbuff->color565(20, 20, 20));
        display->printf("      WiFi Scan\n");

        display->setTextSize(1);
        display->setCursor(0, 28);
        display->printf(" Scanning...\n");
        displayUpdate();
        display->setCursor(0, 28);

        if (!_wifi_tested)
        {
            _wifi_tested = true;

            // Set WiFi to station mode and disconnect from an AP if it was previously connected.
            WiFi.mode(WIFI_STA);
            WiFi.disconnect();
            delay(100);

            /* ------------------------------------- Scan test ------------------------------------- */
            printf("Wifi scan start\n");
            int n = WiFi.scanNetworks();
            if (n == 0)
            {
                printf("scan error\n");
            }
            else
            {
                for (int i = 0; i < 13; ++i)
                {
                    // display->setTextColor((WiFi.RSSI(i) > -75) ? TFT_GREEN : TFT_RED, TFT_BLACK);
                    // display->printf(" %s ", WiFi.SSID(i).c_str());
                    // display->printf(" %d\n", WiFi.RSSI(i));
                    // displayUpdate();

                    _wifi_list[i].rssi = WiFi.RSSI(i);
                    _wifi_list[i].ssid = WiFi.SSID(i);

                    switch (WiFi.encryptionType(i))
                    {
                    case WIFI_AUTH_OPEN:
                        _wifi_list[i].ssid += " (open)";
                        break;
                    case WIFI_AUTH_WEP:
                        _wifi_list[i].ssid += " (WEP)";
                        break;
                    case WIFI_AUTH_WPA_PSK:
                        _wifi_list[i].ssid += " (WPA)";
                        break;
                    case WIFI_AUTH_WPA2_PSK:
                        _wifi_list[i].ssid += " (WPA2)";
                        break;
                    case WIFI_AUTH_WPA_WPA2_PSK:
                        _wifi_list[i].ssid += " (WPA+WPA2)";
                        break;
                    case WIFI_AUTH_WPA2_ENTERPRISE:
                        _wifi_list[i].ssid += " (WPA2-EAP)";
                        break;
                    case WIFI_AUTH_WPA3_PSK:
                        _wifi_list[i].ssid += " (WPA3)";
                        break;
                    case WIFI_AUTH_WPA2_WPA3_PSK:
                        _wifi_list[i].ssid += " (WPA2+WPA3)";
                        break;
                    case WIFI_AUTH_WAPI_PSK:
                        _wifi_list[i].ssid += " (WAPI)";
                        break;
                    default:
                        break;
                    }
                }
            }
            // Delete the scan result to free memory for code below.
            WiFi.scanDelete();

            WiFi.disconnect(true);
            esp_wifi_disconnect();
            esp_wifi_stop();
            esp_wifi_deinit();
            esp_wifi_clear_ap_list();
        }

        /* Display result */
        for (int i = 0; i < 13; ++i)
        {
            display->setTextColor((_wifi_list[i].rssi > -75) ? TFT_GREEN : TFT_RED, TFT_BLACK);
            display->printf(" %s ", _wifi_list[i].ssid.c_str());
            display->printf(" %d\n", _wifi_list[i].rssi);
            displayUpdate();

            // _wifi_list[i].rssi = WiFi.RSSI(i);
            // _wifi_list[i].ssid = WiFi.SSID(i);
        }

        waitNext();
    }

    void TEST::wifi_test()
    {
        printf("wifi test\n");

        if (!is_test_mode)
        {
            wifi_display_normal_mode();
        }

        printf("quit wifi test\n");
    }
}
