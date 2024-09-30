# Compiler options and flags
CPPOPT = -g -Og -D_DEBUG
# Uncomment if needed:
# -O2 -Os -Ofast
# -fprofile-generate -fprofile-use
CPPFLAGS = $(CPPOPT) -Wall -ansi -pedantic -std=c++11 -Iinclude
# -Wparentheses -Wno-unused-parameter -Wformat-security
# -fno-rtti -std=c++11 -std=c++98

# Build directory
BUILD_DIR = build

# Directories for includes and source files
INCLUDE_DIR = include
SRC_DIR = src
TEST_DIR = test

# Documents and scripts
DOCS = Tasks.txt
SCRS =

# Headers and source files
HDRS = $(INCLUDE_DIR)/defs.h $(INCLUDE_DIR)/Iterator.h $(INCLUDE_DIR)/Scan.h \
        $(INCLUDE_DIR)/Filter.h $(INCLUDE_DIR)/Sort.h $(INCLUDE_DIR)/Witness.h \
		$(INCLUDE_DIR)/parser.h \
		$(INCLUDE_DIR)/PriorityQueue.h
SRCS = $(SRC_DIR)/defs.cpp $(SRC_DIR)/Assert.cpp Main.cpp Test.cpp \
       $(SRC_DIR)/Iterator.cpp $(SRC_DIR)/Scan.cpp $(SRC_DIR)/Filter.cpp \
       $(SRC_DIR)/Sort.cpp $(SRC_DIR)/Witness.cpp $(SRC_DIR)/parser.cpp \
	   $(SRC_DIR)/PriorityQueue.cpp

# Object files for Main and Test
MAIN_OBJS = $(BUILD_DIR)/defs.o $(BUILD_DIR)/Assert.o $(BUILD_DIR)/Main.o \
            $(BUILD_DIR)/Iterator.o $(BUILD_DIR)/Scan.o $(BUILD_DIR)/Filter.o \
            $(BUILD_DIR)/Sort.o $(BUILD_DIR)/Witness.o $(BUILD_DIR)/parser.o \
			$(BUILD_DIR)/PriorityQueue.o
TEST_OBJS = $(BUILD_DIR)/defs.o $(BUILD_DIR)/Assert.o $(BUILD_DIR)/Test.o \
            $(BUILD_DIR)/Iterator.o $(BUILD_DIR)/Scan.o $(BUILD_DIR)/Filter.o \
            $(BUILD_DIR)/Sort.o $(BUILD_DIR)/Witness.o $(BUILD_DIR)/parser.o \
			$(BUILD_DIR)/PriorityQueue.o
# RCS assists
REV = -q -f
MSG = no message

# Default target: compile both Main.exe and Test.exe
all: $(BUILD_DIR) Main.exe Test.exe

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilation of Main.exe
Main.exe: $(MAIN_OBJS)
	g++ $(CPPFLAGS) -o $(BUILD_DIR)/Main.exe $(MAIN_OBJS)

# Compilation of Test.exe
Test.exe: $(TEST_OBJS)
	g++ $(CPPFLAGS) -o $(BUILD_DIR)/Test.exe $(TEST_OBJS)

# Tracing both Main.exe and Test.exe
trace: Test.exe Main.exe Makefile
	@date > $(BUILD_DIR)/trace
	@size -t $(BUILD_DIR)/Test.exe $(TEST_OBJS) | sort -r >> $(BUILD_DIR)/trace
	@size -t $(BUILD_DIR)/Main.exe $(MAIN_OBJS) | sort -r >> $(BUILD_DIR)/trace
	./$(BUILD_DIR)/Test.exe >> $(BUILD_DIR)/trace
	./$(BUILD_DIR)/Main.exe >> $(BUILD_DIR)/trace

# Object file dependencies
$(BUILD_DIR)/defs.o: $(SRC_DIR)/defs.cpp $(INCLUDE_DIR)/defs.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/defs.cpp -o $(BUILD_DIR)/defs.o

$(BUILD_DIR)/Assert.o: $(SRC_DIR)/Assert.cpp $(INCLUDE_DIR)/defs.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Assert.cpp -o $(BUILD_DIR)/Assert.o

$(BUILD_DIR)/Main.o: Main.cpp $(INCLUDE_DIR)/defs.h $(INCLUDE_DIR)/Iterator.h \
                     $(INCLUDE_DIR)/Scan.h $(INCLUDE_DIR)/Filter.h $(INCLUDE_DIR)/Sort.h \
                     $(INCLUDE_DIR)/Witness.h $(INCLUDE_DIR)/PriorityQueue.h
	g++ $(CPPFLAGS) -c Main.cpp -o $(BUILD_DIR)/Main.o

$(BUILD_DIR)/Test.o: Test.cpp $(INCLUDE_DIR)/defs.h $(INCLUDE_DIR)/Iterator.h \
                     $(INCLUDE_DIR)/Scan.h $(INCLUDE_DIR)/Filter.h $(INCLUDE_DIR)/Sort.h \
                     $(INCLUDE_DIR)/Witness.h $(INCLUDE_DIR)/PriorityQueue.h \
					 $(TEST_DIR)/PriorityQueueTest.h
	g++ $(CPPFLAGS) -c Test.cpp -o $(BUILD_DIR)/Test.o

$(BUILD_DIR)/Iterator.o: $(SRC_DIR)/Iterator.cpp $(INCLUDE_DIR)/Iterator.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Iterator.cpp -o $(BUILD_DIR)/Iterator.o

$(BUILD_DIR)/Scan.o: $(SRC_DIR)/Scan.cpp $(INCLUDE_DIR)/Scan.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Scan.cpp -o $(BUILD_DIR)/Scan.o

$(BUILD_DIR)/Filter.o: $(SRC_DIR)/Filter.cpp $(INCLUDE_DIR)/Filter.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Filter.cpp -o $(BUILD_DIR)/Filter.o

$(BUILD_DIR)/Sort.o: $(SRC_DIR)/Sort.cpp $(INCLUDE_DIR)/Sort.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Sort.cpp -o $(BUILD_DIR)/Sort.o

$(BUILD_DIR)/Witness.o: $(SRC_DIR)/Witness.cpp $(INCLUDE_DIR)/Witness.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/Witness.cpp -o $(BUILD_DIR)/Witness.o

$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.cpp $(INCLUDE_DIR)/parser.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/parser.cpp -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/PriorityQueue.o: $(SRC_DIR)/PriorityQueue.cpp $(INCLUDE_DIR)/PriorityQueue.h
	g++ $(CPPFLAGS) -c $(SRC_DIR)/PriorityQueue.cpp -o $(BUILD_DIR)/PriorityQueue.o

# Utility targets
list: Makefile
	echo Makefile $(HDRS) $(SRCS) $(DOCS) $(SCRS) > list

count: list
	@wc `cat list`

# Check-in (ci) and check-out (co) commands for RCS
ci:
	ci $(REV) -m"$(MSG)" $(HDRS) $(SRCS) $(DOCS) $(SCRS)
	ci -l $(REV) -m"$(MSG)" Makefile

co:
	co $(REV) -l $(HDRS) $(SRCS) $(DOCS) $(SCRS)

# Clean target to remove generated files but keep the build directory
clean:
	@rm -f $(BUILD_DIR)/*
