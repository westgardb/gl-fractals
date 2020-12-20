# define the compiler
CXX = g++

# compiler flags
CXXFLAGS = -Wall -Wextra -g

# library linker flags
LDFLAGS = -lSDL2 -lGL -lGLEW

# build directory
BUILDDIR = build

# target output executable
TARGET = main

# source c files
SRCS = main.cpp debug.cpp

# object files
OBJS = $(addprefix $(BUILDDIR)/, $(SRCS:.cpp=.o))

# build targets
default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(BUILDDIR)/main.o: main.cpp debug.cpp debug.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

$(BUILDDIR)/debug.o: debug.cpp debug.hpp
	$(CXX) $(CXXFLAGS) -c debug.cpp -o $@
