#ifndef GFX_H_
#define GFX_H_

#define WIDTH 128
#define HEIGHT 64
#define OFFSET_X 8
#define OFFSET_Y 12

struct screen {
	char ch;
	float fg_red;		/* foreground colors */
	float fg_green;
	float fg_blue;
	float bg_red;		/* background colors */
	float bg_green;
	float bg_blue;
};

struct color_type {
	float red;
	float green;
	float blue;
} screen_fg_color, screen_bg_color;

struct screen s[WIDTH][HEIGHT];

/* set background color for future use */
void screen_set_bg_color(float red, float green, float blue);

/* set foreground color for future use */
void screen_set_fg_color(float red, float green, float blue);

/* change character and color of character on screen */
void screen_fg_update_color(int x, int y, char ch);

/* change character on screen */
void screen_fg_update(int x, int y, char ch);

/* change color of background color on screen */
void screen_bg_update(int x, int y, float red, float green, float blue);

/* update block of screen with color */
void screen_bg_block_update(int start_x, int start_y, int end_x, int end_y);

/* update block of screen with color - draw border around outside */
void screen_bg_block_update_border(int start_x, int start_y, int end_x, int end_y);

/* move to location and print string using fg color */
void screen_mv_add_str(int x, int y, char *str);

/* sets all characters to null, fg and bg to black */
void screen_clear();

/* draw bg arrow for coordinates. Inputs are offsets. */
void screen_draw_arrow(int x, int y);

/* display axis marks along with counter for each square */
void screen_show_x();

/* show list of all printable characters along with their numerical value */
void screen_show_chars();

/* complete update of a character on screen */
void screen_update(int x, int y, char ch, float fg_red, float fg_green, float fg_blue, float bg_red, float bg_green, float bg_blue);

void screen_fx_random();

#endif /* GFX_H_ */
