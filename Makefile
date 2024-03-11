# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -g -Wextra -Wconversion -pedantic -std=gnu11 -fopenmp

# Source files
SRCS := $(wildcard ./*.cpp)

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable
TARGET := MBM

# Default target
all: $(TARGET)
	rm -f $(OBJS)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET)
