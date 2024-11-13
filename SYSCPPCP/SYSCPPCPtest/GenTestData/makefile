# Makefile for Linux (Ubuntu)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17
DEBUGFLAGS = -g -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Directories
INCLUDES = -I../../SYSCPPCPheaders
SRCDIR = ../src
BUILDDIR = build
DEBUGDIR = $(BUILDDIR)/debug
RELEASEDIR = $(BUILDDIR)/release
LIBDIR_DEBUG = ../../SYSCPPCPlibs/debug
LIBDIR_RELEASE = ../../SYSCPPCPlibs/release

# Source and object files
SRC = GenTestData.cpp
OBJ_DEBUG = $(SRC:%.cpp=$(DEBUGDIR)/%.o)
OBJ_RELEASE = $(SRC:%.cpp=$(RELEASEDIR)/%.o)

# Targets
TARGET_DEBUG = $(DEBUGDIR)/GenTestData
TARGET_RELEASE = $(RELEASEDIR)/GenTestData

# Default target
.PHONY: all
all: debug release

# Debug target
debug: $(DEBUGDIR) $(TARGET_DEBUG)

$(DEBUGDIR):
	@mkdir -p $@

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CXX) $(OBJ_DEBUG) -L$(LIBDIR_DEBUG) -lItemd -lCustomerd -lPersond -lInvoiced -lFamilyd -lDogd -lChildd -lCatd -lSYSCPPCPd -o $@

$(DEBUGDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(INCLUDES) -c $< -o $@

# Release target
release: $(RELEASEDIR) $(TARGET_RELEASE)

$(RELEASEDIR):
	@mkdir -p $@

$(TARGET_RELEASE): $(OBJ_RELEASE)
	$(CXX) $(OBJ_RELEASE) -L$(LIBDIR_RELEASE) -lItem -lCustomer -lPerson -lInvoice -lFamily -lDog -lChild -lCat -lSYSCPPCP -o $@

$(RELEASEDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(INCLUDES) -c $< -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)