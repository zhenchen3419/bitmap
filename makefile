DIR_INC= ./include
DIR_SRC= ./src
DIR_OBJ= ./obj
DIR_BIN= ./bin

SRC= $(wildcard ${DIR_SRC}/*.cpp)
OBJ= $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC}))
INC= $(wildcard ${DIR_INC}/*.hpp)


TARGET= main

BIN_TARGET= ${DIR_BIN}/${TARGET}

cc= g++
macro_def= -DDEBUG -D__LZCNT__ -D__POPCNT__ -D__BMI__

CFLAGS= -O0 -I${DIR_INC} ${macro_def} -std=c++11 -mlzcnt -mbmi
LFLAGS=

${BIN_TARGET}:${OBJ}
	$(cc) $(OBJ) -o $@ $(LFLAGS)
${OBJ}:${SRC} ${INC}
	$(cc) -c $(SRC) -o $@ $(CFLAGS)

clean:
	rm ./obj/* -r
	rm ./bin/* -r

test:
	@$(BIN_TARGET)
