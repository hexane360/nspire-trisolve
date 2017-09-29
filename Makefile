DEBUG = FALSE

GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GXXFLAGS = --std=c++11 -Wall -W -marm
LDFLAGS =
ZEHNFLAGS = --name "$(EXE)" --ndless-min 45 --touchpad-support 1 --clickpad-support 0 --color-support 1 --240x320-support 0

ifeq ($(DEBUG),FALSE)
	GXXFLAGS += -Os
else
	GXXFLAGS += -O0 -g
endif

all: triangle.tns

main.o : main.cpp Triangle.h constants.h font.h
	$(GXX) $(GXXFLAGS) -Wno-switch -c main.cpp
Triangle.o : Triangle.cpp Triangle.h Dimension.h Message.h constants.h
	$(GXX) $(GXXFLAGS) -c Triangle.cpp
Message.o : Message.cpp Message.h Vector2.h constants.h font.h
	$(GXX) $(GXXFLAGS) -c Message.cpp
Dimension.o : Dimension.cpp Dimension.h Vector2.h constants.h font.h
	$(GXX) $(GXXFLAGS) -c Dimension.cpp
font.o : font.cpp font.h Vector2.h
	$(GXX) $(GXXFLAGS) -c font.cpp
font.h : font_*.h;

triangle.elf: main.o Triangle.o Dimension.o Message.o font.o
	$(LD) $^ -o $@ $(LDFLAGS)

triangle.tns: triangle.elf
	$(GENZEHN) --input $^ --output $@ $(ZEHNFLAGS)

clean:
	rm -f *.o triangle.tns triangle.elf
