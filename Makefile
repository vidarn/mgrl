OUT = roguelike
OBJ = main.o
OBJ += overlay.o callback_overlay.o list_overlay.o
OBJ += dungeon.o tile.o room.o
OBJ += level.o 
OBJ += actor.o creature.o player.o
OBJ += messages.o
OBJ += ability.o
OBJ += $(patsubst %.cpp,  %.o, $(wildcard rooms/*.cpp))
FLG = -g
LIB = -ltcod -ltcodxx

$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(LIB)

%.o: %.cpp %.h Makefile
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm `find . -name '*.o'`
