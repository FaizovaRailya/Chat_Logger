#pragma once
#include <string>
#include <fstream>
#include "Sha1.h"


class User {
	std::string name_;
	std::string login_;
	uint* password_;

public:
	User() = default;
	User(const std::string& name, const std::string& login, uint* password);
	~User() = default;
	const std::string& getName() const;
	const std::string& getLogin() const;
	uint* getPassword() const;
};