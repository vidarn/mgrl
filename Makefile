all:
	g++ main.cpp overlay.cpp callback_overlay.cpp list_overlay.cpp dungeon.cpp actor.cpp player.cpp tile.cpp -g -o roguelike -ltcod -ltcodxx

