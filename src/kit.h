// kit v0.2 | public domain - no warranty implied; use at your own risk

#ifndef KIT_H
#define KIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#endif

enum {
    KIT_SCALE2X    = (1 << 0),
    KIT_SCALE3X    = (1 << 1),
    KIT_SCALE4X    = (1 << 2),
    KIT_HIDECURSOR = (1 << 3),
    KIT_FPS30      = (1 << 4),
    KIT_FPS144     = (1 << 5),
    KIT_FPSINF     = (1 << 6),
};

typedef union { struct { uint8_t b, g, r, a; }; uint32_t w; } kit_Color;
typedef struct { int x, y, w, h; } kit_Rect;
typedef struct { kit_Color *pixels; int w, h; } kit_Image;
typedef struct { kit_Rect rect; int xadv; } kit_Glyph;
typedef struct { kit_Image *image; kit_Glyph glyphs[256]; } kit_Font;

typedef struct {
    bool wants_quit;
    bool hide_cursor;
    // input
    int char_buf[32];
    uint8_t key_state[256];
    uint8_t mouse_state[16];
    struct { int x, y; } mouse_pos;
    struct { int x, y; } mouse_delta;
    // time
    double step_time;
    double prev_time;
    // graphics
    kit_Rect clip;
    kit_Font *font;
    kit_Image *screen;
    // windows
    int win_w, win_h;
    HWND hwnd;
    HDC hdc;
} kit_Context;

#define kit_max(a, b) ((a) > (b) ? (a) : (b))
#define kit_min(a, b) ((a) < (b) ? (a) : (b))
#define kit_lengthof(a) (sizeof(a) / sizeof((a)[0]))

#define kit_rect(X, Y, W, H) ((kit_Rect) { (X), (Y), (W), (H) })
#define kit_rgba(R, G, B, A) ((kit_Color) { .r = (R), .g = (G), .b = (B), .a = (A) })
#define kit_rgb(R, G, B) kit_rgba(R, G, B, 0xff)
#define kit_alpha(A) kit_rgba(0xff, 0xff, 0xff, A)

#define KIT_BIG_RECT kit_rect(0, 0, 0xffffff, 0xffffff)
#define KIT_WHITE    kit_rgb(0xff, 0xff, 0xff)
#define KIT_BLACK    kit_rgb(0, 0, 0)

kit_Context* kit_create(const char *title, int w, int h, int flags);
void kit_destroy(kit_Context *ctx);
bool kit_step(kit_Context *ctx, double *dt);
void* kit_read_file(char *filename, int *len);

kit_Image* kit_create_image(int w, int h);
kit_Image* kit_load_image_file(char *filename);
kit_Image* kit_load_image_mem(void *data, int len);
void kit_destroy_image(kit_Image *img);

kit_Font* kit_load_font_file(char *filename);
kit_Font* kit_load_font_mem(void *data, int len);
void kit_destroy_font(kit_Font *font);
int kit_text_width(kit_Font *font, char *text);

int  kit_get_char(kit_Context *ctx);
bool kit_key_down(kit_Context *ctx, int key);
bool kit_key_pressed(kit_Context *ctx, int key);
bool kit_key_released(kit_Context *ctx, int key);
void kit_mouse_pos(kit_Context *ctx, int *x, int *y);
void kit_mouse_delta(kit_Context *ctx, int *x, int *y);
bool kit_mouse_down(kit_Context *ctx, int button);
bool kit_mouse_pressed(kit_Context *ctx, int button);
bool kit_mouse_released(kit_Context *ctx, int button);

void kit_clear(kit_Context *ctx, kit_Color color);
void kit_set_clip(kit_Context *ctx, kit_Rect rect);
void kit_draw_point(kit_Context *ctx, kit_Color color, int x, int y);
void kit_draw_rect(kit_Context *ctx, kit_Color color, kit_Rect rect);
void kit_draw_line(kit_Context *ctx, kit_Color color, int x1, int y1, int x2, int y2);
void kit_draw_image(kit_Context *ctx, kit_Image *img, int x, int y);
void kit_draw_image2(kit_Context *ctx, kit_Color color, kit_Image *img, int x, int y, kit_Rect src);
void kit_draw_image3(kit_Context *ctx, kit_Color mul_color, kit_Color add_color, kit_Image *img, kit_Rect dst, kit_Rect src);
int  kit_draw_text(kit_Context *ctx, kit_Color color, const char *text, int x, int y);
int  kit_draw_text2(kit_Context *ctx, kit_Color color, kit_Font *font, const char *text, int x, int y);

#endif // KIT_H
