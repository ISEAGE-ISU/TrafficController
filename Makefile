CURLDIR=-I/tmp/curlbin/include -L/tmp/curlbin/lib
all:
	g++ -c ControlPanel.cpp
	g++ ControlPanel.o main.cpp -lncurses -lmenu
	#gcc main.cpp file_ops.cpp $(CURLDIR) -lncurses -lmenu -Wno-write-strings -g -lcurl
