#pragma once

#include <signal.h>     // for catching exit signals
#include <string>
#include <iostream>
#include "gyro_mov.hpp"
#include "base_values.hpp"
#include "BrickPi3.h"
#include <cstdio>
#include <iostream>

void exit_signal_handler(int signo);
bool voltageIsSafe();