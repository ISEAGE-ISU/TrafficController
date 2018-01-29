CXX_FLAGS =-std=c++17 -Wall -Wno-sign-compare -O2 -DVULN1
C_FLAGS =-Wall -O2 -DVULN1
LIBS = /tmp/ncurses-6.0/lib/libmenu.a /tmp/ncurses-6.0/lib/libncurses.a -lz -lstdc++fs -lpthread -lssl -lcrypto -lcryptopp -larchive -lcurl
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
	g++ $(CXX_FLAGS) -o $(TARGET) $^ $(LIBS)
clean:
	rm *.o
install:
	cp firmware.bin /opt/firmware.bin
	echo admin > /opt/password.txt
	chmod u+s /opt/firmware.bin

#g++ -std=c++17 -Wall -Wno-sign-compare -O2 -DVULN1  -DCURL_STATICLIB -o firmware.bin ControlPanel.o PasswordDB.o HTTPServer.o main.o mongoose.o FileOps.o -lstdc++fs -lpthread  libcryptopp.a libcurl.a libarchive.a /usr/lib64/libmenu.a /usr/lib64/libssl.a /usr/lib64/libcrypto.a /usr/lib64/libz.a /usr/lib64/libncurses.a -static-libgcc -ldl -Wl,--no-as-needed /usr/lib64/libtinfo.a -lcryptopp 
