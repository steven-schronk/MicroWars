#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glut.h>

#include "lib/lib_random.h"
#include "gfx.h"

#define MAP_MAX 10		// width and height of this map

struct player_type {
	int strength;
	int health;
	int turns_total;
	int turns_player;
} player;

struct map_type {
	struct unit_type *unit;
	enum { mountain, sea, grassland, forrest, road } terrain;
} map[MAP_MAX][MAP_MAX];

struct unit_type {
	int moveable;
	int strength;
	int health;
};

/* setup map structure for game */
void init_map(){
	int i, j;
	/* set basic start state for each tile */
	for (i = 0; i < MAP_MAX; i++)
		for (j = 0; j < MAP_MAX; j++){
			map[i][j].unit = NULL;
			map[i][j].terrain = random_int(0, road); /* TODO: length */
			/* create bases and starting units for each side */
			if (random_int(0, 5) == 4){
				map[i][j].unit = malloc(sizeof(struct unit_type));
				map[i][j].unit->moveable = random_int(0, 1);
				map[i][j].unit->strength = random_int(0, 100);
				map[i][j].unit->health = random_int(0, 1);
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

void disp_inventory(){
	int offset_x = 25, offset_y = 15;
	screen_set_fg_color(1.0, 1.0, 1.0);
	screen_set_bg_color(0.25, 0.25, 0.25);
	screen_bg_block_update_border(offset_x, offset_y, offset_x+28, offset_y+28);
	screen_mv_add_str(offset_x+5, offset_y + 25, "Inventory and Status");
	screen_set_fg_color(1.0, 0.65, 0.23);
	offset_x+=3;
	screen_set_fg_color(1.0, 0.65, 0.23);
	screen_mv_add_str(offset_x, offset_y+20, "Experience Points");
	screen_set_fg_color(1.0, 0.0, 0.0);
	screen_mv_add_str(offset_x, offset_y+17, "Strength");
	screen_set_fg_color(0.0, 0.0, 1.0);
	screen_mv_add_str(offset_x, offset_y+14, "Dexterity");
	screen_set_fg_color(0.0, 1.0, 0.0);
	screen_mv_add_str(offset_x, offset_y+11, "Magic");
	screen_set_fg_color(1.0, 0.65, 0.23);
	screen_mv_add_str(offset_x, offset_y+8, "Defense");
}

/* general information about unit types */
void disp_info();

void disp_main_menu();

void disp_options(){
	printf("Q=Quit\tH=Help\n");
}

void disp_map(){
	int x, y;
	screen_clear();
	screen_set_fg_color(1.0f, 1.0f, 1.0f);
	screen_set_bg_color(0.0f, 0.0f, 0.3f);
	screen_bg_block_update(1,11,100,12);
	screen_mv_add_str(WIDTH/2, 12, "MANA");
	screen_set_bg_color(0.35f, 0.0f, 0.0f);
	screen_bg_block_update(1,13,100,14);
	screen_mv_add_str(WIDTH/2-1, 14, "HEALTH");
	screen_set_bg_color(0.125f, 0.125f, 0.125f);
	screen_bg_block_update_border(0, 0, WIDTH-1, 10);

	/* draw map in top left corner */
	for(x = 0; x < MAP_MAX; x++)
		for(y = 0; y < MAP_MAX; y++)
			if(map[x][y].terrain) {
				switch(map[x][y].terrain) {
					// TODO: Update background color instead!!
					case 0 : screen_set_bg_color(0.25f, 0.375f, 0.875); break;
					case 1 : screen_set_bg_color(0.125f, 0.375f, 0.875); break;
					case 2 : screen_set_bg_color(0.25f, 0.375f, 0.175); break;
					case 3 : screen_set_bg_color(0.625f, 0.375f, 0.875); break;
					case 4 : screen_set_bg_color(0.25f, 0.175f, 0.875); break;
					default: screen_set_bg_color(0.0f, 0.0f, 0.0f); break;
				}
				screen_bg_update(x, y);
			}
}

/* show statistics about game */
void disp_stats();

void disp_title_screen(){
	/* draw floppy disk */
	screen_set_bg_color(0.027f, 0.23f, 0.45f);
	screen_bg_block_update(37, 17, 73, 40);

	screen_set_bg_color(0.0f, 0.0f, 0.0f);
	screen_bg_block_update(38, 18, 39, 18); 	/* copy protect hole */

	screen_set_bg_color(0.3f, 0.3f, 0.3f);
	screen_bg_block_update(44, 32, 67, 40); 	/* metal sliding window */

	screen_set_bg_color(0.027f, 0.23f, 0.45f);
	screen_bg_block_update(58, 33, 62, 39); 	/* hole in window */

	screen_set_bg_color(0.75f, 0.75f, 0.75f);
	screen_bg_block_update(42, 17, 70, 30);		/* label on disk */

	screen_set_fg_color(0.25f, 0.25f, 0.25f);
	screen_str_center_horiz(HEIGHT/2+2, "MicroWars");
	screen_str_center_horiz(HEIGHT/2, "Steven Ray Schronk");
	screen_str_center_horiz(HEIGHT/2-4, "Press Any Key To Continue");
}

void keyboard(unsigned char key, int x, int y){
	x = 0;
	y = 0;
	key = tolower(key);
	printf("Keypress: %c\n", key);
	switch (key){
	case 'a':
		//msleep(100);
		screen_bg_color.rd = 0.434;
		screen_bg_color.gr = 0.345;
		screen_bg_color.bl = 0.432;
		screen_clear_colors();
		screen_fg_color.rd = 1.0;
		screen_fg_color.gr = 1.0;
		screen_fg_color.bl = 1.0;
		screen_str_center_horiz(HEIGHT/2, "Console Edition");
		screen_str_center_vert(WIDTH/2, "Console Edition");
		screen_fx_colors();
		break;
	case 'b':
		screen_fx_random();
		break;
	case 'c':
		screen_show_chars();
		break;
	case 's':
		draw_player_stats();
		break;
	case 'k':
		screen_fx_random_square();
		break;
	case 'l':
		//move_entity(selected_entity.x, selected_entity.y, selected_entity.x-1, selected_entity.y);
		break;
	case 'm':
		disp_map();
		break;
	case 'i':
		disp_inventory();
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

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0,0);
	glutCreateWindow("MicroWars");
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);

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
