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
#define ghost_width 18
#define player_height 18
#define ghost_height 18
const int diff = ((TILESIZE - player_width) / 2);
#define NUMROW 15
#define NUMCOL 15
#define baseSpeed 2
#define ghostSpeed 2
const int ghosts_number = 1;

using namespace std;
using namespace sf;

enum class tile_type {
	none, wall, start, target, score
};

int walls[NUMROW][NUMCOL] = {
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0,  0, -1},
	{-1,  0, -1,  0, -1, -1, -1, -1, -1,  0, -1, -1,  0, -1, -1},
	{-1,  0, -1,  0,  0,  0,  0, -1,  0,  0, -1, -1,  0, -1, -1},
	{-1,  0, -1,  0, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0, -1},
	{-1,  0, -1,  0,  0,  0,  0,  0,  0,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1,  0, -1, -1,  0, -1, -1,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1,  0, -1,  0,  0,  0, -1,  0, -1,  0,  0,  0, -1},
	{-1,  0,  0,  0, -1, -1, -1, -1, -1,  0,  0,  0, -1, -1, -1},
	{-1,  0, -1,  0,  0,  0,  0,  0, -1,  0, -1,  0, -1,  0, -1},
	{-1,  0, -1, -1, -1, -1,  0, -1, -1,  0, -1,  0, -1,  0, -1},
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

struct ghost {

	Sprite sprite;
	float speed;
	int moving_direction;
	int frames_per_tile;

	vector<tile> shortest_path;
	int shortest_path_index;
	int algo_window_BFS;
	int step_counts_BFS;
	int num_tiles_past_BFS;

	int step_counts_rand;

}ghosts[ghosts_number];

struct target {
	int initial_x = (2 * TILESIZE) - 10;
	int initial_y = (NUMROW - 1) * TILESIZE - 10;
};

//function returns the row and col of the tile  
void get_tile_cor(float x, float y, int& row, int& col) {
	col = (int)x / TILESIZE;
	row = (int)y / TILESIZE;
	//if ((int)x % 20 == 0)
	//	col = ((int)x / TILESIZE) - 1;
	//if ((int)y % 20 == 0)
	//	row = ((int)y / TILESIZE) - 1;
}

//check if the sprite is in the same tile or not 
bool same_tile_horz(Sprite& sprite) {
	bool condition_1 = false;
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	int row_1, row_2, col_1, col_2;
	get_tile_cor(x + (player_width / 2), y - (player_height / 2), row_1, col_1);
	get_tile_cor(x + (player_width / 2), y + (player_height / 2), row_2, col_2);

	if (row_1 == row_2 && col_1 == col_2)
		condition_1 = true;

	return condition_1;
}
bool same_tile_vert(Sprite& sprite) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false;
	int row_1, row_2, col_1, col_2;
	get_tile_cor(x - (player_width / 2), y - (player_height / 2), row_1, col_1);
	get_tile_cor(x + (player_width / 2), y - (player_height / 2), row_2, col_2);
	if (row_1 == row_2 && col_1 == col_2)
		condition_1 = true;

	return condition_1;
}

//movement
void move_right(Sprite& sprite, int& lastKeyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_horz(sprite);

	int row, col;

	get_tile_cor(x + (player_width / 2) + baseSpeed + 0.0001, y, row, col);
	if (mp[row][col].type == tile_type::wall) {
		if ((col * TILESIZE) - (x + (player_width / 2) + diff) > 0) {

			if (condition_1 && condition_2)
			{
				sprite.move((col * TILESIZE) - (x - (player_width / 2)), 0);
				lastKeyPressed = 0;
			}
		}
		else {
			condition_2 = false;
		}
	}
	else {
		if (condition_1 && condition_2)
		{
			sprite.move(baseSpeed, 0);
			lastKeyPressed = 0;
		}
	}
}
void move_left(Sprite& sprite, int& last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_horz(sprite);

	int row, col;


	get_tile_cor(x - ((player_width / 2)) - baseSpeed - 0.001, y, row, col);

	if (mp[row][col].type == tile_type::wall)
	{
		if ((x - (player_width / 2) - diff) - ((col * TILESIZE) + TILESIZE) > 0)
		{
			sprite.move(-((x - (player_width / 2)) - ((col * TILESIZE) + TILESIZE)), 0);
			last_keyPressed = 2;
		}
		else
		{
			condition_2 = false;
		}
	}

	else {
		if (condition_1 && condition_2) {
			sprite.move(-baseSpeed, 0);
			last_keyPressed = 2;
		}
	}
}
void move_up(Sprite& sprite, int& last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_vert(sprite);

	int row, col;

	get_tile_cor(x, y - baseSpeed - (player_height / 2) - 0.001, row, col);

	if (mp[row][col].type == tile_type::wall) {
		if (x, (y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE) > 0) {
			sprite.move(0, -((y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE)));
			last_keyPressed = 1;
		}
		else
			condition_2 = false;
	}
	else
		if (condition_1 && condition_2)
			sprite.move(0, -baseSpeed), last_keyPressed = 1;
}
void move_down(Sprite& sprite, int& last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_vert(sprite);

	int row, col;

	get_tile_cor(x, y + baseSpeed + (player_height / 2) + 0.001, row, col);
	if (mp[row][col].type == tile_type::wall) {

		if ((row * TILESIZE) - (y + (player_height / 2) + diff) > 0)
		{
			sprite.move(0, (row * TILESIZE) - (y + (player_height / 2) + diff));
			last_keyPressed = 3;
		}
		else
			condition_2 = false;
	}
	else
		if (condition_1 && condition_2) sprite.move(0, baseSpeed), last_keyPressed = 3;
}


//movement cach 
void change_direction(Sprite& sprite, int& keyPressed, int& lastKeyPressed, int row, int col) {

	//right
	if (keyPressed == 0) {
		if (same_tile_horz(sprite))
		{
			if (mp[row][col + 1].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
			}
		}
	}
	//left
	if (keyPressed == 2) {
		if (same_tile_horz(sprite))
		{
			if (mp[row][col - 1].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
			}
		}
	}
	//up
	if (keyPressed == 1) {
		if (same_tile_vert(sprite))
		{
			if (mp[row - 1][col].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
			}
		}
	}
	//down
	if (keyPressed == 3) {
		if (same_tile_vert(sprite))
		{
			if (mp[row + 1][col].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
			}
		}
	}
}

//bfs
bool exist_in_closed(tile* tile, vector <struct tile>& closed) {
	bool ans = 0;
	for (int i = 0; i < closed.size(); i++) {

		if (tile->row == closed[i].row && tile->column == closed[i].column)
			ans = 1;
	}
	return ans;
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

		if (current->column + 1 <= NUMCOL) {
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
		if (current->row + 1 <= NUMROW) {
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

//random moving 
bool check_wall(int& direction, Sprite& ghost)
{
	bool can_move = true, condition_1 = false;
	float x = ghost.getPosition().x, y = ghost.getPosition().y;
	int row_1, row_2, col_1, col_2;
	int row, col;

	//right
	if (direction == 0)
	{
		condition_1 = same_tile_horz(ghost);

		get_tile_cor(x + (ghost_width / 2) + ghostSpeed + 0.0001, y, row, col);
		if (walls[row][col] == -1 || !condition_1)
			can_move = false;
	}
	//up
	else if (direction == 1)
	{
		condition_1 = same_tile_vert(ghost);

		get_tile_cor(x, y - ghostSpeed - (ghost_height / 2) - 0.0001, row, col);
		if (walls[row][col] == -1 || !condition_1)
			can_move = false;
	}
	//left
	else if (direction == 2)
	{
		condition_1 = same_tile_horz(ghost);

		get_tile_cor(x - ((ghost_width / 2) - 0.0001) - ghostSpeed, y, row, col);
		if (walls[row][col] == -1 || !condition_1)
			can_move = false;

	}
	//down
	else if (direction == 3)
	{
		condition_1 = same_tile_vert(ghost);

		get_tile_cor(x, y + ghostSpeed + (ghost_height / 2) + 0.0001, row, col);
		if (walls[row][col] == -1 || !condition_1)
			can_move = false;
	}
	return can_move;
}
void move_random(int& direction, Sprite& ghost)
{
	srand((int)time(0));

	int avaialble_ways = 0;

	int random_direction = rand() % 4;

	for (int moves = 0; moves < 4; moves++)
	{
		if (moves != (2 + direction) % 4)
		{
			if (check_wall(moves, ghost) != 0)

				avaialble_ways++;
		}
	}

	if (avaialble_ways > 0)
	{
		while (check_wall(random_direction, ghost) == 0 || random_direction == (2 + direction) % 4)
		{
			random_direction = rand() % 4;
		}

		direction = random_direction;
	}
	else
	{
		direction = (2 + direction) % 4;
	}
	if (direction == 0)
		ghost.move(ghostSpeed, 0);

	else  if (direction == 1)
		ghost.move(0, -ghostSpeed);

	else if (direction == 2)
		ghost.move(-ghostSpeed, 0);

	else if (direction == 3)
		ghost.move(0, ghostSpeed);
}

bool check_walls(Sprite& sprite, int& direction) {
	float x = sprite.getPosition().x, y = sprite.getPosition().y;
	int row, col;
	bool check = true;

	get_tile_cor(x, y, row, col);
	//right
	if (direction == 0) {
		if (mp[row][col + 1].type == tile_type::wall)
			check = false;
	}
	//left
	else if (direction == 2) {
		if (mp[row][col - 1].type == tile_type::wall)
			check = false;
	}
	//up
	else if (direction == 1) {
		if (mp[row - 1][col].type == tile_type::wall)
			check = false;
	}
	//down
	else if (direction == 3) {
		if (mp[row + 1][col].type == tile_type::wall)
			check = false;
	}

	return check;
}

void random_direction(Sprite& sprite, int& direction) {

	srand((int)time(0));

	int avaialble_ways = 0;

	int random_direction = rand() % 4;

	for (int moves = 0; moves < 4; moves++)
	{
		if (moves != (2 + direction) % 4)
		{
			if (check_walls(sprite, moves) != 0)

				avaialble_ways++;
		}
	}

	if (avaialble_ways > 0)
	{
		while (check_walls(sprite, random_direction) == 0 || random_direction == (2 + direction) % 4)
		{
			random_direction = rand() % 4;
		}

		direction = random_direction;
	}
	else
	{
		direction = (2 + direction) % 4;
	}
}

void catch_target(ghost& ghost, Sprite& target, float& initial_x, float& initial_y) {

		//collision with the player
		if (target.getGlobalBounds().contains(ghost.sprite.getPosition().x, ghost.sprite.getPosition().y)) {
			ghost.algo_window_BFS = 15;
			ghost.num_tiles_past_BFS = 15;
			ghost.frames_per_tile = TILESIZE / ghost.speed;
			ghost.step_counts_BFS = 0;
			ghost.sprite.setPosition(30, 30);
			target.setPosition(initial_x, initial_y);
		}
		//if the ghost finished a whole tile 
		if (ghost.step_counts_BFS % ghost.frames_per_tile == 0) {
			int row, col;
			float x = ghost.sprite.getPosition().x,
				y = ghost.sprite.getPosition().y;
			get_tile_cor(x, y, row, col);
			ghost.step_counts_BFS = 0;

			//if the tile past equals the tiles needed to repeat the algo, or if it's first time for the algo to run.
			if (ghost.num_tiles_past_BFS == ghost.algo_window_BFS) {
				int row_1, col_1;
				float x_1 = target.getPosition().x,
					y_1 = target.getPosition().y;

				get_tile_cor(x_1, y_1, row_1, col_1);
				tile* start_pointer = &mp[row][col];
				start_pointer->parent = NULL;
				tile* target_pointer = &mp[row_1][col_1];

				find_optimal_path(start_pointer, target_pointer, &ghost.shortest_path);
				ghost.num_tiles_past_BFS = 0;
				ghost.shortest_path_index = ghost.shortest_path.size() - 1;

			}
			else {
				ghost.num_tiles_past_BFS++;
			}

			//if the ghost finished the whole path needed to catch the player last run -> move random.
			if (ghost.shortest_path_index == -1) {

				if (ghost.step_counts_rand % ghost.frames_per_tile == 0) {

					ghost.step_counts_rand = 0;
					random_direction(ghost.sprite, ghost.moving_direction);
				}
				ghost.step_counts_rand++;
			}
			else
			{
				tile next_tile = ghost.shortest_path[ghost.shortest_path_index];
				ghost.shortest_path_index--;
				int col_diff = col - next_tile.column;
				int row_diff = row - next_tile.row;

				//left
				if (col_diff == 1) {
					ghost.moving_direction = 2;
				}
				//right
				else if (col_diff == -1) {
					ghost.moving_direction = 0;
				}
				//up
				else if (row_diff == 1) {
					ghost.moving_direction = 1;
				}
				//down
				else {
					ghost.moving_direction = 3;
				}
			}
		}
		ghost.step_counts_BFS++;
}

void main() {

	RenderWindow window(VideoMode(WIDTH, HEIGH), "map test", Style::Default);
	window.setFramerateLimit(60);
	Texture ghost_texture;
	ghost_texture.loadFromFile("C:/programming/cpp/BFS sfml test/target.png");
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
	float player_y = (NUMROW - 1) * TILESIZE - 10;

	Sprite player;
	player.setOrigin((9), (9));
	player.setTexture(player_texture);
	player.setPosition(player_x, player_y);

	Sprite start_sprite;
	start_sprite.setTexture(start);
	start_sprite.setOrigin(9, 9);
	start_sprite.setPosition(3 * TILESIZE + TILESIZE / 2, 5 * TILESIZE + TILESIZE / 2);

	Sprite sprite_target;
	sprite_target.setTexture(target);
	sprite_target.setOrigin(9, 9);
	sprite_target.setPosition(TILESIZE + TILESIZE / 2, TILESIZE + TILESIZE / 2);

	Image icon;
	icon.loadFromFile("C:/programming/cpp/BFS sfml test/40px-04.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	int keyPressed = -1;
	int last_keyPressed = 1;

	ghosts[0].sprite.setTexture(ghost_texture);
	ghosts[0].speed = baseSpeed;
	ghosts[0].frames_per_tile = TILESIZE / ghosts[0].speed;
	ghosts[0].sprite.setOrigin(player_width / 2, player_height / 2);
	ghosts[0].sprite.setPosition(TILESIZE + TILESIZE / 2, TILESIZE + TILESIZE / 2);

	ghosts[0].algo_window_BFS = 10;
	ghosts[0].num_tiles_past_BFS = 10;
	ghosts[0].step_counts_BFS = 0;

	ghosts[0].step_counts_rand = 0;

	while (window.isOpen()) {

		float initial_x = (2 * TILESIZE) - 10;
		float initial_y = (NUMROW - 1) * TILESIZE - 10;

		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) {
				if (walls[i][j] == -1) {
					mp[i][j].type = tile_type::wall;
					mp[i][j].tile_sprite.setTexture(wall);
				}
				if (walls[i][j] == 0) {
					mp[i][j].type = tile_type::score;
					mp[i][j].tile_sprite.setTexture(none);
					mp[i][j].dot.setRadius(5.0f);
					mp[i][j].dot.setOrigin(5.0f, 5.0f);
					mp[i][j].dot.setPosition((j * TILESIZE + TILESIZE / 2), (i * TILESIZE + TILESIZE / 2));
				}
				if (walls[i][j] == 3) {
					mp[i][j].type = tile_type::none;
					mp[i][j].tile_sprite.setTexture(none);
				}
				mp[i][j].tile_sprite.setPosition((j * TILESIZE), (i * TILESIZE));
				mp[i][j].row = i;
				mp[i][j].column = j;
			}
		}

		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();

			if (Keyboard::isKeyPressed(Keyboard::Right))
				keyPressed = 0;

			if (Keyboard::isKeyPressed(Keyboard::Left))
				keyPressed = 2;

			if (Keyboard::isKeyPressed(Keyboard::Up))
				keyPressed = 1;

			if (Keyboard::isKeyPressed(Keyboard::Down))
				keyPressed = 3;
		}

		//handling the movement 

		float x_player = player.getPosition().x, y_player = player.getPosition().y;
		int row, col;
		get_tile_cor(x_player, y_player, row, col);

		change_direction(player, keyPressed, last_keyPressed, row, col);

		if (last_keyPressed == 0) {
			move_right(player, last_keyPressed);
		}
		else if (last_keyPressed == 1) {
			move_up(player, last_keyPressed);
		}
		else if (last_keyPressed == 2) {
			move_left(player, last_keyPressed);
		}
		else if (last_keyPressed == 3) {
			move_down(player, last_keyPressed);
		}

		//Score 
		get_tile_cor(x_player, y_player, row, col);

		if (mp[row][col].type == tile_type::score) {
			if (mp[row][col].dot.getGlobalBounds().contains(player.getPosition().x, player.getPosition().y)) {
				walls[row][col] = 3;
			}
		}

		//BFS <<--

		float x_start = start_sprite.getPosition().x, y_start = start_sprite.getPosition().y;
		int row_start, col_start;
		get_tile_cor(x_start, y_start, row_start, col_start);

		float x_target = sprite_target.getPosition().x, y_target = sprite_target.getPosition().y;
		int row_target, col_target;
		get_tile_cor(x_target, y_target, row_target, col_target);

		//step sount -> how many step/frame the ghost made.
		//frames per tile -> how many frames/steps needed to move a whole full tile.
		//algo window -> how many frames needed to repeat the algo.
		
		for (int i = 0; i < ghosts_number; i++) {

			catch_target(ghosts[i], player, initial_x, initial_y);

			if (ghosts[i].moving_direction == 0) {
				move_right(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 2) {
				move_left(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 1) {
				move_up(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 3) {
				move_down(ghosts[i].sprite, ghosts[i].moving_direction);
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

		for (int i = 0; i < ghosts_number; i++) {
			window.draw(ghosts[i].sprite);
		}
		window.draw(player);
		window.display();

	}
}
