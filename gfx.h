#ifndef GFX_H_
#define GFX_H_

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define WIDTH 113	/* number of horizontal squares */
#define HEIGHT 51	/* number of vertical squares */

#define OFFSET_X 9 	/* number of pixels width for each square */
#define OFFSET_Y 15 /* number of pixels height for each square */

struct screen {
	char ch;
	float fg_rd;		/* foreground colors */
	float fg_gr;
	float fg_bl;
	float bg_rd;		/* background colors */
	float bg_gr;
	float bg_bl;
};

struct color_type {
	float rd;
	float gr;
	float bl;
} screen_fg_color, screen_bg_color;

struct screen s[WIDTH][HEIGHT];

/* copy fg and bg color to this location */
void screen_copy(int x, int y);

/* copy color at this position into foreground color buffer */
void screen_copy_fg(int x, int y);

/* copy background at this position into background color buffer */
void screen_copy_bg(int x, int y);

/* set background color for future use */
void screen_set_bg_color(float red, float green, float blue);

/* set foreground color for future use */
void screen_set_fg_color(float red, float green, float blue);

/* change character and color of character on screen */
void screen_fg_update_color(int x, int y, char ch);

/* change single character on screen */
void screen_char_update(int x, int y, char ch);

/* change color of background to current color at this location */
void screen_bg_update(int x, int y);

/* change color of foreground to current color at this location */
void screen_fg_update(int x, int y);

/* update block of screen with color */
void screen_bg_block_update(int start_x, int start_y, int end_x, int end_y);

/* update block of screen with color - draw border around outside */
void screen_bg_block_update_border(int start_x, int start_y, int end_x, int end_y);

/* move to location and print string using fg color */
void screen_mv_add_str(int x, int y, char *str);

/* move to location and print vertical centered string using fg color */
void screen_str_center_vert(int x, char *str);

/* move to location and print horizontal centered string using fg color */
void screen_str_center_horiz(int y, char *str);

/* sets all characters to fg and bg colors */
void screen_clear_colors(void);

/* sets all characters to null, fg and bg to black */
void screen_clear(void);

/* draw bg arrow for coordinates. Inputs are offsets. */
void screen_draw_arrow(int x, int y);

/* display axis marks along with counter for each square */
void screen_show_x(void);

/* show list of all characters along with their numerical value */
void screen_show_chars(void);

/* random colors and chars on screen */
void screen_fx_random(void);

/* demonstrate color range on screen - approximation */
void screen_fx_colors(void);

/* demonstrate random color square on screen */
void screen_fx_random_square(void);

/* sleep for number of milliseconds */
void inline msleep(unsigned int ms);

void display();

void init();

void reshape();

#endif /* GFX_H_ */
