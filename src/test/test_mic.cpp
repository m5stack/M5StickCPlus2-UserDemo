/**
 * @file test_mic.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"
// #include "fft/fft.h"

// extern const unsigned char ImageData[768];

static LGFX_Sprite *__display_ptr;
// #define Disbuff __display_ptr

namespace TEST
{

    static SemaphoreHandle_t xSemaphore = NULL;
    static SemaphoreHandle_t start_dis = NULL;
    static SemaphoreHandle_t start_fft = NULL;
    // static int8_t i2s_readraw_buff[2048];
    // static uint8_t fft_dis_buff[241][128] = {0};
    static uint16_t posData = 160;

    static int8_t *_new_i2s_readraw_buff = nullptr;
    static uint8_t **_new_fft_dis_buff = nullptr;

    static void MicRecordfft(void *arg)
    {
        // int16_t *buffptr;
        // size_t bytesread;
        // uint16_t count_n = 0;
        // float adc_data;
        // double data = 0;
        // uint16_t ydata;

        // while (1) {
        //     xSemaphoreTake(start_fft, portMAX_DELAY);
        //     xSemaphoreGive(start_fft);
        //     fft_config_t *real_fft_plan =
        //     fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
        //     i2s_read(I2S_NUM_0, (char *)_new_i2s_readraw_buff, 2048, &bytesread,
        //             (100 / portTICK_RATE_MS));
        //     buffptr = (int16_t *)_new_i2s_readraw_buff;

        //     for (count_n = 0; count_n < real_fft_plan->size; count_n++) {
        //         adc_data =
        //             (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -2000, 2000);
        //         real_fft_plan->input[count_n] = adc_data;
        //     }
        //     fft_execute(real_fft_plan);

        //     xSemaphoreTake(xSemaphore, 100 / portTICK_RATE_MS);
        //     for (count_n = 1; count_n < real_fft_plan->size / 4; count_n++) {
        //         data = sqrt(real_fft_plan->output[2 * count_n] *
        //                         real_fft_plan->output[2 * count_n] +
        //                     real_fft_plan->output[2 * count_n + 1] *
        //                         real_fft_plan->output[2 * count_n + 1]);
        //         if ((count_n - 1) < 128) {
        //             data  = (data > 2000) ? 2000 : data;
        //             ydata = map(data, 0, 2000, 0, 255);
        //             _new_fft_dis_buff[posData][128 - count_n] = ydata;
        //         }
        //     }

        //     posData++;
        //     if (posData >= 241) {
        //         posData = 0;
        //     }
        //     xSemaphoreGive(xSemaphore);
        //     fft_destroy(real_fft_plan);
        // }
    }

    static void Drawdisplay(void *arg)
    {
        uint16_t count_x = 0, count_y = 0;
        uint16_t colorPos;
        while (1)
        {
            xSemaphoreTake(start_dis, portMAX_DELAY);
            xSemaphoreGive(start_dis);
            xSemaphoreTake(xSemaphore, 500 / portTICK_RATE_MS);
            for (count_y = 0; count_y < 128; count_y++)
            {
                for (count_x = 0; count_x < 240; count_x++)
                {
                    if ((count_x + (posData % 240)) > 240)
                    {
                        colorPos =
                            _new_fft_dis_buff[count_x + (posData % 240) - 240][count_y];
                    }
                    else
                    {
                        colorPos = _new_fft_dis_buff[count_x + (posData % 240)][count_y];
                    }

                    __display_ptr->drawPixel(
                        count_x, count_y + 4,
                        __display_ptr->color565(ImageData[colorPos * 3 + 0],
                                                ImageData[colorPos * 3 + 1],
                                                ImageData[colorPos * 3 + 2]));
                    /*
                    disbuff[ count_y * 160 + count_x ].r =  ImageData[ colorPos * 3
                    + 0 ]; disbuff[ count_y * 160 + count_x ].g =  ImageData[
                    colorPos * 3 + 1 ]; disbuff[ count_y * 160 + count_x ].b =
                    ImageData[ colorPos * 3 + 2 ];
                    */
                }
            }
            xSemaphoreGive(xSemaphore);
            delay(5);
            /*
            Disbuff.setTextColor(WHITE);
            Disbuff.setTextSize(1);
            Disbuff.fillRect(0,0,70,18,Disbuff.color565(20,20,20));
            Disbuff.drawString("MicroPhone",5,5,1);
            */
            __display_ptr->pushSprite(0, 0);
        }
    }

    static TaskHandle_t xhandle_display = NULL;
    static TaskHandle_t xhandle_fft = NULL;

    void TEST::DisplayMicro()
    {
        __display_ptr->fillRect(0, 0, 160, 80, __display_ptr->color565(0, 0, 0));
        __display_ptr->pushSprite(0, 0);

        // printf("nihao\n");

        // xSemaphoreGive(start_dis);
        // xSemaphoreGive(start_fft);
        // while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        //     xSemaphoreGive(start_dis);
        //     xSemaphoreGive(start_fft);
        //     M5.update();
        //     // delay(100);
        //     xSemaphoreTake(start_dis, portMAX_DELAY);
        //     xSemaphoreTake(start_fft, portMAX_DELAY);
        // }

        /* Enable mic display */
        xSemaphoreGive(start_dis);
        xSemaphoreGive(start_fft);
        waitNext();
        printf("wait dis mutex\n");
        xSemaphoreTake(start_dis, portMAX_DELAY);
        printf("wait fft mutex\n");
        xSemaphoreTake(start_fft, portMAX_DELAY);
        printf("mutex take ok\n");
        /* Wait update mic display task */
        delay(200);

        // xSemaphoreTake( start_dis , portMAX_DELAY  );
        // xSemaphoreTake( start_fft , portMAX_DELAY  );

        // while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        //     M5.update();
        //     checkAXPPress();
        //     M5.Beep.tone(4000);
        //     delay(10);
        // }
        // delay(50);
        // M5.Beep.mute();
    }

    void TEST::mic_test_one_task()
    {
        display->fillScreen(TFT_BLACK);

        /* Mic data */
        int16_t *buffptr;
        size_t bytesread;
        uint16_t count_n = 0;
        float adc_data;
        double data = 0;
        uint16_t ydata;
        uint32_t update_count = 0;

        /* Display data */
        uint16_t count_x = 0, count_y = 0;
        uint16_t colorPos;

        while (1)
        {

            // /* Mic get data */
            // // if ((millis() - update_count) > 5) {

            //     fft_config_t *real_fft_plan =
            //     // fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
            //     i2s_read(I2S_NUM_0, (char *)_new_i2s_readraw_buff, 2048, &bytesread,
            //             (100 / portTICK_RATE_MS));
            //     buffptr = (int16_t *)_new_i2s_readraw_buff;

            //     for (count_n = 0; count_n < real_fft_plan->size; count_n++) {
            //         adc_data =
            //             (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -2000, 2000);
            //         real_fft_plan->input[count_n] = adc_data;
            //     }
            //     fft_execute(real_fft_plan);

            //     for (count_n = 1; count_n < real_fft_plan->size / 4; count_n++) {
            //         data = sqrt(real_fft_plan->output[2 * count_n] *
            //                         real_fft_plan->output[2 * count_n] +
            //                     real_fft_plan->output[2 * count_n + 1] *
            //                         real_fft_plan->output[2 * count_n + 1]);
            //         if ((count_n - 1) < 128) {
            //             data  = (data > 2000) ? 2000 : data;
            //             ydata = map(data, 0, 2000, 0, 255);
            //             _new_fft_dis_buff[posData][128 - count_n] = ydata;
            //         }
            //     }

            //     posData++;
            //     if (posData >= 241) {
            //         posData = 0;
            //     }
            //     fft_destroy(real_fft_plan);

            // //     update_count = millis();
            // // }

            /* Display */
            for (count_y = 0; count_y < 128; count_y++)
            {
                for (count_x = 0; count_x < 240; count_x++)
                {
                    if ((count_x + (posData % 240)) > 240)
                    {
                        colorPos =
                            _new_fft_dis_buff[count_x + (posData % 240) - 240][count_y];
                    }
                    else
                    {
                        colorPos = _new_fft_dis_buff[count_x + (posData % 240)][count_y];
                    }

                    display->drawPixel(
                        count_x, count_y + 4,
                        display->color565(ImageData[colorPos * 3 + 0],
                                          ImageData[colorPos * 3 + 1],
                                          ImageData[colorPos * 3 + 2]));
                    /*
                    disbuff[ count_y * 160 + count_x ].r =  ImageData[ colorPos * 3
                    + 0 ]; disbuff[ count_y * 160 + count_x ].g =  ImageData[
                    colorPos * 3 + 1 ]; disbuff[ count_y * 160 + count_x ].b =
                    ImageData[ colorPos * 3 + 2 ];
                    */
                }
            }
            // delay(5);
            /*
            Disbuff.setTextColor(WHITE);
            Disbuff.setTextSize(1);
            Disbuff.fillRect(0,0,70,18,Disbuff.color565(20,20,20));
            Disbuff.drawString("MicroPhone",5,5,1);
            */
            display->pushSprite(0, 0);

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }
    }

    // #define I2S_DATA_PIN 34
    // #define I2S_SCLK_PIN 0

#define PIN_CLK 0
#define PIN_DATA 34

    bool InitI2SMicroPhone()
    {
        esp_err_t err = ESP_OK;
        i2s_config_t i2s_config = {
            .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
            // .sample_rate = 44100,
            .sample_rate = 48000,
            .bits_per_sample =
                I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
            .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
            .communication_format =
                I2S_COMM_FORMAT_STAND_I2S, // Set the format of the communication.
#else                                      // 设置通讯格式
            .communication_format = I2S_COMM_FORMAT_I2S,
#endif
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 2,
            .dma_buf_len = 128,
        };

        i2s_pin_config_t pin_config;
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
        pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif
        pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
        pin_config.ws_io_num = PIN_CLK;
        pin_config.data_out_num = I2S_PIN_NO_CHANGE;
        pin_config.data_in_num = PIN_DATA;

        err += i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
        err += i2s_set_pin(I2S_NUM_0, &pin_config);
        // err += i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT,
        //                 I2S_CHANNEL_MONO);
        err += i2s_set_clk(I2S_NUM_0, 48000, I2S_BITS_PER_SAMPLE_16BIT,
                           I2S_CHANNEL_MONO);
        // i2s_set_clk(0)

        if (err != ESP_OK)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void TEST::mic_init()
    {
        // InitI2SMicroPhone();
        printf("mic init %s\n", InitI2SMicroPhone() ? "ok" : "failed");

        // __display_ptr = Disbuff;

        // xSemaphore = xSemaphoreCreateMutex();
        // start_dis  = xSemaphoreCreateMutex();
        // start_fft  = xSemaphoreCreateMutex();

        // xSemaphoreTake(start_dis, portMAX_DELAY);
        // xSemaphoreTake(start_fft, portMAX_DELAY);

        // xTaskCreate(Drawdisplay, "Drawdisplay", 1024 * 2, (void *)0, 4,
        //             &xhandle_display);
        // xTaskCreate(MicRecordfft, "MicRecordfft", 1024 * 2, (void *)0, 5,
        //             &xhandle_fft);
    }

    bool is_first_time = true;

    void TEST::mic_test()
    {
        printf("mic test\n");

        // /* Alloc buffer */
        // _new_i2s_readraw_buff = nullptr;
        // _new_i2s_readraw_buff = new int8_t[2048];

        // printf("%p\n", _new_i2s_readraw_buff);

        // if (_new_i2s_readraw_buff == nullptr) {
        //     printf("buffer readraw alloc failed\n");
        //     return;
        // }

        // // [241][128]
        // int a = 241;
        // int b = 128;
        // _new_fft_dis_buff = nullptr;
        // _new_fft_dis_buff = new uint8_t*[a];

        // if (_new_fft_dis_buff == nullptr) {
        //     printf("buffer fftdis alloc failed\n");
        //     return;
        // }

        // for (int i = 0; i < a; i++) {
        //     _new_fft_dis_buff[i] = new uint8_t[b];
        // }

        // /* memset */
        // if (is_first_time) {
        //     for (int i = 0; i < a; i++) {
        //         for (int j = 0; j < b; j++) {
        //             _new_fft_dis_buff[i][j] = 0;
        //         }
        //     }
        //     // is_first_time = false;
        // }

        /* malloc way */

        _new_i2s_readraw_buff = NULL;
        _new_i2s_readraw_buff = (int8_t *)malloc(2048 * sizeof(int8_t));
        if (_new_i2s_readraw_buff == NULL)
        {
            printf("buffer readraw alloc failed\n");
            return;
        }

        // [241][128]
        int a = 241;
        int b = 128;
        _new_fft_dis_buff = NULL;
        // _new_fft_dis_buff = new uint8_t*[a];
        _new_fft_dis_buff = (uint8_t **)malloc(a * sizeof(uint8_t *));

        if (_new_fft_dis_buff == NULL)
        {
            printf("buffer fftdis alloc failed\n");
            return;
        }

        for (int i = 0; i < a; i++)
        {
            _new_fft_dis_buff[i] = NULL;
            // _new_fft_dis_buff[i] = new uint8_t[b];
            _new_fft_dis_buff[i] = (uint8_t *)malloc(b * sizeof(uint8_t));
            if (_new_fft_dis_buff[i] == NULL)
            {
                printf("buffer fftdis:%d alloc failed\n", i);
                return;
            }
        }

        /* memset */
        if (is_first_time)
        {
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < b; j++)
                {
                    _new_fft_dis_buff[i][j] = 0;
                }
            }
            // is_first_time = false;
        }

        // printf("create mutex\n");
        // __display_ptr = Disbuff;
        // xSemaphore = xSemaphoreCreateMutex();
        // start_dis  = xSemaphoreCreateMutex();
        // start_fft  = xSemaphoreCreateMutex();
        // xSemaphoreTake(start_dis, portMAX_DELAY);
        // xSemaphoreTake(start_fft, portMAX_DELAY);

        // printf("create tasks\n");
        // xhandle_display = NULL;
        // xTaskCreate(Drawdisplay, "Drawdisplay", 1024 * 2, NULL, 10,
        //             &xhandle_display);

        // if (xhandle_display == NULL) {
        //     printf("task Drawdisplay create failed\n");
        //     return;
        // }

        // xhandle_fft = NULL;
        // xTaskCreate(MicRecordfft, "MicRecordfft", 1024 * 2, NULL, 11,
        //             &xhandle_fft);
        // if (xhandle_display == NULL) {
        //     printf("task MicRecordfft create failed\n");
        //     return;
        // }

        // display->fillScreen(TFT_BLACK);
        // DisplayMicro();

        // printf("delect tasks\n");
        // vTaskDelete(xhandle_display);
        // vTaskDelete(xhandle_fft);

        mic_test_one_task();

        // /* Release buffer */
        // delete [] _new_i2s_readraw_buff;

        // for (int i = 0; i < a; i++) {
        //     delete [] _new_fft_dis_buff[i];
        // }
        // delete [] _new_fft_dis_buff;

        /* free way */
        printf("free %p\n", _new_i2s_readraw_buff);
        free(_new_i2s_readraw_buff);

        for (int i = 0; i < a; i++)
        {
            // delete [] _new_fft_dis_buff[i];
            // printf("free %p\n", _new_fft_dis_buff[i]);
            free(_new_fft_dis_buff[i]);
        }
        // delete [] _new_fft_dis_buff;
        printf("free %p\n", _new_fft_dis_buff);
        free(_new_fft_dis_buff);

        printf("quit mic test\n");
    }

    void TEST::new_mic_test()
    {
        printf("mic test\n");

        display->fillScreen(TFT_BLACK);
        display->setFont(&fonts::Font0);
        display->setTextSize(2);
        display->setTextColor(TFT_WHITE);
        // display->setCursor(0, 6);

        // display->fillRect(0, 0, 240, 25, Disbuff->color565(20, 20, 20));
        // display->printf("       Mic test\n");

        /* Alloc buffer */
        printf("new mic_buffer[%d]\n", display->width());
        int16_t *mic_buffer = nullptr;
        mic_buffer = new int16_t[display->width()];
        if (mic_buffer == nullptr)
        {
            printf("failed\n");
            return;
        }

        int _color_list[] = {
            TFT_YELLOW,
            TFT_ORANGE,
            TFT_GREENYELLOW,
            TFT_PINK,
            TFT_BROWN,
            TFT_GOLD,
            TFT_SILVER,
        };
        int color_num = 0;

        size_t bytes_read = 0;
        while (1)
        {

            /* Read mic */
            i2s_read(I2S_NUM_0, (void *)mic_buffer, (sizeof(int16_t) * display->width()), &bytes_read, (100 / portTICK_RATE_MS));

            // printf("read %d\n", bytes_read);
            // delay(100);

            display->fillScreen(TFT_BLACK);

            for (int i = 0; i < 240; i++)
            {

                mic_buffer[i] = mic_buffer[i] / 20;

                // printf("m:%d\n", mic_buffer[i]);

                // display->fillCircle(i, mic_buffer[i], 1, TFT_YELLOW);

                display->drawPixel(i, mic_buffer[i], _color_list[color_num]);
                color_num++;
                if (color_num >= 7)
                    color_num = 0;
            }

            display->setCursor(0, 6);
            display->fillRect(0, 0, 240, 25, Disbuff->color565(20, 20, 20));
            display->printf("      Mic test\n");
            displayUpdate();

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }

        /* Free buffer */
        printf("free mic buffer\n");
        delete[] mic_buffer;

        printf("quit mic test\n");
    }

    void TEST::new_mic_test_fft()
    {
        printf("mic test\n");

        display->fillScreen(TFT_BLACK);
        display->setFont(&fonts::Font0);
        display->setTextSize(2);
        display->setTextColor(TFT_WHITE);

        // /* Alloc buffer */
        // printf("new mic_buffer[%d]\n", display->width());
        // int16_t* mic_buffer = nullptr;
        // mic_buffer = new int16_t[display->width()];
        // if (mic_buffer == nullptr) {
        //     printf("failed\n");
        //     return;
        // }

        arduinoFFT _FFT;

        /*
        These values can be changed in order to evaluate the functions
        */
        const uint16_t _samples = 256; // This value MUST ALWAYS be a power of 2
        const double _samplingFrequency = 48000;

        /*
        These are the input and output vectors
        Input vectors receive computed results from FFT
        */
        // static double vReal[samples];
        // static double vImag[samples];
        double *_vReal;
        double *_vReal_old;
        double *_vImag;
        int16_t *_rawData;

        /* Init */
        display->setRotation(2);
        /* Alloc buffer */
        _vReal = new double[_samples];
        _vReal_old = new double[_samples]();
        _vImag = new double[_samples];
        _rawData = new int16_t[_samples];

        int _color_list[] = {
            TFT_YELLOW,
            TFT_ORANGE,
            TFT_GREENYELLOW,
            TFT_PINK,
            TFT_BROWN,
            TFT_GOLD,
            TFT_SILVER,
        };
        int color_num = 0;

        size_t bytes_read = 0;
        while (1)
        {

            /* Read mic */
            // i2s_read(I2S_NUM_0, (void*)mic_buffer, (sizeof(int16_t) * display->width()), &bytes_read, (100 / portTICK_RATE_MS));
            // printf("read %d\n", bytes_read);
            // delay(100);

            i2s_read(I2S_NUM_0, (void *)_rawData, (sizeof(int16_t) * _samples), &bytes_read, (100 / portTICK_RATE_MS));
            // printf("read %d\n", bytes_read);

            display->fillScreen(TFT_BLACK);

            /* Copy data */
            for (int i = 0; i < _samples; i++)
            {
                _vReal[i] = (double)_rawData[i];
                _vImag[i] = 0.0;
            }

            /* FFT */
            _FFT = arduinoFFT(_vReal, _vImag, _samples, _samplingFrequency);
            _FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
            _FFT.Compute(FFT_FORWARD);
            _FFT.ComplexToMagnitude();
            // printf("fft done\n");

            /* Draw raw wave */
            color_num = 0;
            for (int i = 0; i < _samples; i++)
            {
                display->drawPixel(i, (_rawData[i] / 20), _color_list[color_num]);
                color_num++;
                if (color_num >= 7)
                    color_num = 0;
            }

            double value = 0.0;
            for (int i = 4; i < (_samples / 4); i += 2)
            {
                /* Draw falling bricks */
                value = (_vReal_old[i] + _vReal_old[i + 1]) / 2;
                display->fillRoundRect(8 * i - 28, 2 + value - 8, 8, 8, 2, _color_list[color_num]);

                /* Draw FFT */
                value = (_vReal[i] + _vReal[i + 1]) / 2;
                value = value / 10;
                if (value > 135)
                    value = 135;
                display->fillRoundRect(8 * i - 28, 2, 8, value, 2, _color_list[color_num]);

                color_num++;
                if (color_num >= 7)
                    color_num = 0;
            }

            // double value = 0.0;
            // for (int i = 8; i < (_samples / 2); i += 2) {
            //     /* Draw falling bricks */
            //     value = (_vReal_old[i] + _vReal_old[i+1]) / 2;
            //     display->fillRoundRect(2 * i - 15, value - 2, 2, 2, 1, _color_list[color_num]);

            //     /* Draw FFT */
            //     value = (_vReal[i] + _vReal[i+1]) / 2;
            //     value = value / 10;
            //     if (value > 135)
            //         value = 135;
            //     display->fillRoundRect(2 * i - 15, 0, 2, value, 1, _color_list[color_num]);

            //     color_num++;
            //     if (color_num >= 7)
            //         color_num = 0;
            // }

            /* Update old buffer (falling bricks) */
            for (int i = 0; i < _samples; i++)
            {
                /* Fall */
                _vReal_old[i] -= 4;

                /* If higher hit */
                value = _vReal[i] / 10;
                if (value > 135)
                    value = 135;
                if (value > _vReal_old[i])
                    _vReal_old[i] = value;
            }

            // display->setCursor(0, 6);
            // display->fillRect(0, 0, 240, 25, Disbuff->color565(20, 20, 20));
            // display->printf("      Mic test\n");

            displayUpdate();

            checkReboot();
            if (checkNext())
            {
                break;
            }
        }

        /* Free buffer */
        printf("free mic buffer\n");
        // delete [] mic_buffer;
        delete[] _rawData;
        delete[] _vReal;
        delete[] _vImag;
        delete[] _vReal_old;

        display->setRotation(0);

        printf("quit mic test\n");
    }

}
