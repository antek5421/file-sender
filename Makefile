CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I$(INCLUDEDIR)
LDFLAGS = -lcrypto -lssl

SRCDIR = src
INCLUDEDIR = include
BUILDDIR = build
TARGETDIR = .
SRC = ./main.c $(SRCDIR)/client.c $(SRCDIR)/server.c $(SRCDIR)/hash.c
OBJ = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
EXEC = $(TARGETDIR)/program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(EXEC)

.PHONY: all clean
