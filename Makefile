# Makefile for the project

## Macros
CC = g++
FLAGS = -std=c++11
LIBS = -lGL -lGLU -lm
BUILDDIR = obj
SRCDIR = src
TARGET = bin/murmuration


SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC = -I include

$(TARGET): $(OBJECTS)
  @echo " Linking..."
  $(CC) $(FLAGS) -o $(TARGET) $^ $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
  @mkdir -p $(BUILDDIR)
  $(CC) $(FLAGS) -c $@ $< $(INC)


.PHONY: clean

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf Binary

