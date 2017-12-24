#include "ControlPanel.h"

ControlPanel::ControlPanel() {
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
  items = (ITEM**)malloc(menu_len * sizeof(ITEM *));
  bzero(items, menu_len * sizeof(ITEM*));

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
  set_menu_sub(menu, derwin(main_win, MENU_HEIGHT - 5, MENU_WIDTH -2, 5, 2));
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
  menuHandler.insert(std::make_pair("Reboot Device", std::bind(&ControlPanel::MenuRebootDevice, this)));
  menuHandler.insert(std::make_pair("Upload Traffic Light Programming", std::bind(&ControlPanel::MenuUploadTrafficProgramming, this)));
  menuHandler.insert(std::make_pair("Set Light Status", std::bind(&ControlPanel::MenuSetLightStatus, this)));
  menuHandler.insert(std::make_pair("Get Light Status", std::bind(&ControlPanel::MenuGetLightStatus, this)));
  menuHandler.insert(std::make_pair("Update Firmware", std::bind(&ControlPanel::MenuUpdateFirmware, this)));
  menuHandler.insert(std::make_pair("View Device Info", std::bind(&ControlPanel::MenuViewDeviceInfo, this)));
  menuHandler.insert(std::make_pair("Exit", std::bind(&ControlPanel::MenuExit, this)));
}

ControlPanel::~ControlPanel() {

}

void ControlPanel::PrintStatus(std::string msg) {
  wclear(status_bar);
  box(status_bar, 0, 0);
  wattron(status_bar, COLOR_PAIR(1));
  mvwprintw(status_bar, 0, 2, "Status:");
  wattroff(status_bar, COLOR_PAIR(1));

  mvwprintw(status_bar, 1, 3, "%s", msg.c_str());
  wrefresh(status_bar);
}

void ControlPanel::InputLoop() {
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

void ControlPanel::Shutdown() {
  unpost_menu(menu);
  for(int i = 0; i < ARRAY_SIZE(menu_choices); ++i)
      free_item(items[i]);
  free_menu(menu);
  delwin(main_win);
  delwin(status_bar);
  endwin();
  exit(0);
}

std::string ControlPanel::GetInput(std::string prompt) {
  char input[100] = {0};
  PrintStatus(prompt);
  echo();
  mvwgetnstr(status_bar, 1, prompt.length() + 4, input, 99);
  noecho();
  return std::string(input);
}

void ControlPanel::MenuSetLightStatus() {

}

void ControlPanel::MenuGetLightStatus() {

}

void ControlPanel::MenuUpdateFirmware() {
  std::string tftp_ip = GetInput("Enter TFTP IP:");
  std::string tftp_file = GetInput("Enter Filename:");

  PrintStatus("Fetching from tftp://" + tftp_ip + "/" +  tftp_file);
}

void ControlPanel::MenuUploadTrafficProgramming() {
  std::string tftp_ip = GetInput("Enter TFTP Server IP:");
  std::string tftp_file = GetInput("Enter Filename:");

  PrintStatus("Fetching from tftp://" + tftp_ip + "/" +  tftp_file);
}

void ControlPanel::MenuRebootDevice() {

}

void ControlPanel::MenuViewDeviceInfo() {
  char printbuf[100] = {0};
  snprintf(printbuf, 100, "IP: %s\t\t\tFirmware Version: %s", "0.0.0.0", FW_VER);
  PrintStatus(printbuf);
}

void ControlPanel::MenuExit() {
  Shutdown();
}
