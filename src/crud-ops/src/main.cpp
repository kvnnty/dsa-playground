#include "CSVDatabase.h"
#include "UserInterface.h"
#include <iostream>

int main() {
    try {
        CSVDatabase db("users_database.csv");
        UserInterface ui(db);
        ui.run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
