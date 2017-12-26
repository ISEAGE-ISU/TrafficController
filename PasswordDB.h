#ifndef PASSWORD_DB_H
#define PASSWORD_DB_H

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

class PasswordDB {
private:
  std::string password;
  void CreateDefaultPassword();

public:
  PasswordDB();

  std::string GetPassword();
  bool SetPassword(std::string newPass, std::string oldPass);
};

#endif
