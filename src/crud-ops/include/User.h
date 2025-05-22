#ifndef USER_H
#define USER_H

#include <string>

struct User
{
  int id;
  std::string name;
  int age;

  User();
  User(int _id, const std::string &_name, int _age);
};

#endif