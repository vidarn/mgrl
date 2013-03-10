all:
	g++ main.cpp overlay.cpp callback_overlay.cpp list_overlay.cpp dungeon.cpp tile.cpp -g -o roguelike -lncurses -ltcod -ltcodxx

