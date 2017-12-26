#include "ControlPanel.h"

CDC::ControlPanel::ControlPanel() {
  pwDb = new PasswordDB();
  loggedIn = false;

  initscr();
  start_color();
  cbreak();
  curs_set(0);
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
  getmaxyx(stdscr, max_row, max_col);

  int menu_len = ARRAY_SIZE(menu_choices);
  items = (ITEM**)calloc(menu_len + 2, sizeof(ITEM *));

  for(int i = 0; i < menu_len; ++i)
        (items)[i] = new_item(menu_choices[i], "");
  (items)[menu_len + 1] = (ITEM *)NULL;

  menu = new_menu(items);

  set_menu_fore(menu, COLOR_PAIR(1) | A_REVERSE);
  set_menu_back(menu, COLOR_PAIR(2));
  set_menu_grey(menu, COLOR_PAIR(3));


  status_bar = subwin(stdscr, 4, max_col, max_row - 4, 0);
  box(status_bar, 0, 0);

  main_win = subwin(stdscr, MENU_HEIGHT, MENU_WIDTH, ((max_row - MENU_HEIGHT)/2) - 2, (max_col - MENU_WIDTH)/ 2);

  keypad(main_win, TRUE);
  set_menu_win(menu, main_win);
  set_menu_sub(menu, derwin(main_win, MENU_HEIGHT - 5, MENU_WIDTH -6, 5, 6));
  set_menu_mark(menu, " > ");
  box(main_win, 0, 0);

  wattron(status_bar, COLOR_PAIR(1));
  mvwprintw(status_bar, 0, 2, "Status:");
  wattroff(status_bar, COLOR_PAIR(1));

  wattron(main_win, COLOR_PAIR(1));
  mvwprintw(main_win, 1, (MENU_WIDTH - strlen("City of Ames Traffic"))/2, "%s", "City of Ames Traffic");
  wattroff(main_win, COLOR_PAIR(1));

  mvwaddch(main_win, 2, 0, ACS_LTEE);
  mvwhline(main_win, 2, 1, ACS_HLINE, MENU_WIDTH - 1);
  mvwaddch(main_win, 2, MENU_WIDTH - 1, ACS_RTEE);

  refresh();
  post_menu(menu);
  doupdate();
  wrefresh(main_win);
  wrefresh(status_bar);

  //Setup Menu handlers
  menuHandler.insert(std::make_pair("Reboot Device", std::bind(&CDC::ControlPanel::MenuRebootDevice, this)));
  menuHandler.insert(std::make_pair("Upload Traffic Light Programming", std::bind(&CDC::ControlPanel::MenuUploadTrafficProgramming, this)));
  menuHandler.insert(std::make_pair("Set Light Status", std::bind(&CDC::ControlPanel::MenuSetLightStatus, this)));
  menuHandler.insert(std::make_pair("Get Light Status", std::bind(&CDC::ControlPanel::MenuGetLightStatus, this)));
  menuHandler.insert(std::make_pair("Update Firmware", std::bind(&CDC::ControlPanel::MenuUpdateFirmware, this)));
  menuHandler.insert(std::make_pair("View Device Info", std::bind(&CDC::ControlPanel::MenuViewDeviceInfo, this)));
  menuHandler.insert(std::make_pair("Exit", std::bind(&CDC::ControlPanel::MenuExit, this)));
  menuHandler.insert(std::make_pair("Change Admin Password", std::bind(&CDC::ControlPanel::MenuChangeAdminPassword, this)));
}

CDC::ControlPanel::~ControlPanel() {
  delete pwDb;
}

template <typename T>
void CDC::ControlPanel::PrintStatus(const T& msg) {
  wclear(status_bar);
  box(status_bar, 0, 0);
  wattron(status_bar, COLOR_PAIR(1));
  mvwprintw(status_bar, 0, 2, "Status:");
  wattroff(status_bar, COLOR_PAIR(1));

  std::stringstream printable;
  printable << msg;

  mvwprintw(status_bar, 1, 3, "%s", printable.str().c_str());
  wrefresh(status_bar);
}

void CDC::ControlPanel::InputLoop() {
  int c;
  while((c = wgetch(main_win)) != KEY_F(1)) {
      switch(c) {
            case 'q':
              Shutdown();
              break;
            case KEY_DOWN:
              menu_driver(menu, REQ_DOWN_ITEM);
              PrintStatus("");
              break;
            case KEY_UP:
              menu_driver(menu, REQ_UP_ITEM);
              PrintStatus("");
              break;
            case 10: /* Enter */
              try {
                menuHandler[item_name(current_item(menu))](); //Invoke selected item handler
              }
              catch (...) {}
              wrefresh(status_bar);
              wrefresh(main_win);
              break;
    }
    refresh();
    wrefresh(main_win);
  }
}

void CDC::ControlPanel::Shutdown() {
  unpost_menu(menu);
  for(int i = 0; i < ARRAY_SIZE(menu_choices); ++i)
      free_item(items[i]);
  free_menu(menu);
  delwin(main_win);
  delwin(status_bar);
  endwin();
  exit(0);
}

std::string CDC::ControlPanel::GetInput(const std::string &prompt) {
  char input[100] = {0};
  PrintStatus(prompt);
  echo();
  mvwgetnstr(status_bar, 1, prompt.length() + 4, input, 99);
  noecho();
  return std::string(input);
}

void CDC::ControlPanel::MenuSetLightStatus() {

}

void CDC::ControlPanel::MenuGetLightStatus() {

}

void CDC::ControlPanel::MenuUpdateFirmware() {
  std::string tftp_ip = GetInput("Enter TFTP Server IP:");
  std::string tftp_file = GetInput("Enter Filename:");
  std::string url("tftp://");
  double fileSize;

  url += tftp_ip + "/" + tftp_file;

  PrintStatus("Fetching from " + url);

  if (CDC::FileOps::tftp_download(url, "firmware.bin", &fileSize)) {
    std::stringstream msg;
    msg << "Success! Downloaded " << fileSize << " bytes.";
    PrintStatus(msg.str());
  }
}

void CDC::ControlPanel::MenuUploadTrafficProgramming() {
  if (loggedIn) {
    std::string tftp_ip = GetInput("Enter TFTP Server IP:");
    std::string tftp_file = GetInput("Enter Filename:");
    std::string url("tftp://");
    double fileSize;

    url += tftp_ip + "/" + tftp_file;

    PrintStatus("Fetching from " + url);

    if (CDC::FileOps::tftp_download(url, PROGRAMMING_FILE, &fileSize)) {
      std::stringstream msg;
      msg << "Success! Downloaded " << fileSize << " bytes.";
      PrintStatus(msg.str());
    }
    else {
      PrintStatus("There was an error downloading from " + url);
    }
  }
  else {
    LoginPrompt();
  }
}

void CDC::ControlPanel::MenuRebootDevice() {

}

void CDC::ControlPanel::MenuViewDeviceInfo() {
  char printbuf[100] = {0};
  snprintf(printbuf, 100, "IP: %s\t\t\tFirmware Version: %s", "0.0.0.0", FW_VER);
  PrintStatus(printbuf);
}

void CDC::ControlPanel::MenuExit() {
  Shutdown();
}

void CDC::ControlPanel::LoginPrompt() {
  std::string userPasswd = GetInput("Enter Admin Password:");
  std::string storedPasswd;

  #ifdef VULN1
  if (userPasswd.compare("opensesame") == 0) { //Simple backdoor vulnerability
    loggedIn = true;
  }
  #endif

  if (pwDb->GetPassword().compare(userPasswd) != 0) {
    PrintStatus("Invalid password!");
  }
  else {
    PrintStatus("Success! You may now access admin functions");
    loggedIn = true;
  }
}

void CDC::ControlPanel::MenuChangeAdminPassword() {
  std::string userPasswd = GetInput("Enter Old Admin Password:");

  if (pwDb->GetPassword().compare(userPasswd) != 0) {
    PrintStatus("Invalid old password!");
  }
  else {
    std::string newPass = GetInput("Enter New Admin Password:");
    std::string newPassRepeated = GetInput("Enter New Admin Password Again:");

    if (newPass.compare(newPassRepeated) != 0) {
      PrintStatus("Passwords do not match!");
    }
    else {
      if (pwDb->SetPassword(newPass, userPasswd)) {
        PrintStatus("Success!");
      }
      else {
        PrintStatus("Failed to set new password!");
      }
    }
  }
}
