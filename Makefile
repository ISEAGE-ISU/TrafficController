CXX_FLAGS =-std=c++17 -Wall -Wno-sign-compare -O2
C_FLAGS =-Wall -O2
LIBS = -lncurses -lmenu -lstdc++fs -lpthread -lcurl -lcryptopp -larchive
DEPS = ControlPanel.cpp PasswordDB.cpp main.cpp HTTPServer.cpp mongoose.c FileOps.cpp
TARGET = firmware.bin

all : $(TARGET)
	python sign.py firmware.bin | tee manifest
	tar cvzf firmware.tar.gz firmware.bin manifest
%.o : %.cpp
	g++ -c -o $@ $< $(CXX_FLAGS)
%.o : %.c
	gcc -c -o $@ $< $(C_FLAGS)
$(TARGET) : ControlPanel.o PasswordDB.o HTTPServer.o main.o mongoose.o FileOps.o
	g++ -o $(TARGET) $^ $(LIBS)
clean:
	rm *.o
install:
	cp firmware.bin /opt/firmware.bin
	echo admin > /opt/password.txt
  chmod u+s /opt/firmware.bin
