
Executable = MeSoBot.exe


CC = clang++
INCDIR=inc
CFLAGS = -std=c++17 -fmax-errors=1 -ljsoncpp -lcurl -I$(INCDIR)


SRCDIR := src
BUILDDIR = build
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRC))



$(BUILDDIR)/$(Executable):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(BUILDDIR)/%.o:$(SRCDIR)/%.cpp
	@echo ##$^ -> $@
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: clean run
clean:
	rm -frv $(BUILDDIR)/*

run: $(BUILDDIR)/$(Executable)
	./$<

github:
	git add .
	git commit -am "$(shell date)"
	git push "https://loganer%40vivaldi.net:$(shell cat ../git_token.txt)@github.com/Tarcaxoxide/MeSoBot.git"