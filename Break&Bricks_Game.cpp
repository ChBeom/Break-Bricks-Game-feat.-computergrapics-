#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#include <math.h>
#include<iostream>
#include<ctime>

#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>
#include <Digitalv.h>

#define STB_IMAGE_IMPLEMENTATION

using namespace std;
#pragma warning(disable: 4996)

#define	PI				3.1415
#define	polygon_num		50

GLfloat		winHeight = 781.0;
GLfloat		winWidth = 940.0;
GLfloat		ball_xspeed = 0.0;
GLfloat		ball_yspeed = 0.0;

int			down_count = 0;
int			start_count; //���ۿ��� Ȯ��

struct _Point {
	float	x;
	float	y;
};

struct Color {
	float	r;
	float	g;
	float	b;
};

struct Ball {
	_Point	center;
	int		collision_count;
	float	radius;
	Color	color;
};

struct Block {
	_Point l_d;  // ���� �Ʒ�
	_Point l_u;  // ���� ��
	_Point r_d;  // ������ �Ʒ�
	_Point r_u;  // ������ ��
	Color	b_color; //���� 
	int		coll_count;  //�浹 ī��Ʈ
};

Ball fixed_ball, moving_ball, moving_balls;
Block stick_bar, stick_bars;
_Point	velocity, left_down, left_up, right_up, right_down;
Block block[1000][10];
int		block_num, block_rows, block_rows_num;
Color backgroud;
int rand_num;

int s_x, s_y;
int b_x, b_y;
int l_x, l_y;
int a_x, a_y;

int score_n;
char score[24];
int life_n;
char life[24];
int xx, yy;

MCI_OPEN_PARMS openBgm;
MCI_PLAY_PARMS playBgm;
MCI_OPEN_PARMS opensound1;
#define BGM "sound/background_music.mp3"
#define SOUND "sound/ball_col.mp3"
int dwID;

// ���� ���� -----------------------------------------------------------------------------------

//�� �ʱ�ȭ �Լ�
void init_block(void) {
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 6; j++) {
			block[i][j].coll_count = 0;
			block[i][j].b_color.r = 0.38;
			block[i][j].b_color.g = 0.86;
			block[i][j].b_color.b = 0.85;
		}
	}
	for (int i = 50; i < 100; i++) {
		for (int j = 0; j < 6; j++) {
			block[i][j].coll_count = 0;
			block[i][j].b_color.r = 0.38;
			block[i][j].b_color.g = 0.86;
			block[i][j].b_color.b = 0.85;
		}
	}
	for (int i = 100; i < 200; i++) {
		for (int j = 0; j < 6; j++) {
			block[i][j].coll_count = 0;
			block[i][j].b_color.r = 0.38;
			block[i][j].b_color.g = 0.86;
			block[i][j].b_color.b = 0.85;
		}
	}
	block[rand_num][3].b_color.r = 1.0;
	block[rand_num][3].b_color.g = 1.0;
	block[rand_num][3].b_color.b = 0.5;
}

// �ʱ�ȭ �Լ�
void init(void) {
	//��� ��
	backgroud.r = 1.0;
	backgroud.g = 1.0;
	backgroud.b = 1.0;
	//----------------------------------------------------
	//��輱 ��ǥ��
	left_down.x = -10;
	left_down.y = -200;

	right_down.x = 781;
	right_down.y = -200;

	left_up.x = -10;
	left_up.y = 810;

	right_up.x = 781;
	right_up.y = 810;

	//----------------------------------------------------
		//����� ��ǥ  // ���� 150, ���� 20
	stick_bar.l_d.x = 325;
	stick_bar.l_u.x = 325;
	stick_bar.r_d.x = 475;
	stick_bar.r_u.x = 475;
	stick_bar.l_d.y = 130;
	stick_bar.r_d.y = 130;
	stick_bar.l_u.y = 150;
	stick_bar.r_u.y = 150;

	//�׵θ�
	stick_bars.l_d.x = stick_bar.l_d.x - 2.5;
	stick_bars.l_u.x = stick_bar.l_u.x - 2.5;
	stick_bars.r_d.x = stick_bar.r_d.x + 2.5;
	stick_bars.r_u.x = stick_bar.r_u.x + 2.5;
	stick_bars.l_d.y = stick_bar.r_d.y - 2.5;
	stick_bars.r_d.y = stick_bar.l_d.y - 2.5;
	stick_bars.l_u.y = stick_bar.l_u.y + 2.5;
	stick_bars.r_u.y = stick_bar.r_u.y + 2.5;
	//-----------------------------------------------------
		// �� ����
	start_count = 0;

	fixed_ball.center.x = 750 / (time(NULL) % 10);  //���� �� ��ġ
	fixed_ball.center.y = winHeight / 2;

	moving_ball.center.x = ((stick_bars.r_u.x - stick_bars.l_u.x) / 2) + stick_bars.l_u.x;  //�� ��ġ
	moving_ball.center.y = stick_bars.l_u.y + 10;

	moving_balls.center.x = ((stick_bars.r_u.x - stick_bars.l_u.x) / 2) + stick_bars.l_u.x;  //�� �׵θ� ��ġ
	moving_balls.center.y = stick_bars.l_u.y + 10;

	velocity.x = ball_xspeed;
	velocity.y = ball_yspeed; //�� ���ǵ�

//------------------------------------------------------
	fixed_ball.collision_count = 0;
	fixed_ball.radius = 10.0;

	moving_ball.radius = 7.0;
	moving_balls.radius = moving_ball.radius + 2;

	//------------------------------------------------------------------
	block_num = 1000; // �� ������
	block_rows = 0;
	block_rows_num = 0;
	rand_num = time(NULL) % 10;
	init_block();
	//--------------------------------------------------------------
	//�۾� ��ǥ��
	s_x = 816, s_y = 406;
	b_x = 879, b_y = 403;
	l_x = 816, l_y = 369;
	a_x = 879, a_y = 369;
	life_n = 3;//���� 3��
	score_n = 0; //���� 0������ ����
	xx = 0;
	yy = 0;
}

//������ â ũ�� ����
void ChangeSize(GLsizei w, GLsizei h) {
	if (h == 0)
		h = 1;
	glViewport(xx, yy, w, h);

	if (w <= h) {
		winHeight = 781.0f * h / w;
		winWidth = 781.0f;
	}
	else {
		winWidth = 781.0f * w / h;
		winHeight = 781.0f;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, winWidth, 0, winHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//�浹 ���� Ȯ�� �Լ�
void Collision_Detection_to_Walls(void) {

	//�� �浹 -----------------------------------------------------------------------------------------------------
	if (moving_balls.center.x <= moving_balls.radius || moving_balls.center.x + 1.0 * moving_balls.radius >= winWidth) {
		velocity.x = -velocity.x; //�� �� ��
	}
	/*
	if (moving_ball.center.y <= moving_ball.radius || moving_ball.center.y + 1.0 * moving_ball.radius >= winHeight) {
		velocity.y = -velocity.y; // �� �Ʒ� ��
	}*/
	if (moving_balls.center.y + 1.0 * moving_balls.radius >= winHeight - 5) { // ���� ��
		velocity.y = -velocity.y;
	}
	//-------------------------------------------------------------------------------------------------------------

	//���� �浹 -----------------------------------------------------------------------------------------------------------------------------------------------
	if (moving_balls.center.x >= right_up.x - moving_balls.radius && right_down.y <= moving_balls.center.y && moving_balls.center.y <= right_up.y && moving_balls.center.x <= right_up.x + moving_balls.radius) {
		velocity.x = -velocity.x; // ������ ����
	}
	if (moving_balls.center.x <= left_up.x + moving_balls.radius && left_down.y <= moving_balls.center.y && moving_balls.center.y <= left_up.y && moving_balls.center.x >= left_up.x - moving_balls.radius) {
		velocity.x = -velocity.x;  // ���� ����
	}
	if (moving_balls.center.y >= right_up.y - moving_balls.radius && left_up.x <= moving_balls.center.x && moving_balls.center.x <= right_up.x && moving_balls.center.y <= right_up.y + moving_balls.radius) {
		velocity.y = -velocity.y;  // ���� ����
	}
	if (moving_balls.center.y <= right_down.y + moving_balls.radius && left_down.x <= moving_balls.center.x && moving_balls.center.x <= right_down.x && moving_balls.center.y >= right_down.y - moving_balls.radius) {
		down_count += 1;
		velocity.y = -velocity.y; // �Ʒ��� ����
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------

	//����� �浹 --------------------------------------------------------------------------------------------------------------------------------------------
	if (moving_balls.center.y <= stick_bars.r_d.y + moving_balls.radius && stick_bars.l_d.x <= moving_balls.center.x && moving_balls.center.x <= stick_bars.r_d.x && moving_balls.center.y >= stick_bars.r_d.y - (moving_balls.radius + 3)) {
		velocity.y = -velocity.y;  // �Ʒ� �����
	}
	if (moving_balls.center.y >= stick_bars.r_u.y - moving_balls.radius && stick_bars.l_u.x + 77.5 == moving_balls.center.x && moving_balls.center.y <= stick_bars.r_u.y + (moving_balls.radius)) {
		velocity.y = -velocity.y;  // ���� �����
	}
	if (moving_balls.center.y >= stick_bars.r_u.y - moving_balls.radius && stick_bars.l_u.x + 77.5 < moving_balls.center.x && moving_balls.center.x <= stick_bars.r_u.x && moving_balls.center.y <= stick_bars.r_u.y + (moving_balls.radius)) {
		velocity.y = -velocity.y;  // ���� ����� ������ �κ�
		if (velocity.x < 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = -2.0;
			velocity.x = ball_xspeed;
		}
		else if (velocity.x >= 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = 1.0 * -ball_xspeed;
			ball_xspeed = 2.0;
			velocity.x = ball_xspeed;
		}
	}
	if (moving_balls.center.y >= stick_bars.r_u.y - moving_balls.radius && stick_bars.l_u.x <= moving_balls.center.x && moving_balls.center.x < stick_bars.l_u.x + 77.5 && moving_balls.center.y <= stick_bars.r_u.y + (moving_balls.radius)) {
		velocity.y = -velocity.y;  // ���� ����� ���� �κ�
		if (velocity.x <= 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = -1.0;
			velocity.x = ball_xspeed;
		}
		else if (velocity.x >= 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = 1.0 * -ball_xspeed;
			ball_xspeed = 1.0;
			velocity.x = ball_xspeed;
		}
	}
	if (moving_balls.center.x <= stick_bars.l_u.x + moving_balls.radius && stick_bars.l_d.y <= moving_balls.center.y && moving_balls.center.y <= stick_bars.l_u.y && moving_balls.center.x >= stick_bars.l_u.x - (moving_balls.radius)) {
		velocity.x = -velocity.x;  // ���� �����
	}
	if (moving_balls.center.x >= stick_bars.r_u.x - moving_balls.radius && stick_bars.r_d.y <= moving_balls.center.y && moving_balls.center.y <= stick_bars.r_u.y && moving_balls.center.x <= stick_bars.r_u.x + (moving_balls.radius)) {
		velocity.x = -velocity.x; // ������ �����
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------

		//�� 2�� �浹
	float	distance;
	//�� �浹���� ��� 
	distance = sqrt((fixed_ball.center.x - moving_balls.center.x) * (fixed_ball.center.x - moving_balls.center.x) + (fixed_ball.center.y - moving_balls.center.y) * (fixed_ball.center.y - moving_balls.center.y));
	if (block[rand_num][3].coll_count >= 1) {
		if (fixed_ball.collision_count == 0) {
			if (distance <= fixed_ball.radius + moving_balls.radius) {
				velocity.y = -velocity.y;
				fixed_ball.collision_count = 1;
			}
		}
	}
}

//�����浹 üũ
void Check_blocks(void) {
	for (int i = 0; i < 200; i++) {
		for (int j = 1; j < 6; j++) {
			if (i < 50) {
				if (block[i][j].coll_count == 0 && moving_balls.center.y <= block[i][j].r_d.y + moving_balls.radius + 0.5 && block[i][j].l_d.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_d.x && moving_balls.center.y >= block[i][j].r_d.y - moving_balls.radius + 0.5) {
					block[i][j].coll_count = 1;
					score_n += 1;
					velocity.y = -velocity.y;  // �Ʒ� ������
					break;
				}
				if (block[i][j].coll_count == 0 && moving_balls.center.y >= block[i][j].r_u.y - moving_balls.radius && block[i][j].l_u.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_u.x && moving_balls.center.y <= block[i][j].r_u.y + moving_balls.radius) {
					block[i][j].coll_count = 1;
					score_n += 1;
					velocity.y = -velocity.y;  // ���� ������
					break;
				}
				if (block[i][j].coll_count == 0 && moving_balls.center.x <= block[i][j].l_u.x + moving_balls.radius && block[i][j].l_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].l_u.y && moving_balls.center.x >= block[i][j].l_u.x - moving_balls.radius) {
					block[i][j].coll_count = 1;
					score_n += 1;
					velocity.x = -velocity.x;  // ���� ������
					break;
				}
				if (block[i][j].coll_count == 0 && moving_balls.center.x >= block[i][j].r_u.x - moving_balls.radius && block[i][j].r_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].r_u.y && moving_balls.center.x <= block[i][j].r_u.x + moving_balls.radius) {
					block[i][j].coll_count = 1;
					score_n += 1;
					velocity.x = -velocity.x; // ������ ������
					break;
				}

			}
			else {
				if (block[i][j].coll_count == 0 && moving_balls.center.y <= block[i][j].r_d.y + moving_balls.radius + 0.5 && block[i][j].l_d.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_d.x && moving_balls.center.y >= block[i][j].r_d.y - moving_balls.radius + 0.5) {
					block[i][j].coll_count = 1;
					velocity.y = -velocity.y;  // �Ʒ� ������
					break;
				}
				if (block[i][j].coll_count == 0 && moving_balls.center.y >= block[i][j].r_u.y - moving_balls.radius && block[i][j].l_u.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_u.x && moving_balls.center.y <= block[i][j].r_u.y + moving_balls.radius) {
					block[i][j].coll_count = 1;
					velocity.y = -velocity.y;  // ���� ������
					break;
				}
				if (block[i][j].coll_count == 0 && moving_balls.center.x <= block[i][j].l_u.x + moving_balls.radius && block[i][j].l_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].l_u.y && moving_balls.center.x >= block[i][j].l_u.x - moving_balls.radius) {
					block[i][j].coll_count = 1;
					velocity.x = -velocity.x;  // ���� ������
					break;
				}
				if ((block[i][j].coll_count == 0) && moving_balls.center.x >= block[i][j].r_u.x - moving_balls.radius && block[i][j].r_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].r_u.y && moving_balls.center.x <= block[i][j].r_u.x + moving_balls.radius) {
					block[i][j].coll_count = 1;
					velocity.x = -velocity.x; // ������ ������
					break;
				}

				if (block[i][j].coll_count == 1 && moving_balls.center.y <= block[i][j].r_d.y + moving_balls.radius + 0.5 && block[i][j].l_d.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_d.x && moving_balls.center.y >= block[i][j].r_d.y - moving_balls.radius + 0.5) {
					block[i][j].coll_count = 2;
					score_n += 1;
					velocity.y = -velocity.y;  // �Ʒ� ������
					break;
				}
				if (block[i][j].coll_count == 1 && moving_balls.center.y >= block[i][j].r_u.y - moving_balls.radius && block[i][j].l_u.x <= moving_balls.center.x && moving_balls.center.x <= block[i][j].r_u.x && moving_balls.center.y <= block[i][j].r_u.y + moving_balls.radius) {
					block[i][j].coll_count = 2;
					score_n += 1;
					velocity.y = -velocity.y;  // ���� ������
					break;
				}
				if (block[i][j].coll_count == 1 && moving_balls.center.x <= block[i][j].l_u.x + moving_balls.radius && block[i][j].l_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].l_u.y && moving_balls.center.x >= block[i][j].l_u.x - moving_balls.radius) {
					block[i][j].coll_count = 2;
					score_n += 1;
					velocity.x = -velocity.x;  // ���� ������
					break;
				}
				if ((block[i][j].coll_count == 1) && moving_balls.center.x >= block[i][j].r_u.x - moving_balls.radius && block[i][j].r_d.y <= moving_balls.center.y && moving_balls.center.y <= block[i][j].r_u.y && moving_balls.center.x <= block[i][j].r_u.x + moving_balls.radius) {
					block[i][j].coll_count = 2;
					score_n += 1;
					velocity.x = -velocity.x; // ������ ������
					break;
				}

			}
		}
	}
}

// �� ���� �Լ�
void Modeling_block(void) {
	int starty = 697;
	int startx = -146;
	int height = 0;
	// i�� �� j�� ��
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 6; j++) {
			if (i < 50) {
				if (block[i][j].coll_count == 0) {
					if (j == 0) {
						block[i][j].l_u.x = startx;
						block[i][j].l_d.x = startx;
						block[i][j].r_u.x = startx + 146;
						block[i][j].r_d.x = startx + 146;
						block[i][j].r_d.y = (starty + height) - block_rows;
						block[i][j].l_d.y = (starty + height) - block_rows;
						block[i][j].r_u.y = ((starty + 20) + height) - block_rows;
						block[i][j].l_u.y = ((starty + 20) + height) - block_rows;
						glBegin(GL_POLYGON);
						glColor3f(block[i][j].b_color.r, block[i][j].b_color.g, block[i][j].b_color.b);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
					else {
						block[i][j].l_u.x = block[i][j - 1].r_u.x + 1;
						block[i][j].l_d.x = block[i][j - 1].r_d.x + 1;
						block[i][j].r_u.x = block[i][j].l_u.x + 155;
						block[i][j].r_d.x = block[i][j].l_d.x + 155;
						block[i][j].r_d.y = block[i][j - 1].l_d.y;
						block[i][j].l_d.y = block[i][j].r_d.y;
						block[i][j].r_u.y = block[i][j].r_d.y + 20;
						block[i][j].l_u.y = block[i][j].r_u.y;

						glBegin(GL_POLYGON);
						glColor3f(block[i][j].b_color.r, block[i][j].b_color.g, block[i][j].b_color.b);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
				}
			}
			else {
				if (block[i][j].coll_count == 0) {
					if (j == 0) {
						block[i][j].l_u.x = startx;
						block[i][j].l_d.x = startx;
						block[i][j].r_u.x = startx + 146;
						block[i][j].r_d.x = startx + 146;
						block[i][j].r_d.y = (starty + height) - block_rows;
						block[i][j].l_d.y = (starty + height) - block_rows;
						block[i][j].r_u.y = ((starty + 20) + height) - block_rows;
						block[i][j].l_u.y = ((starty + 20) + height) - block_rows;
						glBegin(GL_POLYGON);
						glColor3f(0.36, 0.73, 0.89);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
					else {
						block[i][j].l_u.x = block[i][j - 1].r_u.x + 1;
						block[i][j].l_d.x = block[i][j - 1].r_d.x + 1;
						block[i][j].r_u.x = block[i][j].l_u.x + 155;
						block[i][j].r_d.x = block[i][j].l_d.x + 155;
						block[i][j].r_d.y = block[i][j - 1].l_d.y;
						block[i][j].l_d.y = block[i][j].r_d.y;
						block[i][j].r_u.y = block[i][j].r_d.y + 20;
						block[i][j].l_u.y = block[i][j].r_u.y;

						glBegin(GL_POLYGON);
						glColor3f(0.36, 0.73, 0.89);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
				}
				if (block[i][j].coll_count == 1) {
					if (j == 0) {
						block[i][j].l_u.x = startx;
						block[i][j].l_d.x = startx;
						block[i][j].r_u.x = startx + 146;
						block[i][j].r_d.x = startx + 146;
						block[i][j].r_d.y = (starty + height) - block_rows;
						block[i][j].l_d.y = (starty + height) - block_rows;
						block[i][j].r_u.y = ((starty + 20) + height) - block_rows;
						block[i][j].l_u.y = ((starty + 20) + height) - block_rows;
						glBegin(GL_POLYGON);
						glColor3f(block[i][j].b_color.r, block[i][j].b_color.g, block[i][j].b_color.b);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
					else {
						block[i][j].l_u.x = block[i][j - 1].r_u.x + 1;
						block[i][j].l_d.x = block[i][j - 1].r_d.x + 1;
						block[i][j].r_u.x = block[i][j].l_u.x + 155;
						block[i][j].r_d.x = block[i][j].l_d.x + 155;
						block[i][j].r_d.y = block[i][j - 1].l_d.y;
						block[i][j].l_d.y = block[i][j].r_d.y;
						block[i][j].r_u.y = block[i][j].r_d.y + 20;
						block[i][j].l_u.y = block[i][j].r_u.y;

						glBegin(GL_POLYGON);
						glColor3f(block[i][j].b_color.r, block[i][j].b_color.g, block[i][j].b_color.b);
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
						glEnd();
					}
				}
			}
		}
		height += 21;
	}
	//���� ��
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 6; j++) {
			if (i < 50) {
				if (block[i][j].coll_count == 1) {
					if (j == 0) {
						block[i][j].l_u.x = startx;
						block[i][j].l_d.x = startx;
						block[i][j].r_u.x = startx + 146;
						block[i][j].r_d.x = startx + 146;
						block[i][j].r_d.y = (starty + height) - block_rows;
						block[i][j].l_d.y = (starty + height) - block_rows;
						block[i][j].r_u.y = ((starty + 20) + height) - block_rows;
						block[i][j].l_u.y = ((starty + 20) + height) - block_rows;
						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
					}
					else {
						block[i][j].l_u.x = block[i][j - 1].r_u.x + 1;
						block[i][j].l_d.x = block[i][j - 1].r_d.x + 1;
						block[i][j].r_u.x = block[i][j].l_u.x + 155;
						block[i][j].r_d.x = block[i][j].l_d.x + 155;
						block[i][j].r_d.y = block[i][j - 1].l_d.y;
						block[i][j].l_d.y = block[i][j].r_d.y;
						block[i][j].r_u.y = block[i][j].r_d.y + 20;
						block[i][j].l_u.y = block[i][j].r_u.y;


						glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
						glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
						glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
						glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
					}
				}
			}
			else {
				if (block[i][j].coll_count == 2) {
					block[i][j].l_u.x = block[i][j - 1].r_u.x + 1;
					block[i][j].l_d.x = block[i][j - 1].r_d.x + 1;
					block[i][j].r_u.x = block[i][j].l_u.x + 155;
					block[i][j].r_d.x = block[i][j].l_d.x + 155;
					block[i][j].r_d.y = block[i][j - 1].l_d.y;
					block[i][j].l_d.y = block[i][j].r_d.y;
					block[i][j].r_u.y = block[i][j].r_d.y + 20;
					block[i][j].l_u.y = block[i][j].r_u.y;


					glVertex2f(block[i][j].l_u.x, block[i][j].l_u.y);
					glVertex2f(block[i][j].l_d.x, block[i][j].l_d.y);
					glVertex2f(block[i][j].r_d.x, block[i][j].r_d.y);
					glVertex2f(block[i][j].r_u.x, block[i][j].r_u.y);
				}
			}
		}
		height += 21;
	}
}

//�� �׸���
void Modeling_Circle(float radius, _Point CC) {

	float	delta;

	delta = 2 * PI / polygon_num;

	glBegin(GL_POLYGON);

	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));

	glEnd();

}

// ����� �����
void Modeling_stick_bar(void) {

	glBegin(GL_POLYGON);
	glColor3f(0.96, 0.85, 0.55);
	glVertex2f(stick_bar.l_d.x, stick_bar.l_d.y);
	glVertex2f(stick_bar.l_u.x, stick_bar.l_u.y);
	glVertex2f(stick_bar.r_u.x, stick_bar.r_u.y);
	glVertex2f(stick_bar.r_d.x, stick_bar.r_d.y);
	glEnd();
}

// ����� �׵θ� �����
void Modeling_stick_bars(void) {
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(stick_bars.l_d.x, stick_bars.l_d.y);
	glVertex2f(stick_bars.l_u.x, stick_bars.l_u.y);
	glVertex2f(stick_bars.r_u.x, stick_bars.r_u.y);
	glVertex2f(stick_bars.r_d.x, stick_bars.r_d.y);
	glEnd();
}

//��輱 �����
void Modeling_Axis(void) {
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0); // �Ʒ� ��
	glVertex2i(left_down.x, left_down.y);
	glVertex2i(right_down.x, right_down.y);

	glColor3f(0.0, 0.0, 0.0); // �� ��
	glVertex2i(left_up.x, left_up.y);
	glVertex2i(right_up.x, right_up.y);

	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(left_down.x, left_down.y); //���� ��
	glVertex2f(left_up.x, left_up.y);

	/*
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(right_down.x, right_down.y); //���� ��
	glVertex2f(right_up.x, right_up.y);
	*/

	glEnd();
}

//�۾�����
void text(int x, int y, char* str) {
	glRasterPos2f(x, y);
	int len = (int)strlen(str);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}

//�� ����������
void game_over(void) {
	if (start_count == 0) {
		moving_ball.center.x = ((stick_bars.r_u.x - stick_bars.l_u.x) / 2) + stick_bars.l_u.x;  //�� ��ġ
		moving_ball.center.y = stick_bars.l_u.y + 10;
		ball_xspeed = 0;
		ball_yspeed = 0;

		velocity.x = ball_xspeed;
		velocity.y = ball_yspeed;
	}
	life_n -= 1;
	score_n -= 10;
}

//����
void playing_bgm(void) {
	openBgm.lpstrElementName = BGM;
	openBgm.lpstrDeviceType = "mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
	dwID = openBgm.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&playBgm);
	/*
	openBgm.lpstrElementName = BGM;
	openBgm.lpstrDeviceType = "mpegvideo";
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
	dwID = openBgm.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&openBgm);*/
}

//ȿ����
void playing_sound(void) {
	opensound1.lpstrElementName = SOUND;    //���� ����    
	opensound1.lpstrDeviceType = "mpegvideo";    //mp3 ����   
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&opensound1);
	dwID = opensound1.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&opensound1);
}

//���
void background_img(void) {
	glShadeModel(GL_SMOOTH);
	glBegin(GL_POLYGON);
	glColor3f(0.61, 0.7, 0.79);
	glVertex2f(800, 0);
	glVertex2f(0, 0);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0, 800);
	glVertex2f(800, 800);
	glEnd();
}

//display �Լ�
void RenderScene(void) {
	glClearColor(backgroud.r, backgroud.g, backgroud.b, 0.0); // ���
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winWidth, 0, winHeight);
	//-------------------------------------------------------
	//���
	background_img();
	//-------------------------------------------------------
	//���� ���� �Լ�
	Modeling_Axis();
	//--------------------------------------------------------
		// ������ �޹��
	glBegin(GL_POLYGON);
	glColor3f(0.69, 0.93, 0.68);
	glVertex2f(782, 0);
	glVertex2f(782, 782);
	glVertex2f(2000, 782);
	glVertex2f(2000, 0);
	glEnd();
	//-------------------------------------------------------
	//����� ���� �Լ�
	Modeling_stick_bars();
	Modeling_stick_bar();
	//-------------------------------------------------------
	//�� ���� �Լ�
	Modeling_block();
	//-------------------------------------------------------
		//������ �� �̺�Ʈ ó��
	if (block[rand_num][3].coll_count >= 1) {
		if (fixed_ball.collision_count == 0) {
			glColor3f(0.5, 0.5, 1.0);
			Modeling_Circle(fixed_ball.radius, fixed_ball.center);
		}
		if (fixed_ball.collision_count == 1) {
			left_down.y += 290;

			right_down.y += 290;

			glutPostRedisplay();

			moving_ball.center.x = right_down.x / 2;
			moving_ball.center.y = right_up.y / 2;
			fixed_ball.collision_count += 1;
		}
	}
	if (down_count == 5) {
		left_down.y -= 290;

		right_down.y -= 290;

		glutPostRedisplay();
		down_count = 0;
	}
	//-------------------------------------------------------
		// �浹 �Լ� 
	Collision_Detection_to_Walls();
	Check_blocks();
	//--------------------------------------------------------
	//�� �ؿ� �� ������ �� ��������� Ȯ��
	for (int i = 0; i < 200; i++) {
		for (int j = 1; j < 6; j++) {
			if (i < 50) {
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 1) {
					if (block[i][j].coll_count == 1) {
						block_rows_num += 1;
						block[i][j].coll_count = 2;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 157) {
					if (block[i][j].coll_count == 1) {
						block_rows_num += 1;
						block[i][j].coll_count = 2;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 313) {
					if (block[i][j].coll_count == 1) {
						block_rows_num += 1;
						block[i][j].coll_count = 2;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 469) {
					if (block[i][j].coll_count == 1) {
						block_rows_num += 1;
						block[i][j].coll_count = 2;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 625) {
					if (block[i][j].coll_count == 1) {
						block_rows_num += 1;
						block[i][j].coll_count = 2;
					}
				}
			}
			else {

				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 1) {
					if (block[i][j].coll_count == 2) {
						block_rows_num += 1;
						block[i][j].coll_count = 3;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 157) {
					if (block[i][j].coll_count == 2) {
						block_rows_num += 1;
						block[i][j].coll_count = 3;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 313) {
					if (block[i][j].coll_count == 2) {
						block_rows_num += 1;
						block[i][j].coll_count = 3;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 469) {
					if (block[i][j].coll_count == 2) {
						block_rows_num += 1;
						block[i][j].coll_count = 3;
					}
				}
				if (block[i][j].l_d.y == 697 && block[i][j].l_d.x == 625) {
					if (block[i][j].coll_count == 2) {
						block_rows_num += 1;
						block[i][j].coll_count = 3;
					}
				}
			}
		}
	}
	// Ȯ�� �� ���� ������
	if (block_rows_num == 5) {
		for (int i = 0; i < 21; i++) {
			block_rows += 1;
			glutPostRedisplay();
		}
		block_rows_num = 0;
	}

	//--------------------------------------------------------
		// �����̴� ���� ��ġ ��ȭ 
	moving_ball.center.x += velocity.x;
	moving_ball.center.y += velocity.y;
	moving_balls.center.x = moving_ball.center.x;
	moving_balls.center.y = moving_ball.center.y;
	//-------------------------------------------------------- 
		// �����̴� �� �׸���
	if (life_n >= 0) {
		glColor3f(1.0, 1.0, 1.0);
		Modeling_Circle(moving_balls.radius, moving_balls.center);
		glColor3f(1.0, 0.34, 0.5);
		Modeling_Circle(moving_ball.radius, moving_ball.center);
	}
	//--------------------------------------------------------
		// ������
	char a[] = "score: ";
	glColor3f(0.0, 0.0, 0.0);
	text(s_x, s_y, a);

	std::sprintf(score, "%d", score_n);
	glColor3f(0.0, 0.0, 0.0);
	text(b_x, b_y, score);

	char b[] = "life: ";
	glColor3f(0.0, 0.0, 0.0);
	text(l_x, l_y, b);

	std::sprintf(life, "%d", life_n);
	glColor3f(0.0, 0.0, 0.0);
	text(a_x, a_y, life);
	//--------------------------------------------------------
	//���� ����������
	if (life_n >= 0) {
		if (moving_ball.center.y <= -1) {
			start_count = 0;
			game_over();
		}
	}
	//--------------------------------------------------------
	//���ȭ��
	//--------------------------------------------------------
	glutSwapBuffers();
	glFlush();

}

// Ű���� ó�� �Լ�
void MyKey(unsigned char key, int x, int y) {
	switch (key) {
		// �� x�� ���ǵ� ����
	case 'a': {
		if (velocity.x >= 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed += 1.0;
			velocity.x = ball_xspeed;
		}
		else if (velocity.x < 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = 1.0 * -ball_xspeed;
			ball_xspeed -= 1.0;
			velocity.x = ball_xspeed;
		}
		break;
	}
			// �� x�� ���ǵ� ����
	case 's': {
		if (velocity.x > 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed -= 1.0;
			velocity.x = ball_xspeed;
		}
		else if (velocity.x < 0) {
			ball_xspeed = fabs(ball_xspeed);
			ball_xspeed = 1.0 * -ball_xspeed;
			ball_xspeed += 1.0;
			velocity.x = ball_xspeed;
		}
		else if (velocity.x == 0) {
			velocity.x = velocity.x;
		}
		break;
	}
			// �� y�� ���ǵ� ����
	case 'd': {
		if (velocity.y >= 0) {
			ball_yspeed = fabs(ball_yspeed);
			ball_yspeed += 1.0;
			velocity.y = ball_yspeed;
		}
		else if (velocity.y < 0) {
			ball_yspeed = fabs(ball_yspeed);
			ball_yspeed = 1.0 * -ball_yspeed;
			ball_yspeed -= 1.0;
			velocity.y = ball_yspeed;
		}
		break;
	}
			// �� y�� ���ǵ� ���� 
	case 'f': {
		if (velocity.y > 0) {
			ball_yspeed = fabs(ball_yspeed);
			ball_yspeed -= 1.0;
			velocity.y = ball_yspeed;
		}
		else if (velocity.y < 0) {
			ball_yspeed = fabs(ball_yspeed);
			ball_yspeed = 1.0 * -ball_yspeed;
			ball_yspeed += 1.0;
			velocity.y = ball_yspeed;
		}
		else if (velocity.y == 0) {
			velocity.y = velocity.y;
		}
		break;
	}
			//���� ��ŸƮ
	case 'r': {
		if (life_n >= 0) {
			if (start_count == 0) {
				ball_yspeed = 5;
				velocity.y = -5;
				start_count = 1;
				break;
			}
		}
		else {
			break;
		}
	}
	case 'q': {
		cout << moving_ball.center.x << endl;
		cout << stick_bars.l_u.x << endl;
		cout << stick_bars.r_u.x << endl;
		break;
	}
	case '1': {
		xx += 1;
		break;
	}
	case '2': {
		xx -= 1;
		break;
	}
	case '3': {
		yy += 1;
		break;
	}
	case '4': {
		yy -= 1;
		break;
	}
	default:	break;
	}
	glutPostRedisplay();
}

// ����Ű ó�� �Լ�
void MySpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: {
		if (stick_bars.l_d.x > 0 && stick_bars.l_u.x > 0) {
			stick_bar.l_d.x -= 15;
			stick_bar.l_u.x -= 15;
			stick_bar.r_d.x -= 15;
			stick_bar.r_u.x -= 15;

			stick_bars.l_d.x -= 15;
			stick_bars.l_u.x -= 15;
			stick_bars.r_d.x -= 15;
			stick_bars.r_u.x -= 15;
			if (start_count == 0) {
				moving_ball.center.x = ((stick_bars.r_u.x - stick_bars.l_u.x) / 2) + stick_bars.l_u.x;
				break;
			}
			glutPostRedisplay();
			break;
		}
		else {
			break;
		}
		break;
	}
	case GLUT_KEY_RIGHT: {
		if (stick_bars.r_d.x < 769.5 && stick_bars.r_u.x < 769.5) {
			stick_bar.l_d.x += 15;
			stick_bar.l_u.x += 15;
			stick_bar.r_d.x += 15;
			stick_bar.r_u.x += 15;

			stick_bars.l_d.x += 15;
			stick_bars.l_u.x += 15;
			stick_bars.r_d.x += 15;
			stick_bars.r_u.x += 15;
			if (start_count == 0) {
				moving_ball.center.x = ((stick_bars.r_u.x - stick_bars.l_u.x) / 2) + stick_bars.l_u.x;
				break;
			}
			glutPostRedisplay();
			break;
		}
		else {
			break;
		}
		break;
	}
	case GLUT_KEY_DOWN: {

		break;
	}
	case GLUT_KEY_UP: {

		break;
	}
	default:   break;
	}
	glutPostRedisplay();
}

//���� �Լ�
void main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Break & Bricks Game");
	init();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutKeyboardFunc(MyKey);
	glutSpecialFunc(MySpecial);
	playing_bgm();
	glutMainLoop();

}