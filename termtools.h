/*
 * MIT License
 * 
 * Copyright (c) 2026 TERMTOOLS Contributors
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * termtools.h  --  Single-header portable terminal + tool toolkit
 *
 * C89/C90 compatible (C99 used only for <stdint.h> if available;
 * falls back to manual typedefs on ancient compilers).
 *
 * Platforms tested / targeted:
 *   Windows 95 / 98 / ME / NT 4 / 2000 / XP / Vista / 7 / 8 / 10 / 11
 *   Linux 2.0+, macOS 10.0+, BSD variants, Solaris, IRIX, AIX, BeOS
 *   DOS (DJGPP), Haiku, Plan 9 (limited), any POSIX.1 system
 *
 * USAGE
 *   #define TERMTOOLS_IMPLEMENTATION   (in exactly ONE .c / .cpp file)
 *   #include "termtools.h"
 *
 * LICENSE: MIT
 *
 *  _____ _____ ____  __  __ _____ ___   ___  _     ____
 * |_   _| ____|  _ \|  \/  |_   _/ _ \ / _ \| |   / ___|
 *   | | |  _| | |_) | |\/| | | || | | | | | | |   \___ \
 *   | | | |___|  _ <| |  | | | || |_| | |_| | |___ ___) |
 *   |_| |_____|_| \_\_|  |_| |_| \___/ \___/|_____|____/
 *
 * And as always, thanks for using our software <3! - TERMTOOLS Contributors
 */

#if !defined(_WIN32) && !defined(__MSDOS__)
#  if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 199309L
#    undef  _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 199309L
#  endif
#  if !defined(_XOPEN_SOURCE)
#    define _XOPEN_SOURCE 600
#  endif
#endif

#ifndef TERMTOOLS_H
#define TERMTOOLS_H


#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINNT__)
#  define TT_WINDOWS 1
#elif defined(__MSDOS__) || defined(MSDOS) || defined(_MSDOS)
#  define TT_DOS 1
#elif defined(__APPLE__) && defined(__MACH__)
#  define TT_MACOS 1
#  define TT_POSIX 1
#elif defined(__linux__)
#  define TT_LINUX 1
#  define TT_POSIX 1
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#  define TT_BSD   1
#  define TT_POSIX 1
#elif defined(__sun) || defined(sun)
#  define TT_SOLARIS 1
#  define TT_POSIX   1
#elif defined(_AIX)
#  define TT_AIX   1
#  define TT_POSIX 1
#elif defined(__BEOS__) || defined(__HAIKU__)
#  define TT_HAIKU 1
#  define TT_POSIX 1
#else
#  define TT_POSIX 1                            
#endif


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  include <stdint.h>
   typedef uint8_t   tt_u8;
   typedef int8_t    tt_i8;
   typedef uint16_t  tt_u16;
   typedef int16_t   tt_i16;
   typedef uint32_t  tt_u32;
   typedef int32_t   tt_i32;
   typedef uint64_t  tt_u64;
   typedef int64_t   tt_i64;
#elif defined(_MSC_VER)
   typedef unsigned __int8   tt_u8;
   typedef signed   __int8   tt_i8;
   typedef unsigned __int16  tt_u16;
   typedef signed   __int16  tt_i16;
   typedef unsigned __int32  tt_u32;
   typedef signed   __int32  tt_i32;
   typedef unsigned __int64  tt_u64;
   typedef signed   __int64  tt_i64;
#else
   typedef unsigned char           tt_u8;
   typedef signed   char           tt_i8;
   typedef unsigned short          tt_u16;
   typedef signed   short          tt_i16;
   typedef unsigned int            tt_u32;
   typedef signed   int            tt_i32;
   typedef unsigned long long      tt_u64;
   typedef signed   long long      tt_i64;
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#ifdef TT_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <conio.h>
#  include <io.h>
#  include <direct.h>
#  include <sys/stat.h>
#  define TT_PATH_SEP '\\'
#  define TT_NEWLINE  "\r\n"
#  ifdef _MSC_VER
#    pragma comment(lib, "winmm.lib")                    
#  endif
#else
#  include <unistd.h>
#  include <sys/ioctl.h>
#  include <sys/stat.h>
#  include <sys/time.h>
#  include <termios.h>
#  include <fcntl.h>
#  include <dirent.h>
#  include <signal.h>
#  define TT_PATH_SEP '/'
#  define TT_NEWLINE  "\n"
#endif


#ifdef __cplusplus
#  define TT_EXTERN extern "C"
#else
#  define TT_EXTERN extern
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define TT_INLINE static __inline__
#elif defined(_MSC_VER)
#  define TT_INLINE static __inline
#else
#  define TT_INLINE static
#endif

#ifdef _MSC_VER
#  pragma warning(disable: 4996)
#endif


#ifndef NULL
#  define NULL ((void*)0)
#endif

typedef int tt_bool;
#define TT_TRUE  1
#define TT_FALSE 0


#ifdef TT_WINDOWS
#  if defined(_MSC_VER) && _MSC_VER < 1900
     TT_INLINE int tt_snprintf(char *buf, size_t n, const char *fmt, ...) {
         int r;
         va_list ap;
         va_start(ap, fmt);
         r = _vsnprintf(buf, n, fmt, ap);
         va_end(ap);
         if (r < 0 || (size_t)r >= n) {
             if (n > 0) buf[n-1] = '\0';
             r = (int)n - 1;
         }
         return r;
     }
     TT_INLINE int tt_vsnprintf(char *buf, size_t n, const char *fmt, va_list ap) {
         int r = _vsnprintf(buf, n, fmt, ap);
         if (r < 0 || (size_t)r >= n) {
             if (n > 0) buf[n-1] = '\0';
             r = (int)n - 1;
         }
         return r;
     }
#  else
#    define tt_snprintf  snprintf
#    define tt_vsnprintf vsnprintf
#  endif
#else
#  define tt_snprintf  snprintf
#  define tt_vsnprintf vsnprintf
#endif


#ifdef TT_WINDOWS
#  define tt_strtok_r strtok_s
#else
#  define tt_strtok_r strtok_r
#endif


TT_INLINE char *tt_strdup(const char *s) {
    size_t n;
    char *p;
    if (!s) return NULL;
    n = strlen(s) + 1;
    p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}


#ifdef TT_WINDOWS
#  define tt_strcasecmp  _stricmp
#  define tt_strncasecmp _strnicmp
#else
#  include <strings.h>
#  define tt_strcasecmp  strcasecmp
#  define tt_strncasecmp strncasecmp
#endif


TT_INLINE void tt_sleep_ms(unsigned int ms) {
#ifdef TT_WINDOWS
    Sleep(ms);
#elif defined(_POSIX_C_SOURCE) || defined(TT_POSIX)
    struct timespec ts;
    ts.tv_sec  = (time_t)(ms / 1000);
    ts.tv_nsec = (long)((ms % 1000) * 1000000L);
    nanosleep(&ts, NULL);
#else
    usleep((useconds_t)ms * 1000);
#endif
}

TT_INLINE void tt_sleep_sec(double sec) {
    if (sec <= 0.0) return;
    tt_sleep_ms((unsigned int)(sec * 1000.0));
}


TT_INLINE double tt_time(void) {
#ifdef TT_WINDOWS
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return (double)cnt.QuadPart / (double)freq.QuadPart;
#elif defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
#endif
}


typedef struct { int w; int h; } TT_Size;

TT_INLINE TT_Size tt_term_size(void) {
    TT_Size sz;
    sz.w = 80;
    sz.h = 24;
#ifdef TT_WINDOWS
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            sz.w = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
            sz.h = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;
        }
    }
#elif defined(TT_POSIX)
    {
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
            sz.w = (int)ws.ws_col;
            sz.h = (int)ws.ws_row;
        }
    }
#endif
    if (sz.w < 1) sz.w = 80;
    if (sz.h < 1) sz.h = 24;
    return sz;
}


static int tt__ansi_ok = -1;                      

TT_INLINE int tt_ansi_supported(void) {
    if (tt__ansi_ok >= 0) return tt__ansi_ok;
    tt__ansi_ok = 0;
#ifdef TT_WINDOWS
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode)) {
            if (SetConsoleMode(h, mode | 0x0004)) {
                tt__ansi_ok = 1;
            }
        }
    }
#else
    {
        const char *t = getenv("TERM");
        if (t && strcmp(t, "dumb") != 0) tt__ansi_ok = 1;
    }
#endif
    return tt__ansi_ok;
}


TT_INLINE void tt_write(const char *s) {
    if (!s) return;
#ifdef TT_WINDOWS
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written = 0;
        int len = (int)strlen(s);
        int wlen = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
        if (wlen > 1) {
            wchar_t *wbuf = (wchar_t*)malloc((size_t)wlen * sizeof(wchar_t));
            if (wbuf) {
                MultiByteToWideChar(CP_UTF8, 0, s, -1, wbuf, wlen);
                WriteConsoleW(h, wbuf, (DWORD)(wlen - 1), &written, NULL);
                free(wbuf);
                return;
            }
        }
        WriteConsoleA(h, s, (DWORD)len, &written, NULL);
    }
#else
    fputs(s, stdout);
    fflush(stdout);
#endif
}

TT_INLINE void tt_writef(const char *fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    tt_vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    tt_write(buf);
}


TT_INLINE void tt_goto(int x, int y) {
    if (x < 1) x = 1;
    if (y < 1) y = 1;
    if (tt_ansi_supported()) {
        char buf[32];
        tt_snprintf(buf, sizeof(buf), "\033[%d;%dH", y, x);
        tt_write(buf);
    }
#ifdef TT_WINDOWS
    else {
        COORD c; c.X = (SHORT)(x-1); c.Y = (SHORT)(y-1);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }
#endif
}

TT_INLINE void tt_cursor_up(int n)    { if(tt_ansi_supported()){ char b[16]; tt_snprintf(b,sizeof(b),"\033[%dA",n); tt_write(b); } }
TT_INLINE void tt_cursor_down(int n)  { if(tt_ansi_supported()){ char b[16]; tt_snprintf(b,sizeof(b),"\033[%dB",n); tt_write(b); } }
TT_INLINE void tt_cursor_right(int n) { if(tt_ansi_supported()){ char b[16]; tt_snprintf(b,sizeof(b),"\033[%dC",n); tt_write(b); } }
TT_INLINE void tt_cursor_left(int n)  { if(tt_ansi_supported()){ char b[16]; tt_snprintf(b,sizeof(b),"\033[%dD",n); tt_write(b); } }

TT_INLINE void tt_hide_cursor(void) {
    if (tt_ansi_supported()) tt_write("\033[?25l");
#ifdef TT_WINDOWS
    else {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO ci;
        GetConsoleCursorInfo(h, &ci);
        ci.bVisible = FALSE;
        SetConsoleCursorInfo(h, &ci);
    }
#endif
}

TT_INLINE void tt_show_cursor(void) {
    if (tt_ansi_supported()) tt_write("\033[?25h");
#ifdef TT_WINDOWS
    else {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO ci;
        GetConsoleCursorInfo(h, &ci);
        ci.bVisible = TRUE;
        SetConsoleCursorInfo(h, &ci);
    }
#endif
}

TT_INLINE void tt_clear(void) {
    if (tt_ansi_supported()) { tt_write("\033[2J"); tt_goto(1,1); }
#ifdef TT_WINDOWS
    else {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD c = {0,0};
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD written;
        if (GetConsoleScreenBufferInfo(h, &csbi)) {
            DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
            FillConsoleOutputCharacterA(h, ' ', size, c, &written);
            FillConsoleOutputAttribute(h, csbi.wAttributes, size, c, &written);
        }
        SetConsoleCursorPosition(h, c);
    }
#endif
}

TT_INLINE void tt_clear_line(void) {
    if (tt_ansi_supported()) tt_write("\033[2K");
#ifdef TT_WINDOWS
    else {
        TT_Size sz = tt_term_size();
        int i;
        for (i = 0; i < sz.w; i++) tt_write(" ");
    }
#endif
}

TT_INLINE void tt_cls(void) {
#ifdef TT_WINDOWS
    system("cls");
#else
    system("clear");
#endif
}


#define TT_RESET     "\033[0m"
#define TT_BOLD      "\033[1m"
#define TT_DIM       "\033[2m"
#define TT_ITALIC    "\033[3m"
#define TT_UNDERLINE "\033[4m"
#define TT_BLINK     "\033[5m"
#define TT_INVERT    "\033[7m"
#define TT_STRIKE    "\033[9m"

TT_INLINE char *tt_fg(char *buf, size_t n, int code) {
    tt_snprintf(buf, n, "\033[38;5;%dm", code);
    return buf;
}
TT_INLINE char *tt_bg(char *buf, size_t n, int code) {
    tt_snprintf(buf, n, "\033[48;5;%dm", code);
    return buf;
}
TT_INLINE char *tt_rgb_fg(char *buf, size_t n, int r, int g, int b) {
    tt_snprintf(buf, n, "\033[38;2;%d;%d;%dm", r, g, b);
    return buf;
}
TT_INLINE char *tt_rgb_bg(char *buf, size_t n, int r, int g, int b) {
    tt_snprintf(buf, n, "\033[48;2;%d;%d;%dm", r, g, b);
    return buf;
}

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define TT_FG(code)       (tt_fg((char[24]){0},24,(code)))
#  define TT_BG(code)       (tt_bg((char[24]){0},24,(code)))
#  define TT_RGB_FG(r,g,b)  (tt_rgb_fg((char[32]){0},32,(r),(g),(b)))
#  define TT_RGB_BG(r,g,b)  (tt_rgb_bg((char[32]){0},32,(r),(g),(b)))
#endif

#ifdef TT_WINDOWS
TT_INLINE void tt_win32_set_color(int fg16, int bg16) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, (WORD)((bg16 << 4) | fg16));
}
TT_INLINE void tt_win32_reset_color(void) {
    tt_win32_set_color(7, 0);
}
#endif


TT_INLINE void tt_draw(int x, int y, const char *text) {
    tt_goto(x, y);
    tt_write(text);
}

TT_INLINE void tt_drawf(int x, int y, const char *fmt, ...) {
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    tt_vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    tt_draw(x, y, buf);
}

TT_INLINE void tt_fill(int x, int y, int w, int h, char ch) {
    char *row;
    int j;
    if (w <= 0 || h <= 0) return;
    row = (char*)malloc((size_t)w + 1);
    if (!row) return;
    memset(row, ch, (size_t)w);
    row[w] = '\0';
    for (j = 0; j < h; j++) tt_draw(x, y + j, row);
    free(row);
}


typedef struct {
    const char *tl, *tr, *bl, *br, *h, *v;
} TT_BoxStyle;

static const TT_BoxStyle TT_BOX_SINGLE = {"+"  ,  "+"  ,  "+"  ,  "+"  , "-", "|"};                 
static const TT_BoxStyle TT_BOX_UNICODE= {"\xe2\x94\x8c","\xe2\x94\x90",
                                           "\xe2\x94\x94","\xe2\x94\x98",
                                           "\xe2\x94\x80","\xe2\x94\x82"};                         
static const TT_BoxStyle TT_BOX_DOUBLE = {"\xe2\x95\x94","\xe2\x95\x97",
                                           "\xe2\x95\x9a","\xe2\x95\x9d",
                                           "\xe2\x95\x90","\xe2\x95\x91"};
static const TT_BoxStyle TT_BOX_ROUND  = {"\xe2\x95\xad","\xe2\x95\xae",
                                           "\xe2\x95\xb0","\xe2\x95\xaf",
                                           "\xe2\x94\x80","\xe2\x94\x82"};

TT_INLINE void tt_box(int x, int y, int w, int h,
                      const TT_BoxStyle *style, const char *title) {
    int i;
    char row[1024];
    int pos;
    if (w < 2 || h < 2) return;
    if (!style) style = &TT_BOX_SINGLE;

    pos = 0;
    pos += (int)strlen(style->tl);
    tt_goto(x, y);
    tt_write(style->tl);
    for (i = 0; i < w-2; i++) tt_write(style->h);
    tt_write(style->tr);

    for (i = 1; i < h-1; i++) {
        tt_draw(x, y+i, style->v);
        tt_draw(x+w-1, y+i, style->v);
    }

    tt_goto(x, y+h-1);
    tt_write(style->bl);
    for (i = 0; i < w-2; i++) tt_write(style->h);
    tt_write(style->br);

    if (title) {
        tt_draw(x+2, y, title);
    }
    (void)row; (void)pos;
}


TT_INLINE void tt_progress(int x, int y, int w, double value, double maxv,
                            char fill_ch, char empty_ch) {
    int filled, i;
    char *buf;
    if (maxv <= 0.0) maxv = 1.0;
    if (value < 0.0) value = 0.0;
    if (value > maxv) value = maxv;
    filled = (int)(value / maxv * w);
    buf = (char*)malloc((size_t)(w + 1));
    if (!buf) return;
    for (i = 0;       i < filled; i++) buf[i] = fill_ch;
    for (i = filled;  i < w;      i++) buf[i] = empty_ch;
    buf[w] = '\0';
    tt_draw(x, y, buf);
    free(buf);
}


TT_INLINE int tt_kbhit(void) {
#ifdef TT_WINDOWS
    return _kbhit();
#elif defined(TT_POSIX)
    {
        struct timeval tv = {0,0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0;
    }
#else
    return 0;
#endif
}

TT_INLINE int tt_getch(void) {
#ifdef TT_WINDOWS
    if (!_kbhit()) return 0;
    return _getwch();                                              
#elif defined(TT_POSIX)
    {
        struct termios oldt, newt;
        int ch = 0;
        fd_set fds;
        struct timeval tv = {0,0};
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        if (select(STDIN_FILENO+1, &fds, NULL, NULL, &tv) > 0)
            ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#else
    return 0;
#endif
}

TT_INLINE int tt_waitkey(void) {
#ifdef TT_WINDOWS
    return _getwch();
#elif defined(TT_POSIX)
    {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= (tcflag_t)~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#else
    return getchar();
#endif
}


#ifdef TT_POSIX
static struct termios tt__orig_termios;
static int tt__raw_active = 0;

TT_INLINE void tt_raw_mode_enter(void) {
    struct termios raw;
    if (tt__raw_active) return;
    tcgetattr(STDIN_FILENO, &tt__orig_termios);
    raw = tt__orig_termios;
    raw.c_iflag &= (tcflag_t)~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= (tcflag_t)~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= (tcflag_t)~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    tt__raw_active = 1;
}

TT_INLINE void tt_raw_mode_leave(void) {
    if (!tt__raw_active) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tt__orig_termios);
    tt__raw_active = 0;
}
#endif


typedef struct {
    int    fps;
    double last;
    double dt;
    double acc;
    int    frame_count;
    double fps_actual;
} TT_Clock;

TT_INLINE void tt_clock_init(TT_Clock *c, int fps) {
    c->fps        = fps > 0 ? fps : 60;
    c->last       = tt_time();
    c->dt         = 0.0;
    c->acc        = 0.0;
    c->frame_count= 0;
    c->fps_actual = (double)fps;
}

TT_INLINE double tt_clock_tick(TT_Clock *c) {
    double now = tt_time();
    double frame_time = 1.0 / (double)c->fps;
    double elapsed = now - c->last;
    double delay = frame_time - elapsed;
    if (delay > 0.0) tt_sleep_ms((unsigned int)(delay * 1000.0));
    now      = tt_time();
    c->dt    = now - c->last;
    c->last  = now;
    c->acc  += c->dt;
    c->frame_count++;
    if (c->acc >= 1.0) {
        c->fps_actual  = (double)c->frame_count / c->acc;
        c->frame_count = 0;
        c->acc         = 0.0;
    }
    return c->dt;
}


typedef struct {
    double x, y;
    double vx, vy;
    double ax, ay;
} TT_Body;

TT_INLINE void tt_body_init(TT_Body *b, double x, double y) {
    b->x = x; b->y = y;
    b->vx = b->vy = b->ax = b->ay = 0.0;
}

TT_INLINE void tt_body_update(TT_Body *b, double dt) {
    b->vx += b->ax * dt;
    b->vy += b->ay * dt;
    b->x  += b->vx * dt;
    b->y  += b->vy * dt;
}


typedef struct { int x, y, w, h; } TT_Rect;

TT_INLINE tt_bool tt_rect_collide(TT_Rect a, TT_Rect b) {
    return !(a.x + a.w <= b.x || a.x >= b.x + b.w ||
             a.y + a.h <= b.y || a.y >= b.y + b.h);
}


TT_INLINE void tt_sprite(int x, int y, const char **lines) {
    int i;
    for (i = 0; lines[i]; i++) tt_draw(x, y+i, lines[i]);
}

TT_INLINE void tt_erase_sprite(int x, int y, const char **lines) {
    int i;
    for (i = 0; lines[i]; i++) {
        int len = (int)strlen(lines[i]);
        char *blank = (char*)malloc((size_t)len + 1);
        if (blank) { memset(blank, ' ', (size_t)len); blank[len] = '\0'; tt_draw(x, y+i, blank); free(blank); }
    }
}


typedef struct {
    const char **frames;
    int    count;
    double speed;
    double timer;
    int    index;
} TT_Anim;

TT_INLINE void tt_anim_init(TT_Anim *a, const char **frames, int count, double speed) {
    a->frames = frames;
    a->count  = count;
    a->speed  = speed;
    a->timer  = 0.0;
    a->index  = 0;
}

TT_INLINE const char *tt_anim_step(TT_Anim *a, double dt) {
    a->timer += dt;
    if (a->timer >= a->speed) {
        a->timer = 0.0;
        a->index = (a->index + 1) % a->count;
    }
    return a->frames[a->index];
}


TT_INLINE double tt_lerp(double s, double e, double t)          { return s + (e-s)*t; }
TT_INLINE double tt_ease_in(double s, double e, double t)       { return s + (e-s)*t*t; }
TT_INLINE double tt_ease_out(double s, double e, double t)      { double u = 1.0-t; return s + (e-s)*(1.0-u*u); }
TT_INLINE double tt_ease_inout(double s, double e, double t)    { t = t < 0.5 ? 2.0*t*t : -1.0+2.0*(2.0-t)*t; return s + (e-s)*t; }
TT_INLINE double tt_bounce(double s, double e, double t) {
    if (t < 1.0/2.75)      { t = 7.5625*t*t; }
    else if (t < 2.0/2.75) { t -= 1.5/2.75;  t = 7.5625*t*t + 0.75; }
    else if (t < 2.5/2.75) { t -= 2.25/2.75; t = 7.5625*t*t + 0.9375; }
    else                   { t -= 2.625/2.75; t = 7.5625*t*t + 0.984375; }
    return s + (e-s)*t;
}
TT_INLINE double tt_elastic(double s, double e, double t) {
    double p = 0.3;
    if (t <= 0.0) return s;
    if (t >= 1.0) return e;
    return s + (e-s) * (-pow(2.0, 10.0*(t-1.0)) * sin((t-1.0-p/4.0)*(2.0*3.14159265358979323846)/p));
}


TT_INLINE char *tt_gradient_text(const char *text,
                                  int r1, int g1, int b1,
                                  int r2, int g2, int b2) {
    int len = (int)strlen(text);
    char *out = (char*)malloc((size_t)(len * 25 + 8));
    char *p = out;
    int i;
    if (!out) return NULL;
    for (i = 0; i < len; i++) {
        double ratio = (len > 1) ? (double)i / (double)(len-1) : 0.0;
        int r = r1 + (int)((r2-r1) * ratio);
        int g = g1 + (int)((g2-g1) * ratio);
        int b = b1 + (int)((b2-b1) * ratio);
        p += tt_snprintf(p, 32, "\033[38;2;%d;%d;%dm%c", r, g, b, text[i]);
    }
    tt_snprintf(p, 8, "\033[0m");
    return out;
}


TT_INLINE void tt_wave_text(int x, int y, const char *text,
                             double amplitude, double frequency, double phase) {
    int i, len = (int)strlen(text);
    for (i = 0; i < len; i++) {
        int off = (int)(amplitude * sin((double)i * frequency + phase));
        char buf[8];
        buf[0] = text[i]; buf[1] = '\0';
        tt_draw(x + i, y + off, buf);
    }
}


TT_INLINE void tt_typewriter(int x, int y, const char *text, unsigned int delay_ms) {
    int i, len = (int)strlen(text);
    char buf[8];
    tt_goto(x, y);
    for (i = 0; i < len; i++) {
        buf[0] = text[i]; buf[1] = '\0';
        tt_write(buf);
        if (delay_ms > 0) tt_sleep_ms(delay_ms);
    }
}


TT_INLINE void tt_scrolling_text(int x, int y, const char *text,
                                  int width, unsigned int delay_ms) {
    int len = (int)strlen(text);
    int i;
    char *buf;
    if (len <= width) { tt_draw(x, y, text); return; }
    buf = (char*)malloc((size_t)width + 1);
    if (!buf) return;
    for (i = 0; i <= len - width; i++) {
        memcpy(buf, text + i, (size_t)width);
        buf[width] = '\0';
        tt_draw(x, y, buf);
        if (delay_ms > 0) tt_sleep_ms(delay_ms);
    }
    free(buf);
}


TT_INLINE void tt_zoom_text(int x, int y, const char *text, int scale) {
    int len = (int)strlen(text);
    int i, s;
    char buf[2] = {0,0};
    if (scale < 1) scale = 1;
    for (i = 0; i < len; i++) {
        buf[0] = text[i];
        for (s = 0; s < scale; s++) tt_write(buf);
    }
    (void)x; (void)y;
    tt_goto(x, y);
    for (i = 0; i < len; i++) {
        for (s = 0; s < scale; s++) { buf[0] = text[i]; tt_write(buf); }
    }
}


TT_INLINE double tt_raycast(const char **map2d, int map_h,
                             double px, double py, double angle, int maxd) {
    double d;
    for (d = 0.0; d < (double)maxd; d += 0.1) {
        int mx = (int)(px + cos(angle) * d);
        int my = (int)(py + sin(angle) * d);
        if (my < 0 || my >= map_h) return (double)maxd;
        if (!map2d[my]) return (double)maxd;
        if (map2d[my][mx] == '#') return d;
    }
    return (double)maxd;
}


TT_INLINE void tt_fov(const char **map2d, int map_w, int map_h,
                      int px, int py, int radius,
                      char **vis_grid) {
    int a, map_r;
    double ang_step = 3.14159265358979323846 / 180.0;
    for (a = 0; a < 360; a++) {
        double ang = (double)a * ang_step;
        for (map_r = 0; map_r < radius * 10; map_r++) {
            int cx = (int)(px + cos(ang) * (double)map_r * 0.1);
            int cy = (int)(py + sin(ang) * (double)map_r * 0.1);
            if (cx < 0 || cy < 0 || cx >= map_w || cy >= map_h) break;
            vis_grid[cy][cx] = '*';
            if (map2d[cy][cx] == '#') break;
        }
    }
}


TT_INLINE void tt_tilemap(int x, int y, const char **grid, int rows,
                           const char *tiles[256]) {
    int j;
    for (j = 0; j < rows; j++) {
        int i;
        const char *row = grid[j];
        for (i = 0; row[i]; i++) {
            unsigned char c = (unsigned char)row[i];
            const char *tile = tiles[c] ? tiles[c] : "?";
            tt_draw(x + i, y + j, tile);
        }
    }
}


TT_INLINE void tt_minimap(int x, int y, const char **grid,
                           int map_w, int map_h,
                           int player_x, int player_y, int view_radius) {
    int dy, dx;
    for (dy = -view_radius; dy <= view_radius; dy++) {
        for (dx = -view_radius; dx <= view_radius; dx++) {
            int gx = player_x + dx;
            int gy = player_y + dy;
            char ch;
            if (gx < 0 || gy < 0 || gx >= map_w || gy >= map_h) ch = ' ';
            else if (gx == player_x && gy == player_y)            ch = '@';
            else ch = grid[gy][gx];
        {
                char buf2[2];
                buf2[0] = ch; buf2[1] = '\0';
                tt_draw(x + dx + view_radius, y + dy + view_radius, buf2);
            }
        }
    }
}


TT_INLINE void tt_bar_chart(int x, int y, const double *data, int count,
                             int width, int height, int ansi_color) {
    int i, j;
    double max_val = 0.0;
    char col_buf[24];
    (void)width;
    for (i = 0; i < count; i++) if (data[i] > max_val) max_val = data[i];
    if (max_val == 0.0) max_val = 1.0;
    for (i = 0; i < count; i++) {
        int bar_h = (int)(data[i] / max_val * (double)height);
        for (j = 0; j < bar_h; j++) {
            tt_fg(col_buf, sizeof(col_buf), ansi_color);
            tt_draw(x + i*2, y + height - j, col_buf);
            tt_write("#");
            tt_write(TT_RESET);
        }
        {
            char idx[4];
            tt_snprintf(idx, sizeof(idx), "%d", i);
            tt_draw(x + i*2, y + height + 1, idx);
        }
    }
}


TT_INLINE void tt_table(int x, int y,
                         const char **headers, int col_count,
                         const char ***rows, int row_count,
                         const int *col_widths) {
    int i, j;
    tt_goto(x, y);
    tt_write("+");
    for (i = 0; i < col_count; i++) {
        int k;
        for (k = 0; k < col_widths[i]; k++) tt_write("-");
        tt_write("+");
    }
    tt_goto(x, y+1);
    tt_write("|");
    for (i = 0; i < col_count; i++) {
        int pad = col_widths[i] - (int)strlen(headers[i]);
        int lpad = pad/2, rpad = pad - lpad;
        int k;
        for (k = 0; k < lpad; k++) tt_write(" ");
        tt_write(headers[i]);
        for (k = 0; k < rpad; k++) tt_write(" ");
        tt_write("|");
    }
    tt_goto(x, y+2);
    tt_write("+");
    for (i = 0; i < col_count; i++) {
        int k;
        for (k = 0; k < col_widths[i]; k++) tt_write("-");
        tt_write("+");
    }
    for (j = 0; j < row_count; j++) {
        tt_goto(x, y+3+j);
        tt_write("|");
        for (i = 0; i < col_count; i++) {
            int k;
            int len = (int)strlen(rows[j][i]);
            int pad = col_widths[i] - len;
            int lpad = pad/2, rpad = pad - lpad;
            for (k = 0; k < lpad; k++) tt_write(" ");
            tt_write(rows[j][i]);
            for (k = 0; k < rpad; k++) tt_write(" ");
            tt_write("|");
        }
    }
    tt_goto(x, y+3+row_count);
    tt_write("+");
    for (i = 0; i < col_count; i++) {
        int k;
        for (k = 0; k < col_widths[i]; k++) tt_write("-");
        tt_write("+");
    }
}


TT_INLINE void tt_toast(const char *message, unsigned int duration_ms,
                         int position) {
    TT_Size sz = tt_term_size();
    int mlen = (int)strlen(message);
    int bx, by;
    char buf[256];
    if      (position == 1) { bx = 1;                     by = sz.h - 1; }
    else if (position == 2) { bx = (sz.w - mlen) / 2;    by = sz.h - 1; }
    else                    { bx = sz.w - mlen - 2;       by = 1; }
    tt_snprintf(buf, sizeof(buf), "[%s]", message);
    tt_draw(bx, by, buf);
    tt_sleep_ms(duration_ms);
    tt_fill(bx, by, mlen + 2, 1, ' ');
}


typedef struct {
    int         x, y;
    const char **frames;
    int          frame_count;
    int          index;
    int          color;
} TT_Spinner;

#ifdef __GNUC__
static const char *TT_SPINNER_BAR[]  __attribute__((unused)) = {"-", "\\", "|", "/"};
static const char *TT_SPINNER_DOTS[] __attribute__((unused)) = {".  ", ".. ", "...", " ..", "  .", "   "};
#else
static const char *TT_SPINNER_BAR[]  = {"-", "\\", "|", "/"};
static const char *TT_SPINNER_DOTS[] = {".  ", ".. ", "...", " ..", "  .", "   "};
#endif

TT_INLINE void tt_spinner_init(TT_Spinner *s, int x, int y,
                                const char **frames, int count, int color) {
    s->x = x; s->y = y;
    s->frames = frames;
    s->frame_count = count;
    s->index = 0;
    s->color = color;
}

TT_INLINE void tt_spinner_tick(TT_Spinner *s) {
    char col[24];
    tt_fg(col, sizeof(col), s->color);
    tt_goto(s->x, s->y);
    tt_write(col);
    tt_write(s->frames[s->index]);
    tt_write(TT_RESET);
    s->index = (s->index + 1) % s->frame_count;
}


typedef struct {
    int          x, y;
    const char **items;
    int          count;
    int          selected;
    int          sel_color;
    int          norm_color;
} TT_Menu;

TT_INLINE void tt_menu_init(TT_Menu *m, int x, int y,
                             const char **items, int count,
                             int sel_color, int norm_color) {
    m->x = x; m->y = y;
    m->items = items;
    m->count = count;
    m->selected = 0;
    m->sel_color = sel_color;
    m->norm_color = norm_color;
}

TT_INLINE void tt_menu_draw(const TT_Menu *m) {
    int i;
    char col[24];
    for (i = 0; i < m->count; i++) {
        tt_fg(col, sizeof(col), (i == m->selected) ? m->sel_color : m->norm_color);
        tt_goto(m->x, m->y + i);
        tt_write(col);
        tt_write(m->items[i]);
        tt_write(TT_RESET);
    }
}

TT_INLINE void tt_menu_up(TT_Menu *m)   { if (m->selected > 0)              m->selected--; }
TT_INLINE void tt_menu_down(TT_Menu *m) { if (m->selected < m->count - 1)  m->selected++; }
TT_INLINE const char *tt_menu_get(const TT_Menu *m) { return m->items[m->selected]; }


#define TT_CONSOLE_MAX_LINES 64
#define TT_CONSOLE_LINE_LEN  256

typedef struct {
    int  x, y, width, height;
    char lines[TT_CONSOLE_MAX_LINES][TT_CONSOLE_LINE_LEN];
    int  head;
    int  count;
} TT_Console;

TT_INLINE void tt_console_init(TT_Console *c, int x, int y, int w, int h) {
    c->x = x; c->y = y; c->width = w; c->height = h;
    c->head = 0; c->count = 0;
    memset(c->lines, 0, sizeof(c->lines));
}

TT_INLINE void tt_console_log(TT_Console *c, const char *msg) {
    int idx = (c->head + c->count) % TT_CONSOLE_MAX_LINES;
    tt_snprintf(c->lines[idx], TT_CONSOLE_LINE_LEN, "%s", msg);
    if (c->count < TT_CONSOLE_MAX_LINES) c->count++;
    else c->head = (c->head + 1) % TT_CONSOLE_MAX_LINES;
}

TT_INLINE void tt_console_logf(TT_Console *c, const char *fmt, ...) {
    char buf[TT_CONSOLE_LINE_LEN];
    va_list ap;
    va_start(ap, fmt);
    tt_vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    tt_console_log(c, buf);
}

TT_INLINE void tt_console_draw(const TT_Console *c) {
    int i, visible = (c->count < c->height) ? c->count : c->height;
    for (i = 0; i < visible; i++) {
        int idx = (c->head + c->count - visible + i) % TT_CONSOLE_MAX_LINES;
        char trunc[TT_CONSOLE_LINE_LEN];
        tt_snprintf(trunc, (size_t)c->width + 1, "%s", c->lines[idx]);
        tt_draw(c->x, c->y + i, trunc);
    }
}


typedef struct {
    double duration;
    double remaining;
    int    active;
    int    loop;
    void (*callback)(void*);
    void  *userdata;
} TT_Timer;

TT_INLINE void tt_timer_init(TT_Timer *t, double dur, int loop,
                              void (*cb)(void*), void *ud) {
    t->duration  = dur;
    t->remaining = dur;
    t->active    = 0;
    t->loop      = loop;
    t->callback  = cb;
    t->userdata  = ud;
}

TT_INLINE void tt_timer_start(TT_Timer *t)  { t->remaining = t->duration; t->active = 1; }
TT_INLINE void tt_timer_stop(TT_Timer *t)   { t->active = 0; }
TT_INLINE void tt_timer_reset(TT_Timer *t)  { t->remaining = t->duration; }

TT_INLINE int tt_timer_update(TT_Timer *t, double dt) {
    if (!t->active) return 0;
    t->remaining -= dt;
    if (t->remaining <= 0.0) {
        if (t->callback) t->callback(t->userdata);
        if (t->loop) t->remaining = t->duration;
        else         t->active = 0;
        return 1;
    }
    return 0;
}


#define TT_MAX_EVENTS    64
#define TT_MAX_LISTENERS 16

typedef void (*TT_EventCallback)(const void *data, void *userdata);

typedef struct {
    TT_EventCallback cb;
    void *userdata;
} TT__Listener;

typedef struct {
    TT__Listener listeners[TT_MAX_EVENTS][TT_MAX_LISTENERS];
    int          counts[TT_MAX_EVENTS];
} TT_EventBus;

TT_INLINE void tt_bus_init(TT_EventBus *bus) {
    memset(bus, 0, sizeof(*bus));
}

TT_INLINE void tt_bus_subscribe(TT_EventBus *bus, int event_id,
                                 TT_EventCallback cb, void *ud) {
    int n;
    if (event_id < 0 || event_id >= TT_MAX_EVENTS) return;
    n = bus->counts[event_id];
    if (n >= TT_MAX_LISTENERS) return;
    bus->listeners[event_id][n].cb       = cb;
    bus->listeners[event_id][n].userdata = ud;
    bus->counts[event_id]++;
}

TT_INLINE void tt_bus_emit(TT_EventBus *bus, int event_id, const void *data) {
    int i;
    if (event_id < 0 || event_id >= TT_MAX_EVENTS) return;
    for (i = 0; i < bus->counts[event_id]; i++)
        bus->listeners[event_id][i].cb(data, bus->listeners[event_id][i].userdata);
}


typedef struct {
    int    w, h;
    int    cell_w, cell_h;
    void **cells;                                                       
} TT_Grid;

TT_INLINE int tt_grid_init(TT_Grid *g, int w, int h, int cw, int ch) {
    g->w = w; g->h = h; g->cell_w = cw; g->cell_h = ch;
    g->cells = (void**)calloc((size_t)(w*h), sizeof(void*));
    return g->cells != NULL;
}

TT_INLINE void tt_grid_free(TT_Grid *g) { free(g->cells); g->cells = NULL; }

TT_INLINE void tt_grid_set(TT_Grid *g, int x, int y, void *val) {
    int gx = x / g->cell_w, gy = y / g->cell_h;
    if (gx >= 0 && gx < g->w && gy >= 0 && gy < g->h)
        g->cells[gy * g->w + gx] = val;
}

TT_INLINE void *tt_grid_get(const TT_Grid *g, int x, int y) {
    int gx = x / g->cell_w, gy = y / g->cell_h;
    if (gx >= 0 && gx < g->w && gy >= 0 && gy < g->h)
        return g->cells[gy * g->w + gx];
    return NULL;
}


typedef struct { double x, y; } TT_Camera;

TT_INLINE void tt_cam_init(TT_Camera *c, double x, double y) { c->x = x; c->y = y; }
TT_INLINE void tt_cam_move(TT_Camera *c, double dx, double dy) { c->x += dx; c->y += dy; }
TT_INLINE void tt_cam_apply(const TT_Camera *c, double wx, double wy,
                             int *sx, int *sy) {
    *sx = (int)(wx - c->x);
    *sy = (int)(wy - c->y);
}


#define TT_MAX_LAYERS  8
#define TT_MAX_LAYER_CMDS 256

typedef struct { int x, y; char text[128]; } TT_LayerCmd;

typedef struct {
    TT_LayerCmd cmds[TT_MAX_LAYERS][TT_MAX_LAYER_CMDS];
    int         counts[TT_MAX_LAYERS];
} TT_LayerBuf;

TT_INLINE void tt_layer_init(TT_LayerBuf *lb) { memset(lb, 0, sizeof(*lb)); }

TT_INLINE void tt_layer_push(TT_LayerBuf *lb, int z, int x, int y, const char *text) {
    int n;
    if (z < 0 || z >= TT_MAX_LAYERS) return;
    n = lb->counts[z];
    if (n >= TT_MAX_LAYER_CMDS) return;
    lb->cmds[z][n].x = x;
    lb->cmds[z][n].y = y;
    tt_snprintf(lb->cmds[z][n].text, sizeof(lb->cmds[z][n].text), "%s", text);
    lb->counts[z]++;
}

TT_INLINE void tt_layer_flush(TT_LayerBuf *lb) {
    int z, i;
    for (z = 0; z < TT_MAX_LAYERS; z++) {
        for (i = 0; i < lb->counts[z]; i++)
            tt_draw(lb->cmds[z][i].x, lb->cmds[z][i].y, lb->cmds[z][i].text);
        lb->counts[z] = 0;
    }
}


#define TT_MAX_BINDINGS 128

typedef struct {
    char key;
    void (*action)(void*);
    void *userdata;
} TT_Binding;

typedef struct {
    TT_Binding bindings[TT_MAX_BINDINGS];
    int        count;
} TT_KeyBinder;

TT_INLINE void tt_keybinder_init(TT_KeyBinder *kb) { kb->count = 0; }

TT_INLINE void tt_keybinder_bind(TT_KeyBinder *kb, char key,
                                  void (*action)(void*), void *ud) {
    if (kb->count >= TT_MAX_BINDINGS) return;
    kb->bindings[kb->count].key      = key;
    kb->bindings[kb->count].action   = action;
    kb->bindings[kb->count].userdata = ud;
    kb->count++;
}

TT_INLINE int tt_keybinder_handle(TT_KeyBinder *kb, char key) {
    int i;
    for (i = 0; i < kb->count; i++) {
        if (kb->bindings[i].key == key) {
            kb->bindings[i].action(kb->bindings[i].userdata);
            return 1;
        }
    }
    return 0;
}


TT_INLINE void tt_beep(unsigned int freq_hz, unsigned int dur_ms) {
#ifdef TT_WINDOWS
    Beep(freq_hz, dur_ms);
#elif defined(TT_LINUX) || defined(TT_POSIX)
    {
        FILE *tty = fopen("/dev/tty", "w");
        if (tty) {
            fprintf(tty, "\a");
            fclose(tty);
        } else {
            printf("\a");
            fflush(stdout);
        }
        (void)freq_hz; (void)dur_ms;
    }
#else
    printf("\a"); fflush(stdout);
    (void)freq_hz; (void)dur_ms;
#endif
}


typedef struct {
    tt_u32 sample_rate;
    tt_u16 channels;
    tt_u16 bits_per_sample;
    tt_u32 num_samples;                    
    float *data;                                                    
} TT_Wav;

TT_INLINE void tt__write_u16le(FILE *f, tt_u16 v) {
    fputc((int)(v & 0xFF), f);
    fputc((int)((v >> 8) & 0xFF), f);
}
TT_INLINE void tt__write_u32le(FILE *f, tt_u32 v) {
    fputc((int)(v & 0xFF), f);
    fputc((int)((v >> 8)  & 0xFF), f);
    fputc((int)((v >> 16) & 0xFF), f);
    fputc((int)((v >> 24) & 0xFF), f);
}
TT_INLINE tt_u16 tt__read_u16le(FILE *f) {
    tt_u16 v  = (tt_u16)fgetc(f);
    v        |= (tt_u16)((tt_u16)fgetc(f) << 8);
    return v;
}
TT_INLINE tt_u32 tt__read_u32le(FILE *f) {
    tt_u32 v  = (tt_u32)fgetc(f);
    v        |= (tt_u32)((tt_u32)fgetc(f) << 8);
    v        |= (tt_u32)((tt_u32)fgetc(f) << 16);
    v        |= (tt_u32)((tt_u32)fgetc(f) << 24);
    return v;
}

TT_INLINE int tt_wav_write(const char *path, const float *data,
                            tt_u32 num_samples, tt_u16 channels,
                            tt_u32 sample_rate, tt_u16 bits) {
    FILE *f;
    tt_u16 block_align, fmt_code;
    tt_u32 byte_rate, data_bytes, i;

    if (bits != 8 && bits != 16 && bits != 32) return -1;
    f = fopen(path, "wb");
    if (!f) return -1;

    block_align = (tt_u16)(channels * bits / 8);
    byte_rate   = sample_rate * block_align;
    data_bytes  = num_samples * channels * (bits / 8);
    fmt_code    = (bits == 32) ? 3 : 1;                          

    fputs("RIFF", f);
    tt__write_u32le(f, 36 + data_bytes);
    fputs("WAVE", f);

    fputs("fmt ", f);
    tt__write_u32le(f, 16);
    tt__write_u16le(f, fmt_code);
    tt__write_u16le(f, channels);
    tt__write_u32le(f, sample_rate);
    tt__write_u32le(f, byte_rate);
    tt__write_u16le(f, block_align);
    tt__write_u16le(f, bits);

    fputs("data", f);
    tt__write_u32le(f, data_bytes);

    for (i = 0; i < num_samples * channels; i++) {
        float s = data[i];
        if (s >  1.0f) s =  1.0f;
        if (s < -1.0f) s = -1.0f;
        if (bits == 8) {
            fputc((int)((s * 127.0f) + 128.0f), f);
        } else if (bits == 16) {
            tt_i16 v = (tt_i16)(s * 32767.0f);
            fputc((int)(v & 0xFF), f);
            fputc((int)((v >> 8) & 0xFF), f);
        } else {                   
            fwrite(&s, 4, 1, f);
        }
    }

    fclose(f);
    return 0;
}

TT_INLINE int tt_wav_read(const char *path, TT_Wav *out) {
    FILE *f;
    char tag[5];
    tt_u32 chunk_size, data_size;
    tt_u16 fmt_code, channels, bits;
    tt_u32 sample_rate, i;

    memset(out, 0, sizeof(*out));
    f = fopen(path, "rb");
    if (!f) return -1;

    if (fread(tag, 1, 4, f) != 4) { fclose(f); return -2; }
    tag[4] = '\0';
    if (strcmp(tag, "RIFF") != 0) { fclose(f); return -3; }
    tt__read_u32le(f);                 
    if (fread(tag, 1, 4, f) != 4 || strcmp(tag, "WAVE") != 0) { fclose(f); return -4; }

    fmt_code = 1; channels = 1; bits = 16; sample_rate = 44100; data_size = 0;
    while (!feof(f)) {
        char id[5];
        if (fread(id, 1, 4, f) != 4) break;
        id[4] = '\0';
        chunk_size = tt__read_u32le(f);
        if (strcmp(id, "fmt ") == 0) {
            fmt_code    = tt__read_u16le(f);
            channels    = tt__read_u16le(f);
            sample_rate = tt__read_u32le(f);
            tt__read_u32le(f);                
            tt__read_u16le(f);                  
            bits        = tt__read_u16le(f);
            if (chunk_size > 16) fseek(f, (long)(chunk_size - 16), SEEK_CUR);
        } else if (strcmp(id, "data") == 0) {
            data_size = chunk_size;
            break;
        } else {
            fseek(f, (long)chunk_size, SEEK_CUR);
        }
    }

    if (data_size == 0 || channels == 0 || bits == 0) { fclose(f); return -5; }

    out->sample_rate    = sample_rate;
    out->channels       = channels;
    out->bits_per_sample= bits;
    out->num_samples    = data_size / (channels * (bits / 8));
    out->data = (float*)malloc(out->num_samples * channels * sizeof(float));
    if (!out->data) { fclose(f); return -6; }

    for (i = 0; i < out->num_samples * channels; i++) {
        if (bits == 8) {
            int b = fgetc(f);
            out->data[i] = (float)(b - 128) / 128.0f;
        } else if (bits == 16) {
            tt_i16 v;
            tt_u8 lo = (tt_u8)fgetc(f);
            tt_u8 hi = (tt_u8)fgetc(f);
            v = (tt_i16)((tt_u16)lo | ((tt_u16)hi << 8));
            out->data[i] = (float)v / 32768.0f;
        } else if (bits == 32 && fmt_code == 3) {
            float v;
            if (fread(&v, 4, 1, f) == 1) out->data[i] = v;
        } else if (bits == 32) {
            tt_i32 v = (tt_i32)tt__read_u32le(f);
            out->data[i] = (float)v / 2147483648.0f;
        }
    }

    fclose(f);
    return 0;
}

TT_INLINE void tt_wav_free(TT_Wav *w) { free(w->data); w->data = NULL; }

TT_INLINE void tt_wav_generate_sine(TT_Wav *out, double freq_hz,
                                     double duration_sec, tt_u32 sample_rate) {
    tt_u32 n = (tt_u32)(duration_sec * (double)sample_rate);
    tt_u32 i;
    out->sample_rate     = sample_rate;
    out->channels        = 1;
    out->bits_per_sample = 16;
    out->num_samples     = n;
    out->data = (float*)malloc(n * sizeof(float));
    if (!out->data) return;
    for (i = 0; i < n; i++)
        out->data[i] = (float)sin(2.0 * 3.14159265358979323846 * freq_hz * (double)i / (double)sample_rate);
}


typedef struct {
    int    w, h;
    int    maxval;
    int    channels;                             
    tt_u8 *data;                                          
} TT_Image;

TT_INLINE void tt__skip_pnm_whitespace_and_comments(FILE *f) {
    int c;
    again:
    c = fgetc(f);
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') c = fgetc(f);
    if (c == '#') {
        while (c != '\n' && c != EOF) c = fgetc(f);
        goto again;
    }
    if (c != EOF) ungetc(c, f);
}

TT_INLINE int tt__read_pnm_int(FILE *f) {
    int v = 0; int c;
    tt__skip_pnm_whitespace_and_comments(f);
    c = fgetc(f);
    while (c >= '0' && c <= '9') { v = v*10 + (c-'0'); c = fgetc(f); }
    if (c != EOF) ungetc(c, f);
    return v;
}

TT_INLINE int tt_image_read_pnm(const char *path, TT_Image *out) {
    FILE *f;
    char magic[3];
    int w, h, maxval, binary, ch_count, total, i;

    memset(out, 0, sizeof(*out));
    f = fopen(path, "rb");
    if (!f) return -1;

    if (fread(magic, 1, 2, f) != 2) { fclose(f); return -2; }
    magic[2] = '\0';

    if      (strcmp(magic, "P2") == 0) { ch_count = 1; binary = 0; }
    else if (strcmp(magic, "P5") == 0) { ch_count = 1; binary = 1; }
    else if (strcmp(magic, "P3") == 0) { ch_count = 3; binary = 0; }
    else if (strcmp(magic, "P6") == 0) { ch_count = 3; binary = 1; }
    else { fclose(f); return -3; }

    w      = tt__read_pnm_int(f);
    h      = tt__read_pnm_int(f);
    maxval = tt__read_pnm_int(f);
    if (w <= 0 || h <= 0 || maxval <= 0) { fclose(f); return -4; }

    fgetc(f);

    total     = w * h * ch_count;
    out->w    = w; out->h = h;
    out->maxval   = maxval;
    out->channels = ch_count;
    out->data = (tt_u8*)malloc((size_t)total);
    if (!out->data) { fclose(f); return -5; }

    if (binary) {
        if (maxval <= 255) {
            fread(out->data, 1, (size_t)total, f);
        } else {
            for (i = 0; i < total; i++) {
                int hi = fgetc(f), lo = fgetc(f);
                out->data[i] = (tt_u8)(((hi << 8) | lo) >> 8);
            }
        }
    } else {
        for (i = 0; i < total; i++) {
            out->data[i] = (tt_u8)tt__read_pnm_int(f);
        }
    }

    fclose(f);
    return 0;
}

TT_INLINE int tt_image_write_pgm(const char *path, const TT_Image *img) {
    FILE *f;
    if (img->channels != 1) return -1;
    f = fopen(path, "wb");
    if (!f) return -2;
    fprintf(f, "P5\n%d %d\n255\n", img->w, img->h);
    fwrite(img->data, 1, (size_t)(img->w * img->h), f);
    fclose(f);
    return 0;
}

TT_INLINE int tt_image_write_ppm(const char *path, const TT_Image *img) {
    FILE *f;
    if (img->channels != 3) return -1;
    f = fopen(path, "wb");
    if (!f) return -2;
    fprintf(f, "P6\n%d %d\n255\n", img->w, img->h);
    fwrite(img->data, 1, (size_t)(img->w * img->h * 3), f);
    fclose(f);
    return 0;
}

TT_INLINE void tt_image_free(TT_Image *img) { free(img->data); img->data = NULL; }

TT_INLINE void tt_image_to_ascii(int x, int y, const TT_Image *img, int scale) {
    static const char *shades = " .:-=+*#%@";
    int row, col;
    if (scale < 1) scale = 1;
    for (row = 0; row < img->h; row += scale) {
        for (col = 0; col < img->w; col += scale) {
            int pix;
            if (img->channels == 1) {
                pix = img->data[row * img->w + col];
            } else {
                int base = (row * img->w + col) * 3;
                pix = (img->data[base] + img->data[base+1] + img->data[base+2]) / 3;
            }
            {
                char c2[2];
                c2[0] = shades[pix * 10 / 256]; c2[1] = '\0';
                tt_draw(x + col/scale, y + row/scale, c2);
            }
        }
    }
}


TT_INLINE char *tt_strtrim(char *s) {
    char *start = s, *end;
    while (isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);
    end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end-1))) end--;
    *end = '\0';
    return s;
}

TT_INLINE char *tt_strreplace(const char *src, const char *old_str, const char *new_str) {
    size_t olen = strlen(old_str);
    size_t nlen = strlen(new_str);
    size_t count = 0;
    const char *p;
    char *result, *q;
    size_t total;

    if (olen == 0) return tt_strdup(src);
    for (p = src; (p = strstr(p, old_str)) != NULL; p += olen) count++;
    total = strlen(src) + count * (nlen - olen) + 1;
    result = (char*)malloc(total);
    if (!result) return NULL;
    q = result;
    p = src;
    while (*p) {
        if (strncmp(p, old_str, olen) == 0) {
            memcpy(q, new_str, nlen);
            q += nlen; p += olen;
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';
    return result;
}

TT_INLINE int tt_strsplit(const char *src, char delim,
                           char **out, int out_max, char **buf_out) {
    char *buf = tt_strdup(src);
    char *p;
    int count = 0;
    if (!buf) { *buf_out = NULL; return 0; }
    *buf_out = buf;
    p = buf;
    while (*p && count < out_max) {
        out[count++] = p;
        while (*p && *p != delim) p++;
        if (*p == delim) { *p++ = '\0'; }
    }
    return count;
}

TT_INLINE int tt_startswith(const char *s, const char *prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}
TT_INLINE int tt_endswith(const char *s, const char *suffix) {
    size_t sl = strlen(s), xl = strlen(suffix);
    if (xl > sl) return 0;
    return strcmp(s + sl - xl, suffix) == 0;
}

TT_INLINE char *tt_strjoin(const char **parts, int count, const char *sep) {
    size_t sep_len = strlen(sep);
    size_t total = 1;
    int i;
    char *out, *p;
    for (i = 0; i < count; i++) total += strlen(parts[i]) + sep_len;
    out = (char*)malloc(total);
    if (!out) return NULL;
    p = out;
    for (i = 0; i < count; i++) {
        size_t l = strlen(parts[i]);
        memcpy(p, parts[i], l); p += l;
        if (i < count-1) { memcpy(p, sep, sep_len); p += sep_len; }
    }
    *p = '\0';
    return out;
}

TT_INLINE char *tt_strndup(const char *s, size_t n) {
    size_t len = strlen(s);
    char *p;
    if (n < len) len = n;
    p = (char*)malloc(len + 1);
    if (!p) return NULL;
    memcpy(p, s, len);
    p[len] = '\0';
    return p;
}

TT_INLINE char *tt_itoa_buf(int v, char *buf, int buf_sz) {
    tt_snprintf(buf, (size_t)buf_sz, "%d", v);
    return buf;
}
TT_INLINE char *tt_ftoa_buf(double v, int decimals, char *buf, int buf_sz) {
    char fmt[16];
    tt_snprintf(fmt, sizeof(fmt), "%%.%df", decimals);
    tt_snprintf(buf, (size_t)buf_sz, fmt, v);
    return buf;
}

TT_INLINE double tt_atof_safe(const char *s, double def) {
    char *end;
    double v;
    if (!s || !*s) return def;
    v = strtod(s, &end);
    return (end != s) ? v : def;
}
TT_INLINE int tt_atoi_safe(const char *s, int def) {
    char *end;
    long v;
    if (!s || !*s) return def;
    v = strtol(s, &end, 10);
    return (end != s) ? (int)v : def;
}

TT_INLINE int tt_clamp_i(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }
TT_INLINE double tt_clamp_f(double v, double lo, double hi) { return v < lo ? lo : v > hi ? hi : v; }
TT_INLINE int tt_min_i(int a, int b) { return a < b ? a : b; }
TT_INLINE int tt_max_i(int a, int b) { return a > b ? a : b; }
TT_INLINE double tt_min_f(double a, double b) { return a < b ? a : b; }
TT_INLINE double tt_max_f(double a, double b) { return a > b ? a : b; }


#define TT_PI 3.14159265358979323846
#define TT_TAU (2.0 * TT_PI)
#define TT_DEG2RAD(d) ((d) * TT_PI / 180.0)
#define TT_RAD2DEG(r) ((r) * 180.0 / TT_PI)

TT_INLINE double tt_wrap_angle(double a) {
    while (a >  TT_PI) a -= TT_TAU;
    while (a < -TT_PI) a += TT_TAU;
    return a;
}

TT_INLINE double tt_dist2d(double x1, double y1, double x2, double y2) {
    double dx = x2-x1, dy = y2-y1;
    return sqrt(dx*dx + dy*dy);
}

static tt_u32 tt__rand_state = 12345;
TT_INLINE void   tt_srand(tt_u32 seed) { tt__rand_state = seed; }
TT_INLINE tt_u32 tt_rand(void) {
    tt__rand_state = tt__rand_state * 1664525u + 1013904223u;
    return tt__rand_state;
}
TT_INLINE double tt_randf(void) { return (double)(tt_rand() >> 1) / (double)0x7FFFFFFF; }
TT_INLINE int tt_rand_range(int lo, int hi) {
    if (hi <= lo) return lo;
    return lo + (int)(tt_rand() % (tt_u32)(hi - lo + 1));
}


TT_INLINE long tt_file_read_all(const char *path, char **out) {
    FILE *f;
    long size;
    char *buf;
    *out = NULL;
    f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);
    buf = (char*)malloc((size_t)size + 1);
    if (!buf) { fclose(f); return -2; }
    fread(buf, 1, (size_t)size, f);
    buf[size] = '\0';
    fclose(f);
    *out = buf;
    return size;
}

TT_INLINE int tt_file_write_all(const char *path, const char *data, size_t len) {
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fwrite(data, 1, len, f);
    fclose(f);
    return 0;
}

TT_INLINE int tt_file_exists(const char *path) {
#ifdef TT_WINDOWS
    return _access(path, 0) == 0;
#else
    return access(path, F_OK) == 0;
#endif
}

TT_INLINE long tt_file_size(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return (long)st.st_size;
}

TT_INLINE int tt_mkdir(const char *path) {
#ifdef TT_WINDOWS
    return _mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}

TT_INLINE const char *tt_basename(const char *path) {
    const char *p = strrchr(path, '/');
#ifdef TT_WINDOWS
    const char *p2 = strrchr(path, '\\');
    if (p2 > p) p = p2;
#endif
    return p ? p+1 : path;
}

TT_INLINE void tt_path_join(char *out, size_t n, const char *a, const char *b) {
    size_t la = strlen(a);
    tt_snprintf(out, n, "%s", a);
    if (la > 0 && a[la-1] != TT_PATH_SEP && b[0] != TT_PATH_SEP)
        tt_snprintf(out + la, n - la, "%c%s", TT_PATH_SEP, b);
    else
        tt_snprintf(out + la, n - la, "%s", b);
}

TT_INLINE int tt_list_dir(const char *path, char entries[][256], int max_entries) {
    int count = 0;
#ifdef TT_WINDOWS
    WIN32_FIND_DATAA fd;
    HANDLE h;
    char pattern[512];
    tt_snprintf(pattern, sizeof(pattern), "%s\\*", path);
    h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;
    do {
        if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
            tt_snprintf(entries[count++], 256, "%s", fd.cFileName);
    } while (FindNextFileA(h, &fd) && count < max_entries);
    FindClose(h);
#elif defined(TT_POSIX)
    DIR *d = opendir(path);
    struct dirent *ent;
    if (!d) return 0;
    while ((ent = readdir(d)) && count < max_entries) {
        if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
            tt_snprintf(entries[count++], 256, "%s", ent->d_name);
    }
    closedir(d);
#endif
    return count;
}


TT_INLINE void *tt_memdup(const void *src, size_t n) {
    void *p = malloc(n);
    if (p) memcpy(p, src, n);
    return p;
}

TT_INLINE void tt_memzero(volatile void *p, size_t n) {
    volatile unsigned char *b = (volatile unsigned char*)p;
    size_t i;
    for (i = 0; i < n; i++) b[i] = 0;
}

typedef struct {
    unsigned char *base;
    size_t used;
    size_t cap;
} TT_Arena;

TT_INLINE int tt_arena_init(TT_Arena *a, size_t cap) {
    a->base = (unsigned char*)malloc(cap);
    a->used = 0;
    a->cap  = cap;
    return a->base != NULL;
}

TT_INLINE void *tt_arena_alloc(TT_Arena *a, size_t n) {
    size_t align = sizeof(void*);
    size_t aligned_n = (n + align - 1) & ~(align - 1);
    void *p;
    if (a->used + aligned_n > a->cap) return NULL;
    p = a->base + a->used;
    a->used += aligned_n;
    return p;
}

TT_INLINE void tt_arena_reset(TT_Arena *a) { a->used = 0; }
TT_INLINE void tt_arena_free(TT_Arena *a)  { free(a->base); a->base = NULL; a->used = a->cap = 0; }


#define TT_HT_LOAD_MAX 0.7
#define TT_HT_MIN_CAP  16

typedef struct { char *key; void *val; } TT__HtSlot;

typedef struct {
    TT__HtSlot *slots;
    int count;
    int cap;
} TT_HTable;

TT_INLINE tt_u32 tt__fnv1a(const char *s) {
    tt_u32 h = 2166136261u;
    while (*s) { h ^= (tt_u8)*s++; h *= 16777619u; }
    return h;
}

TT_INLINE int tt_ht_init(TT_HTable *ht, int initial_cap) {
    ht->cap   = initial_cap > TT_HT_MIN_CAP ? initial_cap : TT_HT_MIN_CAP;
    ht->count = 0;
    ht->slots = (TT__HtSlot*)calloc((size_t)ht->cap, sizeof(TT__HtSlot));
    return ht->slots != NULL;
}

TT_INLINE void tt_ht_free(TT_HTable *ht) {
    int i;
    for (i = 0; i < ht->cap; i++) free(ht->slots[i].key);
    free(ht->slots);
    ht->slots = NULL;
}

static int tt_ht_set(TT_HTable *ht, const char *key, void *val);

static int tt__ht_resize(TT_HTable *ht) {
    int old_cap = ht->cap, i;
    TT__HtSlot *old = ht->slots;
    TT_HTable nt;
    tt_ht_init(&nt, old_cap * 2);
    for (i = 0; i < old_cap; i++)
        if (old[i].key) tt_ht_set(&nt, old[i].key, old[i].val);
    for (i = 0; i < old_cap; i++) free(old[i].key);
    free(old);
    ht->slots = nt.slots;
    ht->cap   = nt.cap;
    return 1;
}

TT_INLINE int tt_ht_set(TT_HTable *ht, const char *key, void *val) {
    tt_u32 h;
    int idx;
    if ((double)ht->count / (double)ht->cap > TT_HT_LOAD_MAX)
        tt__ht_resize(ht);
    h   = tt__fnv1a(key);
    idx = (int)(h % (tt_u32)ht->cap);
    while (ht->slots[idx].key && strcmp(ht->slots[idx].key, key) != 0)
        idx = (idx + 1) % ht->cap;
    if (!ht->slots[idx].key) {
        ht->slots[idx].key = tt_strdup(key);
        ht->count++;
    }
    ht->slots[idx].val = val;
    return 1;
}

TT_INLINE void *tt_ht_get(const TT_HTable *ht, const char *key) {
    tt_u32 h = tt__fnv1a(key);
    int idx  = (int)(h % (tt_u32)ht->cap);
    while (ht->slots[idx].key) {
        if (strcmp(ht->slots[idx].key, key) == 0) return ht->slots[idx].val;
        idx = (idx + 1) % ht->cap;
    }
    return NULL;
}

TT_INLINE void tt_ht_del(TT_HTable *ht, const char *key) {
    tt_u32 h = tt__fnv1a(key);
    int idx  = (int)(h % (tt_u32)ht->cap);
    while (ht->slots[idx].key) {
        if (strcmp(ht->slots[idx].key, key) == 0) {
            free(ht->slots[idx].key);
            ht->slots[idx].key = NULL;
            ht->slots[idx].val = NULL;
            ht->count--;
            return;
        }
        idx = (idx + 1) % ht->cap;
    }
}


typedef struct {
    void  *data;
    int    count;
    int    cap;
    size_t elem_size;
} TT_DArr;

TT_INLINE void tt_darr_init(TT_DArr *a, size_t elem_size) {
    a->data      = NULL;
    a->count     = 0;
    a->cap       = 0;
    a->elem_size = elem_size;
}

TT_INLINE int tt_darr_push(TT_DArr *a, const void *elem) {
    if (a->count >= a->cap) {
        int new_cap = a->cap ? a->cap * 2 : 8;
        void *p = realloc(a->data, (size_t)new_cap * a->elem_size);
        if (!p) return 0;
        a->data = p;
        a->cap  = new_cap;
    }
    memcpy((unsigned char*)a->data + (size_t)a->count * a->elem_size,
           elem, a->elem_size);
    a->count++;
    return 1;
}

TT_INLINE void *tt_darr_at(const TT_DArr *a, int i) {
    if (i < 0 || i >= a->count) return NULL;
    return (unsigned char*)a->data + (size_t)i * a->elem_size;
}

TT_INLINE void tt_darr_free(TT_DArr *a) { free(a->data); a->data = NULL; a->count = a->cap = 0; }


typedef struct TT_ListNode {
    void               *data;
    struct TT_ListNode *next;
} TT_ListNode;

TT_INLINE TT_ListNode *tt_list_push_front(TT_ListNode **head, void *data) {
    TT_ListNode *n = (TT_ListNode*)malloc(sizeof(TT_ListNode));
    if (!n) return NULL;
    n->data = data;
    n->next = *head;
    *head   = n;
    return n;
}

TT_INLINE void tt_list_free(TT_ListNode **head) {
    TT_ListNode *n = *head;
    while (n) { TT_ListNode *next = n->next; free(n); n = next; }
    *head = NULL;
}


typedef struct {
    unsigned char *buf;
    int            cap;                          
    int            head, tail;
    size_t         elem_size;
} TT_RingBuf;

TT_INLINE int tt_ring_init(TT_RingBuf *r, int cap, size_t elem_size) {
    int c = 1; while (c < cap) c <<= 1;
    r->buf       = (unsigned char*)malloc((size_t)c * elem_size);
    r->cap       = c;
    r->head      = 0;
    r->tail      = 0;
    r->elem_size = elem_size;
    return r->buf != NULL;
}

TT_INLINE int tt_ring_push(TT_RingBuf *r, const void *elem) {
    int next = (r->tail + 1) & (r->cap - 1);
    if (next == r->head) return 0;           
    memcpy(r->buf + (size_t)r->tail * r->elem_size, elem, r->elem_size);
    r->tail = next;
    return 1;
}

TT_INLINE int tt_ring_pop(TT_RingBuf *r, void *out) {
    if (r->head == r->tail) return 0;            
    memcpy(out, r->buf + (size_t)r->head * r->elem_size, r->elem_size);
    r->head = (r->head + 1) & (r->cap - 1);
    return 1;
}

TT_INLINE void tt_ring_free(TT_RingBuf *r) { free(r->buf); r->buf = NULL; }


TT_INLINE tt_u32 tt_crc32(const void *data, size_t len) {
    static tt_u32 table[256];
    static int init = 0;
    const tt_u8 *p = (const tt_u8*)data;
    tt_u32 crc = 0xFFFFFFFFu;
    size_t i;
    if (!init) {
        tt_u32 k;
        for (k = 0; k < 256; k++) {
            tt_u32 c = k; int j;
            for (j = 0; j < 8; j++)
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            table[k] = c;
        }
        init = 1;
    }
    for (i = 0; i < len; i++)
        crc = table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}


TT_INLINE char *tt_base64_encode(const tt_u8 *src, size_t src_len) {
    static const char enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t out_len = ((src_len + 2) / 3) * 4 + 1;
    char *out = (char*)malloc(out_len);
    size_t i = 0, j = 0;
    if (!out) return NULL;
    while (i < src_len) {
        tt_u32 b = (tt_u32)src[i++] << 16;
        if (i < src_len) b |= (tt_u32)src[i++] << 8;
        if (i < src_len) b |= (tt_u32)src[i++];
        out[j++] = enc[(b >> 18) & 0x3F];
        out[j++] = enc[(b >> 12) & 0x3F];
        out[j++] = (i - (src_len - (src_len%3 ? src_len%3 : 3)) >= 2) ? '=' : enc[(b >>  6) & 0x3F];
        out[j++] = (i - (src_len - (src_len%3 ? src_len%3 : 3)) >= 1) ? '=' : enc[(b      ) & 0x3F];
    }
    j = 0;
    for (i = 0; i < src_len; ) {
        tt_u32 b = (tt_u32)src[i++] << 16;
        int rem = (int)(src_len - i);
        if (rem >= 1) b |= (tt_u32)src[i++] << 8;
        if (rem >= 2) b |= (tt_u32)src[i++];
        out[j++] = enc[(b >> 18) & 0x3F];
        out[j++] = enc[(b >> 12) & 0x3F];
        out[j++] = (rem >= 1) ? enc[(b >> 6) & 0x3F] : '=';
        out[j++] = (rem >= 2) ? enc[(b     ) & 0x3F] : '=';
    }
    out[j] = '\0';
    return out;
}


TT_INLINE void tt_hexdump(const void *data, size_t len, FILE *out) {
    const tt_u8 *p = (const tt_u8*)data;
    size_t i;
    for (i = 0; i < len; i++) {
        if (i % 16 == 0) fprintf(out, "%04x  ", (unsigned)i);
        fprintf(out, "%02x ", p[i]);
        if (i % 8 == 7) fprintf(out, " ");
        if (i % 16 == 15) {
            int k;
            fprintf(out, " |");
            for (k = -15; k <= 0; k++) fprintf(out, "%c", isprint(p[i+k]) ? p[i+k] : '.');
            fprintf(out, "|\n");
        }
    }
    if (len % 16) {
        int k;
        size_t trail = len % 16;
        for (k = (int)trail; k < 16; k++) fprintf(out, "   %s", k==8?"  ":"");
        fprintf(out, " |");
        for (k = 0; k < (int)trail; k++) fprintf(out, "%c", isprint(p[len - trail + k]) ? p[len - trail + k] : '.');
        fprintf(out, "|\n");
    }
}


TT_INLINE int tt_utf8_charlen(const tt_u8 *s) {
    if (!s || !*s) return 0;
    if ((*s & 0x80) == 0x00) return 1;
    if ((*s & 0xE0) == 0xC0) return 2;
    if ((*s & 0xF0) == 0xE0) return 3;
    if ((*s & 0xF8) == 0xF0) return 4;
    return 1;                          
}

TT_INLINE tt_u32 tt_utf8_decode(const tt_u8 **s) {
    int n = tt_utf8_charlen(*s);
    tt_u32 cp = 0;
    int i;
    switch (n) {
    case 1: cp = **s & 0x7F; break;
    case 2: cp = **s & 0x1F; break;
    case 3: cp = **s & 0x0F; break;
    case 4: cp = **s & 0x07; break;
    }
    (*s)++;
    for (i = 1; i < n; i++) { cp = (cp << 6) | (**s & 0x3F); (*s)++; }
    return cp;
}

TT_INLINE int tt_utf8_encode(tt_u32 cp, tt_u8 *buf) {
    if (cp < 0x80)    { buf[0] = (tt_u8)cp; return 1; }
    if (cp < 0x800)   { buf[0] = (tt_u8)(0xC0|(cp>>6)); buf[1] = (tt_u8)(0x80|(cp&0x3F)); return 2; }
    if (cp < 0x10000) {
        buf[0] = (tt_u8)(0xE0|(cp>>12));
        buf[1] = (tt_u8)(0x80|((cp>>6)&0x3F));
        buf[2] = (tt_u8)(0x80|(cp&0x3F));
        return 3;
    }
    buf[0] = (tt_u8)(0xF0|(cp>>18));
    buf[1] = (tt_u8)(0x80|((cp>>12)&0x3F));
    buf[2] = (tt_u8)(0x80|((cp>>6)&0x3F));
    buf[3] = (tt_u8)(0x80|(cp&0x3F));
    return 4;
}

TT_INLINE int tt_utf8_strlen(const char *s) {
    const tt_u8 *p = (const tt_u8*)s;
    int count = 0;
    while (*p) { p += tt_utf8_charlen(p); count++; }
    return count;
}


typedef struct { tt_u32 cp; const char *sym; } TT__EmojiMap;

static const TT__EmojiMap tt__emoji_table[] = {
    {0x1F600,":-)"}, {0x1F601,":D"},  {0x1F602,"XD"}, {0x1F603,":-)"},
    {0x1F604,":D"},  {0x1F605,"^^;"}, {0x1F606,"XD"}, {0x1F607,"O:-)"},
    {0x1F608,">;->"},{0x1F60D,"<3"},  {0x1F60E,"B-)"},  {0x1F610,"-_-"},
    {0x1F614,":("},  {0x1F615,":-?"}, {0x1F621,">:("},  {0x1F622,":'-{"},
    {0x1F623,"dx"},  {0x1F624,">:("}, {0x1F625,":'-("}, {0x1F629,":("},
    {0x1F62D,"T_T"}, {0x1F631,"D:"}, {0x1F633,":O"}, {0x1F63A,"^.^"},
    {0x2764,"<3"},   {0x1F493,"<3"}, {0x1F494,"</3"},{0x1F495,"<3"},
    {0x1F496,"<3"},  {0x1F499,"<3"}, {0x1F49A,"<3"}, {0x1F9E1,"<3"},
    {0x2665,"<3"},   {0x2666,"<>"},  {0x2660,"(s)"}, {0x2663,"(c)"},
    {0x2605,"*"},    {0x2606,"*"},   {0x2713,"[v]"}, {0x2714,"[v]"},
    {0x2717,"[x]"},  {0x2718,"[x]"},
    {0x2757,"!"},    {0x2753,"?"},   {0x2754,"?"},   {0x2755,"!"},
    {0x203C,"!!"},   {0x2049,"!?"},
    {0x27A1,"->"},   {0x2B05,"<-"},  {0x2B06,"^"},   {0x2B07,"v"},
    {0x25B6,">"},    {0x25C0,"<"},   {0x23F0,"[T]"}, {0x231B,"[T]"},
    {0x23F3,"[T]"},  {0x1F550,"[T]"},{0x1F551,"[T]"},{0x1F552,"[T]"},
    {0x1F4BB,"[PC]"},{0x1F4BE,"[S]"},{0x1F4BF,"[D]"},{0x1F4C1,"[/]"},
    {0x1F4C2,"[/]"},{0x1F4C3,"[P]"},{0x1F4C4,"[P]"},{0x1F4C5,"[C]"},
    {0x1F4C6,"[C]"},{0x1F4CB,"[P]"},{0x1F4CC,"[@]"},{0x1F4CD,"[*]"},
    {0x1F4CE,"[&]"},{0x1F4CF,"[-]"},{0x1F527,"[W]"},{0x1F528,"[H]"},
    {0x1F529,"[N]"},{0x1F52A,"[!]"},{0x1F52B,"[G]"},{0x1F4A1,"[!]"},
    {0x1F4A5,"[!]"},{0x1F4A6,"~"},  {0x1F4A7,"~"},  {0x1F525,"[F]"},
    {0x1F4A9,"(S)"},
    {0x1F331,"[>]"},{0x1F332,"[T]"},{0x1F333,"[T]"},{0x1F334,"[+]"},
    {0x1F335,"[|]"},{0x1F340,"[+]"},{0x1F341,"[L]"},{0x1F342,"[L]"},
    {0x2602,"[A]"}, {0x2614,"[A]"}, {0x26C4,"[S]"}, {0x2600,"[O]"},
    {0x1F31E,"[O]"},{0x1F314,"[C]"},{0x2B50,"*"},   {0x1F320,"*"},
    {0x1F534,"[R]"},{0x1F7E0,"[O]"},{0x1F7E1,"[Y]"},{0x1F7E2,"[G]"},
    {0x1F7E3,"[P]"},{0x1F7E4,"[B]"},{0x2B24,"[O]"}, {0x25AA,"[.]"},
    {0x25AB,"[.]"}, {0x25FD,"[.]"},  {0x25FE,"[.]"},
    {0x2714,"[+]"}, {0x2716,"[x]"},  {0x274C,"[x]"},{0x274E,"[x]"},
    {0x2705,"[v]"}, {0x26A0,"[!]"},  {0x1F6AB,"[-]"},
    {0x1F408,"[=^.^=]"},{0x1F415,"[dog]"},{0x1F40D,"[~]"},
    {0x1F422,"[T]"},    {0x1F418,"[E]"},   {0x1F427,"[>]"},
    {0x1F44D,"[+1]"},{0x1F44E,"[-1]"},{0x1F44B,"[~]"},{0x1F44F,"*clap*"},
    {0x1F64F,"[_]"},{0x1F4AA,"[M]"},
    {0x1F3AE,"[>]"},{0x1F579,"[>]"},
    {0, NULL}
};

TT_INLINE int tt_strip_emoji(const char *in, char *out, size_t out_sz) {
    const tt_u8 *p  = (const tt_u8*)in;
    char *q         = out;
    char *end       = out + out_sz - 1;
    int replacements = 0;

    while (*p && q < end) {
        tt_u32 cp;
        const tt_u8 *before = p;
        int i;
        int cl = tt_utf8_charlen(p);

        {
            const tt_u8 *tmp = p;
            cp = tt_utf8_decode(&tmp);
        }

        for (i = 0; tt__emoji_table[i].sym; i++) {
            if (tt__emoji_table[i].cp == cp) {
                const char *sym = tt__emoji_table[i].sym;
                size_t sym_len  = strlen(sym);
                if (q + sym_len <= end) {
                    memcpy(q, sym, sym_len);
                    q += sym_len;
                    p += cl;
                    replacements++;
                    goto next_cp;
                }
            }
        }

        if ((cp >= 0x1F000 && cp <= 0x1FFFF) ||
            (cp >= 0x2600  && cp <= 0x27FF)  ||
            (cp >= 0xFE00  && cp <= 0xFE0F)) {                          
            p += cl;
            goto next_cp;
        }

        {
            int k;
            for (k = 0; k < cl && q < end; k++) *q++ = (char)*before++;
            p += cl;
        }
        next_cp:;
    }
    *q = '\0';
    return replacements;
}


TT_INLINE void tt_strip_ansi(const char *in, char *out, size_t out_sz) {
    const char *p = in;
    char *q = out;
    char *end = out + out_sz - 1;
    while (*p && q < end) {
        if (*p == '\033' && *(p+1) == '[') {
            p += 2;
            while (*p && !isalpha((unsigned char)*p)) p++;
            if (*p) p++;
        } else {
            *q++ = *p++;
        }
    }
    *q = '\0';
}


typedef struct {
    double start;
    double elapsed;
    int    iterations;
} TT_Bench;

TT_INLINE void tt_bench_start(TT_Bench *b) {
    b->start      = tt_time();
    b->elapsed    = 0.0;
    b->iterations = 0;
}

TT_INLINE void tt_bench_end(TT_Bench *b, int iters) {
    b->elapsed    = tt_time() - b->start;
    b->iterations = iters;
}

TT_INLINE void tt_bench_print(const TT_Bench *b, const char *label) {
    double ops = (b->elapsed > 0.0) ? (double)b->iterations / b->elapsed : 0.0;
    printf("[BENCH] %s: %d ops in %.4fs (%.0f ops/s)\n",
           label, b->iterations, b->elapsed, ops);
}


#define TT_LOG_DEBUG 0
#define TT_LOG_INFO  1
#define TT_LOG_WARN  2
#define TT_LOG_ERROR 3

typedef struct {
    int   min_level;
    FILE *out;
    int   use_color;
    int   show_time;
} TT_Logger;

TT_INLINE void tt_log_init(TT_Logger *l, int min_level, FILE *out,
                            int use_color, int show_time) {
    l->min_level = min_level;
    l->out       = out ? out : stderr;
    l->use_color = use_color;
    l->show_time = show_time;
}

TT_INLINE void tt_log(TT_Logger *l, int level, const char *fmt, ...) {
    static const char *labels[] = {"DEBUG","INFO ","WARN ","ERROR"};
    static const char *colors[] = {"\033[36m","\033[32m","\033[33m","\033[31m"};
    va_list ap;
    char buf[1024];

    if (!l || level < l->min_level) return;
    va_start(ap, fmt);
    tt_vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (l->show_time) {
        time_t t = time(NULL);
        char ts[32];
        strftime(ts, sizeof(ts), "%H:%M:%S", localtime(&t));
        if (l->use_color)
            fprintf(l->out, "%s[%s] [%s%s\033[0m] %s\n",
                    "\033[90m", ts, colors[level < 4 ? level : 3],
                    labels[level < 4 ? level : 3], buf);
        else
            fprintf(l->out, "[%s] [%s] %s\n", ts, labels[level < 4 ? level : 3], buf);
    } else {
        if (l->use_color)
            fprintf(l->out, "[%s%s\033[0m] %s\n",
                    colors[level < 4 ? level : 3],
                    labels[level < 4 ? level : 3], buf);
        else
            fprintf(l->out, "[%s] %s\n", labels[level < 4 ? level : 3], buf);
    }
    fflush(l->out);
}


#define TT_INI_MAX_ENTRIES 128
#define TT_INI_KEY_LEN      64
#define TT_INI_VAL_LEN     256

typedef struct {
    char section[TT_INI_KEY_LEN];
    char key[TT_INI_KEY_LEN];
    char value[TT_INI_VAL_LEN];
} TT_IniEntry;

typedef struct {
    TT_IniEntry entries[TT_INI_MAX_ENTRIES];
    int         count;
} TT_Ini;

TT_INLINE int tt_ini_parse(const char *text, TT_Ini *ini) {
    char section[TT_INI_KEY_LEN] = "";
    const char *p = text;
    ini->count = 0;
    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
        if (!*p) break;
        if (*p == ';' || *p == '#') {              
            while (*p && *p != '\n') p++;
        } else if (*p == '[') {              
            const char *s = ++p;
            while (*p && *p != ']' && *p != '\n') p++;
            tt_snprintf(section, TT_INI_KEY_LEN, "%.*s", (int)(p-s), s);
            if (*p == ']') p++;
        } else {                
            const char *ks = p;
            char key[TT_INI_KEY_LEN], val[TT_INI_VAL_LEN];
            while (*p && *p != '=' && *p != '\n') p++;
            tt_snprintf(key, TT_INI_KEY_LEN, "%.*s", (int)(p-ks), ks);
            tt_strtrim(key);
            if (*p == '=') {
                const char *vs;
                p++;
                vs = p;
                while (*p && *p != '\n') p++;
                tt_snprintf(val, TT_INI_VAL_LEN, "%.*s", (int)(p-vs), vs);
                tt_strtrim(val);
                if (ini->count < TT_INI_MAX_ENTRIES) {
                    TT_IniEntry *e = &ini->entries[ini->count++];
                    tt_snprintf(e->section, TT_INI_KEY_LEN, "%s", section);
                    tt_snprintf(e->key,     TT_INI_KEY_LEN, "%s", key);
                    tt_snprintf(e->value,   TT_INI_VAL_LEN, "%s", val);
                }
            }
        }
    }
    return ini->count;
}

TT_INLINE int tt_ini_parse_file(const char *path, TT_Ini *ini) {
    char *buf;
    long sz = tt_file_read_all(path, &buf);
    int r;
    if (sz < 0) return -1;
    r = tt_ini_parse(buf, ini);
    free(buf);
    return r;
}

TT_INLINE const char *tt_ini_get(const TT_Ini *ini, const char *section,
                                   const char *key) {
    int i;
    for (i = 0; i < ini->count; i++) {
        if (tt_strcasecmp(ini->entries[i].section, section) == 0 &&
            tt_strcasecmp(ini->entries[i].key, key) == 0)
            return ini->entries[i].value;
    }
    return NULL;
}


#define TT_CSV_MAX_COLS 64
#define TT_CSV_MAX_ROWS 4096

typedef struct {
    char  **cells;                                 
    int     nrows, ncols;
    char   *_buf;                           
} TT_Csv;

TT_INLINE void tt_csv_free(TT_Csv *csv);

TT_INLINE int tt_csv_parse(const char *text, TT_Csv *csv, char delim) {
    const char *p = text;
    int rows = 0, cols = 0, max_cols = 0;
    char *buf;
    char **cells;
    char *bp;
    int r, c;

    while (*p) { if (*p++ == '\n') rows++; }
    if (p > text && *(p-1) != '\n') rows++;
    if (rows == 0) { csv->cells = NULL; csv->nrows = csv->ncols = 0; csv->_buf = NULL; return 0; }

    p = text;
    cols = 1;
    while (*p && *p != '\n') { if (*p++ == delim) cols++; }
    max_cols = cols;

    buf = tt_strdup(text);
    if (!buf) return -1;
    cells = (char**)calloc((size_t)(rows * max_cols), sizeof(char*));
    if (!cells) { free(buf); return -2; }

    bp = buf;
    for (r = 0; r < rows && *bp; r++) {
        for (c = 0; c < max_cols; c++) {
            char *start = bp;
            while (*bp && *bp != delim && *bp != '\n' && *bp != '\r') bp++;
            cells[r * max_cols + c] = start;
            if (*bp == delim) { *bp++ = '\0'; }
            else if (*bp == '\r' || *bp == '\n') {
                *bp = '\0';
                while (*bp == '\r' || *bp == '\n') bp++;
                break;
            } else { break; }
        }
    }

    csv->cells = cells;
    csv->nrows = rows;
    csv->ncols = max_cols;
    csv->_buf  = buf;
    return rows;
}

TT_INLINE void tt_csv_free(TT_Csv *csv) {
    free(csv->cells);
    free(csv->_buf);
    csv->cells = NULL;
    csv->_buf  = NULL;
    csv->nrows = csv->ncols = 0;
}

TT_INLINE const char *tt_csv_cell(const TT_Csv *csv, int row, int col) {
    if (row < 0 || row >= csv->nrows || col < 0 || col >= csv->ncols) return NULL;
    return csv->cells[row * csv->ncols + col];
}


TT_INLINE void tt_crosshair(int center_only) {
    TT_Size sz = tt_term_size();
    int cx = sz.w / 2, cy = sz.h / 2;
    int i;
    if (!center_only) {
        tt_goto(1, cy);
        for (i = 0; i < sz.w; i++)
            tt_write(i == cx-1 ? "+" : "-");
        for (i = 1; i <= sz.h; i++) {
            if (i != cy) tt_draw(cx, i, "|");
        }
    } else {
        tt_draw(cx, cy, "+");
    }
}


TT_INLINE void tt_fluid_drop(int x, int y, int radius, int ansi_color) {
    int dy, dx;
    char col[24];
    tt_fg(col, sizeof(col), ansi_color);
    for (dy = -radius; dy <= radius; dy++) {
        for (dx = -radius; dx <= radius; dx++) {
            if (dx*dx + dy*dy <= radius*radius) {
                tt_goto(x+dx, y+dy);
                tt_write(col);
                tt_write("*");
                tt_write(TT_RESET);
            }
        }
    }
}


TT_INLINE void tt_screen_noise(double density) {
    static const char *chars[] = {".", ":", "+", "*", "#"};
    TT_Size sz = tt_term_size();
    int x, y;
    for (y = 1; y <= sz.h; y++) {
        for (x = 1; x <= sz.w; x++) {
            if (tt_randf() < density) {
                tt_draw(x, y, chars[tt_rand() % 5]);
            }
        }
    }
}


typedef struct { const char *cmd; const char *desc; } TT_HelpItem;

TT_INLINE void tt_help_menu(int x, int y,
                             const TT_HelpItem *items, int count) {
    int max_cmd = 0, i;
    for (i = 0; i < count; i++) {
        int l = (int)strlen(items[i].cmd);
        if (l > max_cmd) max_cmd = l;
    }
    tt_box(x, y, max_cmd + 20, count + 3, &TT_BOX_SINGLE, "HELP");
    for (i = 0; i < count; i++) {
        char line[128];
        tt_snprintf(line, sizeof(line), "%-*s  %s",
                    max_cmd, items[i].cmd, items[i].desc);
        tt_draw(x+2, y+2+i, line);
    }
}


typedef struct { const char *key; const char *val; } TT_DebugVar;

TT_INLINE void tt_debug_overlay(const TT_DebugVar *vars, int count) {
    int i;
    char col[24];
    tt_fg(col, sizeof(col), 9);          
    tt_draw(1, 1, col);
    tt_write("[DEBUG]");
    tt_write(TT_RESET);
    for (i = 0; i < count; i++) {
        char line[128];
        tt_snprintf(line, sizeof(line), "%-16s: %s", vars[i].key, vars[i].val);
        tt_draw(1, 2+i, line);
    }
}


typedef struct { double pos; const char *label; } TT_TimelineEvent;

TT_INLINE void tt_timeline(int x, int y, const TT_TimelineEvent *events,
                            int count, int width) {
    int i;
    char row[512];
    memset(row, '-', (size_t)width);
    row[width] = '\0';
    tt_draw(x, y, row);
    for (i = 0; i < count; i++) {
        int idx = (int)(events[i].pos * width);
        if (idx >= 0 && idx < width) {
            tt_draw(x + idx, y-1, "^");
            tt_draw(x + idx - (int)strlen(events[i].label)/2, y+1, events[i].label);
        }
    }
}


TT_INLINE void tt_fps_counter(int x, int y, double fps) {
    char buf[32];
    tt_snprintf(buf, sizeof(buf), "FPS:%.0f", fps);
    tt_draw(x, y, buf);
}


TT_INLINE void tt_digital_clock(int x, int y, const char *fmt) {
    time_t t = time(NULL);
    char buf[64];
    struct tm *tm_info = localtime(&t);
    strftime(buf, sizeof(buf), fmt ? fmt : "%H:%M:%S", tm_info);
    tt_draw(x, y, TT_BOLD);
    tt_write(buf);
    tt_write(TT_RESET);
}


TT_INLINE void tt_weather_effect(int x, int y, int width,
                                  const char *effect, double density) {
    const char *ch = "|";
    int count, i;
    if      (strcmp(effect, "snow") == 0) ch = "*";
    else if (strcmp(effect, "sand") == 0) ch = ".";
    count = (int)((double)width * density);
    for (i = 0; i < count; i++) {
        int pos = x + tt_rand_range(0, width-1);
        tt_draw(pos, y, ch);
    }
}


TT_INLINE void tt_file_explorer(int x, int y, const char *path, int max_items) {
    char entries[64][256];
    int count, i;
    char header[512];
    tt_snprintf(header, sizeof(header), "[DIR] %s", path);
    tt_draw(x, y, header);
    count = tt_list_dir(path, entries, max_items < 64 ? max_items : 64);
    for (i = 0; i < count; i++) {
        char full[512], line[512];
        char is_dir = ' ';
#ifdef TT_WINDOWS
        DWORD attr;
        tt_path_join(full, sizeof(full), path, entries[i]);
        attr = GetFileAttributesA(full);
        is_dir = (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) ? 'D' : 'F';
#elif defined(TT_POSIX)
        struct stat st;
        tt_path_join(full, sizeof(full), path, entries[i]);
        is_dir = (stat(full, &st) == 0 && S_ISDIR(st.st_mode)) ? 'D' : 'F';
#endif
        tt_snprintf(line, sizeof(line), "[%c] %s", is_dir, entries[i]);
        tt_draw(x, y+1+i, line);
    }
}


typedef struct { int x, y; } TT_Point;

TT_INLINE void tt_motion_trail(const TT_Point *history, int count,
                                const char *ch, int fade) {
    int i;
    for (i = 0; i < count; i++) {
        char col[24];
        int ci = fade ? (8 + i / 2) : 10;
        if (ci > 15) ci = 15;
        tt_fg(col, sizeof(col), ci);
        tt_goto(history[i].x, history[i].y);
        tt_write(col);
        tt_write(ch);
        tt_write(TT_RESET);
    }
}


TT_INLINE void tt_shadow_text(int x, int y, const char *text,
                               int fg_color, int shadow_color) {
    char col[24];
    tt_fg(col, sizeof(col), shadow_color);
    tt_draw(x+1, y+1, col);
    tt_write(text);
    tt_write(TT_RESET);
    tt_fg(col, sizeof(col), fg_color);
    tt_draw(x, y, col);
    tt_write(text);
    tt_write(TT_RESET);
}


TT_INLINE void tt_modal(const char *prompt, const char **options,
                         int opt_count, int width) {
    TT_Size sz = tt_term_size();
    int x = (sz.w - width) / 2;
    int y = (sz.h - 5)     / 2;
    int i;
    char btn_text[256];
    char *p;
    tt_box(x, y, width, 5, &TT_BOX_DOUBLE, NULL);
    tt_draw(x+2, y+1, prompt);
    btn_text[0] = '\0';
    p = btn_text;
    for (i = 0; i < opt_count; i++) {
        if (i > 0) { *p++ = ' '; *p++ = '|'; *p++ = ' '; }
        tt_snprintf(p, (size_t)(btn_text + sizeof(btn_text) - p), "%s", options[i]);
        p += strlen(p);
    }
    tt_draw(x + (width - (int)strlen(btn_text)) / 2, y+3, btn_text);
}


TT_INLINE void tt_health_bar(int x, int y, int current, int max_val, int bar_width) {
    double pct = (max_val > 0) ? (double)current / (double)max_val : 0.0;
    int filled = (int)(pct * (double)bar_width);
    int r, g, i;
    char meter[64];

    pct = tt_clamp_f(pct, 0.0, 1.0);
    r   = (int)(255.0 * (1.0 - pct));
    g   = (int)(255.0 * pct);

    tt_goto(x, y);
    for (i = 0; i < bar_width; i++) {
        char col[32];
        tt_rgb_fg(col, sizeof(col), r, g, 0);
        tt_write(col);
        tt_write(i < filled ? "#" : ".");
    }
    tt_write(TT_RESET);
    tt_snprintf(meter, sizeof(meter), " %d/%d", current, max_val);
    tt_write(meter);
}


TT_INLINE void tt_loading_screen(const char *message, double duration_sec) {
    TT_Size sz = tt_term_size();
    int x = sz.w / 2 - 10, y = sz.h / 2;
    double start = tt_time();
    int frame = 0;
    static const char *spin[] = {"-", "\\", "|", "/"};
    char full[256];
    while (tt_time() - start < duration_sec) {
        tt_snprintf(full, sizeof(full), "%s %s", spin[frame++ % 4], message);
        tt_draw(x, y, full);
        tt_sleep_ms(100);
    }
    tt_fill(x, y, (int)strlen(full) + 2, 1, ' ');
}


TT_INLINE void tt_screen_flash(int ansi_color, unsigned int ms) {
    TT_Size sz = tt_term_size();
    char col[24];
    tt_fg(col, sizeof(col), ansi_color);
    tt_write(col);
    tt_fill(1, 1, sz.w, sz.h, ' ');
    tt_write(TT_RESET);
    tt_sleep_ms(ms);
    tt_clear();
}


typedef struct { int x, y; } TT_Cell;

TT_INLINE int tt_astar(const char **map2d, int map_w, int map_h,
                        TT_Cell start, TT_Cell goal,
                        TT_Cell *out_path, int max_path) {
    int *g_cost   = (int*)malloc((size_t)(map_w * map_h) * sizeof(int));
    int *f_cost   = (int*)malloc((size_t)(map_w * map_h) * sizeof(int));
    int *parent   = (int*)malloc((size_t)(map_w * map_h) * sizeof(int));
    int *open_buf = (int*)malloc((size_t)(map_w * map_h) * sizeof(int));
    int  open_cnt = 0;
    int  result   = -1;
    int  i;

    if (!g_cost || !f_cost || !parent || !open_buf) goto cleanup;

    for (i = 0; i < map_w * map_h; i++) {
        g_cost[i] = 0x7FFFFFFF;
        f_cost[i] = 0x7FFFFFFF;
        parent[i] = -1;
    }

#define ASTAR_IDX(cx,cy) ((cy)*map_w+(cx))
#define ASTAR_H(ax,ay,bx,by) (abs((ax)-(bx))+abs((ay)-(by)))

    {
        int si = ASTAR_IDX(start.x, start.y);
        g_cost[si] = 0;
        f_cost[si] = ASTAR_H(start.x, start.y, goal.x, goal.y);
        open_buf[open_cnt++] = si;
    }

    while (open_cnt > 0) {
        int best = 0, j;
        for (j = 1; j < open_cnt; j++)
            if (f_cost[open_buf[j]] < f_cost[open_buf[best]]) best = j;

        {
            int cur = open_buf[best];
            int cx  = cur % map_w, cy = cur / map_w;
            open_buf[best] = open_buf[--open_cnt];

            if (cx == goal.x && cy == goal.y) {
                int path_len = 0, c2 = cur;
                while (c2 >= 0 && path_len < max_path) path_len++, c2 = parent[c2];
                result = path_len;
                c2 = cur;
                for (i = path_len-1; i >= 0 && c2 >= 0; i--) {
                    out_path[i].x = c2 % map_w;
                    out_path[i].y = c2 / map_w;
                    c2 = parent[c2];
                }
                goto cleanup;
            }

            {
                static const int dx[] = {1,-1,0,0};
                static const int dy[] = {0,0,1,-1};
                for (i = 0; i < 4; i++) {
                    int nx = cx + dx[i], ny = cy + dy[i];
                    int ni;
                    if (nx < 0 || ny < 0 || nx >= map_w || ny >= map_h) continue;
                    if (map2d[ny][nx] == '#') continue;
                    ni = ASTAR_IDX(nx, ny);
                    {
                        int ng = g_cost[cur] + 1;
                        if (ng < g_cost[ni]) {
                            g_cost[ni] = ng;
                            f_cost[ni] = ng + ASTAR_H(nx,ny,goal.x,goal.y);
                            parent[ni] = cur;
                            if (open_cnt < map_w * map_h)
                                open_buf[open_cnt++] = ni;
                        }
                    }
                }
            }
        }
    }

#undef ASTAR_IDX
#undef ASTAR_H

cleanup:
    free(g_cost); free(f_cost); free(parent); free(open_buf);
    return result;
}


typedef struct { int bg; int fg; const char *name; } TT_Theme;

static const TT_Theme TT_THEMES[] = {
    {0,  15, "default"},
    {0,   2, "matrix"},
    {4,  11, "retro"},
    {8,  12, "solarized"},
    {235,186,"gruvbox"},
    {17, 159,"ocean"},
    {53, 219,"purple"},
    {0, 0, NULL}
};

TT_INLINE void tt_apply_theme(const char *name) {
    int i;
    char bg[24], fg[24];
    for (i = 0; TT_THEMES[i].name; i++) {
        if (tt_strcasecmp(TT_THEMES[i].name, name) == 0) {
            tt_bg(bg, sizeof(bg), TT_THEMES[i].bg);
            tt_fg(fg, sizeof(fg), TT_THEMES[i].fg);
            tt_write(bg);
            tt_write(fg);
            return;
        }
    }
}


TT_INLINE void tt_get_cursor_pos(int *x, int *y) {
    *x = 1; *y = 1;
#ifdef TT_POSIX
    {
        struct termios old, raw;
        char buf[32]; int i = 0, c;
        tcgetattr(STDIN_FILENO, &old);
        raw = old;
        raw.c_lflag &= (tcflag_t)~(ECHO | ICANON);
        raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        write(STDOUT_FILENO, "\033[6n", 4);
        while (i < (int)sizeof(buf)-1) {
            ssize_t n = read(STDIN_FILENO, buf+i, 1);
            if (n != 1) break;
            if (buf[i++] == 'R') break;
        }
        buf[i] = '\0';
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
        if (buf[0] == '\033' && buf[1] == '[') {
            c = sscanf(buf+2, "%d;%d", y, x);
            (void)c;
        }
    }
#endif
}


typedef struct {
    TT_Size last;
    void (*callback)(TT_Size, void*);
    void *userdata;
} TT_ResizeWatcher;

TT_INLINE void tt_resize_watcher_init(TT_ResizeWatcher *rw,
                                       void (*cb)(TT_Size, void*), void *ud) {
    rw->last     = tt_term_size();
    rw->callback = cb;
    rw->userdata = ud;
}

TT_INLINE TT_Size tt_resize_watcher_check(TT_ResizeWatcher *rw) {
    TT_Size curr = tt_term_size();
    if (curr.w != rw->last.w || curr.h != rw->last.h) {
        rw->last = curr;
        if (rw->callback) rw->callback(curr, rw->userdata);
    }
    return curr;
}


typedef void (*TT_ToolUpdate)(double dt, void *ctx);
typedef void (*TT_ToolDraw)(void *ctx);

TT_INLINE void tt_tool_run(TT_ToolUpdate update, TT_ToolDraw draw,
                            int fps, void *ctx) {
    TT_Clock clk;
    tt_clock_init(&clk, fps);
    for (;;) {
        double dt = tt_clock_tick(&clk);
        update(dt, ctx);
        draw(ctx);
    }
}


TT_INLINE void tt_version_banner(const char *name, const char *version,
                                  const char *author) {
    char col[24];
    tt_cls();
    tt_shadow_text(5, 3, name, 12, 8);
    tt_fg(col, sizeof(col), 14);
    tt_draw(5, 6, col);
    tt_writef("v%s  by %s", version, author);
    tt_write(TT_RESET);
    tt_sleep_ms(1500);
}


TT_INLINE void tt_self_test(void) {
    TT_Bench bench;
    TT_Console con;
    char stripped[256];
    int i;

    tt_cls();
    tt_draw(1, 1, TT_BOLD "[ termtools.h self-test ]" TT_RESET);

    tt_strip_emoji("Hello [*] World [!] Test", stripped, sizeof(stripped));
    tt_drawf(1, 3, "Emoji strip: %s", stripped);

    tt_drawf(1, 4, "UTF-8 strlen('hello'): %d", tt_utf8_strlen("hello"));

    tt_drawf(1, 5, "CRC32('hello'): %08X", tt_crc32("hello", 5));

    tt_bench_start(&bench);
    for (i = 0; i < 10000; i++) { volatile int x = i * i; (void)x; }
    tt_bench_end(&bench, 10000);
    tt_bench_print(&bench, "10k multiplies");

    tt_console_init(&con, 1, 7, 60, 6);
    for (i = 0; i < 6; i++) tt_console_logf(&con, "Log line %d", i);
    tt_console_draw(&con);

    tt_fps_counter(1, 14, 60.0);

    tt_progress(1, 16, 40, 75.0, 100.0, '#', '.');

    tt_box(45, 3, 30, 8, &TT_BOX_DOUBLE, " TEST BOX ");

    tt_draw(1, 18, "\033[32mSelf-test OK\033[0m");
    tt_sleep_ms(3000);
}


#define TT_DRAW(x,y,s)        tt_draw((x),(y),(s))
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define TT_DRAWF(x,y,...)   tt_drawf((x),(y),__VA_ARGS__)
#endif
#define TT_GOTO(x,y)          tt_goto((x),(y))
#define TT_CLEAR()            tt_clear()
#define TT_SLEEP_MS(ms)       tt_sleep_ms(ms)
#define TT_SLEEP(s)           tt_sleep_sec(s)
#define TT_HIDE_CURSOR()      tt_hide_cursor()
#define TT_SHOW_CURSOR()      tt_show_cursor()
#define TT_SIZE()             tt_term_size()
#define TT_SNPRINTF           tt_snprintf
#define TT_VSNPRINTF          tt_vsnprintf
#define TT_STRDUP             tt_strdup
#define TT_CLAMP(v,lo,hi)     tt_clamp_i((v),(lo),(hi))
#define TT_CLAMPF(v,lo,hi)    tt_clamp_f((v),(lo),(hi))
#define TT_RAND()             tt_rand()
#define TT_RANDF()            tt_randf()
#define TT_RAND_RANGE(lo,hi)  tt_rand_range((lo),(hi))


#endif                  

#ifdef TERMTOOLS_IMPLEMENTATION
#endif
