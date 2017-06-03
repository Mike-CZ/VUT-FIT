CC =g++
CFLAGS =-pedantic -Wextra
FILES =Main.cpp Client.cpp Message.cpp Parameters.cpp
RESULT =sipklient

$(RESULT): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(RESULT) -lssl -lcrypto
