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
#define baseSpeed 2

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

bool exist_in_closed(tile* tile, vector <struct tile>& closed) {
	bool ans = 0;
	for (int i = 0; i < closed.size(); i++) {

		if (tile->row == closed[i].row && tile->column == closed[i].column)
			ans = 1;
	}
	return ans;
}

//function returns the row and col of the tile  
void get_tile_cor(float x, float y, int& row, int& col) {
	col = (int)x / TILESIZE;
	row = (int)y / TILESIZE;
	//if ((int)x % 20 == 0)
	//	col = ((int)x / TILESIZE) - 1;
	//if ((int)y % 20 == 0)
	//	row = ((int)y / TILESIZE) - 1;
}

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

//positive and negative diff in these functions exist since the player sprite
//is not the same pixel size in width and height as the tile so i want to leave an equal distance
//between the wall and the player while colliding.

void move_right(Sprite& sprite, int& lastKeyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_horz(sprite);

	int row, col;

	get_tile_cor(x + (player_width / 2) + baseSpeed +0.0001 , y, row, col);
	if (mp[row][col].type == tile_type::wall) {
		if ((col * TILESIZE) - (x + (player_width / 2) + diff) > 0) {

			if (condition_1 && condition_2)
			{
				sprite.move((col * TILESIZE) - (x - (player_width / 2)), 0);
				lastKeyPressed = 1;
			}
		}
		else {
			condition_2 = false, lastKeyPressed = -1;
		}
	}
	else {
		if (condition_1 && condition_2) 
		{
			sprite.move(baseSpeed, 0);
			lastKeyPressed = 1;
		}
	}
}
void move_left(Sprite& sprite ,int & last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_horz(sprite);

	int row, col;


	get_tile_cor(x - ((player_width / 2)) - baseSpeed - 0.001 , y, row, col);

	if (mp[row][col].type == tile_type::wall)
	{
		if ((x - (player_width / 2) - diff) - ((col * TILESIZE) + TILESIZE) > 0)
		{
			sprite.move(-((x - (player_width / 2)) - ((col * TILESIZE) + TILESIZE)), 0); 
			last_keyPressed = 2;
		}
		else
		{
			condition_2 = false , last_keyPressed = -1;
		}
	}

	else {
		if (condition_1 && condition_2) {
			sprite.move(-baseSpeed, 0);
			last_keyPressed = 2;
		}
	}
}
void move_up(Sprite& sprite , int & last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_vert(sprite);

	int row, col;

	get_tile_cor(x, y - baseSpeed - (player_height / 2) - 0.001 , row, col);

	if (mp[row][col].type == tile_type::wall) {
		if (x, (y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE) > 0) {
			sprite.move(0, -((y - (player_height / 2) - diff) - (row * TILESIZE + TILESIZE)));
			last_keyPressed = 3;
		}
		else
			condition_2 = false, last_keyPressed = -1;
	}
	else
		if (condition_1 && condition_2)
			sprite.move(0, -baseSpeed), last_keyPressed = 3;
}
void move_down(Sprite& sprite , int& last_keyPressed) {
	float y = sprite.getPosition().y, x = sprite.getPosition().x;
	bool condition_1 = false, condition_2 = true;

	condition_1 = same_tile_vert(sprite);

	int row, col;

	get_tile_cor(x, y + baseSpeed + (player_height / 2) + 0.001, row, col);
	if (mp[row][col].type == tile_type::wall) {

		if ((row * TILESIZE) - (y + (player_height / 2) + diff) > 0)
		{
			sprite.move(0, (row * TILESIZE) - (y + (player_height / 2) + diff));
			last_keyPressed = 4;
		}
		else
			condition_2 = false;
	}
	else
		if (condition_1 && condition_2) sprite.move(0, baseSpeed), last_keyPressed = 4;
}

void check_tiles(Sprite& sprite, int& keyPressed , int&lastKeyPressed , int row, int col) {
	
	//right
	if (keyPressed == 1) {
		if (same_tile_horz(sprite))
		{
			if (mp[row][col + 1].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
				//sprite.move(baseSpeed, 0);
			}
		}
	}
	//left
	if (keyPressed == 2) {
		if (same_tile_horz(sprite))
		{
			if (mp[row][col - 1].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
				//move_left(sprite, lastKeyPressed);
				//sprite.move(- baseSpeed, 0);
			}
		}
	}
	//up
	if (keyPressed == 3) {
		if (same_tile_vert(sprite))
		{
			if (mp[row - 1][col].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
				//move_up(sprite, lastKeyPressed);
			}
		}
	}
	//down
	if (keyPressed == 4) {
		if (same_tile_vert(sprite))
		{
			if (mp[row + 1][col].type != tile_type::wall) {
				lastKeyPressed = keyPressed;
				//move_down(sprite, lastKeyPressed);
			}
		}
	}
}
//
//void catch_target(vector <struct tile> get_path, Sprite& start) {
//	
//		for (int i = get_path.size() - 1; i >= 0; i--) {
//			//right
//			int row, col;
//			float x = start.getPosition().x, y = start.getPosition().y;
//			get_tile_cor(x, y, row, col);
//			while ((get_path[i].tile_sprite.getPosition().x-10 != start.getPosition().x) && (get_path[i].tile_sprite.getPosition().y != start.getPosition().y-10))
//			{
//				
//				if (get_path[i].column > col) {
//					move_right(start);
//				}
//				//left
//				if (get_path[i].column < col) {
//					move_left(start);
//				}
//				//up
//				if (get_path[i].row < row) {
//					move_up(start);
//				}
//				//down
//				if (get_path[i].row > row) {
//					move_down(start);
//				}
//			}
//		}
//	}
//

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

const int ghosts_number = 1;
struct ghoust {

	
	Sprite sprite;
	int algo_window;
	float speed;
	int frames_per_tile;
	int step_counts;
	vector<tile> shortest_path;
	int shortest_path_index;
	int moving_direction;
	int num_tiles_past;

}ghosts[ghosts_number];

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
	float px_per_frame = 0.33;

	// 0 for empty space
	// 1 for target
	// 2 for start
	// -1 for wall 
	Sprite start_sprite;
	start_sprite.setTexture(start);
	start_sprite.setOrigin(9, 9);
	start_sprite.setPosition(3 * TILESIZE + TILESIZE/2 ,5 * TILESIZE + TILESIZE/2);

	Sprite sprite_target;
	sprite_target.setTexture(target);
	sprite_target.setOrigin(9, 9);
	sprite_target.setPosition(TILESIZE + TILESIZE / 2, TILESIZE + TILESIZE / 2);



	int keyPressed = -1;
	int last_keyPressed = 1;


	ghosts[0].sprite.setTexture(ghost_texture);
	ghosts[0].algo_window = 15;
	ghosts[0].num_tiles_past = 15;
	ghosts[0].speed = 2;
	ghosts[0].frames_per_tile = TILESIZE / ghosts[0].speed;
	ghosts[0].step_counts = 0;
	ghosts[0].sprite.setOrigin(9, 9);
	ghosts[0].sprite.setPosition(30, 30);


	while (window.isOpen()) {

		for (int i = 0; i < NUMROW; i++) {
			for (int j = 0; j < NUMCOL; j++) {
				if (walls[i][j] == -1) {
					mp[i][j].type = tile_type::wall;
					mp[i][j].tile_sprite.setTexture(wall);
				}
				/*else if (walls[i][j] == 1) {
					mp[i][j].type = tile_type::target;
					mp[i][j].tile_sprite.setTexture(target);
				}*/
				/*else if (walls[i][j] == 2) {
					mp[i][j].type = tile_type::start;
					mp[i][j].tile_sprite.setTexture(start);
				}*/
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
				keyPressed = 1;

			if (Keyboard::isKeyPressed(Keyboard::Left))
				keyPressed = 2;

			if (Keyboard::isKeyPressed(Keyboard::Up)) 
				keyPressed = 3;

			if (Keyboard::isKeyPressed(Keyboard::Down)) 
				keyPressed = 4;
		}
		//handling the movement 

		float x_player = player.getPosition().x, y_player = player.getPosition().y;
		int row, col;
		get_tile_cor(x_player, y_player, row, col);

		check_tiles(player, keyPressed, last_keyPressed , row ,col);

		if (last_keyPressed == 1) {
			move_right(player, last_keyPressed);
		}
		if (last_keyPressed == 2) {
			move_left(player, last_keyPressed);
		}
		if (last_keyPressed == 3) {
			move_up(player, last_keyPressed);
		}
		if (last_keyPressed == 4) {
			move_down(player, last_keyPressed);
		}

		//Score 
		
		get_tile_cor(x_player ,y_player, row, col);

		if (mp[row][col].type == tile_type::score) {
			if (mp[row][col].dot.getGlobalBounds().contains(player.getPosition().x, player.getPosition().y)) {
				walls[row][col] = 3;
			}
		}

		//BFS 
		float x_start = start_sprite.getPosition().x, y_start = start_sprite.getPosition().y;
		int row_start, col_start;
		get_tile_cor(x_start, y_start, row_start, col_start);

		float x_target = sprite_target.getPosition().x, y_target = sprite_target.getPosition().y;
		int row_target, col_target;
		get_tile_cor(x_target, y_target, row_target, col_target);


		for (int i = 0; i < ghosts_number; i++) {

			//col
			if (player.getGlobalBounds().contains(ghosts[i].sprite.getPosition().x, ghosts[i].sprite.getPosition().y)) {
				ghosts[0].algo_window = 15;
				ghosts[0].num_tiles_past = 15;
				ghosts[0].frames_per_tile = TILESIZE / ghosts[0].speed;
				ghosts[0].step_counts = 0;
				ghosts[0].sprite.setPosition(30, 30);
				player.setPosition(player_x, player_y);
			}

			if (ghosts[i].step_counts % ghosts[i].frames_per_tile == 0 ) {
				int row, col;
				float x = ghosts[i].sprite.getPosition().x,
					  y = ghosts[i].sprite.getPosition().y;
				get_tile_cor(x, y, row, col);
				ghosts[i].step_counts = 0;
				if (ghosts[i].num_tiles_past == ghosts[i].algo_window || ghosts[i].shortest_path_index == -1) {
					int row_1, col_1;
					float x_1 = player.getPosition().x,
					y_1 = player.getPosition().y;

				
					get_tile_cor(x_1, y_1, row_1, col_1);
					tile* start_pointer = &mp[row][col];
					start_pointer->parent = NULL;
					tile* target_pointer = &mp[row_1][col_1];

					find_optimal_path(start_pointer, target_pointer, &ghosts[i].shortest_path);
					ghosts[i].num_tiles_past = 0;
					ghosts[i].shortest_path_index = ghosts[i].shortest_path.size()-1;

				}
				else {
					ghosts[i].num_tiles_past++;
				}
				
				tile next_tile = ghosts[i].shortest_path[ghosts[i].shortest_path_index];
				ghosts[i].shortest_path_index--;
				int col_diff = col - next_tile.column;
				int row_diff = row - next_tile.row;
				if (col_diff == 1) {
					ghosts[i].moving_direction = 2;
				}
				else if (col_diff == -1) {
					ghosts[i].moving_direction = 1;

				}
				else if (row_diff == 1) {
					ghosts[i].moving_direction = 3;

				}
				else {
					ghosts[i].moving_direction = 4;
				}
			}
			
			if (ghosts[i].moving_direction == 1) {
				move_right(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 2) {
				move_left(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 3) {
				move_up(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			else if (ghosts[i].moving_direction == 4) {
				move_down(ghosts[i].sprite, ghosts[i].moving_direction);
			}
			
			ghosts[i].step_counts++;

		}

	

		//find_optimal_path(start_pointer, target_pointer, &get_path);

		//catch_target(get_path, start_sprite);

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
		//window.draw(start_sprite);
		//window.draw(sprite_target);
		window.display();

	}
}


