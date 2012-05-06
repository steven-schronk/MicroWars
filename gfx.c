#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/lib_random.h"
#include "gfx.h"
#include "windows.h"

void screen_copy(int x, int y){
	assert(x >= 0 || x < WIDTH);
	assert(y >= 0 || y < HEIGHT);
	s[x][y].fg_rd = screen_fg_color.rd;
	s[x][y].fg_gr = screen_fg_color.gr;
	s[x][y].fg_bl = screen_fg_color.bl;
	s[x][y].bg_rd = screen_bg_color.rd;
	s[x][y].bg_gr = screen_bg_color.gr;
	s[x][y].bg_bl = screen_bg_color.bl;
}

void screen_copy_fg(int x, int y){
	assert(x >= 0 || x < WIDTH);
	assert(y >= 0 || y < HEIGHT);
	s[x][y].fg_rd = screen_fg_color.rd;
	s[x][y].fg_gr = screen_fg_color.gr;
	s[x][y].fg_bl = screen_fg_color.bl;
}

void screen_copy_bg(int x, int y){
	assert(x >= 0 || x < WIDTH);
	assert(y >= 0 || y < HEIGHT);
	s[x][y].bg_rd = screen_bg_color.rd;
	s[x][y].bg_gr = screen_bg_color.gr;
	s[x][y].bg_bl = screen_bg_color.bl;
}

void screen_set_bg_color(float red, float green, float blue){
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);
	screen_bg_color.rd = red;
	screen_bg_color.gr = green;
	screen_bg_color.bl = blue;
}

void screen_set_fg_color(float red, float green, float blue){
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);
	screen_fg_color.rd = red;
	screen_fg_color.gr = green;
	screen_fg_color.bl = blue;
}

void screen_fg_update_color(int x, int y, char ch){
	assert(x < WIDTH);
	assert(y < HEIGHT);
	s[x][y].ch = ch;
	s[x][y].bg_rd = screen_bg_color.rd;
	s[x][y].bg_bl = screen_bg_color.bl;
	s[x][y].bg_gr = screen_bg_color.gr;
	s[x][y].fg_rd = screen_fg_color.rd;
	s[x][y].fg_bl = screen_fg_color.bl;
	s[x][y].fg_gr = screen_fg_color.gr;
}

void screen_char_update(int x, int y, char ch){
	assert(x < WIDTH);
	assert(y < HEIGHT);
	s[x][y].ch = ch;
}

void screen_bg_update(int x, int y){
	assert(x <= WIDTH);
	assert(y <= HEIGHT);
	assert(x >= 0);
	assert(y >= 0);
	s[x][y].bg_rd = screen_bg_color.rd;
	s[x][y].bg_gr = screen_bg_color.gr;
	s[x][y].bg_bl = screen_bg_color.bl;
}

void screen_bg_block_update(int start_x, int start_y, int end_x, int end_y){
	int x, y;
	assert(start_x >= 0);
	assert(start_y >= 0);
	assert(end_x >= 0);
	assert(end_y >= 0);
	assert(start_x <= end_x);
	assert(start_y <= end_y);
	for(x = start_x; x <= end_x && x < WIDTH; x++){
		for(y = start_y; y <= end_y && y < HEIGHT; y++){
			screen_set_bg_color(screen_bg_color.rd, screen_bg_color.gr, screen_bg_color.bl);
			screen_bg_update(x, y);
		}
	}
}

void screen_bg_block_update_border(int start_x, int start_y, int end_x, int end_y){
	int x, y;
	assert(start_x >= 0);
	assert(start_y >= 0);
	assert(end_x >= 0);
	assert(end_y >= 0);
	assert(start_x <= end_x);
	assert(start_y <= end_y);
	for(x = start_x; x <= end_x && x < WIDTH; x++){
		for(y = start_y; y <= end_y && y < HEIGHT; y++){
			screen_set_bg_color(screen_bg_color.rd, screen_bg_color.gr, screen_bg_color.bl);
			screen_bg_update(x, y);
		}
	}
	/* top horiz border */
	for(x = start_x; x <= end_x && x < WIDTH; x++){ s[x][start_y].ch = 18; }
	/* bottom horiz border */
	for(x = start_x; x <= end_x && x < WIDTH; x++){ s[x][end_y].ch = 18; }
	/* left border */
	for(y = start_y-1; y <= end_y-1 && y < HEIGHT; y++){ s[start_x][y].ch = 25; }
	/* right border */
	for(y = start_y-1; y <= end_y-1 && y < HEIGHT; y++){ s[end_x][y].ch = 25; }
	s[end_x][start_y].ch = 11; /* bottom right */
	s[end_x][end_y].ch = 12; /* top left corner */
	s[start_x][end_y].ch = 13; /* top left corner */
	s[start_x][start_y].ch = 14; /* bottom left corner */
}

void screen_mv_add_str(int x, int y, char *str){
	assert(x);
	assert(y);
	assert(str);

	while(*str && x < WIDTH){
		screen_char_update(x, y, *str);
		str++;
		x++;
	}
}

void screen_str_center_vert(int x, char *str){
	assert(str);
	assert(x >= 0 || x <= WIDTH);
	int char_count, y;
	char_count = strlen(str);
	y = (HEIGHT/2) + (char_count/2);
	while(*str){ screen_fg_update_color(x, y--, *str++); }
}

void screen_str_center_horiz(int y, char *str){
	assert(str);
	assert(y >= 0 || y <= WIDTH);
	int char_count, x;
	char_count = strlen(str);
	x = (WIDTH/2) - (char_count/2);
	while(*str){ screen_fg_update_color(x++, y, *str++); }
}

void screen_clear_colors(){
	int x, y;
	for(x = 0; x < WIDTH; x++)
		for(y = 0; y < HEIGHT; y++){
			s[x][y].ch = '\0';
			s[x][y].fg_rd   = screen_fg_color.rd;
			s[x][y].fg_gr = screen_fg_color.gr;
			s[x][y].fg_bl  = screen_fg_color.bl;
			s[x][y].bg_rd   = screen_bg_color.rd;
			s[x][y].bg_gr = screen_bg_color.gr;
			s[x][y].bg_bl  = screen_bg_color.bl;
		}
}

void screen_clear(){
	int x, y;
	for(x = 0; x < WIDTH; x++)
		for(y = 0; y < HEIGHT; y++){
			s[x][y].ch = '\0';
			s[x][y].fg_rd = 1.0f;
			s[x][y].fg_gr = 1.0f;
			s[x][y].fg_bl = 1.0f;
			s[x][y].bg_rd = 0.0f;
			s[x][y].bg_gr = 0.0f;
			s[x][y].bg_bl = 0.0f;
		}
}

void screen_draw_arrow(int x, int y){
	screen_set_bg_color(0.0f, 0.0f, 0.3f);
	/* longest part of bottom block */
	screen_bg_block_update(x+4, y+4, x+17, y+6);

	/* point of x arrow */
	screen_bg_block_update(x+14, y,   x+14, y+10);
	screen_bg_block_update(x+15, y+1, x+15, y+9);
	screen_bg_block_update(x+16, y+2, x+16, y+8);
	screen_bg_block_update(x+17, y+3, x+17, y+7);
	screen_bg_block_update(x+18, y+4, x+18, y+6);
	screen_bg_block_update(x+19, y+5, x+19, y+5);
	screen_bg_block_update(x+20, y+5, x+20, y+5);

	screen_set_bg_color(0.35f, 0.0f, 0.0f);
	/* longest part of top block */
	screen_bg_block_update(x+4, y+4, x+6, y+17);

	/* point of y arrow */
	screen_bg_block_update(x+4, y+4,  x+6,  y+17);
	screen_bg_block_update(x,   y+14, x+10, y+14);
	screen_bg_block_update(x+1, y+15, x+9,  y+15);
	screen_bg_block_update(x+2, y+16, x+8,  y+16);
	screen_bg_block_update(x+1, y+15, x+8,  y+15);
	screen_bg_block_update(x+5, y+18, x+5,  y+18);
}

void screen_show_x(){
	int x, y;
	float r = 0.1f, g = 0.1f, b = 0.1f;
	char ch = '0';
	char row[5], column[5];

	screen_clear();

	/* vertical grid lines */
	for(y=0, ch='0';y<HEIGHT;y++){
		if(y%10){ r += .001; }
		for(x=0, ch='0';x<WIDTH;x+=2){
			screen_set_bg_color(r,g,b);
			screen_bg_update(x,y);
		}
	}

	/* horizontal grid lines */
	for(y=0, ch='0';y<HEIGHT;y+=2){
		for(x=0, ch='0';x<WIDTH;x++)
			screen_set_bg_color(0.1f,0.1f,0.1f);
			screen_bg_update(x,y);
	}

	/* horizontal numbers */
	for(y=0, ch='0';y<HEIGHT;y+=10){
		for(x=0, ch='0';x<WIDTH;x++){
			screen_char_update(x,y,ch);
			if(ch == '9') { ch = '0'; } else { ch++; }
		}
	}

	/* vertical numbers */
	for(x=0, ch='0';x<WIDTH;x+=10){
		for(y=0, ch='0';y<HEIGHT;y++){
			screen_char_update(x,y,ch);
			if(ch == '9') { ch = '0'; } else { ch++; }
		}
	}

	/* mark blocks by tens */
	x = 0; y = 0;
	while(x < 120){
		y = 0;
		while(y < 60){
			sprintf(row, "%d", y);
			sprintf(column, "%d", x);
			screen_mv_add_str(x+3, y+5, "X=");
			screen_mv_add_str(x+5, y+5, column);
			screen_mv_add_str(x+3, y+4, "Y=");
			screen_mv_add_str(x+5, y+4, row);
			y += 10;
		}
			x += 10;
	}
	screen_draw_arrow(1,1);
}

void screen_show_chars(){
	char ch = 0;
	int x = 5, y = HEIGHT-5;
	char str[5];
	screen_clear();
	screen_mv_add_str(WIDTH/2-9, 62, "Character Map");
	screen_set_bg_color(0.25, 0.25, 0.25);
	screen_set_fg_color(1.0, 1.0, 0.0);

	while(ch < 127) {
		screen_fg_update_color(x, y, ch);
		sprintf(str, "%d", ch);
		screen_mv_add_str(x + 3, y, str);
		ch++;
		y -= 3;
		printf("%d\n", x);
		if(y < 5) { y = HEIGHT-5; x+= 16; }
	}
}

void screen_fx_random(){
	int x, y;
	random_seed();
	for(x=0; x < WIDTH; x++){
		for(y = 0; y < HEIGHT; y++){
			s[x][y].bg_bl =  (float)random_int(1,10000)/10000;
			s[x][y].bg_rd =   (float)random_int(1,10000)/10000;
			s[x][y].bg_gr = (float)random_int(1,10000)/10000;
			s[x][y].fg_bl =  (float)random_int(1,10000)/10000;
			s[x][y].fg_rd =   (float)random_int(1,10000)/10000;
			s[x][y].fg_gr = (float)random_int(1,10000)/10000;
			s[x][y].ch = random_int(0, 128);
		}
	}
}

void screen_fx_colors(){
	int x, y;
	float offset = (float)1/(WIDTH*HEIGHT);
	float h_offset = (float)1/HEIGHT*2;
	screen_bg_color.bl =  0.0f;
	screen_bg_color.rd =   1.0f;

	for(x=0; x < WIDTH; x++){
		screen_bg_color.gr = 1.0f;
		for(y = 1; y < HEIGHT; y++){
			 screen_copy(x, y);
			 screen_bg_color.bl += offset;
			 screen_bg_color.rd -= offset;
			 if(y > HEIGHT/2){ screen_bg_color.gr += h_offset; }
			 else { screen_bg_color.gr -= h_offset; }
		}
	}
	offset = (float)1/WIDTH;
	screen_bg_color.bl =  0.0f;
	screen_bg_color.rd =   0.0f;
	screen_bg_color.gr = 0.0f;
	screen_fg_color.bl =  1.0f;
	screen_fg_color.rd =   1.0f;
	screen_fg_color.gr = 1.0f;
	y = 0;
	for(x = 0; x < WIDTH; x++){
		screen_copy(x, y);
		screen_bg_color.bl += offset;
		screen_bg_color.rd += offset;
		screen_bg_color.gr += offset;
		screen_fg_color.bl -= offset;
		screen_fg_color.rd -= offset;
		screen_fg_color.gr -= offset;
		s[x][y].ch = 1;
	}
}

void screen_fx_random_square(){
	int a, b, c, d;
	screen_bg_color.rd = (float)random_int(1,10000)/10000;
	screen_bg_color.gr = (float)random_int(1,10000)/10000;
	screen_bg_color.bl = (float)random_int(1,10000)/10000;
	a = random_int(0, WIDTH);
	b = random_int(a, WIDTH);
	c = random_int(0, HEIGHT);
	d = random_int(c, HEIGHT);
	screen_bg_block_update(a, c, b, d);
}

void inline msleep(unsigned int ms){
#ifdef WIN32
	Sleep(ms);
#else
	// usleep argument must be under 1 000 000
	if (ms > 1000) sleep(ms/1000000);
	usleep((ms % 1000000) * 1000);
#endif
}
