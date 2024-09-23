# Compiler options and flags
CPPOPT = -g -Og -D_DEBUG
# Uncomment if needed:
# -O2 -Os -Ofast
# -fprofile-generate -fprofile-use
CPPFLAGS = $(CPPOPT) -Wall -ansi -pedantic -std=c++11
# -Wparentheses -Wno-unused-parameter -Wformat-security
# -fno-rtti -std=c++11 -std=c++98

# Documents and scripts
DOCS = Tasks.txt
SCRS =

# Headers and source files
HDRS = defs.h Iterator.h Scan.h Filter.h Sort.h Witness.h
SRCS = defs.cpp Assert.cpp Main.cpp Test.cpp Iterator.cpp Scan.cpp Filter.cpp Sort.cpp Witness.cpp

# Object files for Main and Test
MAIN_OBJS = defs.o Assert.o Main.o Iterator.o Scan.o Filter.o Sort.o Witness.o
TEST_OBJS = defs.o Assert.o Test.o Iterator.o Scan.o Filter.o Sort.o Witness.o

# RCS assists
REV = -q -f
MSG = no message

# Default target: compile both Main.exe and Test.exe
all: Main.exe Test.exe

# Compilation of Main.exe
Main.exe: $(MAIN_OBJS)
	g++ $(CPPFLAGS) -o Main.exe $(MAIN_OBJS)

# Compilation of Test.exe
Test.exe: $(TEST_OBJS)
	g++ $(CPPFLAGS) -o Test.exe $(TEST_OBJS)

# Tracing both Main.exe and Test.exe
trace: Test.exe Main.exe Makefile
	@date > trace
	@size -t Test.exe $(TEST_OBJS) | sort -r >> trace
	@size -t Main.exe $(MAIN_OBJS) | sort -r >> trace
	./Test.exe >> trace
	./Main.exe >> trace

# Object file dependencies
defs.o: defs.cpp defs.h
Assert.o: Assert.cpp defs.h
Main.o: Main.cpp defs.h Iterator.h Scan.h Filter.h Sort.h Witness.h
Test.o: Test.cpp defs.h Iterator.h Scan.h Filter.h Sort.h Witness.h
Iterator.o: Iterator.cpp Iterator.h
Scan.o: Scan.cpp Scan.h
Filter.o: Filter.cpp Filter.h
Sort.o: Sort.cpp Sort.h
Witness.o: Witness.cpp Witness.h

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

# Clean target to remove generated files
clean:
	@rm -f $(MAIN_OBJS) $(TEST_OBJS) Test.exe Main.exe Test.exe.stackdump Main.exe.stackdump trace