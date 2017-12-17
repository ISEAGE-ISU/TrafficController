CURLDIR=-I/tmp/curlbin/include -L/tmp/curlbin/lib
all:
	gcc main.cpp file_ops.cpp $(CURLDIR) -lncurses -lmenu -Wno-write-strings -g -lcurl -lform -lpanel
