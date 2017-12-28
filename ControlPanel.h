#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <stdio.h>
#include <menu.h>
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <map>
#include <functional>
#include <unistd.h>
#include <sstream>
#include <sys/reboot.h>
#include <linux/reboot.h>

#include "PasswordDB.h"
#include "FileOps.h"
#include "HTTPServer.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MENU_WIDTH 50
#define MENU_HEIGHT 18
#define FW_VER "1.0"
#define EXE_PATH "/opt/firmware.bin"

namespace CDC {
  class ControlPanel {
  private:
    ITEM **items;
    MENU *menu;
    int max_row, max_col;
    WINDOW *main_win, *status_bar;
    bool loggedIn;
    PasswordDB *pwDb;
    std::string flag;

    const char *menu_choices[8] = {
                            "Reboot Device",
                            "Upload Traffic Light Programming",
                            "View Programming",
                            "Change Admin Password",
                            "Update Firmware",
                            "Get/Set Flag",
                            "View Device Info",
                            "Exit",
                      };
    std::map<std::string, std::function<void()>> menuHandler;

    template <typename T>
    void PrintStatus(const T& msg);
    std::string GetInput(const std::string &prompt);
    void Shutdown();

    void MenuRebootDevice();
    void MenuUploadTrafficProgramming();
    void MenuViewProgramming();
    void MenuUpdateFirmware();
    void MenuViewDeviceInfo();
    void MenuExit();
    void MenuChangeAdminPassword();
    void MenuGetSetFlag();

    void LoginPrompt();

  public:
    void InputLoop();
    ControlPanel();
    ~ControlPanel();
  };
}
#endif
