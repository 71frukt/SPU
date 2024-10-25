ÑXX = g++
FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
		-Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo \
		-Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE -std=c++11 

LIB_STACK = ../stack/lib
LIB_STACK_OBJ_DIR = ../stack/bin
LIB_STACK_SRC = $(notdir $(wildcard $(LIB_STACK)/*.cpp))
LIB_STACK_OBJ_NAMES = $(addsuffix .o,$(basename $(LIB_STACK_SRC)))
LIB_STACK_OBJ_PATH = $(addprefix $(LIB_STACK_OBJ_DIR)/,$(LIB_STACK_OBJ_NAMES))

LIB_OBJ_PATH = $(LIB_STACK_OBJ_PATH)

SOURCES = 	$(wildcard *.cpp)
OBJECTS_DIR = ../bin
BUILD_DIR = ../build
OBJ = $(addsuffix .o,$(basename $(SOURCES)))
OBJ_PATH = $(addprefix $(OBJECTS_DIR)/,$(OBJ))

INCLUDE_PATH = -I $(LIB_STACK)
EXECUTABLE = compiler


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) lib
	$(CXX) $(OBJ_PATH) $(LIB_OBJ_PATH) -o $(BUILD_DIR)/$(EXECUTABLE) 

lib:
	$(MAKE) --directory=$(LIB_STACK)

%.o: %.cpp
	$(CXX) -c $< -o $(OBJECTS_DIR)/$@ $(INCLUDE_PATH) $(FLAGS)

clean:
	rm -rf *.exe
	rm -rf *.o