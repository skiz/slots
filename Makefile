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
CXXFLAGS =-std=c++11 -g # -Wall -pthread -Wextra
TESTLIB = -lgtest
LIB = $(shell sdl2-config --libs) -lconfig++ -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ltinyxml2 -lphysfs
INC = -Iinclude $(shell sdl2-config --cflags) -I/usr/local/include
GTEST_DIR = extern/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
CPPFLAGS += -isystem $(GTEST_DIR)/include
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIB)

# Builds the target
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

clean:
	$(RM) -r $(BUILDDIR)/*.o \
	  $(TARGET) \
	  $(BINDIR)/test \
	  $(TESTDIR)/*.o \
	  $(TESTDIR)/*.a 



$(TESTDIR)/%.o: $(TESTSRC)/%.cpp
	mkdir -p $(TESTDIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

test: $(TESTDIR)/asset_manager_test.o $(TESTDIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $(BINDIR)/$@
	./$(BINDIR)/test

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
