# Makefile for Linux (Ubuntu) Project Converted from Visual Studio Project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++2a
DEBUGFLAGS = -g -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Directories
SRCDIR = .
DEBUGDIR = ../debug
RELEASEDIR = ../release

# Source and object files
SRC = OODBGenerator.cpp
OBJ_DEBUG = $(patsubst %.cpp,$(DEBUGDIR)/%.o,$(SRC))
OBJ_RELEASE = $(patsubst %.cpp,$(RELEASEDIR)/%.o,$(SRC))

# Targets
TARGET_DEBUG = $(DEBUGDIR)/SYSCPPCPGenerator
TARGET_RELEASE = $(RELEASEDIR)/SYSCPPCPGenerator

# Default target
.PHONY: all
default: debug

# Debug target
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJ_DEBUG)
	@mkdir -p $(DEBUGDIR)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -o $@ $^

$(DEBUGDIR)/%.o: %.cpp
	@mkdir -p $(DEBUGDIR)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -c $< -o $@

# Release target
release: $(TARGET_RELEASE)

$(TARGET_RELEASE): $(OBJ_RELEASE)
	@mkdir -p $(RELEASEDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -o $@ $^

$(RELEASEDIR)/%.o: %.cpp
	@mkdir -p $(RELEASEDIR)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) -c $< -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(DEBUGDIR) $(RELEASEDIR)