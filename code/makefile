# source files.

SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src%, ./buildtemp%.o, $(SRC))

OUT = bin/CAL1516T2GE

# include directories
INCLUDES = -I./src -I/usr/local/include

# compiler
CCC = g++ -std=c++14 -pipe

# library paths
LIBS = -L/usr/local/lib -lboost_filesystem -lboost_system -lcurl -lcurlpp

.SUFFIXES: .cpp

all: default
default: CCFLAGS = -O2 -march=native
default: $(OUT)

debug: CCFLAGS = -O0 -g -Wall -Wextra -pedantic -Wdouble-promotion -Wshadow -Wfloat-equal -Wcast-align -Wcast-qual -Wwrite-strings -Wconversion -Wsign-conversion -Wuseless-cast -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wdisabled-optimization -Wstack-protector -Winline -Wswitch-default -Wswitch-enum
debug: $(OUT)

./buildtemp/%.o: src/%
	mkdir -p ./buildtemp
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	mkdir -p bin
	$(CCC) $(INCLUDES) $(CCFLAGS) $(OBJ) $(LIBS) -o $(OUT)

clean:
	rm -f $(OBJ) $(OUT)

test:
	echo $(SRC)
	echo $(OBJ)
