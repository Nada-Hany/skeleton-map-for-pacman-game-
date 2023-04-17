#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#define WIDTH 300
#define HEIGH 300
#define TILESIZE 20
#define	Y_OFFSET 0
#define X_OFFSET 0
#define player_width 18
#define player_height 18
const int diff = ((TILESIZE - player_width) / 2);
#define NUMROW 15
#define NUMCOL 15
#define baseSpeed 1

using namespace std;
using namespace sf;

enum class tile_type {
	none, wall, start, target, score 
};

int walls[NUMROW][NUMCOL] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0, -1},
	{-1,  0, -1,  0, -1, -1, -1, -1, -1,  0,  1, -1,  0, -1, -1},
	{-1,  0, -1,  0,  0,  0,  0, -1,  0,  0, -1, -1,  0, -1, -1},
	{-1,  0, -1,  0, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0, -1},
	{-1,  0, -1,  0,  0,  0,  0,  0,  0,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1,  0, -1, -1,  0, -1, -1,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1,  0, -1,  0,  0,  0, -1,  0, -1,  0,  0,  0, -1},
	{-1,  0,  0,  0, -1, -1, -1, -1, -1,  0,  0,  0, -1, -1, -1},
	{-1,  0, -1,  0,  0,  0,  0,  0, -1,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1, -1, -1, -1,  2, -1, -1,  0, -1,  0, -1,  0, -1},
	{-1,  0,  0,  0,  0,  0,  0, -1,  0,  0, -1,  0,  0,  0, -1},
	{-1, -1,  0, -1,  0, -1,  0, -1, -1,  0, -1, -1, -1,  0, -1},
	{-1,  0,  0,  0,  0, -1,  0,  0,  0,  0,  0,  0, -1,  0, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

typedef struct tile {

	tile_type type;
	Sprite tile_sprite;
	CircleShape dot;
	int row;
	int column;
	struct tile* parent = NULL;
};
tile mp[NUMROW][NUMCOL];

bool exist_in_closed(tile* current, vector<struct tile>& closed);

//function returns the row and col of the corner 
void get_tile_cor(float x, float y, int& row, int& col) {
	col = (int)x / TILESIZE;
	row = (int)y / TILESIZE;
	//if ((int)x % 20 == 0)
	//	col = ((int)x / TILESIZE) - 1;
	//if ((int)y % 20 == 0)
	//	row = ((int)y / TILESIZE) - 1;
}

//positive and negative diff in these functions exist since the player sprite
//is not the same pixel size in width and height as the tile so i want to leave an equal distance
//between the wall and the player while colliding.
void move_right(Sprite& sprite) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	int row_1, row_2, col_1, col_2;
	get_tile_cor(x + (player_width / 2), y - (player_height / 2), row_1, col_1);
	get_tile_cor(x + (player_width / 2), y + (player_height / 2), row_2, col_2);

	if (row_1 == row_2 && col_1 == col_2) condition_1 = true;

	int row, col;

	get_tile_cor(x + (player_width / 2) + baseSpeed, y, row, col);
	if (mp[row][col].type == tile_type::wall) {
		if ((col * TILESIZE) - (x + (player_width / 2) + diff) > 0) {

			if (condition_1 && condition_2)
				sprite.move((col * TILESIZE) - (x - (player_width / 2)), 0);
		}
		else {
			condition_2 = false;
		}
	}
	else {
		if (condition_1 && condition_2) sprite.move(baseSpeed, 0);
	}
}
void move_left(Sprite& sprite) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	int row_1, row_2, col_1, col_2;
	get_tile_cor(x - (player_width / 2), y - (player_height / 2), row_1, col_1);
	get_tile_cor(x - (player_width / 2), y + (player_height / 2), row_2, col_2);

	if (row_1 == row_2 && col_1 == col_2) condition_1 = true;

	int row, col;
	get_tile_cor(x - ((player_width / 2)) - baseSpeed - 0.001 - diff , y, row, col);

	if (mp[row][col].type == tile_type::wall)
	{
		if ((x - (player_width / 2) - diff) - ((col * TILESIZE) + TILESIZE) > 0)
			sprite.move(-((x - (player_width / 2)) - ((col * TILESIZE) + TILESIZE)), 0);
		else
			condition_2 = false;
	}

	else {
		if (condition_1 && condition_2) sprite.move(-baseSpeed, 0);
	}
}
void move_up(Sprite& sprite) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	int row_1, row_2, col_1, col_2;
	get_tile_cor(x - (player_width / 2), y - (player_height / 2), row_1, col_1);
	get_tile_cor(x + (player_width / 2), y - (player_height / 2), row_2, col_2);

	if (row_1 == row_2 && col_1 == col_2) condition_1 = true;

	int row, col;

	get_tile_cor(x, y - baseSpeed - (player_height / 2) - diff, row, col);

	if (mp[row][col].type == tile_type::wall) {
		if (x, (y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE) > 0)
			sprite.move(0, -((y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE)));
		else
			condition_2 = false;
	}
	else
		if (condition_1 && condition_2)
			sprite.move(0, -baseSpeed);
}
void move_down(Sprite& sprite) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	int row_1, row_2, col_1, col_2;
	get_tile_cor(x - (player_width / 2), y + (player_height / 2), row_1, col_1);
	get_tile_cor(x + (player_width / 2), y + (player_height / 2), row_2, col_2);

	if (row_1 == row_2 && col_1 == col_2) condition_1 = true;

	int row, col;

	get_tile_cor(x, y + baseSpeed + (player_height / 2) + diff + 0.001, row, col);
	if (mp[row][col].type == tile_type::wall) {

		if ((row * TILESIZE) - (y + (player_height / 2) + diff) > 0)
			sprite.move(0, (row * TILESIZE) - (y + (player_height / 2) + diff));
		else
			condition_2 = false;
	}
	else
		if (condition_1 && condition_2) sprite.move(0, baseSpeed);
}

 void move_to_target(vector <struct tile> get_path, Sprite& start) {
	
		for (int i = get_path.size(); i >= 0; i--) {
			//right
			int row, col;
			float x = start.getPosition().x, y = start.getPosition().y;
			get_tile_cor(x, y, row, col);
			if (get_path[i].column > col) {
				move_right(start);
			}
			//left
			if (get_path[i].column <col) {
				move_left(start);
			}
			//up
			if (get_path[i].row < row) {
				move_up(start);
			}
			//down
			if (get_path[i].row > row) {
				move_down(start);
			}
		}
	}



 void find_optimal_path(tile* current, tile* target, vector <tile>* get_path) {
	 queue <tile> open;
	 vector <tile> closed;
	 open.push(*current);

	 while (!open.empty()) {

		 current = &mp[open.front().row][open.front().column];
		 open.pop();

		 if (current == target)
			 break;

		 if (current->column + 1 <= 2) {
			 tile* right_tile = &mp[current->row][current->column + 1];
			 if (right_tile->type != tile_type::wall)
			 {
				 bool check_right = exist_in_closed(right_tile, closed);
				 if (!check_right) {
					 open.push(*right_tile);
					 right_tile->parent = &mp[current->row][current->column];
				 }
			 }
		 }
		 if (current->column - 1 >= 0) {
			 tile* left_tile = &mp[current->row][current->column - 1];

			 if (left_tile->type != tile_type::wall)
			 {
				 bool check_left = exist_in_closed(left_tile, closed);
				 if (!check_left) {
					 open.push(*left_tile);
					 left_tile->parent = &mp[current->row][current->column];
				 }
			 }
		 }
		 if (current->row - 1 >= 0) {
			 tile* up_tile = &mp[current->row - 1][current->column];
			 if (up_tile->type != tile_type::wall)
			 {
				 bool check_up = exist_in_closed(up_tile, closed);
				 if (!check_up) {
					 open.push(*up_tile);
					 up_tile->parent = &mp[current->row][current->column];
				 }
			 }
		 }
		 if (current->row + 1 <= 2) {
			 tile* down_tile = &mp[current->row + 1][current->column];

			 if (down_tile->type != tile_type::wall)
			 {
				 bool check_down = exist_in_closed(down_tile, closed);
				 if (!check_down) {
					 open.push(*down_tile);
					 down_tile->parent = &mp[current->row][current->column];
				 }
			 }
		 }
		 closed.push_back(*current);
	 }

	 (*get_path).clear();
	 while (current->parent != NULL) {
		 (*get_path).push_back(*current);
		 current = current->parent;
	 }
 }

void main() {

	RenderWindow window(VideoMode(WIDTH, HEIGH), "map test", Style::Default);
	window.setFramerateLimit(60);

	Texture target;
	target.loadFromFile("C:/programming/cpp/BFS sfml test/target.png");
	Texture wall;
	wall.loadFromFile("C:/programming/cpp/BFS sfml test/wall.png");
	Texture start;
	start.loadFromFile("C:/programming/cpp/BFS sfml test/start.png");
	Texture none;
	none.loadFromFile("C:/programming/cpp/BFS sfml test/none.png");
	Texture player_texture;
	player_texture.loadFromFile("C:/programming/cpp/BFS sfml test/pac.png");

	float player_x = (2 * TILESIZE) - 10;
	float plyaer_y = (NUMROW - 1) * TILESIZE - 10;

	CircleShape scoreDot(5.0f);
	scoreDot.setFillColor(Color::Yellow);
	scoreDot.setOrigin(5, 5);

	Clock clock;
	Sprite player;
	player.setOrigin((9), (9));
	player.setTexture(player_texture);
	player.setPosition(player_x, plyaer_y);
	float px_per_frame = 0.33;

	// 0 for empty space
	// 1 for target
	// 2 for start
	// -1 for wall 

	int keyPressed = -1;
	while (window.isOpen()) {

		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) {
				if (walls[i][j] == -1) {
					mp[i][j].type = tile_type::wall;
					mp[i][j].tile_sprite.setTexture(wall);
				}
				else if (walls[i][j] == 1) {
					mp[i][j].type = tile_type::target;
					mp[i][j].tile_sprite.setTexture(target);
				}
				else if (walls[i][j] == 2) {
					mp[i][j].type = tile_type::start;
					mp[i][j].tile_sprite.setTexture(start);
				}
				if (walls[i][j] == 0) {
					mp[i][j].type = tile_type::score;
					mp[i][j].tile_sprite.setTexture(none);
					mp[i][j].dot.setRadius(5.0f);
					mp[i][j].dot.setOrigin(5.0f, 5.0f);
					mp[i][j].dot.setPosition((mp[i][j].column * TILESIZE + TILESIZE / 2), (mp[i][j].row * TILESIZE + TILESIZE / 2));
				}
				if (walls[i][j] == 3) {
					mp[i][j].type = tile_type::none;
					mp[i][j].tile_sprite.setTexture(none);
				}
				//mp[i][j].tile_sprite.setOrigin(10, 10);
				mp[i][j].tile_sprite.setPosition((j * TILESIZE), (i * TILESIZE));
				mp[i][j].row = i;
				mp[i][j].column = j;
			}
		}

		Time dt = clock.restart();
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right) && (keyPressed == 1 || keyPressed == -1)) {
				keyPressed = 1;
				move_right(player);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left) && (keyPressed == 2 || keyPressed == -1)) {
				keyPressed = 2;
				move_left(player);
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) && (keyPressed == 3 || keyPressed == -1)) {
				keyPressed = 3;
				move_up(player);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && (keyPressed == 4 || keyPressed == -1)) {
				keyPressed = 4;
				move_down(player);
			}

			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Right) {
					keyPressed = -1;
				}
				if (event.key.code == Keyboard::Left) {
					keyPressed = -1;
				}
				if (event.key.code == Keyboard::Up) {
					keyPressed = -1;
				}
				if (event.key.code == Keyboard::Down) {
					keyPressed = -1;
				}
			}
		}
		float x = player.getPosition().x, y = player.getPosition().y;

		int row, col;
		get_tile_cor(x, y, row, col);

		if (mp[row][col].type == tile_type::score) {
			if (mp[row][col].dot.getGlobalBounds().contains(player.getPosition().x, player.getPosition().y)) {
				walls[row][col] = 3;
			}
		}

		window.clear();

		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) {
				window.draw(mp[i][j].tile_sprite);
				if (mp[i][j].type == tile_type::score) {
					window.draw(mp[i][j].dot);
				}
			}
		}

		window.draw(player);
		window.display();

	}
}

bool exist_in_closed(tile* tile, vector <struct tile>& closed) {
	bool ans = 0;
	for (int i = 0; i < closed.size(); i++) {

		if (tile->row == closed[i].row && tile->column == closed[i].column)
			ans = 1;
	}
	return ans;
}
