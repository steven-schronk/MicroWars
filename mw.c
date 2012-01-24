#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "lib/lib_random.h"
#include "gfx.h"

#define MAP_MAX 10

struct unit_type {
	enum { soldier, tank, sub } unit_name;
	int cost;
	int attack_range;
	int attack_strength;
	int move_range; /* number of tiles a unit can move in one turn   */
	int current_strength;
};

static struct unit_type units[3];

struct tile_type {
	struct unit_type *unit;
	enum { mountain, sea, grassland, forrest } terrain_type;
	int tile_selected;	/* user has cursor on this tile */
};

static struct tile_type map[MAP_MAX][MAP_MAX];

struct game_type {
	int money;
	int turns_total;
	int turns_player;
	int money_turn;
	int money_base;
};

static struct game_type game = { 0, 0, 0, 0, 0 };

/* setup map structure for game */
void init_map(){
	int i, j;
	/* set basic start state for each tile */
	for (i = 0; i < MAP_MAX; i++){
		for (j = 0; j < MAP_MAX; j++){
			map[i][j].unit = NULL;
			map[i][j].terrain_type = random_int(0, forrest); /* TODO: length */
			/* create bases and starting units for each side */
			if (random_int(0, 5) == 4){
				map[i][j].unit = malloc(sizeof(struct unit_type));
				map[i][j].unit->unit_name = random_int(0, 3);
			}
		}
	}
}

void draw_player_stats(){
	screen_clear();
	screen_mv_add_str(10, 50, "Strength:");
	screen_mv_add_str(10, 45, "Energy:");
	screen_mv_add_str(10, 40, "Level:");
}

/* verify that this move is possible */
int check_move();

int create_entity(int x, int y, int type);

int delete_entity(int x, int y);

void move_entity(int start_x, int start_y, int end_x, int end_y){

}

/* general information about unit types */
void disp_info();

void disp_main_menu();

void disp_options(){
	printf("Q=Quit\tH=Help\n");
}

/* display generic monster type at position*/
/* TODO: make generic display entity routine */
void disp_monster(int x, int y){
	screen_set_bg_color(0.125f, 0.125f, 0.125f);
	screen_set_fg_color(0.0f, 1.0f, 0.3f);
	screen_fg_update_color(x, y, 'M');
}

void disp_map(){
	screen_clear();
	screen_set_bg_color(0.0f, 0.0f, 0.3f);
	screen_bg_block_update(1,11,100,12);
	screen_mv_add_str(WIDTH/2, 12, "MANA");
	screen_set_bg_color(0.35f, 0.0f, 0.0f);
	screen_bg_block_update(1,13,100,14);
	screen_mv_add_str(WIDTH/2-1, 14, "HEALTH");
	screen_set_bg_color(0.125f, 0.125f, 0.125f);
	screen_bg_block_update_border(0, 0, WIDTH-1, 10);
	disp_monster(20,20);
}

/* show statistics about game */
void disp_stats();

void disp_title_screen(){
	/* draw floppy disk */
	screen_set_bg_color(0.027f, 0.23f, 0.45f);
	screen_set_fg_color(1.0f, 1.0f, 1.0f);
	screen_bg_block_update(2, 4, 3320, 3360);

	/* write name of developer */


	/* title of game */

}

void display(){
	int x, y;

	//printf("%d\n", glutGet(GLUT_WINDOW_WIDTH));

	glClear(GL_COLOR_BUFFER_BIT);
	for(x = 0; x < WIDTH; x++){
		for(y = 0; y < HEIGHT; y++){
			/* background poly */
			glColor3f(s[x][y].bg_red, s[x][y].bg_green, s[x][y].bg_blue);
			//glColor3f(0.25,0.25,0.25);
			glBegin(GL_POLYGON);
				glVertex2i(x*OFFSET_X,y*OFFSET_Y);
				glVertex2i(x*OFFSET_X+OFFSET_X, y*OFFSET_Y);
				glVertex2i(x*OFFSET_X+OFFSET_X, y*OFFSET_Y+OFFSET_Y);
				glVertex2i(x*OFFSET_X, y*OFFSET_Y+OFFSET_Y);
				glVertex2i(x*OFFSET_X,y*OFFSET_Y);
			glEnd();

			/* foreground text */
			glColor3f(s[x][y].fg_red, s[x][y].fg_green, s[x][y].fg_blue);
			glRasterPos2f(x*OFFSET_X, y*OFFSET_Y);
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[x][y].ch);
		}
}
	glFlush();
	glutSwapBuffers();
}

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	/* glEnable(GL_DEPTH_TEST); */
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void keyboard(unsigned char key, int x, int y){
	x = 0;
	y = 0;
	printf("Keypress: %c\n", key);
	switch (key){
	case 'c':
		screen_show_chars();
		break;
	case 's':
		draw_player_stats();
		break;
	case 'l':
		//move_entity(selected_entity.x, selected_entity.y, selected_entity.x-1, selected_entity.y);
		break;
	case 'm':
		disp_map();
		break;
	case 'r':
		//move_entity(selected_entity.x, selected_entity.y, selected_entity.x-1, selected_entity.y);
		break;
	case 'x':
		screen_show_x();
		break;
	case 'q':
		exit(0);
	case 27:
		exit(0);
	break;
	}
	display();
}

int main(int argc, char **argv){
	random_seed();
	init_map();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1024, 768);
	glutReshapeFunc(NULL);
	glutCreateWindow("MicroWars");

	screen_clear();
	disp_title_screen();
	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
