#include "PasswordDB.h"

#include <stdio.h>

PasswordDB::PasswordDB() {
  if (std::experimental::filesystem::exists("password.txt")) {
    std::ifstream pwFile("password.txt");
    if (pwFile.good()) {
      std::string storedPasswd;
      std::getline(pwFile, storedPasswd);
      password = storedPasswd;
    }

    else {
      CreateDefaultPassword();
    }
    pwFile.close();
  }
  else {
    CreateDefaultPassword();
  }
}

bool PasswordDB::SetPassword(std::string newPass, std::string oldPass) {
  std::ofstream pwFile;
  pwFile.open("password.txt", std::ios::out | std::ios::trunc);
  if (pwFile.good() && pwFile.is_open()) {
    pwFile <<  newPass << std::endl;
    pwFile.close();
    return true;
  }
  else {
    return false;
  }
}

std::string PasswordDB::GetPassword() {
  return password;
}

void PasswordDB::CreateDefaultPassword() {
  password = "admin";
  std::ofstream pwFile("password.txt", std::ios::out | std::ios::trunc);
  if (pwFile.good()) {
    pwFile << "admin" << std::endl;
    pwFile.close();
  }
}
