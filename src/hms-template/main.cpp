#include <iostream>
#include "include/graph.h"
#include "include/utility.h"

int main()
{
    Graph graph;
    while (true)
    {
        std::cout << "\n=== Multi-Hospital Management ===\n"
             << "1. Add Hospital\n"
             << "2. Delete Hospital\n"
             << "3. Connect Hospitals\n"
             << "4. Update Hospital Info\n"
             << "5. List Hospitals\n"
             << "6. View Network\n"
             << "7. Manage Hospital\n"
             << "8. Exit\n";
        int choice = readInt("Choose: ", 1, 8);
        if (choice == 8)
            break;
        switch (choice)
        {
            case 1:
                graph.addHospital();
                break;
            case 2:
                graph.deleteHospital();
                break;
            case 3:
                graph.connectHospitals();
                break;
            case 4:
                graph.updateHospitalInfo();
                break;
            case 5:
                graph.listHospitals();
                break;
            case 6:
                graph.displayNetwork();
                break;
            case 7:
                graph.manageHospital();
                break;
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}
