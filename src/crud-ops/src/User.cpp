#include "User.h"

User::User() : id(0), age(0) {}

User::User(int _id, const std::string &_name, int _age)
    : id(_id), name(_name), age(_age) {}