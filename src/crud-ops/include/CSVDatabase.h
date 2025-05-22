#ifndef CSV_DATABASE_H
#define CSV_DATABASE_H

#include "User.h"
#include <string>
#include <vector>

class CSVDatabase
{
private:
  std::string filename;
  const std::string HEADER = "ID,Name,Age";

  std::string trim(const std::string &str);
  bool stringToInt(const std::string &str, int &result);
  std::vector<std::string> parseCSVLine(const std::string &line);
  std::string escapeCSVField(const std::string &field);
  void initializeFile();
  bool skipHeader(std::ifstream &file);

public:
  CSVDatabase(const std::string &dbFile);

  bool isValidId(int id);
  bool isValidName(const std::string &name);
  bool isValidAge(int age);

  bool createUser(int id, const std::string &name, int age);
  void readAllUsers();
  bool findUserById(int id, User &user);
  bool userExists(int id);
  bool updateUser(int id, const std::string &newName, int newAge);
  bool deleteUser(int id);
  void searchUsersByName(const std::string &searchTerm);
  int getUserCount();
};

#endif