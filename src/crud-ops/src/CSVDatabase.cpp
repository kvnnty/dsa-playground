#include "CSVDatabase.h"
#include <fstream>
#include <iostream>
#include <algorithm>

CSVDatabase::CSVDatabase(const std::string &dbFile) : filename(dbFile)
{
  initializeFile();
}

// Implement trim
std::string CSVDatabase::trim(const std::string &str)
{
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

// Implement stringToInt
bool CSVDatabase::stringToInt(const std::string &str, int &result)
{
  try
  {
    std::string trimmed = trim(str);
    if (trimmed.empty())
      return false;
    size_t pos;
    result = std::stoi(trimmed, &pos);
    return pos == trimmed.length();
  }
  catch (const std::exception &)
  {
    return false;
  }
}

// Implement parseCSVLine
std::vector<std::string> CSVDatabase::parseCSVLine(const std::string &line)
{
  std::vector<std::string> fields;
  std::string field;
  bool inQuotes = false;

  for (size_t i = 0; i < line.length(); ++i)
  {
    char c = line[i];
    if (c == '"')
    {
      inQuotes = !inQuotes;
    }
    else if (c == ',' && !inQuotes)
    {
      fields.push_back(trim(field));
      field.clear();
    }
    else
    {
      field += c;
    }
  }
  fields.push_back(trim(field));
  return fields;
}

// Implement escapeCSVField
std::string CSVDatabase::escapeCSVField(const std::string &field)
{
  if (field.find(',') != std::string::npos || field.find('"') != std::string::npos)
  {
    std::string escaped = "\"";
    for (char c : field)
    {
      if (c == '"')
        escaped += "\"\"";
      else
        escaped += c;
    }
    escaped += "\"";
    return escaped;
  }
  return field;
}

// Implement initializeFile
void CSVDatabase::initializeFile()
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
      outFile << HEADER << "\n";
      outFile.close();
      std::cout << "Database file created: " << filename << std::endl;
    }
    else
    {
      throw std::runtime_error("Cannot create database file: " + filename);
    }
  }
  inFile.close();
}

// Implement skipHeader
bool CSVDatabase::skipHeader(std::ifstream &file)
{
  std::string line;
  if (std::getline(file, line))
  {
    return trim(line) == HEADER;
  }
  return false;
}

// Implement validation functions
bool CSVDatabase::isValidId(int id)
{
  return id > 0;
}

bool CSVDatabase::isValidName(const std::string &name)
{
  return !trim(name).empty() && name.length() <= 100;
}

bool CSVDatabase::isValidAge(int age)
{
  return age >= 0 && age <= 150;
}

// Implement createUser
bool CSVDatabase::createUser(int id, const std::string &name, int age)
{
  if (!isValidId(id))
  {
    std::cerr << "Error: Invalid ID. Must be positive integer." << std::endl;
    return false;
  }
  if (!isValidName(name))
  {
    std::cerr << "Error: Invalid name. Must be non-empty and max 100 characters." << std::endl;
    return false;
  }
  if (!isValidAge(age))
  {
    std::cerr << "Error: Invalid age. Must be between 0 and 150." << std::endl;
    return false;
  }
  if (userExists(id))
  {
    std::cerr << "Error: User with ID " << id << " already exists." << std::endl;
    return false;
  }

  std::ofstream outFile(filename, std::ios::app);
  if (!outFile.is_open())
  {
    std::cerr << "Error: Cannot open file for writing." << std::endl;
    return false;
  }

  outFile << id << "," << escapeCSVField(name) << "," << age << "\n";
  outFile.close();
  std::cout << "User added successfully!" << std::endl;
  return true;
}

// Implement readAllUsers
void CSVDatabase::readAllUsers()
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::cerr << "Error: Cannot open file for reading." << std::endl;
    return;
  }

  skipHeader(inFile);
  std::string line;
  bool hasRecords = false;

  std::cout << "\n"
            << std::string(50, '=') << std::endl;
  std::cout << "ALL USERS" << std::endl;
  std::cout << std::string(50, '=') << std::endl;
  std::cout << "ID\tName\t\t\tAge" << std::endl;
  std::cout << std::string(50, '-') << std::endl;

  while (std::getline(inFile, line))
  {
    if (trim(line).empty())
      continue;
    std::vector<std::string> fields = parseCSVLine(line);
    if (fields.size() >= 3)
    {
      hasRecords = true;
      std::cout << fields[0] << "\t"
                << fields[1] << std::string(std::max(1, 20 - (int)fields[1].length()), ' ')
                << fields[2] << std::endl;
    }
  }

  if (!hasRecords)
  {
    std::cout << "No records found." << std::endl;
  }
  std::cout << std::string(50, '=') << std::endl;
  inFile.close();
}

// Implement findUserById
bool CSVDatabase::findUserById(int id, User &user)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::cerr << "Error: Cannot open file for reading." << std::endl;
    return false;
  }

  skipHeader(inFile);
  std::string line;
  while (std::getline(inFile, line))
  {
    if (trim(line).empty())
      continue;
    std::vector<std::string> fields = parseCSVLine(line);
    if (fields.size() >= 3)
    {
      int fileId, fileAge;
      if (stringToInt(fields[0], fileId) && stringToInt(fields[2], fileAge))
      {
        if (fileId == id)
        {
          user.id = fileId;
          user.name = fields[1];
          user.age = fileAge;
          inFile.close();
          return true;
        }
      }
    }
  }
  inFile.close();
  return false;
}

// Implement userExists
bool CSVDatabase::userExists(int id)
{
  User dummy;
  return findUserById(id, dummy);
}

// Implement updateUser
bool CSVDatabase::updateUser(int id, const std::string &newName, int newAge)
{
  if (!isValidName(newName))
  {
    std::cerr << "Error: Invalid name." << std::endl;
    return false;
  }
  if (!isValidAge(newAge))
  {
    std::cerr << "Error: Invalid age." << std::endl;
    return false;
  }

  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::cerr << "Error: Cannot open file for reading." << std::endl;
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  bool foundHeader = skipHeader(inFile);
  bool userFound = false;

  if (foundHeader)
  {
    lines.push_back(HEADER);
  }

  while (std::getline(inFile, line))
  {
    if (trim(line).empty())
      continue;
    std::vector<std::string> fields = parseCSVLine(line);
    if (fields.size() >= 3)
    {
      int fileId;
      if (stringToInt(fields[0], fileId) && fileId == id)
      {
        lines.push_back(std::to_string(id) + "," + escapeCSVField(newName) + "," + std::to_string(newAge));
        userFound = true;
      }
      else
      {
        lines.push_back(line);
      }
    }
    else
    {
      lines.push_back(line);
    }
  }
  inFile.close();

  if (!userFound)
  {
    std::cerr << "Error: User with ID " << id << " not found." << std::endl;
    return false;
  }

  std::ofstream outFile(filename, std::ios::trunc);
  if (!outFile.is_open())
  {
    std::cerr << "Error: Cannot open file for writing." << std::endl;
    return false;
  }

  for (const auto &l : lines)
  {
    outFile << l << "\n";
  }
  outFile.close();
  std::cout << "User updated successfully!" << std::endl;
  return true;
}

// Implement deleteUser
bool CSVDatabase::deleteUser(int id)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::cerr << "Error: Cannot open file for reading." << std::endl;
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  bool foundHeader = skipHeader(inFile);
  bool userFound = false;

  if (foundHeader)
  {
    lines.push_back(HEADER);
  }

  while (std::getline(inFile, line))
  {
    if (trim(line).empty())
      continue;
    std::vector<std::string> fields = parseCSVLine(line);
    if (fields.size() >= 3)
    {
      int fileId;
      if (stringToInt(fields[0], fileId) && fileId == id)
      {
        userFound = true;
      }
      else
      {
        lines.push_back(line);
      }
    }
    else
    {
      lines.push_back(line);
    }
  }
  inFile.close();

  if (!userFound)
  {
    std::cerr << "Error: User with ID " << id << " not found." << std::endl;
    return false;
  }

  std::ofstream outFile(filename, std::ios::trunc);
  if (!outFile.is_open())
  {
    std::cerr << "Error: Cannot open file for writing." << std::endl;
    return false;
  }

  for (const auto &l : lines)
  {
    outFile << l << "\n";
  }
  outFile.close();
  std::cout << "User deleted successfully!" << std::endl;
  return true;
}

// Implement searchUsersByName
void CSVDatabase::searchUsersByName(const std::string &searchTerm)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::cerr << "Error: Cannot open file for reading." << std::endl;
    return;
  }

  skipHeader(inFile);
  std::string line;
  bool found = false;
  std::string lowerSearchTerm = searchTerm;
  std::transform(lowerSearchTerm.begin(), lowerSearchTerm.end(),
                 lowerSearchTerm.begin(), ::tolower);

  std::cout << "\nSearch results for: \"" << searchTerm << "\"" << std::endl;
  std::cout << std::string(50, '-') << std::endl;

  while (std::getline(inFile, line))
  {
    if (trim(line).empty())
      continue;
    std::vector<std::string> fields = parseCSVLine(line);
    if (fields.size() >= 3)
    {
      std::string lowerName = fields[1];
      std::transform(lowerName.begin(), lowerName.end(),
                     lowerName.begin(), ::tolower);
      if (lowerName.find(lowerSearchTerm) != std::string::npos)
      {
        found = true;
        std::cout << "ID: " << fields[0]
                  << ", Name: " << fields[1]
                  << ", Age: " << fields[2] << std::endl;
      }
    }
  }

  if (!found)
  {
    std::cout << "No users found matching: \"" << searchTerm << "\"" << std::endl;
  }
  inFile.close();
}

// Implement getUserCount
int CSVDatabase::getUserCount()
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
    return 0;

  skipHeader(inFile);
  int count = 0;
  std::string line;
  while (std::getline(inFile, line))
  {
    if (!trim(line).empty())
      count++;
  }
  inFile.close();
  return count;
}