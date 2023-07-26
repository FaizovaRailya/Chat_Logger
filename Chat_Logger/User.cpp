#include <iostream>
#include "User.h"


User::User(const std::string& name, const std::string& login, uint* password) :
    name_(name), login_(login), password_(password) {}

const std::string& User::getName() const { return name_; }
const std::string& User::getLogin() const { return login_; }
uint* User::getPassword() const { return password_; }

