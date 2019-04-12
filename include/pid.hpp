#pragma once

#include <iostream>
#include "base_values.hpp"
#include "BrickPi3.h"
#include <unistd.h>

int calc_degrees(int abs);
int PIDcontrol(sensor_gyro_t & Gyro4);