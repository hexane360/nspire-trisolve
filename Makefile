DEBUG = FALSE
EXE = triangle

GCC = nspire-gcc
AS  = nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GXXFLAGS = --std=c++11 -Wall -W -marm
LDFLAGS = #-Wl,--nspireio
ZEHNFLAGS = --name "$(EXE)" --ndless-min 45 --touchpad-support 1 --clickpad-support 0 --color-support 1 --240x320-support 0

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
DISTDIR = .
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).tns

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.cpp %.h
	$(GXX) $(GCCFLAGS) -c $<

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $<

%.o: %.S
	$(AS) -c $<

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $(DISTDIR)/$^ --output $(DISTDIR)/$@ $(ZEHNFLAGS)

clean:
	rm -f *.o $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf
