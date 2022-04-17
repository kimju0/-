#include <bangtal.h>
#include <stdlib.h>
#include <time.h>

ObjectID startButton, endButton, bullet[10], fighter;
SceneID background;
SoundID sound;
TimerID bullet_make[10], bullet_move[10], bullet_reset;
int fighter_coordinate[2] = { 300, 550 }, bullet_x[10] = { 0,0,0,0,0,0,0,0,0,0 },
	bullet_y[10] = { 450, 450, 450, 450, 450, 450, 450, 450, 450, 450 }, a1, a2, b1, b2,
	bullet_num = 0, reset = 0, end = 0, bullet_speed = 1, stage_num = 0;
float make = 1.0f, move = 0.01f;
bool start = 0;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown == true)
		showObject(object);
	return object;
}

//fighter_coordinate[0] > 0 && fighter_coordinate[0] < 550 && fighter_coordinate[1] > 0 && fighter_coordinate[1] < 590

void fightermove(char move) {
	if (move == 'U') {
		fighter_coordinate[0] += 10;
		if (fighter_coordinate[0] > 580)
			fighter_coordinate[0] -= 10;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	}

	if (move == 'D') {
		fighter_coordinate[0] -= 10;
		if (fighter_coordinate[0] < 0)
			fighter_coordinate[0] += 10;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	}

	if (move == 'R') {
		fighter_coordinate[1] += 10;
		if (fighter_coordinate[1] > 570)
			fighter_coordinate[1] -= 10;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	}

	if (move == 'L') {
		fighter_coordinate[1] -= 10;
		if (fighter_coordinate[1] < 10)
			fighter_coordinate[1] += 10;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
	}
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

void bulletrandx() {
	showObject(bullet[bullet_num]);
	srand(time(NULL));
	bullet_x[bullet_num] = 0;
	switch (rand() % 5) {
	case 0:
		bullet_y[bullet_num] = 75;
		locateObject(bullet[bullet_num], background, bullet_x[bullet_num], bullet_y[bullet_num]);
		break;
	case 1:
		bullet_y[bullet_num] = 280;
		locateObject(bullet[bullet_num], background, bullet_x[bullet_num], bullet_y[bullet_num]);
		break;
	case 2: 
		bullet_y[bullet_num] = 380;
		locateObject(bullet[bullet_num], background, bullet_x[bullet_num], bullet_y[bullet_num]);
		break;
	case 3:
		bullet_y[bullet_num] = 480;
		locateObject(bullet[bullet_num], background, bullet_x[bullet_num], bullet_y[bullet_num]);
		break;
	case 4:
		bullet_y[bullet_num] = 582;
		locateObject(bullet[bullet_num], background, bullet_x[bullet_num], bullet_y[bullet_num]);
		break;
	}
	if (bullet_num == 9)
		end = 1;

}

void bulletmake(int x) {
	bullet_x[x] = 0;
	bulletrandx();
}

void collision(int x) {
	a1 = fighter_coordinate[1] - 30;
	a2 = fighter_coordinate[1] + 30;
	b1 = fighter_coordinate[0] - 30;
	b2 = fighter_coordinate[0] + 30;
	if (bullet_x[x] < a2 && bullet_x[x] > a1 && bullet_y[x] < b2 && bullet_y[x] > b1) {
		start = 0;
		showMessage("lose");
		bullet_speed /= 2;
		make *= 1.5;
		stage_num--;
		showObject(startButton);
		locateObject(endButton, background, 340, 150);
		
		startTimer(bullet_reset);
	}
}

void bulletmove(int x) {
	bullet_x[x] += bullet_speed;
	locateObject(bullet[x], background, bullet_x[x], bullet_y[x]);
	startTimer(bullet_move[x]);
	if (bullet_x[x] > 580) {
		hideObject(bullet[x]);
		if (end == 1 && bullet_x[9] > 570) {
			startTimer(bullet_reset);
			locateObject(endButton, background, 340, 150);
			showObject(startButton);
			if (stage_num == 1)
				showMessage("you win \n go to stage 2");
			else if (stage_num == 2)
				showMessage("you win \n go to stage 3");
			else if (stage_num == 3)
				showMessage("you win \n go to stage 4");
			else {
				showMessage("mission clear");
				locateObject(endButton, background, 300, 150);
				hideObject(startButton);
			}
		}
	}
}

void mouse(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton)
		endGame();
	else if (object == startButton) {
		stage_num++;
		end = 0;
		bullet_speed *= 2;
		make /= 1.5;
		start = 1;
		reset = 0;
		bullet_num = 0;
		hideObject(startButton);
		fighter_coordinate[0] = 300;
		fighter_coordinate[1] = 550;
		locateObject(fighter, background, fighter_coordinate[1], fighter_coordinate[0]);
		locateObject(endButton, background, 300, 100);
		bulletrandx();
		//starttimer
		{
			startTimer(bullet_make[0]);
			
			startTimer(bullet_move[0]);
			startTimer(bullet_move[1]);
			startTimer(bullet_move[2]);
			startTimer(bullet_move[3]);
			startTimer(bullet_move[4]);
			startTimer(bullet_move[5]);
			startTimer(bullet_move[6]);
			startTimer(bullet_move[7]);
			startTimer(bullet_move[8]);
			startTimer(bullet_move[9]);
		}
	}
}

void timer(TimerID timer) {
	if (start == 1) {

		if (timer == bullet_move[0]) {
			bulletmove(0);
			collision(0);
		}
		else if (timer == bullet_move[1]) {
			bulletmove(1);
			collision(1);
		}
		else if (timer == bullet_move[2]) {
			bulletmove(2);
			collision(2);
		}
		else if (timer == bullet_move[3]) {
			bulletmove(3);
			collision(3);
		}
		else if (timer == bullet_move[4]) {
			bulletmove(4);
			collision(4);
		}
		else if (timer == bullet_move[5]) {
			bulletmove(5);
			collision(5);
		}
		else if (timer == bullet_move[6]) {
			bulletmove(6);
			collision(6);
		}
		else if (timer == bullet_move[7]) {
			bulletmove(7);
			collision(7);
		}
		else if (timer == bullet_move[8]) {
			bulletmove(8);
			collision(8);
		}
		else if (timer == bullet_move[9]) {
			bulletmove(9);
			collision(9);
		}

		if (timer == bullet_make[0]) {
			bullet_num++;
			startTimer(bullet_make[1]);
			bulletmake(1);
		}
		else if (timer == bullet_make[1]) {
			bullet_num++;
			startTimer(bullet_make[2]);
			bulletmake(2);
		}
		else if (timer == bullet_make[2]) {
			bullet_num++;
			startTimer(bullet_make[3]);
			bulletmake(3);
		}
		else if (timer == bullet_make[3]) {
			bullet_num++;
			startTimer(bullet_make[4]);
			bulletmake(4);
		}
		else if (timer == bullet_make[4]) {
			bullet_num++;
			startTimer(bullet_make[5]);
			bulletmake(5);
		}
		else if (timer == bullet_make[5]) {
			bullet_num++;
			startTimer(bullet_make[6]);
			bulletmake(6);
		}
		else if (timer == bullet_make[6]) {
			bullet_num++;
			startTimer(bullet_make[7]);
			bulletmake(7);
		}
		else if (timer == bullet_make[7]) {
			bullet_num++;
			startTimer(bullet_make[8]);
			bulletmake(8);
		}
		else if (timer == bullet_make[8]) {
			bullet_num++;
			startTimer(bullet_make[9]);
			bulletmake(9);
		}

	}
	if (timer == bullet_reset && reset<10 ) {
		stopTimer(bullet_move[reset]);
		stopTimer(bullet_make[reset]);
		setTimer(bullet_make[reset], make);

		bullet_x[reset] = 0;
		bullet_y[reset] = 0;
		locateObject(bullet[reset], background, bullet_x[reset], bullet_y[reset]);
		hideObject(bullet[reset]);
		reset++;
		startTimer(bullet_reset);
		}
}

int main() {
	// setGameOption();
	{
		setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
		setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
		setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	}

	setMouseCallback(mouse);
	setKeyboardCallback(keyboard);
	setTimerCallback(timer);

	background = createScene("background", "image/background.jpg");

	startButton = createObject("image/start.png", background, 250, 150, true);
	endButton = createObject("image/end.png", background, 340, 150, true);
	fighter = createObject("image/jet-fighter.png", background, fighter_coordinate[1], fighter_coordinate[0], true);
	
	//bullet[] = createObject("image/bullet.png", background, bullet_x[], bullet_y[], false);
	{
		bullet[0] = createObject("image/bullet.png", background, bullet_x[0], bullet_y[0], false);
		bullet[1] = createObject("image/bullet.png", background, bullet_x[1], bullet_y[1], false);
		bullet[2] = createObject("image/bullet.png", background, bullet_x[2], bullet_y[2], false);
		bullet[3] = createObject("image/bullet.png", background, bullet_x[3], bullet_y[3], false);
		bullet[4] = createObject("image/bullet.png", background, bullet_x[4], bullet_y[4], false);
		bullet[5] = createObject("image/bullet.png", background, bullet_x[5], bullet_y[5], false);
		bullet[6] = createObject("image/bullet.png", background, bullet_x[6], bullet_y[6], false);
		bullet[7] = createObject("image/bullet.png", background, bullet_x[7], bullet_y[7], false);
		bullet[8] = createObject("image/bullet.png", background, bullet_x[8], bullet_y[8], false);
		bullet[9] = createObject("image/bullet.png", background, bullet_x[9], bullet_y[9], false);
	}

	//bullet_move[]=createTimer(0.1f);  bullet_make[] = createTimer(2.0f);
	{
		bullet_move[0] = createTimer(move);
		bullet_make[0] = createTimer(make);
		bullet_move[1] = createTimer(move);
		bullet_make[1] = createTimer(make);
		bullet_move[2] = createTimer(move);
		bullet_make[2] = createTimer(make);
		bullet_move[3] = createTimer(move);
		bullet_make[3] = createTimer(make);
		bullet_move[4] = createTimer(move);
		bullet_make[4] = createTimer(make);
		bullet_move[5] = createTimer(move);
		bullet_make[5] = createTimer(make);
		bullet_move[6] = createTimer(move);
		bullet_make[6] = createTimer(make);
		bullet_move[7] = createTimer(move);
		bullet_make[7] = createTimer(make);
		bullet_move[8] = createTimer(move);
		bullet_make[8] = createTimer(make);
		bullet_move[9] = createTimer(move);
		bullet_make[9] = createTimer(make);
	}

	bullet_reset = createTimer(0.01f);

	startGame(background);
}