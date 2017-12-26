CURLDIR=-I/tmp/curlbin/include -L/tmp/curlbin/lib
all:
	g++ -c ControlPanel.cpp
	g++ ControlPanel.o main.cpp -lncurses -lmenu
