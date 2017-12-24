#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <stdio.h>
#include <menu.h>
#include <iostream>
#include <ncurses.h>
#include <string.h>

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
  const char *menu_choices[8] = {
                          "Reboot Device",
                          "Upload Traffic Light Programming",
                          "Set Light Status",
                          "Get Light Status",
                          "Choice 5",
                          "Update Firmware",
                          "View Device Info",
                          "Exit                                   ",
                    };
  void PrintStatus(std::string msg);
  void Shutdown();

public:
  void InputLoop();
  ControlPanel();
  ~ControlPanel();
};

#endif
