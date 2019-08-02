INC_DIR :=  ./include/
SRC_DIR = ./src/
EXE_DIR := ./bin/
CC := g++
CPPFLAGS := -g 
LIBS := -lpthread
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE :=  $(wildcard ./src/*.cc) 
OBJS := $(patsubst %.cc, %.o, $(CPP_FILE))
objs:=$(addsuffix /*.o, $(SRC_DIR))
TARGET := $(addsuffix server.exe,$(EXE_DIR)) 		
$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(INC_FILE) $(LIBS)
%.o:%.cc
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)
object:
	@echo $(objs)
clean:
	rm -rf $(TARGET) $(OBJS)
