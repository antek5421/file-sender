CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

SRCDIR = .
BUILDDIR = build
TARGETDIR = .
SRC = main.c client.c server.c
OBJ = $(SRC:%.c=$(BUILDDIR)/%.o)
EXEC = $(TARGETDIR)/program

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Kompilowanie plików obiektowych w folderze build
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(BUILDDIR)  # Tworzy folder build, jeśli nie istnieje
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c client.h server.h
	$(CC) $(CFLAGS) -c $< -o $(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR) $(EXEC)

.PHONY: all clean
