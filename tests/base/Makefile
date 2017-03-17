include ../../Inc.mk

LIB_PATH=../../lib
OBJ_PATH=../../obj/base
BIN_PATH=../../bin/tests

INCLUDES=-I../..

LIBS = $(LIB_PATH)/libclownfish_base.a

OBJS = $(patsubst %.cc, $(OBJ_PATH)/%.o, $(wildcard *.cc))

TARGET = $(BIN_PATH)/TimestampTest

all: $(TARGET)

$(BIN_PATH)/TimestampTest: $(OBJ_PATH)/TimestampTest.o
	mkdir -p $(BIN_PATH)
	$(CXX) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)

$(OBJ_PATH)/%.o: %.cc
	mkdir -p $(OBJ_PATH)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@
