include ../../Inc.mk

LIB_PATH=../../lib
OBJ_PATH=../../obj/base

INCLUDES=-I../..

TARGET = $(LIB_PATH)/libclownfish_base.a

OBJS = $(patsubst %.cc, $(OBJ_PATH)/%.o, $(wildcard *.cc))

$(TARGET) : $(OBJS)
	mkdir -p $(LIB_PATH)
	$(AR) $(AR_OPTS) $@ $^

$(OBJ_PATH)/%.o: %.cc
	mkdir -p $(OBJ_PATH)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
