 $(SRCDIR)# Makefile for Linux (Ubuntu)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++2a -Wno-unused-parameter -Wno-unused-variable
DEBUGFLAGS = -g -DDEBUG
RELEASEFLAGS = -O2 -DNDEBUG

# Directories
INCLUDES = -I../SmallSQLheaders -I../SYSCPPCPheaders
SRCDIR = ../SmallSQLSource
BUILDDIR = ../SmallSQL
DEBUGDIR = $(BUILDDIR)/debug
RELEASEDIR = $(BUILDDIR)/release
LIBDIR_DEBUG = ../SYSCPPCPlibs/debug
LIBDIR_RELEASE = ../SYSCPPCPlibs/release

# Source and object files
SRC = ###SourceFiles $(SRCDIR)/ValidateUpdate.cpp \
  $(SRCDIR)/ValidateSelect.cpp $(SRCDIR)/ValidateInsert.cpp $(SRCDIR)/ValidateDelete.cpp \
  $(SRCDIR)/SmallSQL.cpp $(SRCDIR)/ProcessUpdate.cpp   $(SRCDIR)/ProcessSelect.cpp \
  $(SRCDIR)/ProcessInsert.cpp $(SRCDIR)/ProcessDelete.cpp  $(SRCDIR)/Common.cpp

OBJ_DEBUG = $(SRC:$(SRCDIR)/%.cpp=$(DEBUGDIR)/%.o)
OBJ_RELEASE = $(SRC:$(SRCDIR)/%.cpp=$(RELEASEDIR)/%.o)

# Targets
TARGET_DEBUG = $(DEBUGDIR)/SmallSQLd
TARGET_RELEASE = $(RELEASEDIR)/SmallSQL

# Default target
.PHONY: all
all: debug release

# Debug target
debug: $(DEBUGDIR) $(TARGET_DEBUG)

$(DEBUGDIR):
	@mkdir -p $@

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CXX) $(OBJ_DEBUG) -L$(LIBDIR_DEBUG) ###LibsDebug -lreadline -lSYSCPPCPd -o $@

$(DEBUGDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) $(INCLUDES) -c $< -o $@

# Release target
release: $(RELEASEDIR) $(TARGET_RELEASE)

$(RELEASEDIR):
	@mkdir -p $@

$(TARGET_RELEASE): $(OBJ_RELEASE)
	$(CXX) $(OBJ_RELEASE) -L$(LIBDIR_RELEASE) ###LibsRelease -lreadline -lSYSCPPCP -o $@

$(RELEASEDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(RELEASEFLAGS) $(INCLUDES) -c $< -o $@

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)