#include <assert.h>
#include <stdio.h>
#include "lib/lib_random.h"
#include "gfx.h"

void screen_set_bg_color(float red, float green, float blue){
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);
	screen_bg_color.red = red;
	screen_bg_color.green = green;
	screen_bg_color.blue = blue;
}

void screen_set_fg_color(float red, float green, float blue){
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);
	screen_fg_color.red = red;
	screen_fg_color.green = green;
	screen_fg_color.blue = blue;
}

void screen_fg_update_color(int x, int y, char ch){
	assert(x < WIDTH);
	assert(y < HEIGHT);
	s[x][y].ch = ch;
	s[x][y].bg_red = screen_bg_color.red;
	s[x][y].bg_blue = screen_bg_color.blue;
	s[x][y].bg_green = screen_bg_color.green;
	s[x][y].fg_red = screen_fg_color.red;
	s[x][y].fg_blue = screen_fg_color.blue;
	s[x][y].fg_green = screen_fg_color.green;
}

void screen_fg_update(int x, int y, char ch){
	assert(x < WIDTH);
	assert(y < HEIGHT);
	s[x][y].ch = ch;
}

void screen_bg_update(int x, int y, float red, float green, float blue){
	assert(x <= WIDTH);
	assert(y <= HEIGHT);
	assert(x >= 0);
	assert(y >= 0);
	s[x][y].bg_red = red;
	s[x][y].bg_green = green;
	s[x][y].bg_blue = blue;
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
			screen_bg_update(x, y, screen_bg_color.red, screen_bg_color.green, screen_bg_color.blue);
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
			screen_bg_update(x, y, screen_bg_color.red, screen_bg_color.green, screen_bg_color.blue);
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
		screen_fg_update(x, y, *str);
		str++;
		x++;
	}
}

void screen_clear(){
	int x, y;
	for(x = 0; x < WIDTH; x++)
		for(y = 0; y < HEIGHT; y++){
			s[x][y].ch = '\0';
			s[x][y].fg_red = 1.0f;
			s[x][y].fg_green = 1.0f;
			s[x][y].fg_blue = 1.0f;
			s[x][y].bg_red = 0.0f;
			s[x][y].bg_green = 0.0f;
			s[x][y].bg_blue = 0.0f;
		}
}

void screen_draw_arrow(int x, int y){
	screen_set_bg_color(0.0f, 0.0f, 0.3f);
	/* longest part of bottom block */
	screen_bg_block_update(x+4, y+4, x+17, y+6);

	/* point of x arrow */
	screen_bg_block_update(x+14, y, x+14, y+10);
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
	screen_bg_block_update(x+4, y+4, x+6, y+17);
	screen_bg_block_update(x, y+14, x+10, y+14);
	screen_bg_block_update(x+1, y+15, x+9, y+15);
	screen_bg_block_update(x+2, y+16, x+8, y+16);
	screen_bg_block_update(x+1, y+15, x+8, y+15);
	screen_bg_block_update(x+5, y+18, x+5, y+18);
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
			screen_bg_update(x,y,r,g,b);
		}
	}

	/* horizontal grid lines */
	for(y=0, ch='0';y<HEIGHT;y+=2){
		for(x=0, ch='0';x<WIDTH;x++){
			screen_bg_update(x,y,0.1f,0.1f,0.1f);
		}
	}

	/* horizontal numbers */
	for(y=0, ch='0';y<HEIGHT;y+=10){
		for(x=0, ch='0';x<WIDTH;x++){
			screen_fg_update(x,y,ch);
			if(ch == '9') { ch = '0'; } else { ch++; }
		}
	}

	/* vertical numbers */
	for(x=0, ch='0';x<WIDTH;x+=10){
		for(y=0, ch='0';y<HEIGHT;y++){
			screen_fg_update(x,y,ch);
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

	/* draw arrows for X and Y axis
	r = 0.25f; g=0.0f; b=0.0f; // dk red
	for(x=2,y=3; x<15; x++)
		screen_bg_update(x, y, r, g, b);
		screen_fg_update(x,y,'X');
	r = 0.0f; g=0.0f; b=0.25f; // dk red
	for(x=2,y=3; y<14; y++)
		screen_bg_update(x, y, r, g, b);
		screen_fg_update(x,y,'Y');
	*/
	screen_draw_arrow(1,1);

}

void screen_show_chars(){
	char ch = 0;
	int x, y = 1, z;
	char str[5];
	char *p_str;
	screen_clear();

	screen_mv_add_str(WIDTH/2-9, 62, "Character Map");

	x=5;y=5;
	while(ch<25){
		screen_fg_update(x, y, ch);
		sprintf(str, "%d", ch);
		p_str = str;
		z = x+3;
		while(*p_str != '\0'){
			screen_fg_update(z, y, *p_str);
			p_str++;
			z++;
		}
		ch++;y=y+2;
	}

	//screen_fg_update(1,1,24);

	x=x+10; y = 5;
	while(ch<50){
		screen_fg_update(x, y, ch);
		sprintf(str, "%d", ch);
		p_str = str;
		z = x+3;
		while(*p_str != '\0'){
			screen_fg_update(z, y, *p_str);
			p_str++;
			z++;
		}
		ch++;
		y=y+2;
	}
}

void screen_update(int x, int y, char ch, float fg_red, float fg_green, float fg_blue, float bg_red, float bg_green, float bg_blue)
{
	// add assertions here
	s[x][y].fg_red = fg_red;
	s[x][y].fg_green = fg_green;
	s[x][y].fg_blue = fg_blue;

	s[x][y].bg_red = bg_red;
	s[x][y].bg_green = bg_green;
	s[x][y].bg_blue = bg_blue;
	s[x][y].ch = ch;
}

void screen_fx_random(){
	int x, y;

	random_seed();

	for(x=0; x < WIDTH; x++){
		for(y = 0; y < HEIGHT; y++){
			s[x][y].bg_blue = random_int(0, 10);
			s[x][y].bg_red = random_int(0, 10);
			s[x][y].bg_green = random_int(0, 10);
			s[x][y].ch = random_int(0, 128);
		}
	}
}
