#ifndef WALK_MAP_H
#define WALK_MAP_H


#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Array2D.cpp"

using namespace std;

enum enemy_t {
	KIRKELLAY,
	ZOMBIE,
	RIDER,
	ENEMY_TYPE_COUNT
};

enum condition_t {
	SLEEPING,
	ONGUARD,
	CHASING
};

struct hero_struct {
	short x;
	short y;

	hero_struct(){
		x = 0;
		y = 0;
	}
};

struct enemy_i { //enemy Info
	string name;
	short radius;
	short view_radius;
	char code_name;

	enemy_i(){
		radius = 0;
		view_radius = 0;
		code_name = 0;
	}
};

vector<enemy_i> enemy_db;

struct enemy_struct {
	short x;
	short y;
	short anchor_x;
	short anchor_y;
	int direction; //percuma pake char
	condition_t condition;
	enemy_i * type;

	enemy_struct( int x, int y, enemy_t type ) {
		anchor_x = x;
		anchor_y = y;
		this -> x = anchor_x;
		this -> y = anchor_y;
		direction = 1;
		condition = ONGUARD;
		this -> type = &enemy_db[ type ];
	}
};


void fill_enemy_db();

class walk_map {
public:
	walk_map( int w, int h );
	~walk_map();

	void print();
	void move_hero( char direction );
	void add_enemy( enemy_struct input );
	void add_random_enemy( int number );
	void add_random_wall( int number );
private:
	int map_width;
	int map_height;
	hero_struct dummy;
	Array2D<bool> field;

	vector<enemy_struct> enemies;

	int find_enemy( int x, int y );
	int find_enemy( int index );
	bool check_exist( int index, int& eindex ); //e index as enemy index in enemies array
	bool check_exist( int index );

	void handle_enemy();
	void encounter( enemy_struct& enemy, bool plus );
	
};

#endif


