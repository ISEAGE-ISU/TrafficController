CXX_FLAGS =-g -std=c++17 -Wall -Wno-sign-compare
C_FLAGS =-g -Wall
LIBS = -lncurses -lmenu -lstdc++fs -lpthread -lcurl
DEPS = ControlPanel.cpp PasswordDB.cpp main.cpp HTTPServer.cpp mongoose.c FileOps.cpp
TARGET = a.out

all : $(TARGET)
%.o : %.cpp
	g++ -c -o $@ $< $(CXX_FLAGS)
%.o : %.c
	gcc -c -o $@ $< $(C_FLAGS)
$(TARGET) : ControlPanel.o PasswordDB.o HTTPServer.o main.o mongoose.o FileOps.o
	g++ -o $(TARGET) $^ $(LIBS)
clean:
	rm *.o
