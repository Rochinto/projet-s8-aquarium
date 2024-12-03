
CFLAGS := -std=c++11
CC := g++ $(CFLAGS)

BUILD_PATH := build
SFML_PATH := SFML
SRC_PATH := src
INCLUDE_PATH := $(SRC_PATH)/include
LIB_PATH := $(SFML_PATH)/lib

LIBS := -lpthread -lsfml-graphics -lsfml-window -lsfml-system
FILES := Main SocketAPI Utils Prompt ReplyManager Aquarium Fish Graphics Config

all: LIBS += -lconfig
all: build SFML displayer

clean:
	rm -rf $(BUILD_PATH)/*
	rm -rf $(SFML_PATH)
	rm -f displayer test_sfml

build:
	mkdir -p build

.PHONY: all clean

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CC) $(CFLAGS) -c -I$(INCLUDE_PATH) -I$(SFML_PATH)/include $< -o $@

#all.o: $(addprefix $(BUILD_PATH), $(addsuffix .o, $(FILES)))

displayer: $(addprefix $(BUILD_PATH)/, $(addsuffix .o, $(FILES)))
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIB_PATH) $(LIBS)

enseirb: CFLAGS += -DENSEIRB
enseirb: build SFML_enseirb displayer

SFML:
	tar -xf SFML-2.5.1-linux-64.tar.gz

SFML_enseirb:
	tar -xf SFML-1.6-linux-64.tar.gz