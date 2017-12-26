#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <stdio.h>
#include <menu.h>
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <map>
#include <functional>

#include "PasswordDB.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MENU_WIDTH 50
#define MENU_HEIGHT 18
#define FW_VER "1.0"

class ControlPanel {
private:
  ITEM **items;
  MENU *menu;
  int max_row, max_col;
  WINDOW *main_win, *status_bar;
  bool loggedIn;
  PasswordDB *pwDb;

  const char *menu_choices[8] = {
                          "Reboot Device",
                          "Upload Traffic Light Programming",
                          "Set Light Status",
                          "Get Light Status",
                          "Change Admin Password",
                          "Update Firmware",
                          "View Device Info",
                          "Exit",
                    };
  std::map<std::string, std::function<void()>> menuHandler;


  void PrintStatus(std::string msg);
  std::string GetInput(std::string prompt);
  void Shutdown();

  void MenuRebootDevice();
  void MenuUploadTrafficProgramming();
  void MenuGetLightStatus();
  void MenuSetLightStatus();
  void MenuUpdateFirmware();
  void MenuViewDeviceInfo();
  void MenuExit();
  void MenuChangeAdminPassword();

  void LoginPrompt();

public:
  void InputLoop();
  ControlPanel();
  ~ControlPanel();
};

#endif
