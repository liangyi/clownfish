CC = gcc
CXX = g++

CFLAGS = -g -Wall -std=c++11

ifeq ($(mode), release)
	CFLAGS += -O2
else
	CFLAGS += -O0
	mode = debug
endif

AR_OPTS=-scurv

