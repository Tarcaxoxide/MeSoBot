
NAME = MeSoBot
CC = clang++

CFLAGS = -Iinc -I/usr/include/python3.9/ -L/usr/lib/python3.9/config-3.9-x86_64-linux-gnu -lpython3.9


SRCDIR := src
BUILDDIR = build
OBJDIR = $(BUILDDIR)


rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

$(BUILDDIR)/$(NAME).exe: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	@echo ##$^ -> $@
	
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -frv $(BUILDDIR)/*

run: $(BUILDDIR)/$(NAME).exe
	./$<