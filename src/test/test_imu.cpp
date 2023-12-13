/**
 * @file test_imu.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "test.h"

typedef struct
{
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct
{
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct
{
    double x;
    double y;
} point_2d_t;

double r_rand = PI / 180;

double r_alpha = 19.47 * PI / 180;
double r_gamma = 20.7 * PI / 180;

double sin_alpha = sin(19.47 * PI / 180);
double cos_alpha = cos(19.47 * PI / 180);
double sin_gamma = sin(20.7 * PI / 180);
double cos_gamma = cos(20.7 * PI / 180);

line_3d_t rect[12] = {
    {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
    {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
    {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
    {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
    {
        .start_point = {-1, -1, 1},
        .end_point = {-1, -1, -1},
    },
    {
        .start_point = {1, -1, 1},
        .end_point = {1, -1, -1},
    },
    {
        .start_point = {1, 1, 1},
        .end_point = {1, 1, -1},
    },
    {
        .start_point = {-1, 1, 1},
        .end_point = {-1, 1, -1},
    },
    {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
    {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
    {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
    {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
};

bool point3Dto2D(point_3d_t *source, point_2d_t *point)
{
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) -
               (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
}

bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y)
{
    *x = point->x + 120;
    *y = 67 - point->y;
    return true;
}

bool printLine3D(LGFX_Sprite *display, line_3d_t *line, uint32_t color)
{
    uint8_t start_x, start_y, end_x, end_y;
    point_2d_t point;
    point3Dto2D(&line->start_point, &point);
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point2DToDisPoint(&point, &end_x, &end_y);

    display->drawLine(start_x, start_y, end_x, end_y, (int)color);

    return true;
}

void RotatePoint(point_3d_t *point, double x, double y, double z)
{
    if (x != 0)
    {
        point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

void RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y,
                 double z)
{
    if (x != 0)
    {
        point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

namespace TEST
{

    void TEST::imu_init()
    {
        imu.Init();
    }

    void TEST::imu_test()
    {
        printf("imu test\n");

        float accX = 0;
        float accY = 0;
        float accZ = 0;

        double theta = 0, last_theta = 0;
        double phi = 0, last_phi = 0;
        double alpha = 0.2;

        line_3d_t x = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
        line_3d_t y = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
        line_3d_t z = {.start_point = {0, 0, 0}, .end_point = {0, 0, 30}};

        line_3d_t rect_source[12];
        line_3d_t rect_dis;
        for (int n = 0; n < 12; n++)
        {
            rect_source[n].start_point.x = rect[n].start_point.x * 30;
            rect_source[n].start_point.y = rect[n].start_point.y * 30;
            rect_source[n].start_point.z = rect[n].start_point.z * 30;
            rect_source[n].end_point.x = rect[n].end_point.x * 30;
            rect_source[n].end_point.y = rect[n].end_point.y * 30;
            rect_source[n].end_point.z = rect[n].end_point.z * 30;
        }

        while (1)
        {
            imu.getAccelData(&accX, &accY, &accZ);
            // printf("%f \t %f \t %f\n", accX, accY, accZ);

            // M5.MPU6886.getAccelData(&accX, &accY, &accZ);
            if ((accX < 1) && (accX > -1))
            {
                theta = asin(-accX) * 57.295;
            }
            if (accZ != 0)
            {
                phi = atan(accY / accZ) * 57.295;
            }

            theta = alpha * theta + (1 - alpha) * last_theta;
            phi = alpha * phi + (1 - alpha) * last_phi;

            // Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
            // Disbuff->setTextSize(1);
            // Disbuff->setCursor(10, 115);
            // Disbuff->printf("theta: %.2f", theta);
            // Disbuff->setCursor(10, 125);
            // Disbuff->printf("phi: %.2f", phi);

            display->fillScreen(TFT_BLACK);
            display->setTextColor(TFT_WHITE, TFT_BLACK);

            if (is_test_mode)
            {
                display->setFont(&fonts::efontCN_24);
                display->setTextSize(1);
                display->setCursor(0, 10);
                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf(" % 0.1f\n", accX);
                display->printf(" % 0.1f\n", accY);
                display->printf(" % 0.1f\n   ^\n 确保数值稳定\n", accZ);
            }
            else
            {
                display->setFont(&fonts::Font0);
                Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
                Disbuff->setTextSize(1);
                Disbuff->setCursor(10, 115);
                Disbuff->printf("%.2f", theta);
                Disbuff->setCursor(10, 125);
                Disbuff->printf("%.2f", phi);
            }

            z.end_point.x = 0;
            z.end_point.y = 0;
            z.end_point.z = 60;
            RotatePoint(&z.end_point, theta, phi, 0);
            RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
            RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

            for (int n = 0; n < 12; n++)
            {
                RotatePoint(&rect_source[n].start_point, &rect_dis.start_point,
                            theta, phi, (double)0);
                RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta,
                            phi, (double)0);
                printLine3D(Disbuff, &rect_dis, TFT_WHITE);
            }
            // Disbuff.fillRect(0,0,160,80,BLACK);
            printLine3D(Disbuff, &x, TFT_RED);
            printLine3D(Disbuff, &y, TFT_GREEN);
            printLine3D(Disbuff, &z, TFT_BLUE);
            /*
            Disbuff.setTextColor(TFT_WHITE);
            Disbuff.setTextSize(1);
            Disbuff.fillRect(0,0,52,18,Disbuff.color565(20,20,20));
            Disbuff.drawString("MPU6886",5,5,1);
            */
            Displaybuff();
            last_theta = theta;
            last_phi = phi;

            delay(20);

            checkReboot();
            if (checkNext())
            {
                printf("quit imu test\n");
                break;
            }
        }
    }

}
