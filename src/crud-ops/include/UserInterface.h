#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "CSVDatabase.h"
#include <string>

class UserInterface
{
private:
  CSVDatabase &db;

  void displayMenu();
  int getIntInput(const std::string &prompt);
  std::string getStringInput(const std::string &prompt);

public:
  UserInterface(CSVDatabase &database);
  void run();
};

#endif