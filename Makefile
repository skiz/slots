CXX = g++
SRCDIR = src
TESTSRC = test
TESTDIR = tbuild
INCDIR = include
BUILDDIR = build
BINDIR = bin
TARGET = bin/slots
SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
CXXFLAGS =-std=c++11 -g -Wall -Wextra #-pthread
TESTLIB = -lgtest
LIB = $(shell sdl2-config --libs) -lconfig++ -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ltinyxml2 -lphysfs
INC = -Iinclude $(shell sdl2-config --cflags) -I/usr/local/include
GTEST_DIR = extern/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Locate all test files
TESTS = $(patsubst ${TESTSRC}/%.cpp,${TESTDIR}/%.o,$(wildcard ${TESTSRC}/*.cpp))

# Locate all objects, but filter out main
OBJS = $(patsubst ${SRCDIR}/%.cpp,${BUILDDIR}/%.o,$(wildcard ${SRCDIR}/*.cpp))
OBJS := $(filter-out ${BUILDDIR}/main.o, $(OBJS))

# Builds the binary
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIB)

# Clean targets
clean:
	$(RM) -r $(BUILDDIR)/*.o \
	  $(TARGET) \
	  $(BINDIR)/test \
	  $(TESTDIR)/*.o \
	  $(TESTDIR)/*.a 

# Generate required test files
golden:
	zip -r $(TESTSRC)/golden.zip ${TESTSRC}/golden

# Run tests
test: $(TESTS) ${OBJS} $(TESTDIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $(LIB) $^ -o $(BINDIR)/$@
	./$(BINDIR)/test

# Builds the application
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

# Builds all the tests
$(TESTDIR)/%.o: $(TESTSRC)/%.cpp
	mkdir -p $(TESTDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

# GTest Library
$(TESTDIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@
$(TESTDIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@
$(TESTDIR)/gtest.a : $(TESTDIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^
$(TESTDIR)/gtest_main.a : $(TESTDIR)/gtest-all.o $(TESTDIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
