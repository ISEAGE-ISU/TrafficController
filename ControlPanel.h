#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <stdio.h>
#include <menu.h>
#include <iostream>
#include <ncurses.h>
#include <string.h>

class ControlPanel {
private:
  ITEM **items;
  MENU *menu;
  int max_row, max_col;
  WINDOW *main_win, *status_bar;
  char *menu_choices[] = {
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

public:
  void InputLoop();
  ControlPanel();
  ~ControlPanel();
};

#endif
