#include "../include/utils.h"

int readInt(const std::string &prompt, int minVal, int maxVal)
{
    while (true)
    {
        std::cout << prompt;
        std::string line;
        if (!getline(std::cin, line))
        {
            std::cin.clear();
            continue;
        }
        try
        {
            int val = std::stoi(line);
            if (val < minVal || val > maxVal)
            {
                std::cout << "Please enter a number";
                if (minVal != INT_MIN && maxVal != INT_MAX)
                    std::cout << " between " << minVal << " and " << maxVal;
                std::cout << ".\n";
                continue;
            }
            return val;
        }
        catch (...)
        {
            std::cout << "Invalid input. Please enter a valid number.\n";
        }
    }
}
