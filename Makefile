CC = gcc
LIBS = -lSDL2 -lSDL2_ttf

TARGET = Main
SRCS = main.c editor.c

all:    $(CC) $(SRCS) -o $(TARGET) $(LIBS)
clean:  rm -f $(TARGET)
