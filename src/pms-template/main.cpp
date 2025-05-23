#include <iostream>
#include "include/utils.h"
#include "include/parking_network.h"

int main()
{
    ParkingNetwork pn;
    while (true)
    {
        std::cout << "\n=== Parking Management System ===\n"
             << "1. Add Parking Lot\n"
             << "2. Update Parking Lot\n"
             << "3. Manage Parking Lot\n"
             << "4. Connect Parking Lots\n"
             << "5. List Parking Lots\n"
             << "6. Display Network\n"
             << "7. Delete Parking Lot\n"
             << "8. Exit\n";
        int choice = readInt("Choose: ", 1, 8);
        if (choice == 8)
            break;
        switch (choice)
        {
            case 1:
                pn.addParkingLot();
                break;
            case 2:
                pn.updateParkingLot();
                break;
            case 3:
                pn.manageParkingLot();
                break;
            case 4:
                pn.connectParkingLots();
                break;
            case 5:
                pn.listParkingLots();
                break;
            case 6:
                pn.displayNetwork();
                break;
            case 7:
                pn.deleteParkingLot();
                break;
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}