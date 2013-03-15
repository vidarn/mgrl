#ifndef COMMON_Y7J3P1ZQ
#define COMMON_Y7J3P1ZQ
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/discrete_distribution.hpp>

extern int SCREEN_W;
extern int SCREEN_H;
extern int DUNGEON_WIN_W;
extern int DUNGEON_WIN_H;
extern bool DEBUG;
extern boost::random::mt19937 RAND;
extern boost::random::uniform_int_distribution<> d100;
extern boost::random::uniform_int_distribution<> d20;
extern boost::random::uniform_int_distribution<> d12;
extern boost::random::uniform_int_distribution<> d10;
extern boost::random::uniform_int_distribution<> d8;
extern boost::random::uniform_int_distribution<> d6;
extern boost::random::uniform_int_distribution<> d4;
extern boost::random::uniform_int_distribution<> d3;
#define ESC 27
#define ENTER 13

#endif /* end of include guard: COMMON_Y7J3P1ZQ */
