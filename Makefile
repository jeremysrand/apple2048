APP=a2048

SYS=$(APP).system.sys
MAPFILE=$(APP).map

INSTALL_DIR=~/Documents/"Apple ]["/Transfer

SRCS=$(wildcard *.c)
ASM=$(wildcard *.s)

C_OBJS=$(SRCS:.c=.o)
ASM_OBJS=$(ASM:.s=.o)
OBJS=$(C_OBJS) $(ASM_OBJS)

PLATFORM=apple2enh
PLATFORM_CFG=-C apple2-system.cfg


all:    $(SYS)

%.o:	%.s
	ca65 -t $(PLATFORM) -o $@ $<

$(SYS): $(ASM_OBJS) $(SRCS)
	cl65 -t $(PLATFORM) $(PLATFORM_CFG) --mapfile $(MAPFILE) -o $(SYS) $(SRCS) $(addprefix --obj ,$(ASM_OBJS)) 

clean:
	rm -f $(SYS) $(OBJS) $(GEN_ASM) $(MAPFILE)

install: $(SYS)
	cp $(SYS) $(INSTALL_DIR)
