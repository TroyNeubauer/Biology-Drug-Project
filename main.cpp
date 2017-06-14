
#include "src/troyberry.h"
#include "sprite.h"
#include "drugs\drug.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <stack>
#include <ctime>
#include <vector>

#define NORTH   0
#define SOUTH   1
#define EAST    2
#define WEST    3

using namespace std;
using namespace troyberry;
using namespace graphics;
using namespace maths;

#define BOARD_WIDTH 48
#define BOARD_HEIGHT 36

#define FRAME_WIDTH 16.0f
#define FRAME_HEIGHT 9.0f

int nGood = 0;
int locX = 1, locY = 1, playerX = 1, playerY = 1, endX = BOARD_WIDTH - 2, endY = BOARD_HEIGHT - 2;
double moveTimer = 0.0, moveSpeed = 0.1;
Renderable2D* player;
Renderable2D* endSprite;
vector<Drug> drugs;
bool won = false;

char* grid = new char[BOARD_HEIGHT * BOARD_WIDTH];

void setPlayer() {
	delete player;
	player = new Sprite(playerX * FRAME_WIDTH / BOARD_WIDTH, playerY * FRAME_HEIGHT / BOARD_HEIGHT, FRAME_WIDTH / BOARD_WIDTH, FRAME_HEIGHT / BOARD_HEIGHT, maths::vec4(0, 0, 1, 1));
	delete endSprite;
	endSprite = new Sprite(endX * FRAME_WIDTH / BOARD_WIDTH, endY * FRAME_HEIGHT / BOARD_HEIGHT, FRAME_WIDTH / BOARD_WIDTH / 2.0f, FRAME_HEIGHT / BOARD_HEIGHT / 2.0f, maths::vec4(1, 0, 1, 1));
}

void printGrid(char* grid) {
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			cout << (grid[x + y * BOARD_WIDTH]);
		}
		cout << endl;
	}
}

int moveEW(int direction, int x) {
	if (direction == EAST)
		return x + 1;
	else if (direction == WEST)
		return x - 1;
	else
		return x;
}

int moveNS(int direction, int y) {
	if (direction == NORTH)
		return y - 1;
	else if (direction == SOUTH)
		return y + 1;
	else
		return y;
}

bool isGood(int x, int y, int direction, char* grid) {
	x = moveEW(direction, x);
	y = moveNS(direction, y);

	if (grid[x + y * BOARD_WIDTH] == '.' || x >= (BOARD_WIDTH - 1) || x <= 0 || y <= 0 || y >= (BOARD_HEIGHT - 1)) {
		return false;
	}

	// check cardinal directions
	if (direction == NORTH) {
		if (grid[x - 1 + y * BOARD_WIDTH] != '.' &&  grid[x + (y - 1) * BOARD_WIDTH] != '.' && grid[x + 1 + y * BOARD_WIDTH] != '.' &&  grid[(x - 1) + (y - 1) * BOARD_WIDTH] != '.' && grid[(x + 1) + (y - 1) * BOARD_WIDTH] != '.') {
			return true;
		}
	}
	if (direction == SOUTH) {
		if (grid[x - 1 + y * BOARD_WIDTH] != '.' &&  grid[x + (y + 1) * BOARD_WIDTH] != '.' && grid[x + 1 + y * BOARD_WIDTH] != '.' && grid[x - 1 + (y + 1) * BOARD_WIDTH] != '.' && grid[x + 1 + (y + 1) * BOARD_WIDTH] != '.') {
			return true;
		}
	}
	if (direction == EAST) {
		if (grid[x + 1 + y * BOARD_WIDTH] != '.' &&  grid[x + (y - 1) * BOARD_WIDTH] != '.' && grid[x + (y + 1) * BOARD_WIDTH] != '.' && grid[x + 1 + (y - 1) * BOARD_WIDTH] != '.' && grid[x + 1 + (y + 1) * BOARD_WIDTH] != '.') {
			return true;
		}
	}
	if (direction == WEST) {
		if (grid[x - 1 + y * BOARD_WIDTH] != '.' &&  grid[x + (y - 1) * BOARD_WIDTH] != '.' && grid[x + (y + 1) *BOARD_WIDTH] != '.' && grid[x - 1 + (y - 1) * BOARD_WIDTH] != '.' && grid[x - 1 + (y + 1) * BOARD_WIDTH] != '.') {
			return true;
		}
	}
	return false;
}

bool move(int x, int y) {
	if (won) return false;
	double totalMoveSpeed = moveSpeed;
	for (unsigned int i = 0; i < drugs.size(); i++) {
		totalMoveSpeed /= drugs[i].getSpeed();
	}
	if (moveTimer > totalMoveSpeed) {
		int newX = playerX + x;
		int newY = playerY + y;
		if (grid[newX + newY * BOARD_WIDTH] == '.') {
			moveTimer = 0.0;
			playerX = newX;
			playerY = newY;
			return true;
		}
	}
	return false;
}


int main()
{
	Timer mazeTimer;

	// init grid
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			grid[x + y * BOARD_WIDTH] = '#';
		}
	}

	//init rand
	srand((unsigned int)time(0));

	//init stacks for xy coords
	stack<int> xValues;
	stack<int> yValues;

	nGood = 0;
	int direction = 0;

	do {
		//find n good moves
		for (int i = 0; i < 4; i++) {
			if (isGood(locX, locY, i, grid))
				nGood++;
		}

		// if only 1 good move, move there
		if (nGood == 1) {
			if (isGood(locX, locY, NORTH, grid))
				locY = moveNS(NORTH, locY);
			else if (isGood(locX, locY, SOUTH, grid))
				locY = moveNS(SOUTH, locY);
			else if (isGood(locX, locY, EAST, grid))
				locX = moveEW(EAST, locX);
			else if (isGood(locX, locY, WEST, grid))
				locX = moveEW(WEST, locX);
		}

		// if no good moves, move back in stack
		else if (nGood == 0) {
			locX = xValues.top();
			locY = yValues.top();
			xValues.pop();
			yValues.pop();
		}

		//if more than 1 good move, push stack
		else if (nGood > 1) {
			xValues.push(locX);
			yValues.push(locY);

			//direction to move randomly chosen
			do {
				direction = rand() % 4;
			} while (!isGood(locX, locY, direction, grid));

			locX = moveEW(direction, locX);
			locY = moveNS(direction, locY);
		}

		// set grid
		grid[locX + locY * BOARD_WIDTH] = '.';
		nGood = 0;

	} while (!xValues.empty());

	int minRandX = endX - 1, minRandY = endY - 1;
	while (grid[endX + endY * BOARD_WIDTH] == '#') {
		printf("Re calculating thing");
		endX = (int)((rand() % 1000000 / 1000000.0) * (BOARD_WIDTH - minRandX)) + minRandX;
	}
	//printGrid(grid);
	cout << "Maze Generation Time: " << mazeTimer.elapsedMillis() << " Millis Seconds" << endl;
	string drugName, strength;
	do {

		getline(cin, drugName);
		if (drugName == "done") break;
		printf("Ok, what strength?\n");
		getline(cin, strength);
		double stren = stod(strength);
		if ((drugName == "caffeine") || (drugName == "caffine")) {
			Drug drug1(2.0, stren, CAFFEINE_ID);
			drugs.push_back(drug1);
			printf("adding caffeine strength %f\n", stren);
		}else if ((drugName == "cocaine") || (drugName == "cocane")) {
			stren *= 3.0;
			Drug drug1(3.0, stren, COCAINE_ID);
			drugs.push_back(drug1);
			printf("adding cocaine strength %f\n", stren);
		} else if ((drugName == "sucrose") || (drugName == "surcose")) {
				Drug drug1(2.0, stren, SUCROSE_ID);
				drugs.push_back(drug1);
				printf("adding sucrose strength %f\n", stren);
		}else if ((drugName == "alcohol") || (drugName == "alchocol")) {
			stren *= 1.5;
			Drug drug1(0.5, stren, ALCOHOL_ID);
			drugs.push_back(drug1);
			printf("adding alcohol strength %f\n", stren);
		}
		else if ((drugName == "cocaine") || (drugName == "cocane")) {
			stren *= 2.0;
			Drug drug1(3.0, stren, COCAINE_ID);
			drugs.push_back(drug1);
			printf("adding cocaine strength %f\n", stren);
		}
		else {
			printf("Unknown drug: %s", drugName.c_str());
		}

	} while (true);

	Window window("Biology Test", 1280, 710);

	mat4 ortho = mat4::orthographic(0.0f, FRAME_WIDTH, FRAME_HEIGHT, 0.0f, -1.0f, 1.0f);
	mat4 matrix = mat4::orthographic(0.0f, FRAME_WIDTH, FRAME_HEIGHT, 0.0f, -1.0f, 1.0f);

	Shader shader("basic.vert", "basic.frag");
	shader.enable();
	shader.setUniformMat4("pr_matrix", ortho);

	Renderable2D* sprites[BOARD_WIDTH * BOARD_HEIGHT];

	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			maths::vec4 color;
			if (grid[x + y * BOARD_WIDTH] == '.') {
				color = maths::vec4(0, 0, 0, 1);
			}
			else {
				color = maths::vec4(rand() % 1000 / 2000.0f + 0.75f, rand() % 1000 / 2000.0f + 0.75f, rand() % 1000 / 2000.0f + 0.75f, 1);
			}
			sprites[x + y * BOARD_WIDTH] = new Sprite(x * FRAME_WIDTH / BOARD_WIDTH, y * FRAME_HEIGHT / BOARD_HEIGHT, FRAME_WIDTH / BOARD_WIDTH, FRAME_HEIGHT / BOARD_HEIGHT, color);
		}
	}
	setPlayer();

	BatchRenderer2D renderer;

	Timer timer;
	double total = 0;
	int frames = 0;
	while (!window.closed())
	{
		timer.reset();
		window.clear();
		renderer.begin();
		bool moved = false;
		if (window.isKeyPressed(GLFW_KEY_W))
		{
			moved |= move(0, -1);
		}
		if (window.isKeyPressed(GLFW_KEY_S))
		{
			moved |= move(0, 1);
		}
		if (window.isKeyPressed(GLFW_KEY_A))
		{
			moved |= move(-1, 0);
		}
		if (window.isKeyPressed(GLFW_KEY_D))
		{
			moved |= move(1, 0);
		}
		if (moved) moveTimer = 0.0;
		setPlayer();
		won |= playerX == endX && playerY == endY;
		for (unsigned int i = 0; i < drugs.size(); i++) {
			drugs[i].change(matrix);
		}
		std::cout << drugs.size() << std::endl;

		for (unsigned int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
		{
			renderer.submit(sprites[i]);
		}
		renderer.submit(player);
		renderer.submit(endSprite);
		renderer.end();
		if(won)
			shader.setUniformMat4("pr_matrix", ortho);
		else
			shader.setUniformMat4("pr_matrix", matrix);
		renderer.flush();
		window.update();
		frames++;
		double elapsedNanos = timer.elapsedNanos();
		moveTimer += timer.elapsedSeconds();
		total += elapsedNanos;
		if (total > 1000000000.0) {
			//std::cout << frames << " fps" << std::endl;
			total = 0.0;
			frames = 0;
		}
	}

	window.close();
	delete grid;
	delete player;
	delete endSprite;
	return 0;
}