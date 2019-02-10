CXX = g++
SRCDIR = src
TESTSRC = test
TESTDIR = tbuild
INCDIR = include
BUILDDIR = build
BINDIR = bin
TARGET = bin/slots
SQLITECPPDIR= extern/SQLiteCpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
SQLITECPP_SOURCES = $(shell find $(SQLITECPPDIR)/src -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
OBJECTS += $(patsubst $(SQLITECPPDIR)/src/%,$(BUILDDIR)/SQLiteCpp/%,$(SQLITECPP_SOURCES:.cpp=.o))
CXXFLAGS =-std=c++11 -g -Wall -Wextra #-pthread
TESTLIB = -lgtest
## Removed -lconfig++
LIB = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -ltinyxml2 -lphysfs -lsqlite3 -L/usr/local/opt/sqlite/lib
INC = -Iinclude $(shell sdl2-config --cflags) -I/usr/local/include -I${SQLITECPPDIR}/include
GTEST_DIR = extern/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
CPPFLAGS += -isystem -I$(GTEST_DIR)/include -I${SQLITECPPDIR}/include ${INC}

NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
ERROR_COLOR=\x1b[31;01m
WARN_COLOR=\x1b[33;01m


# Locate all test files
TESTS = $(patsubst ${TESTSRC}/%.cpp,${TESTDIR}/%.o,$(wildcard ${TESTSRC}/*.cpp))

# SQLiteCpp Objects
SQLITECPPOBJS = $(patsubst ${SQLITECPPDIR}/src/%.cpp,${BUILDDIR}/SQLiteCpp/%.o,$(wildcard ${SQLITECPPDIR}/src/*.cpp))

# Locate all objects, but filter out main
OBJS += $(patsubst ${SRCDIR}/%.cpp,${BUILDDIR}/%.o,$(wildcard ${SRCDIR}/*.cpp))
OBJS := $(filter-out ${BUILDDIR}/main.o, $(OBJS))


# Builds the binary
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $(TARGET) $(LIB)

check:
	@echo $(OBJECTS)

todo:
	@echo "$(WARN_COLOR)" && \
	grep TODO -nr src include && echo "$(NO_COLOR)"

# Clean targets
clean:
	$(RM) -r $(BUILDDIR)/*.o \
	  $(BUILDDIR)/SqliteCpp/*.o \
	  $(BUILDDIR)/SqliteCpp/ \
	  $(TARGET) \
	  $(BINDIR)/test \
	  $(TESTDIR)/*.o \
	  $(TESTDIR)/*.a 

# Generate required test files
golden:
	zip -r $(TESTSRC)/golden.zip ${TESTSRC}/golden

# Run tests
test: $(TESTS) ${OBJS} ${SQLITECPPOBJS} $(TESTDIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $(LIB) $^ -o $(BINDIR)/$@
	./$(BINDIR)/test

# Builds the SQLiteCPP objects
$(BUILDDIR)/SQLiteCpp/%.o: $(SQLITECPPDIR)/src/%.cpp
	mkdir -p $(BUILDDIR)/SQLiteCpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

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
