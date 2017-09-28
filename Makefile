CXX = g++
SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = bin/slots
SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
CXXFLAGS =-std=c++11 -g # -Wall
LIB = $(shell sdl2-config --libs) -lconfig++ -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ltinyxml2
INC = -Iinclude $(shell sdl2-config --cflags) -I/usr/local/include

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR)/*.o $(TARGET)


