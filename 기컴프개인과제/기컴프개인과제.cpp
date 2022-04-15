#include <bangtal.h>
#include <stdlib.h>
#include <time.h>

ObjectID startButton, endButton, bullet, fighter;
SceneID background;
SoundID sound;
TimerID bullet_move, bullet_make;
int fighter_coordinate[2] = { 450, 1150 }, bullet_x = 0, bullet_y = 450, maketime = 0, a1,a2, b1, b2;
bool start = 0;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown)
		showObject(object);
	return object;
}

void bulletmake() {
	startTimer(bullet_make);
}

void bulletmove(int x) {
	bullet_x += x;
	locateObject(bullet, background, bullet_x, bullet_y);
	startTimer(bullet_move);
}

void fightermove(char move) {
	if (move == 'U')
		fighter_coordinate[0]+= 10;
	locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	if (move == 'D')
		fighter_coordinate[0]-= 10;
	locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	if (move == 'R')
		fighter_coordinate[1]+= 10;
	locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	if (move == 'L')
		fighter_coordinate[1]-= 10;
	locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
}

void keyboard(KeyCode key, KeyState state) {
	if (start) {
		if (key == KeyCode::KEY_UP_ARROW)
			fightermove('U');
		else if (key == KeyCode::KEY_DOWN_ARROW)
			fightermove('D');
		else if (key == KeyCode::KEY_RIGHT_ARROW)
			fightermove('R');
		else if (key == KeyCode::KEY_LEFT_ARROW)
			fightermove('L');
	}
}

void bulletrandx(int maketime) {

	srand(time(NULL));
	switch (rand() % 5) {
	case 0: {
		bullet_y = 75;
		locateObject(bullet, background, bullet_x, bullet_y);
	} break;
	case 1: {
		bullet_y = 244;
		locateObject(bullet, background, bullet_x, bullet_y);
	} break;
	case 2: {
		bullet_y = 413;
		locateObject(bullet, background, bullet_x, bullet_y);
	} break;
	case 3: {
		bullet_y = 582;
		locateObject(bullet, background, bullet_x, bullet_y);
	} break;
	case 4: {
		bullet_y = 730;
		locateObject(bullet, background, bullet_x, bullet_y);
	} break;
	}
}

void mouse(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton)
		endGame();
	else if (object == startButton) {
		start = 1;
		hideObject(startButton);
		bullet_x = 0;
		fighter_coordinate[0] = 450;
		fighter_coordinate[1] = 1150;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
		bulletrandx(maketime);
		startTimer(bullet_move);
		startTimer(bullet_make);
	}
}

void collision() {
	a1 = fighter_coordinate[1] - 50;
	a2 = fighter_coordinate[1] + 50;
	b1 = fighter_coordinate[0] - 50;
	b2 = fighter_coordinate[0] + 50;
	if (bullet_x < a2 && bullet_x > a1 && bullet_y < b2 && bullet_y > b1) {
		start = 0;
		showMessage("lose");
		showObject(startButton);
	}
}

void timer(TimerID timer) {
	if (start == 1) {
		if (timer == bullet_move) {
			bulletmove(1);
			collision();
		}
		else if (timer == bullet_make) {
			bulletmake();
		}
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouse);
	setKeyboardCallback(keyboard);
	setTimerCallback(timer);

	background = createScene("background", "image/field.jpg");

	startButton = createObject("image/start.png", background, 600, 150, true);
	endButton = createObject("image/end.png", background, 700, 150, true);
	bullet = createObject("image/bullet.png", background, bullet_x, bullet_y, true);
	fighter = createObject("image/jet-fighter.png", background, fighter_coordinate[1], fighter_coordinate[0], true);

	bullet_move = createTimer(0.1f);
	bullet_make = createTimer(1.0f);

	startGame(background);
}