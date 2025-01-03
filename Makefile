CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lcrypto -lssl

SRCDIR = .
BUILDDIR = build
TARGETDIR = .
SRC = main.c client.c server.c hash.c
OBJ = $(SRC:%.c=$(BUILDDIR)/%.o)
EXEC = $(TARGETDIR)/program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(EXEC)

.PHONY: all clean
