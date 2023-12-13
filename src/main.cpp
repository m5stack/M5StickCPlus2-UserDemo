/**
 * @file main.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>
#include "test/test.h"

TEST::TEST test;

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}
