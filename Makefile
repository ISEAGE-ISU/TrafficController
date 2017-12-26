CXX_FLAGS=-g -std=c++17 -Wall
LIBS = -lncurses -lmenu -lstdc++fs -lpthread -lcurl

all : a.out
a.out : ControlPanel.o PasswordDB.o HTTPServer.o main.o mongoose.o FileOps.o
	g++ $^ $(CXX_FLAGS) $(LIBS) -o $@

clean:
	rm *.o
