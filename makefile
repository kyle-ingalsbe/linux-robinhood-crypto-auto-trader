# Makefile for compiling cr_traider

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -O2

# Libraries
LIBS = -lmysqlclient `pkg-config --cflags --libs gtk+-3.0`

# Target executable name
TARGET = cr_traider

# Source files
SRC = cr_traider.cpp

# Build rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET)
