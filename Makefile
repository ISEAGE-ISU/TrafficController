CURLDIR=-I/tmp/curlbin/include -L/tmp/curlbin/lib
all:
	g++ ControlPanel.cpp main.cpp PasswordDB.cpp -g -lncurses -lmenu -std=c++17 -lstdc++fs -Wall
