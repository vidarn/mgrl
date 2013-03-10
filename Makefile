OUT = roguelike
OBJ = main.o
OBJ += overlay.o callback_overlay.o list_overlay.o
OBJ += dungeon.o tile.o level.o
OBJ += actor.o creature.o player.o
FLG = -g
LIB = -ltcod -ltcodxx

$(OUT):$(OBJ)
	g++ -o $(OUT) $(OBJ) $(FLG) $(LIB)

%.o: %.cpp %.h Makefile
	g++ $(FLG) $(INC) -c $< -o $@

clean:
	rm `find . -name '*.o'`
