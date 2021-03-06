OUT = MGRL
OBJ = main.o
OBJ += actor.o creature.o player.o item.o
OBJ += overlay.o callback_overlay.o list_overlay.o
OBJ += dungeon.o tile.o room.o decoration.o
OBJ += $(patsubst %.cpp,  %.o, $(wildcard rooms/*.cpp))
OBJ += $(patsubst %.cpp,  %.o, $(wildcard decorations/*.cpp))
OBJ += level.o 
OBJ += messages.o
OBJ += ability.o
FLG = -Wall 
INC = -I /usr/include/libtcod
LIB = -ltcod -ltcodxx
CXX = g++

$(OUT):$(OBJ)
	$(CXX) -o $(OUT) $(OBJ) $(FLG) $(LIB) $(INC)

%.o: %.cpp Makefile
	$(CXX) $(FLG) $(INC) -c $< -o $@

clean:
	rm `find . -name '*.o'`
