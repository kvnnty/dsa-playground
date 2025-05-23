#pragma once
#include <iostream>
#include <string>
#include <climits>

// Safe Integer Input
int readInt(const std::string &prompt, int minVal = INT_MIN, int maxVal = INT_MAX);
