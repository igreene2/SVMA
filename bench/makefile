CC = g++
CFLAGS = -std=c++11 -g

INCDIR = -I. -I./json_helper
CURDIR = $(shell pwd)

CFLAGS += $(INCDIR)

all: svma_testbench.out uninterpreted_func.o

clean:
	rm -rf *.out *.o

SVMA_SIM_DIR = /root/workspace/SVMA/build/sim_model
SVMA_MODEL_DIR = $(SVMA_SIM_DIR)/build/CMakeFiles/SVMA.dir/src
SVMA_INCL_DIR = $(SVMA_SIM_DIR)/include

AP_INCLUDE = /root/workspace/practice-ILA/bench/ap_include
SYSTEMC_HOME = /ibuild/systemc-2.3.3
BOOST_HOME = /usr/local

INCDIR += -I$(SVMA_SIM_DIR)/include
INCDIR += -I$(AP_INCLUDE)
INCDIR += -I$(SVMA_INCL_DIR)/*

INCDIR += -I$(SYSTEMC_HOME)/include
INCDIR += -I$(BOOST_HOME)/include

LIBDIR = -L. -L$(SYSTEMC_HOME)/lib -Wl,-rpath=$(SYSTEMC_HOME)/lib
LIBDIR += -L$(BOOST_HOME)/lib

LIBS = -lstdc++ -lsystemc -lboost_chrono -lboost_system

CFLAGS_SVMA = $(INCDIR) $(LIBDIR) -std=c++11 -g

uninterpreted_func.o: uninterpreted_func.cc $(SVMA_SIM_DIR)/include/*
	$(CC) $(CFLAGS_SVMA) -c uninterpreted_func.cc -o uninterpreted_func.o $(LIBS)

svma_testbench.out : uninterpreted_func.o conv_testbench.cc $(SVMA_MODEL_DIR)/*.o $(SVMA_SIM_DIR)/include/*.h
	$(CC) $(CFLAGS_SVMA) conv_testbench.cc uninterpreted_func.o $(SVMA_MODEL_DIR)/*.o -o conv_testbench.out $(LIBS)
