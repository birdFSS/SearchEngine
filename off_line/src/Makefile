INC_DIR :=  ../include/
SRC_DIR = ./
EXE_DIR := ./
CC := g++
CPPFLAGS := -g -Wall
LIBS := -lpthread -lboost_regex -llog4cpp
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE :=  $(wildcard *.cc) 
OBJS := $(patsubst %.cc, %.o, $(CPP_FILE))
objs:=$(addsuffix /*.o, $(SRC_DIR))
TARGET := $(addsuffix buildLib.exe,$(EXE_DIR)) 		
$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(INC_FILE) $(LIBS)
%.o:%.cc
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)
object:
	@echo $(objs)
clean:
	rm -rf $(TARGET) $(OBJS)
