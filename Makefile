#!/bin/make
SHELL=/bin/bash

CXX = g++
CXXFLAGS += -Wall 
c++SrcSuf = cpp

#Objects
MAINO = main.o
GAMMAORGANIZERO = GammaOrganizer.o

OBJS = $(MAINO) $(GAMMAORGANIZERO)

PROGRAM = gammaSearch

.SUFFIXES: .$(c++SrcSuf)

.phony: all clean
all: $(PROGRAM)

clean: 
	@echo "Cleaning..."
	@rm -f $(OBJS) $(PROGRAM)

$(PROGRAM): $(OBJS) 
	$(CXX) $^ -o $@