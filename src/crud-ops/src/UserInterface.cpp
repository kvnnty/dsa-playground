#include "UserInterface.h"
#include <iostream>
#include <limits>

UserInterface::UserInterface(CSVDatabase &database) : db(database) {}

void UserInterface::displayMenu()
{
  std::cout << "\n"
            << std::string(40, '=') << std::endl;
  std::cout << "         USER MANAGEMENT SYSTEM" << std::endl;
  std::cout << std::string(40, '=') << std::endl;
  std::cout << "1. Create User" << std::endl;
  std::cout << "2. Read All Users" << std::endl;
  std::cout << "3. Find User by ID" << std::endl;
  std::cout << "4. Update User" << std::endl;
  std::cout << "5. Delete User" << std::endl;
  std::cout << "6. Search Users by Name" << std::endl;
  std::cout << "7. Show User Count" << std::endl;
  std::cout << "8. Exit" << std::endl;
  std::cout << std::string(40, '-') << std::endl;
}

int UserInterface::getIntInput(const std::string &prompt)
{
  int value;
  while (true)
  {
    std::cout << prompt;
    if (std::cin >> value)
    {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    }
    else
    {
      std::cout << "Invalid input. Please enter a valid number." << std::endl;
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

std::string UserInterface::getStringInput(const std::string &prompt)
{
  std::string input;
  std::cout << prompt;
  std::getline(std::cin, input);
  return input;
}

void UserInterface::run()
{
  std::cout << "Welcome to the User Management System!" << std::endl;
  std::cout << "Database file: users_database.csv" << std::endl;

  while (true)
  {
    displayMenu();
    int choice = getIntInput("Enter your choice (1-8): ");

    switch (choice)
    {
    case 1:
    {
      int id = getIntInput("Enter User ID: ");
      std::string name = getStringInput("Enter User Name: ");
      int age = getIntInput("Enter User Age: ");
      db.createUser(id, name, age);
      break;
    }
    case 2:
      db.readAllUsers();
      break;
    case 3:
    {
      int id = getIntInput("Enter User ID to search: ");
      User user;
      if (db.findUserById(id, user))
      {
        std::cout << "\nUser Found:" << std::endl;
        std::cout << "ID: " << user.id << std::endl;
        std::cout << "Name: " << user.name << std::endl;
        std::cout << "Age: " << user.age << std::endl;
      }
      else
      {
        std::cout << "User with ID " << id << " not found." << std::endl;
      }
      break;
    }
    case 4:
    {
      int id = getIntInput("Enter User ID to update: ");
      if (db.userExists(id))
      {
        std::string newName = getStringInput("Enter new name: ");
        int newAge = getIntInput("Enter new age: ");
        db.updateUser(id, newName, newAge);
      }
      else
      {
        std::cout << "User with ID " << id << " not found." << std::endl;
      }
      break;
    }
    case 5:
    {
      int id = getIntInput("Enter User ID to delete: ");
      if (db.userExists(id))
      {
        std::cout << "Are you sure you want to delete user with ID " << id << "? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        if (confirm == 'y' || confirm == 'Y')
        {
          db.deleteUser(id);
        }
        else
        {
          std::cout << "Delete operation cancelled." << std::endl;
        }
      }
      else
      {
        std::cout << "User with ID " << id << " not found." << std::endl;
      }
      break;
    }
    case 6:
    {
      std::string searchTerm = getStringInput("Enter name to search: ");
      if (!searchTerm.empty())
      {
        db.searchUsersByName(searchTerm);
      }
      break;
    }
    case 7:
      std::cout << "Total users in database: " << db.getUserCount() << std::endl;
      break;
    case 8:
      std::cout << "Thank you for using the User Management System!" << std::endl;
      return;
    default:
      std::cout << "Invalid choice! Please enter a number between 1 and 8." << std::endl;
    }

    std::cout << "\nPress Enter to continue...";
    std::cin.get();
  }
}