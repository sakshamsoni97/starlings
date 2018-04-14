# Makefile for the project

## Macros
CC = g++
FLAGS = -std=c++11
LIBS = -lGL -lGLU -lm
BUILDDIR = obj
SRCDIR = src
TARGET = bin/murmuration





.PHONY: clean

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf Binary

