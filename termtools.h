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
 *   Define to enable socket support
 *   #define TT_ENABLE_SOCKETS
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

#if defined(__GNUC__)
#  define TT_UNUSED __attribute__((unused))
#else
#  define TT_UNUSED
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
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
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


TT_INLINE void tt_circle(int x, int y, int radius, int ansi_color) {
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



TT_INLINE void tt_screen_noise(double density, const char **chars) {
    TT_Size sz = tt_term_size();
    int x, y;
    int count = 0;
    
    while (chars[count]) count++;
    if (count == 0) return;

    for (y = 1; y <= sz.h; y++) {
        for (x = 1; x <= sz.w; x++) {
            if (tt_randf() < density) {
                tt_draw(x, y, chars[tt_rand() % count]);
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



typedef struct {
    tt_u32  *d;                                           
    int      len;                                          
    int      used;                                         
    int      sign;                                         
} TT_BigInt;

TT_INLINE int tt_bigint_init(TT_BigInt *n, int capacity) {
    n->d    = (tt_u32*)calloc((size_t)capacity, sizeof(tt_u32));
    n->len  = capacity;
    n->used = 0;
    n->sign = 1;
    return n->d != NULL;
}

TT_INLINE void tt_bigint_free(TT_BigInt *n) {
    free(n->d); n->d = NULL; n->len = n->used = 0;
}

TT_INLINE void tt_bigint_zero(TT_BigInt *n) {
    memset(n->d, 0, (size_t)n->len * sizeof(tt_u32));
    n->used = 0; n->sign = 1;
}

TT_INLINE void tt_bigint_norm(TT_BigInt *n) {
    while (n->used > 0 && n->d[n->used-1] == 0) n->used--;
}

TT_INLINE int tt_bigint_is_zero(const TT_BigInt *n) {
    int i;
    for (i = 0; i < n->used; i++) if (n->d[i]) return 0;
    return 1;
}

TT_INLINE void tt_bigint_copy(TT_BigInt *dst, const TT_BigInt *src) {
    int cap = src->used < dst->len ? src->used : dst->len;
    memcpy(dst->d, src->d, (size_t)cap * sizeof(tt_u32));
    memset(dst->d + cap, 0, (size_t)(dst->len - cap) * sizeof(tt_u32));
    dst->used = src->used;
    dst->sign = src->sign;
}

TT_INLINE void tt_bigint_set_u32(TT_BigInt *n, tt_u32 v) {
    tt_bigint_zero(n);
    if (v) { n->d[0] = v; n->used = 1; }
}

TT_INLINE void tt_bigint_set_u64(TT_BigInt *n, tt_u64 v) {
    tt_bigint_zero(n);
    n->d[0] = (tt_u32)(v & 0xFFFFFFFFu);
    n->d[1] = (tt_u32)(v >> 32);
    n->used = n->d[1] ? 2 : (n->d[0] ? 1 : 0);
}

TT_INLINE int tt_bigint_cmp_mag(const TT_BigInt *a, const TT_BigInt *b) {
    int i;
    if (a->used != b->used) return a->used > b->used ? 1 : -1;
    for (i = a->used - 1; i >= 0; i--) {
        if (a->d[i] > b->d[i]) return  1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

TT_INLINE int tt_bigint_cmp(const TT_BigInt *a, const TT_BigInt *b) {
    if (a->sign != b->sign) {
        if (tt_bigint_is_zero(a) && tt_bigint_is_zero(b)) return 0;
        return a->sign > b->sign ? 1 : -1;
    }
    if (a->sign == 1) return tt_bigint_cmp_mag(a, b);
    return tt_bigint_cmp_mag(b, a);                        
}

static void tt__bigint_add_mag(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int i, n = (a->used > b->used) ? a->used : b->used;
    tt_u64 carry = 0;
    for (i = 0; i < n || carry; i++) {
        tt_u64 ai = (i < a->used) ? a->d[i] : 0;
        tt_u64 bi = (i < b->used) ? b->d[i] : 0;
        tt_u64 s  = ai + bi + carry;
        r->d[i]   = (tt_u32)(s & 0xFFFFFFFFu);
        carry     = s >> 32;
    }
    r->used = i;
}

static void tt__bigint_sub_mag(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int i;
    tt_i64 borrow = 0;
    for (i = 0; i < a->used; i++) {
        tt_i64 d  = (tt_i64)(tt_u64)a->d[i] - (i < b->used ? (tt_i64)(tt_u64)b->d[i] : 0) - borrow;
        borrow    = d < 0 ? 1 : 0;
        r->d[i]   = (tt_u32)((tt_u64)(d + (borrow ? (tt_i64)0x100000000LL : 0)) & 0xFFFFFFFFu);
    }
    r->used = a->used;
    tt_bigint_norm(r);
}

TT_INLINE void tt_bigint_add(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    if (a->sign == b->sign) {
        tt__bigint_add_mag(r, a, b);
        r->sign = a->sign;
    } else {
        int c = tt_bigint_cmp_mag(a, b);
        if (c == 0) { r->used = 0; r->sign = 1; }
        else if (c > 0) { tt__bigint_sub_mag(r, a, b); r->sign = a->sign; }
        else             { tt__bigint_sub_mag(r, b, a); r->sign = b->sign; }
    }
    if (tt_bigint_is_zero(r)) r->sign = 1;
}

TT_INLINE void tt_bigint_sub(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    TT_BigInt nb; tt_bigint_init(&nb, b->len);
    tt_bigint_copy(&nb, b); nb.sign = -b->sign;
    tt_bigint_add(r, a, &nb);
    tt_bigint_free(&nb);
}

TT_INLINE void tt_bigint_mul_u32(TT_BigInt *r, const TT_BigInt *a, tt_u32 x) {
    int i;
    tt_u64 carry = 0;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = 0; i < a->used; i++) {
        tt_u64 p  = (tt_u64)a->d[i] * x + carry;
        r->d[i]   = (tt_u32)(p & 0xFFFFFFFFu);
        carry     = p >> 32;
    }
    r->used   = a->used;
    if (carry && r->used < r->len) r->d[r->used++] = (tt_u32)carry;
    r->sign   = a->sign;
    tt_bigint_norm(r);
}

TT_INLINE void tt_bigint_mul(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int i, j;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = 0; i < a->used; i++) {
        tt_u64 carry = 0;
        for (j = 0; j < b->used; j++) {
            tt_u64 p = (tt_u64)a->d[i] * b->d[j] + r->d[i+j] + carry;
            r->d[i+j] = (tt_u32)(p & 0xFFFFFFFFu);
            carry      = p >> 32;
        }
        if (i + j < r->len) r->d[i+j] = (tt_u32)carry;
    }
    r->used = a->used + b->used;
    if (r->used > r->len) r->used = r->len;
    r->sign = a->sign * b->sign;
    tt_bigint_norm(r);
    if (tt_bigint_is_zero(r)) r->sign = 1;
}

TT_INLINE tt_u32 tt_bigint_div_u32(TT_BigInt *q, const TT_BigInt *a, tt_u32 x) {
    int i;
    tt_u64 rem = 0;
    memset(q->d, 0, (size_t)q->len * sizeof(tt_u32));
    for (i = a->used - 1; i >= 0; i--) {
        tt_u64 cur = (rem << 32) | a->d[i];
        q->d[i]    = (tt_u32)(cur / x);
        rem        = cur % x;
    }
    q->used = a->used;
    q->sign = a->sign;
    tt_bigint_norm(q);
    return (tt_u32)rem;
}

TT_INLINE void tt_bigint_shl(TT_BigInt *r, const TT_BigInt *a, int n) {
    int wshift = n / 32, bshift = n % 32;
    int i;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = a->used - 1; i >= 0; i--) {
        int dst = i + wshift;
        if (dst < r->len) r->d[dst] |= a->d[i] << bshift;
        if (bshift && dst + 1 < r->len) r->d[dst+1] |= a->d[i] >> (32 - bshift);
    }
    r->used = a->used + wshift + 1;
    if (r->used > r->len) r->used = r->len;
    r->sign = a->sign;
    tt_bigint_norm(r);
}

TT_INLINE void tt_bigint_shr(TT_BigInt *r, const TT_BigInt *a, int n) {
    int wshift = n / 32, bshift = n % 32;
    int i;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = wshift; i < a->used; i++) {
        int dst = i - wshift;
        if (dst < r->len) r->d[dst] |= a->d[i] >> bshift;
        if (bshift && dst + 1 < r->len) r->d[dst+1] |= a->d[i] << (32 - bshift);
    }
    r->used = (a->used > wshift) ? a->used - wshift : 0;
    if (r->used > r->len) r->used = r->len;
    r->sign = a->sign;
    tt_bigint_norm(r);
}

TT_INLINE void tt_bigint_and(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int n = (a->used < b->used) ? a->used : b->used, i;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = 0; i < n && i < r->len; i++) r->d[i] = a->d[i] & b->d[i];
    r->used = n; tt_bigint_norm(r); r->sign = 1;
}

TT_INLINE void tt_bigint_or(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int n = (a->used > b->used) ? a->used : b->used, i;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = 0; i < n && i < r->len; i++) {
        tt_u32 ai = (i < a->used) ? a->d[i] : 0;
        tt_u32 bi = (i < b->used) ? b->d[i] : 0;
        r->d[i] = ai | bi;
    }
    r->used = n; tt_bigint_norm(r); r->sign = 1;
}

TT_INLINE void tt_bigint_xor(TT_BigInt *r, const TT_BigInt *a, const TT_BigInt *b) {
    int n = (a->used > b->used) ? a->used : b->used, i;
    memset(r->d, 0, (size_t)r->len * sizeof(tt_u32));
    for (i = 0; i < n && i < r->len; i++) {
        tt_u32 ai = (i < a->used) ? a->d[i] : 0;
        tt_u32 bi = (i < b->used) ? b->d[i] : 0;
        r->d[i] = ai ^ bi;
    }
    r->used = n; tt_bigint_norm(r); r->sign = 1;
}

TT_INLINE void tt_bigint_gcd(TT_BigInt *g, TT_BigInt *a, TT_BigInt *b, int cap) {
    TT_BigInt ta, tb, tr;
    tt_bigint_init(&ta, cap); tt_bigint_init(&tb, cap); tt_bigint_init(&tr, cap);
    tt_bigint_copy(&ta, a); ta.sign = 1;
    tt_bigint_copy(&tb, b); tb.sign = 1;
    while (!tt_bigint_is_zero(&tb)) {
        int i;
        if (tb.used == 1 && tb.d[0]) {
            tt_u32 r2 = tt_bigint_div_u32(&tr, &ta, tb.d[0]);
            tt_bigint_zero(&ta);
            tt_bigint_copy(&ta, &tb);
            tt_bigint_set_u32(&tb, r2);
            continue;
        }
        {
            TT_BigInt q2, r3;
            tt_bigint_init(&q2, cap); tt_bigint_init(&r3, cap);
            {
                tt_bigint_zero(&r3);
                for (i = ta.used * 32 - 1; i >= 0; i--) {
                    int wi = i / 32, bi2 = i % 32;
                    tt_u32 bit = (ta.d[wi] >> bi2) & 1u;
                    tt_bigint_shl(&q2, &r3, 1);                 
                    tt_bigint_copy(&r3, &q2);
                    if (bit) r3.d[0] |= 1;
                    if (!r3.used) r3.used = 1;
                    if (tt_bigint_cmp_mag(&r3, &tb) >= 0)
                        tt__bigint_sub_mag(&r3, &r3, &tb);
                }
            }
            tt_bigint_copy(&ta, &tb);
            tt_bigint_copy(&tb, &r3);
            tt_bigint_free(&q2); tt_bigint_free(&r3);
        }
    }
    tt_bigint_copy(g, &ta);
    g->sign = 1;
    tt_bigint_free(&ta); tt_bigint_free(&tb); tt_bigint_free(&tr);
}

TT_INLINE char *tt_bigint_to_dec(const TT_BigInt *n, char *buf, int bufsz) {
    TT_BigInt tmp;
    int pos = 0, start;
    char c;
    if (!tt_bigint_init(&tmp, n->len)) { buf[0]='\0'; return buf; }
    tt_bigint_copy(&tmp, n); tmp.sign = 1;
    if (tt_bigint_is_zero(&tmp)) { buf[0]='0'; buf[1]='\0'; tt_bigint_free(&tmp); return buf; }
    if (n->sign == -1 && pos < bufsz-1) buf[pos++] = '-';
    start = pos;
    while (!tt_bigint_is_zero(&tmp) && pos < bufsz-1) {
        tt_u32 rem = tt_bigint_div_u32(&tmp, &tmp, 10);
        buf[pos++] = (char)('0' + rem);
    }
    buf[pos] = '\0';
    {
        int lo = start, hi = pos - 1;
        while (lo < hi) { c = buf[lo]; buf[lo++] = buf[hi]; buf[hi--] = c; }
    }
    tt_bigint_free(&tmp);
    return buf;
}

TT_INLINE char *tt_bigint_to_hex(const TT_BigInt *n, char *buf, int bufsz) {
    static const char hx[] = "0123456789ABCDEF";
    int pos = 0, i, j;
    if (n->sign == -1 && pos < bufsz-1) buf[pos++] = '-';
    if (tt_bigint_is_zero(n)) { buf[pos++]='0'; buf[pos]='\0'; return buf; }
    for (i = n->used - 1; i >= 0; i--) {
        for (j = 28; j >= 0; j -= 4) {
            int nib = (n->d[i] >> j) & 0xF;
            if (pos == (n->sign==-1?1:0) && nib == 0) continue;                         
            if (pos < bufsz-1) buf[pos++] = hx[nib];
        }
    }
    if (pos == (n->sign==-1?1:0)) buf[pos++] = '0';
    buf[pos] = '\0';
    return buf;
}

TT_INLINE void tt_bigint_from_dec(TT_BigInt *n, const char *s) {
    TT_BigInt tmp;
    tt_bigint_zero(n);
    if (*s == '-') { n->sign = -1; s++; } else { n->sign = 1; if (*s=='+') s++; }
    if (!tt_bigint_init(&tmp, n->len)) return;
    for (; *s; s++) {
        if (!isdigit((unsigned char)*s)) continue;
        tt_bigint_mul_u32(&tmp, n, 10);
        tt_bigint_copy(n, &tmp);
        tt_bigint_zero(&tmp);
        n->d[0] += (tt_u32)(*s - '0');
        if (!n->used) n->used = 1;
        tt_bigint_norm(n);
    }
    tt_bigint_free(&tmp);
    if (tt_bigint_is_zero(n)) n->sign = 1;
}

TT_INLINE void tt_bigint_from_hex(TT_BigInt *n, const char *s) {
    int i;
    tt_bigint_zero(n);
    if (*s == '-') { n->sign = -1; s++; } else { n->sign = 1; }
    if (s[0]=='0' && (s[1]=='x'||s[1]=='X')) s += 2;
    for (; *s; s++) {
        int nib;
        if      (*s >= '0' && *s <= '9') nib = *s - '0';
        else if (*s >= 'a' && *s <= 'f') nib = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F') nib = *s - 'A' + 10;
        else continue;
        {
            tt_u32 carry = (tt_u32)nib;
            for (i = 0; i < n->len; i++) {
                tt_u64 v = ((tt_u64)n->d[i] << 4) | carry;
                n->d[i]  = (tt_u32)(v & 0xFFFFFFFFu);
                carry     = (tt_u32)(v >> 32);
            }
            if (n->used < n->len && carry) n->d[n->used] = carry;
        }
        tt_bigint_norm(n);
    }
}

TT_INLINE void tt_bigint_powmod(TT_BigInt *r, const TT_BigInt *base,
                                 const TT_BigInt *exp, const TT_BigInt *m,
                                 int cap) {
    TT_BigInt b, e, tmp, q2;
    int i, wi, bi2;
    tt_bigint_init(&b,  cap); tt_bigint_init(&e,   cap);
    tt_bigint_init(&tmp,cap); tt_bigint_init(&q2,  cap);
    tt_bigint_copy(&b, base);
    tt_bigint_copy(&e, exp);
    tt_bigint_zero(r); r->d[0] = 1; r->used = 1; r->sign = 1;
    {
        TT_BigInt rem2; tt_bigint_init(&rem2, cap);
        tt_bigint_zero(&rem2);
        for (i = b.used * 32 - 1; i >= 0; i--) {
            wi = i/32; bi2 = i%32;
            tt_u32 bit2 = (b.d[wi] >> bi2) & 1u;
            tt_bigint_shl(&q2, &rem2, 1);
            tt_bigint_copy(&rem2, &q2);
            if (bit2) rem2.d[0] |= 1;
            if (!rem2.used) rem2.used = 1;
            if (tt_bigint_cmp_mag(&rem2, m) >= 0) tt__bigint_sub_mag(&rem2, &rem2, m);
        }
        tt_bigint_copy(&b, &rem2);
        tt_bigint_free(&rem2);
    }
    for (i = e.used * 32 - 1; i >= 0; i--) {
        TT_BigInt rr; tt_bigint_init(&rr, cap * 2);
        tt_bigint_mul(&rr, r, r);
        {
            TT_BigInt rem3; tt_bigint_init(&rem3, cap);
            tt_bigint_zero(&rem3);
            {
                int k;
                for (k = rr.used * 32 - 1; k >= 0; k--) {
                    int wk = k/32, bk = k%32;
                    tt_u32 bit3 = (rr.d[wk] >> bk) & 1u;
                    tt_bigint_shl(&q2, &rem3, 1);
                    tt_bigint_copy(&rem3, &q2);
                    if (bit3) rem3.d[0] |= 1;
                    if (!rem3.used) rem3.used = 1;
                    if (tt_bigint_cmp_mag(&rem3, m) >= 0) tt__bigint_sub_mag(&rem3, &rem3, m);
                }
            }
            tt_bigint_copy(r, &rem3);
            tt_bigint_free(&rem3);
        }
        tt_bigint_free(&rr);
        wi  = i/32; bi2 = i%32;
        if ((e.d[wi] >> bi2) & 1u) {
            TT_BigInt rr2, rem4;
            tt_bigint_init(&rr2, cap*2); tt_bigint_init(&rem4, cap);
            tt_bigint_mul(&rr2, r, &b);
            tt_bigint_zero(&rem4);
            {
                int k;
                for (k = rr2.used*32-1; k>=0; k--) {
                    int wk=k/32, bk=k%32;
                    tt_u32 bit4=(rr2.d[wk]>>bk)&1u;
                    tt_bigint_shl(&q2,&rem4,1);
                    tt_bigint_copy(&rem4,&q2);
                    if(bit4){rem4.d[0]|=1; if(!rem4.used) rem4.used=1;}
                    if(tt_bigint_cmp_mag(&rem4,m)>=0) tt__bigint_sub_mag(&rem4,&rem4,m);
                }
            }
            tt_bigint_copy(r, &rem4);
            tt_bigint_free(&rr2); tt_bigint_free(&rem4);
        }
    }
    tt_bigint_free(&b); tt_bigint_free(&e);
    tt_bigint_free(&tmp); tt_bigint_free(&q2);
}

TT_INLINE int tt_bigint_is_prime(const TT_BigInt *n, int cap) {
    static const tt_u32 witnesses[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    TT_BigInt nm1, d2, r2, w2, tmp2;
    int s, i, j;
    if (n->used == 0 || (n->used == 1 && n->d[0] < 2)) return 0;
    if (n->used == 1 && n->d[0] == 2) return 1;
    if (n->d[0] % 2 == 0) return 0;
    tt_bigint_init(&nm1, cap); tt_bigint_init(&d2, cap);
    tt_bigint_init(&r2,  cap); tt_bigint_init(&w2, cap);
    tt_bigint_init(&tmp2,cap);
    tt_bigint_copy(&nm1, n); nm1.d[0]--;
    tt_bigint_norm(&nm1);
    tt_bigint_copy(&d2, &nm1); s = 0;
    while ((d2.d[0] & 1) == 0) { tt_bigint_shr(&d2, &d2, 1); s++; }
    for (i = 0; i < (int)(sizeof(witnesses)/sizeof(witnesses[0])); i++) {
        if (n->used == 1 && n->d[0] <= witnesses[i]) continue;
        tt_bigint_set_u32(&w2, witnesses[i]);
        tt_bigint_powmod(&r2, &w2, &d2, n, cap);
        if ((r2.used==1 && r2.d[0]==1) || tt_bigint_cmp_mag(&r2,&nm1)==0) continue;
        {
            int composite = 1;
            for (j = 0; j < s-1; j++) {
                tt_bigint_mul(&tmp2, &r2, &r2);
                {
                    TT_BigInt rem5; tt_bigint_init(&rem5,cap);
                    tt_bigint_zero(&rem5);
                    {
                        int k;
                        TT_BigInt qsh; tt_bigint_init(&qsh,cap);
                        for(k=tmp2.used*32-1;k>=0;k--){
                            int wk=k/32,bk=k%32;
                            tt_u32 b2=(tmp2.d[wk]>>bk)&1u;
                            tt_bigint_shl(&qsh,&rem5,1);
                            tt_bigint_copy(&rem5,&qsh);
                            if(b2){rem5.d[0]|=1;if(!rem5.used)rem5.used=1;}
                            if(tt_bigint_cmp_mag(&rem5,n)>=0) tt__bigint_sub_mag(&rem5,&rem5,n);
                        }
                        tt_bigint_free(&qsh);
                    }
                    tt_bigint_copy(&r2,&rem5);
                    tt_bigint_free(&rem5);
                }
                if (r2.used==1&&r2.d[0]==1) { composite=1; break; }
                if (tt_bigint_cmp_mag(&r2,&nm1)==0) { composite=0; break; }
            }
            if (composite) {
                tt_bigint_free(&nm1); tt_bigint_free(&d2);
                tt_bigint_free(&r2);  tt_bigint_free(&w2);
                tt_bigint_free(&tmp2);
                return 0;
            }
        }
    }
    tt_bigint_free(&nm1); tt_bigint_free(&d2);
    tt_bigint_free(&r2);  tt_bigint_free(&w2);
    tt_bigint_free(&tmp2);
    return 1;
}

TT_INLINE void tt_bigint_isqrt(TT_BigInt *r, const TT_BigInt *n, int cap) {
    TT_BigInt x, y, tmp3;
    tt_bigint_init(&x, cap); tt_bigint_init(&y, cap); tt_bigint_init(&tmp3, cap);
    if (tt_bigint_is_zero(n)) { tt_bigint_zero(r); goto done_isqrt; }
    tt_bigint_copy(&x, n);
    for (;;) {
        TT_BigInt qd; tt_bigint_init(&qd, cap);
        {
            tt_bigint_zero(&qd);
            TT_BigInt rem6; tt_bigint_init(&rem6, cap);
            tt_bigint_zero(&rem6);
            {
                int k;
                TT_BigInt qsh2; tt_bigint_init(&qsh2, cap);
                for(k=n->used*32-1;k>=0;k--){
                    int wk=k/32,bk=k%32;
                    tt_u32 b3=(n->d[wk]>>bk)&1u;
                    tt_bigint_shl(&qsh2,&rem6,1);
                    tt_bigint_copy(&rem6,&qsh2);
                    if(b3){rem6.d[0]|=1;if(!rem6.used)rem6.used=1;}
                    if(tt_bigint_cmp_mag(&rem6,&x)>=0){
                        tt__bigint_sub_mag(&rem6,&rem6,&x);
                        qd.d[k/32]|=(1u<<(k%32));
                        if(k/32>=qd.used) qd.used=k/32+1;
                    }
                }
                tt_bigint_free(&qsh2);
            }
            tt_bigint_free(&rem6);
        }
        tt__bigint_add_mag(&y, &x, &qd);
        tt_bigint_shr(&tmp3, &y, 1);         
        tt_bigint_free(&qd);
        if (tt_bigint_cmp_mag(&tmp3, &x) >= 0) break;
        tt_bigint_copy(&x, &tmp3);
    }
    tt_bigint_copy(r, &x);
done_isqrt:
    tt_bigint_free(&x); tt_bigint_free(&y); tt_bigint_free(&tmp3);
}

TT_INLINE void tt_bigint_fib(TT_BigInt *r, int n, int cap) {
    TT_BigInt a2, b2, tmp4;
    int i;
    tt_bigint_init(&a2, cap); tt_bigint_init(&b2, cap); tt_bigint_init(&tmp4, cap);
    tt_bigint_set_u32(&a2, 0); tt_bigint_set_u32(&b2, 1);
    if (n == 0) { tt_bigint_copy(r, &a2); goto fib_done; }
    if (n == 1) { tt_bigint_copy(r, &b2); goto fib_done; }
    for (i = 2; i <= n; i++) {
        tt__bigint_add_mag(&tmp4, &a2, &b2);
        tt_bigint_copy(&a2, &b2);
        tt_bigint_copy(&b2, &tmp4);
    }
    tt_bigint_copy(r, &b2);
fib_done:
    tt_bigint_free(&a2); tt_bigint_free(&b2); tt_bigint_free(&tmp4);
}

TT_INLINE void tt_bigint_factorial(TT_BigInt *r, int n, int cap) {
    TT_BigInt tmp5;
    int i;
    tt_bigint_init(&tmp5, cap);
    tt_bigint_set_u32(r, 1);
    for (i = 2; i <= n; i++) {
        tt_bigint_mul_u32(&tmp5, r, (tt_u32)i);
        tt_bigint_copy(r, &tmp5);
    }
    tt_bigint_free(&tmp5);
}


#define TT_FP_MODES 31
static const int TT_FP_MODE_FRAC_BITS[TT_FP_MODES] = {
    2, 4, 8, 16, 24, 32, 64, 128, 256, 512, 1024, 2048, 4096,
    8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576,
    2097152, 4194304, 8388608, 16777216, 33554432, 67108864,
    134217728, 268435456, 536870912, 1073741824
};
static const char *TT_FP_MODE_NAMES[TT_FP_MODES] TT_UNUSED = {
    "Q2.2",       "Q4.4",        "Q8.8",         "Q16.16",
    "Q24.24",     "Q32.32",      "Q64.64",        "Q128.128",
    "Q256.256",   "Q512.512",    "Q1024.1024",    "Q2048.2048",
    "Q4096.4096", "Q8192.8192",  "Q16384.16384",  "Q32768.32768",
    "Q65536.65536","Q131072.131072","Q262144.262144","Q524288.524288",
    "Q1048576.1048576","Q2097152.2097152","Q4194304.4194304",
    "Q8388608.8388608","Q16777216.16777216","Q33554432.33554432",
    "Q67108864.67108864","Q134217728.134217728","Q268435456.268435456",
    "Q536870912.536870912","Q1073741824.1073741824"
};

typedef struct {
    TT_BigInt mag;                             
    int       sign;                 
    int       fb;                          
} TT_Fixed;

TT_INLINE int tt_fp_limbs(int fb) {
    int total_bits = fb * 2;
    return (total_bits + 31) / 32 + 2;                      
}

TT_INLINE int tt_fp_init(TT_Fixed *n, int fb) {
    n->fb   = fb;
    n->sign = 1;
    return tt_bigint_init(&n->mag, tt_fp_limbs(fb));
}

TT_INLINE void tt_fp_free(TT_Fixed *n) { tt_bigint_free(&n->mag); }

TT_INLINE void tt_fp_zero(TT_Fixed *n) { tt_bigint_zero(&n->mag); n->sign = 1; }

TT_INLINE void tt_fp_copy(TT_Fixed *dst, const TT_Fixed *src) {
    tt_bigint_copy(&dst->mag, &src->mag);
    dst->sign = src->sign;
    dst->fb   = src->fb;
}

TT_INLINE void tt_fp_from_dec(TT_Fixed *out, const char *s) {
    int i;
    const char *dot, *frac_s;
    int il, fl;
    TT_BigInt ten, acc, tmp6, dig, int_fp, frac_acc;
    int cap = out->mag.len;

    tt_fp_zero(out);
    if (*s == '-') { out->sign = -1; s++; }
    else { out->sign = 1; if (*s == '+') s++; }

    dot    = strchr(s, '.');
    il     = dot ? (int)(dot - s) : (int)strlen(s);
    frac_s = dot ? dot + 1 : "";
    fl     = (int)strlen(frac_s);
    { int max_fl = (out->fb * 3010LL / 100000) + 4; if (fl > max_fl) fl = max_fl; }

    tt_bigint_init(&ten, cap); tt_bigint_init(&acc, cap);
    tt_bigint_init(&tmp6,cap); tt_bigint_init(&dig, cap);
    tt_bigint_init(&int_fp,  cap);
    tt_bigint_init(&frac_acc,cap);
    tt_bigint_set_u32(&ten, 10);

    for (i = 0; i < il; i++) {
        if (!isdigit((unsigned char)s[i])) continue;
        tt_bigint_mul_u32(&tmp6, &acc, 10);
        tt_bigint_copy(&acc, &tmp6);
        acc.d[0] += (tt_u32)(s[i] - '0');
        if (!acc.used) acc.used = 1;
        tt_bigint_norm(&acc);
    }
    tt_bigint_shl(&int_fp, &acc, out->fb);

    if (fl > 0) {
        TT_BigInt N6, D6, N_shifted, q6, rem7, power10;
        tt_bigint_init(&N6,       cap*2); tt_bigint_init(&D6,       cap*2);
        tt_bigint_init(&N_shifted,cap*2); tt_bigint_init(&q6,       cap*2);
        tt_bigint_init(&rem7,     cap*2); tt_bigint_init(&power10,  cap*2);
        tt_bigint_zero(&N6); tt_bigint_set_u32(&D6, 1);
        for (i = 0; i < fl; i++) {
            if (!isdigit((unsigned char)frac_s[i])) continue;
            tt_bigint_mul_u32(&q6, &N6, 10); tt_bigint_copy(&N6, &q6);
            N6.d[0] += (tt_u32)(frac_s[i]-'0'); if(!N6.used) N6.used=1; tt_bigint_norm(&N6);
            tt_bigint_mul_u32(&q6, &D6, 10); tt_bigint_copy(&D6, &q6);
        }
        tt_bigint_shl(&N_shifted, &N6, out->fb);
        tt_bigint_zero(&frac_acc); tt_bigint_zero(&rem7);
        {
            int k;
            for (k = N_shifted.used*32-1; k >= 0; k--) {
                int wk=k/32, bk=k%32;
                tt_u32 b4 = (N_shifted.d[wk]>>bk)&1u;
                tt_bigint_shl(&q6,&rem7,1); tt_bigint_copy(&rem7,&q6);
                if(b4){rem7.d[0]|=1;if(!rem7.used)rem7.used=1;}
                if(tt_bigint_cmp_mag(&rem7,&D6)>=0){
                    tt__bigint_sub_mag(&rem7,&rem7,&D6);
                    if(k/32 < frac_acc.len){
                        frac_acc.d[k/32] |= (1u<<(k%32));
                        if(k/32 >= frac_acc.used) frac_acc.used = k/32+1;
                    }
                }
            }
        }
        tt_bigint_free(&N6); tt_bigint_free(&D6);
        tt_bigint_free(&N_shifted); tt_bigint_free(&q6);
        tt_bigint_free(&rem7); tt_bigint_free(&power10);
    }

    tt__bigint_add_mag(&out->mag, &int_fp, &frac_acc);
    tt_bigint_norm(&out->mag);

    tt_bigint_free(&ten); tt_bigint_free(&acc); tt_bigint_free(&tmp6);
    tt_bigint_free(&dig); tt_bigint_free(&int_fp); tt_bigint_free(&frac_acc);
    if (tt_bigint_is_zero(&out->mag)) out->sign = 1;
}

TT_INLINE void tt_fp_to_dec(const TT_Fixed *n, char *buf, int bufsz) {
    int fb  = n->fb;
    int cap = n->mag.len;
    TT_BigInt int_part, frac_part, fp_cur, prod, dval, q8, tmp8;
    char ibuf[3200], fbuf[3200];
    int ilen=0, flen=0, max_fd, pos=0, i;

    tt_bigint_init(&int_part, cap); tt_bigint_init(&frac_part, cap);
    tt_bigint_init(&fp_cur,   cap); tt_bigint_init(&prod,      cap);
    tt_bigint_init(&dval,     cap); tt_bigint_init(&q8,        cap);
    tt_bigint_init(&tmp8,     cap);

    tt_bigint_shr(&int_part, &n->mag, fb);

    tt_bigint_copy(&frac_part, &n->mag);
    {
        int w = fb / 32, b5 = fb % 32, k;
        for (k = w+1; k < frac_part.used && k < cap; k++) frac_part.d[k] = 0;
        if (w < cap && b5) frac_part.d[w] &= (1u<<b5)-1u;
        if (w < cap && b5==0) frac_part.d[w] = 0;
        tt_bigint_norm(&frac_part);
    }

    if (tt_bigint_is_zero(&int_part)) {
        ibuf[ilen++] = '0';
    } else {
        tt_bigint_copy(&tmp8, &int_part);
        while (!tt_bigint_is_zero(&tmp8) && ilen < 3199) {
            tt_u32 rem8 = tt_bigint_div_u32(&q8, &tmp8, 10);
            ibuf[ilen++] = (char)('0' + rem8);
            tt_bigint_copy(&tmp8, &q8);
        }
        for (i=0,pos=ilen-1; i<pos; i++,pos--) { char c2=ibuf[i]; ibuf[i]=ibuf[pos]; ibuf[pos]=c2; }
        pos = 0;
    }
    ibuf[ilen] = '\0';

    max_fd = (int)((long)fb * 3010 / 100000) + 4;
    if (max_fd > 1300) max_fd = 1300;
    tt_bigint_copy(&fp_cur, &frac_part);
    for (i = 0; i < max_fd && !tt_bigint_is_zero(&fp_cur) && flen < 3199; i++) {
        tt_bigint_mul_u32(&prod, &fp_cur, 10);
        tt_bigint_shr(&dval, &prod, fb);
        fbuf[flen++] = (char)('0' + (dval.used ? dval.d[0]&0xF : 0));
        tt_bigint_copy(&fp_cur, &prod);
        {
            int w2 = fb/32, b6 = fb%32, k;
            for (k = w2+1; k < fp_cur.used && k < cap; k++) fp_cur.d[k] = 0;
            if (w2 < cap && b6) fp_cur.d[w2] &= (1u<<b6)-1u;
            if (w2 < cap && b6==0) fp_cur.d[w2] = 0;
            tt_bigint_norm(&fp_cur);
        }
    }
    while (flen > 1 && fbuf[flen-1] == '0') flen--;
    fbuf[flen] = '\0';

    pos = 0;
    if (n->sign == -1 && !tt_bigint_is_zero(&n->mag) && pos < bufsz-1)
        buf[pos++] = '-';
    for (i = 0; i < ilen && pos < bufsz-1; i++) buf[pos++] = ibuf[i];
    if (flen > 0) {
        if (pos < bufsz-1) buf[pos++] = '.';
        for (i = 0; i < flen && pos < bufsz-1; i++) buf[pos++] = fbuf[i];
    }
    buf[pos] = '\0';

    tt_bigint_free(&int_part); tt_bigint_free(&frac_part);
    tt_bigint_free(&fp_cur);   tt_bigint_free(&prod);
    tt_bigint_free(&dval);     tt_bigint_free(&q8);
    tt_bigint_free(&tmp8);
}

TT_INLINE void tt_fp_add(TT_Fixed *r, const TT_Fixed *a, const TT_Fixed *b) {
    tt_fp_zero(r);
    r->fb = a->fb;
    if (a->sign == b->sign) {
        tt__bigint_add_mag(&r->mag, &a->mag, &b->mag);
        r->sign = a->sign;
    } else {
        int c = tt_bigint_cmp_mag(&a->mag, &b->mag);
        if (c == 0) { r->sign = 1; }
        else if (c > 0) { tt__bigint_sub_mag(&r->mag, &a->mag, &b->mag); r->sign = a->sign; }
        else            { tt__bigint_sub_mag(&r->mag, &b->mag, &a->mag); r->sign = b->sign; }
    }
    tt_bigint_norm(&r->mag);
    if (tt_bigint_is_zero(&r->mag)) r->sign = 1;
}

TT_INLINE void tt_fp_sub(TT_Fixed *r, const TT_Fixed *a, const TT_Fixed *b) {
    TT_Fixed nb;
    tt_fp_init(&nb, b->fb);
    tt_fp_copy(&nb, b); nb.sign = -b->sign;
    tt_fp_add(r, a, &nb);
    tt_fp_free(&nb);
}

TT_INLINE void tt_fp_mul(TT_Fixed *r, const TT_Fixed *a, const TT_Fixed *b) {
    int cap2 = a->mag.len * 2 + 4;
    TT_BigInt prod2;
    tt_fp_zero(r); r->fb = a->fb;
    tt_bigint_init(&prod2, cap2);
    tt_bigint_mul(&prod2, &a->mag, &b->mag);
    tt_bigint_shr(&r->mag, &prod2, a->fb);
    tt_bigint_norm(&r->mag);
    r->sign = a->sign * b->sign;
    if (tt_bigint_is_zero(&r->mag)) r->sign = 1;
    tt_bigint_free(&prod2);
}

TT_INLINE int tt_fp_div(TT_Fixed *r, const TT_Fixed *a, const TT_Fixed *b) {
    int cap2 = a->mag.len * 2 + 4;
    TT_BigInt num_sh, q9, rem9;
    int k;
    if (tt_bigint_is_zero(&b->mag)) return -1;                  
    tt_fp_zero(r); r->fb = a->fb;
    tt_bigint_init(&num_sh, cap2);
    tt_bigint_init(&q9,     cap2);
    tt_bigint_init(&rem9,   cap2);
    tt_bigint_shl(&num_sh, &a->mag, a->fb);
    tt_bigint_zero(&q9); tt_bigint_zero(&rem9);
    for (k = num_sh.used*32-1; k >= 0; k--) {
        int wk=k/32, bk=k%32;
        tt_u32 bit5=(num_sh.d[wk]>>bk)&1u;
        tt_bigint_shl(&q9, &rem9, 1); tt_bigint_copy(&rem9, &q9);
        if(bit5){rem9.d[0]|=1;if(!rem9.used)rem9.used=1;}
        if(tt_bigint_cmp_mag(&rem9,&b->mag)>=0){
            tt__bigint_sub_mag(&rem9,&rem9,&b->mag);
            if(k/32 < r->mag.len){
                r->mag.d[k/32] |= (1u<<(k%32));
                if(k/32 >= r->mag.used) r->mag.used = k/32+1;
            }
        }
    }
    tt_bigint_norm(&r->mag);
    r->sign = a->sign * b->sign;
    if (tt_bigint_is_zero(&r->mag)) r->sign = 1;
    tt_bigint_free(&num_sh); tt_bigint_free(&q9); tt_bigint_free(&rem9);
    return 0;
}

TT_INLINE void tt_fp_pow_int(TT_Fixed *r, const TT_Fixed *a, int exp) {
    TT_Fixed base9, tmp9;
    int fb9 = a->fb;
    tt_fp_init(&base9, fb9); tt_fp_init(&tmp9, fb9);
    tt_fp_copy(&base9, a);
    tt_fp_zero(r); r->fb = fb9;
    tt_bigint_zero(&r->mag);
    { int w9=fb9/32, b7=fb9%32;
      if(w9 < r->mag.len){ r->mag.d[w9] |= b7 ? (1u<<b7) : 1u; r->mag.used = w9+1; }
    }
    while (exp > 0) {
        if (exp & 1) { tt_fp_mul(&tmp9, r, &base9); tt_fp_copy(r, &tmp9); }
        tt_fp_mul(&tmp9, &base9, &base9); tt_fp_copy(&base9, &tmp9);
        exp >>= 1;
    }
    tt_fp_free(&base9); tt_fp_free(&tmp9);
}


#define TT_EXPR_MAX_TOKENS 256
#define TT_EXPR_NUM_LEN    2048

typedef enum {
    TT_TOK_NUM, TT_TOK_OP, TT_TOK_LPAREN, TT_TOK_RPAREN, TT_TOK_END
} TT_TokType;

typedef struct {
    TT_TokType type;
    char       num_str[TT_EXPR_NUM_LEN];
    char       op_ch;
    int        prec;
    int        rtl;                          
} TT_Token;

static TT_Token tt__tokenize(const char *expr, int *idx) {
    TT_Token tok;
    memset(&tok, 0, sizeof(tok));
    while (isspace((unsigned char)expr[*idx])) (*idx)++;
    if (!expr[*idx])           { tok.type = TT_TOK_END;    return tok; }
    if (expr[*idx] == '(')     { (*idx)++; tok.type = TT_TOK_LPAREN; return tok; }
    if (expr[*idx] == ')')     { (*idx)++; tok.type = TT_TOK_RPAREN; return tok; }
    {
        int unary_minus = 0;
        if ((expr[*idx]=='-'||expr[*idx]=='+') &&
            (isdigit((unsigned char)expr[*idx+1])||expr[*idx+1]=='.')) {
            int prev = *idx - 1;
            while (prev >= 0 && isspace((unsigned char)expr[prev])) prev--;
            if (prev<0||expr[prev]=='('||strchr("+-*/^",expr[prev])) {
                if (expr[*idx] == '-') unary_minus = 1;
                (*idx)++;
            }
        }
        if (isdigit((unsigned char)expr[*idx]) || expr[*idx] == '.') {
            int i = 0;
            tok.type = TT_TOK_NUM;
            if (unary_minus && i < TT_EXPR_NUM_LEN-1) tok.num_str[i++] = '-';
            {
                int has_dot = 0;
                while (i < TT_EXPR_NUM_LEN-1 &&
                       (isdigit((unsigned char)expr[*idx]) ||
                        (!has_dot && expr[*idx]=='.'))) {
                    if (expr[*idx]=='.') has_dot=1;
                    tok.num_str[i++] = expr[(*idx)++];
                }
            }
            tok.num_str[i] = '\0';
            return tok;
        }
    }
    if (strchr("+-*/^", expr[*idx])) {
        tok.type  = TT_TOK_OP;
        tok.op_ch = expr[(*idx)++];
        if      (tok.op_ch=='+' || tok.op_ch=='-') { tok.prec=2; tok.rtl=0; }
        else if (tok.op_ch=='*' || tok.op_ch=='/') { tok.prec=3; tok.rtl=0; }
        else                                        { tok.prec=4; tok.rtl=1; }
        return tok;
    }
    (*idx)++;
    return tt__tokenize(expr, idx);                        
}

TT_INLINE int tt_expr_to_rpn(const char *expr, TT_Token *rpn, int max_rpn) {
    TT_Token ops[64];
    int op_top = 0, rpn_len = 0, idx = 0;
    for (;;) {
        TT_Token tok = tt__tokenize(expr, &idx);
        if (tok.type == TT_TOK_END) break;
        if (tok.type == TT_TOK_NUM) {
            if (rpn_len < max_rpn) rpn[rpn_len++] = tok;
        } else if (tok.type == TT_TOK_LPAREN) {
            if (op_top < 64) ops[op_top++] = tok;
        } else if (tok.type == TT_TOK_RPAREN) {
            while (op_top>0 && ops[op_top-1].type!=TT_TOK_LPAREN)
                if (rpn_len<max_rpn) rpn[rpn_len++] = ops[--op_top];
            if (op_top>0) op_top--;
        } else if (tok.type == TT_TOK_OP) {
            while (op_top>0 && ops[op_top-1].type==TT_TOK_OP) {
                TT_Token top = ops[op_top-1];
                if ((!tok.rtl && tok.prec<=top.prec)||(tok.rtl && tok.prec<top.prec))
                    { if(rpn_len<max_rpn) rpn[rpn_len++]=ops[--op_top]; }
                else break;
            }
            if (op_top<64) ops[op_top++]=tok;
        }
    }
    while (op_top>0 && rpn_len<max_rpn) rpn[rpn_len++]=ops[--op_top];
    return rpn_len;
}

TT_INLINE int tt_expr_eval_fp(const TT_Token *rpn, int len,
                                int fb, TT_Fixed *result) {
    TT_Fixed stack[32];
    int top = 0, i;
    for (i = 0; i < len; i++) {
        if (rpn[i].type == TT_TOK_NUM) {
            if (top >= 32) return -1;
            tt_fp_init(&stack[top], fb);
            tt_fp_from_dec(&stack[top], rpn[i].num_str);
            top++;
        } else {
            TT_Fixed a2, b2;
            if (top < 2) return -2;
            b2 = stack[--top];
            a2 = stack[--top];
            tt_fp_init(&stack[top], fb);
            switch (rpn[i].op_ch) {
                case '+': tt_fp_add(&stack[top], &a2, &b2); break;
                case '-': tt_fp_sub(&stack[top], &a2, &b2); break;
                case '*': tt_fp_mul(&stack[top], &a2, &b2); break;
                case '/':
                    if (tt_fp_div(&stack[top], &a2, &b2) != 0) {
                        tt_fp_free(&a2); tt_fp_free(&b2); return -3;
                    }
                    break;
                case '^': {
                    char ibuf2[32];
                    TT_BigInt exp_mag;
                    tt_bigint_init(&exp_mag, 4);
                    tt_bigint_shr(&exp_mag, &b2.mag, fb);
                    tt_bigint_to_dec(&exp_mag, ibuf2, sizeof(ibuf2));
                    { int exp_v = tt_atoi_safe(ibuf2, 0);
                      tt_fp_pow_int(&stack[top], &a2, exp_v); }
                    tt_bigint_free(&exp_mag);
                    break;
                }
            }
            tt_fp_free(&a2); tt_fp_free(&b2);
            top++;
        }
    }
    if (top != 1) return -4;
    tt_fp_copy(result, &stack[0]);
    tt_fp_free(&stack[0]);
    return 0;
}

TT_INLINE int tt_expr_eval(const char *expr, int mode, char *buf, int bufsz) {
    TT_Token rpn[TT_EXPR_MAX_TOKENS];
    TT_Fixed result;
    int rpn_len, fb, rc;
    if (mode < 0 || mode >= TT_FP_MODES) return -1;
    fb      = TT_FP_MODE_FRAC_BITS[mode];
    rpn_len = tt_expr_to_rpn(expr, rpn, TT_EXPR_MAX_TOKENS);
    tt_fp_init(&result, fb);
    rc = tt_expr_eval_fp(rpn, rpn_len, fb, &result);
    if (rc == 0) tt_fp_to_dec(&result, buf, bufsz);
    else         tt_snprintf(buf, (size_t)bufsz, "<error:%d>", rc);
    tt_fp_free(&result);
    return rc;
}


typedef struct { int value; int has_value; } TT_OptInt;
TT_INLINE TT_OptInt tt_some_int(int v) { TT_OptInt o; o.value=v; o.has_value=1; return o; }
TT_INLINE TT_OptInt tt_none_int(void)  { TT_OptInt o; o.value=0; o.has_value=0; return o; }

typedef struct { double value; int has_value; } TT_OptDbl;
TT_INLINE TT_OptDbl tt_some_dbl(double v) { TT_OptDbl o; o.value=v; o.has_value=1; return o; }
TT_INLINE TT_OptDbl tt_none_dbl(void)     { TT_OptDbl o; o.value=0.0; o.has_value=0; return o; }

typedef struct { void *ptr; int has_value; } TT_OptPtr;
TT_INLINE TT_OptPtr tt_some_ptr(void *p) { TT_OptPtr o; o.ptr=p; o.has_value=1; return o; }
TT_INLINE TT_OptPtr tt_none_ptr(void)    { TT_OptPtr o; o.ptr=NULL; o.has_value=0; return o; }

typedef struct { void *value; int err; int ok; } TT_Result;
TT_INLINE TT_Result tt_ok(void *v)  { TT_Result r; r.value=v; r.err=0; r.ok=1; return r; }
TT_INLINE TT_Result tt_err(int e)   { TT_Result r; r.value=NULL; r.err=e; r.ok=0; return r; }


typedef void (*TT_DeferFn)(void*);
typedef struct { TT_DeferFn fn; void *arg; } TT_DeferEntry;

#define TT_DEFER_STACK_SIZE 32
typedef struct { TT_DeferEntry e[TT_DEFER_STACK_SIZE]; int top; } TT_DeferStack;

TT_INLINE void tt_defer_init(TT_DeferStack *s) { s->top = 0; }
TT_INLINE void tt_defer_push(TT_DeferStack *s, TT_DeferFn fn, void *arg) {
    if (s->top < TT_DEFER_STACK_SIZE) { s->e[s->top].fn=fn; s->e[s->top].arg=arg; s->top++; }
}
TT_INLINE void tt_defer_run(TT_DeferStack *s) {
    while (s->top > 0) { s->top--; s->e[s->top].fn(s->e[s->top].arg); }
}

#define TT_DEFER_BEGIN     TT_DeferStack _tt_ds_; tt_defer_init(&_tt_ds_);
#define TT_DEFER(fn, arg)  tt_defer_push(&_tt_ds_, (TT_DeferFn)(fn), (arg))
#define TT_DEFER_END       tt_defer_run(&_tt_ds_);


#define TT_BITSET_WORDS(n) (((n) + 31) / 32)

typedef struct { tt_u32 *words; int nbits; } TT_Bitset;

TT_INLINE int  tt_bitset_init(TT_Bitset *b, int nbits) {
    b->nbits = nbits;
    b->words = (tt_u32*)calloc((size_t)TT_BITSET_WORDS(nbits), sizeof(tt_u32));
    return b->words != NULL;
}
TT_INLINE void tt_bitset_free(TT_Bitset *b)  { free(b->words); b->words=NULL; }
TT_INLINE void tt_bitset_set(TT_Bitset *b, int i)   { b->words[i/32] |=  (1u<<(i%32)); }
TT_INLINE void tt_bitset_clr(TT_Bitset *b, int i)   { b->words[i/32] &= ~(1u<<(i%32)); }
TT_INLINE void tt_bitset_flip(TT_Bitset *b, int i)  { b->words[i/32] ^=  (1u<<(i%32)); }
TT_INLINE int  tt_bitset_get(const TT_Bitset *b, int i) { return (b->words[i/32] >> (i%32)) & 1; }
TT_INLINE void tt_bitset_clear_all(TT_Bitset *b) { memset(b->words, 0, (size_t)TT_BITSET_WORDS(b->nbits)*4); }
TT_INLINE void tt_bitset_set_all(TT_Bitset *b)   { memset(b->words, 0xFF, (size_t)TT_BITSET_WORDS(b->nbits)*4); }
TT_INLINE int  tt_bitset_count(const TT_Bitset *b) {
    int c=0, i;
    for (i=0; i<TT_BITSET_WORDS(b->nbits); i++) {
        tt_u32 v = b->words[i];
        while (v) { c += v&1; v>>=1; }
    }
    return c;
}
TT_INLINE void tt_bitset_and(TT_Bitset *r, const TT_Bitset *a, const TT_Bitset *b) {
    int i; for (i=0;i<TT_BITSET_WORDS(r->nbits);i++) r->words[i]=a->words[i]&b->words[i];
}
TT_INLINE void tt_bitset_or(TT_Bitset *r, const TT_Bitset *a, const TT_Bitset *b) {
    int i; for (i=0;i<TT_BITSET_WORDS(r->nbits);i++) r->words[i]=a->words[i]|b->words[i];
}
TT_INLINE void tt_bitset_xor(TT_Bitset *r, const TT_Bitset *a, const TT_Bitset *b) {
    int i; for (i=0;i<TT_BITSET_WORDS(r->nbits);i++) r->words[i]=a->words[i]^b->words[i];
}
TT_INLINE int tt_bitset_any(const TT_Bitset *b) {
    int i; for (i=0;i<TT_BITSET_WORDS(b->nbits);i++) if(b->words[i]) return 1; return 0;
}
TT_INLINE int tt_bitset_none(const TT_Bitset *b) { return !tt_bitset_any(b); }


#define TT_MAT_MAX 16
typedef struct { double m[TT_MAT_MAX][TT_MAT_MAX]; int rows, cols; } TT_Mat;

TT_INLINE void tt_mat_zero(TT_Mat *M, int rows, int cols) {
    memset(M->m, 0, sizeof(M->m)); M->rows=rows; M->cols=cols;
}
TT_INLINE void tt_mat_identity(TT_Mat *M, int n) {
    int i; tt_mat_zero(M, n, n);
    for (i=0;i<n;i++) M->m[i][i]=1.0;
}
TT_INLINE void tt_mat_add(TT_Mat *R, const TT_Mat *A, const TT_Mat *B) {
    int i,j; R->rows=A->rows; R->cols=A->cols;
    for (i=0;i<A->rows;i++) for (j=0;j<A->cols;j++) R->m[i][j]=A->m[i][j]+B->m[i][j];
}
TT_INLINE void tt_mat_sub(TT_Mat *R, const TT_Mat *A, const TT_Mat *B) {
    int i,j; R->rows=A->rows; R->cols=A->cols;
    for (i=0;i<A->rows;i++) for (j=0;j<A->cols;j++) R->m[i][j]=A->m[i][j]-B->m[i][j];
}
TT_INLINE void tt_mat_mul(TT_Mat *R, const TT_Mat *A, const TT_Mat *B) {
    int i,j,k; TT_Mat tmp; tt_mat_zero(&tmp, A->rows, B->cols);
    for (i=0;i<A->rows;i++) for (j=0;j<B->cols;j++) for (k=0;k<A->cols;k++)
        tmp.m[i][j] += A->m[i][k]*B->m[k][j];
    *R = tmp;
}
TT_INLINE void tt_mat_scale(TT_Mat *R, const TT_Mat *A, double s) {
    int i,j; *R=*A;
    for (i=0;i<A->rows;i++) for (j=0;j<A->cols;j++) R->m[i][j]*=s;
}
TT_INLINE void tt_mat_transpose(TT_Mat *R, const TT_Mat *A) {
    int i,j; TT_Mat tmp; tt_mat_zero(&tmp, A->cols, A->rows);
    for (i=0;i<A->rows;i++) for (j=0;j<A->cols;j++) tmp.m[j][i]=A->m[i][j];
    *R=tmp;
}
TT_INLINE double tt_mat_lu(TT_Mat *A, int *piv) {
    int i,j,k,n=A->rows; double det=1.0;
    for (i=0;i<n;i++) piv[i]=i;
    for (k=0;k<n;k++) {
        double mx=0.0; int mx_r=k;
        for (i=k;i<n;i++) if (fabs(A->m[i][k])>mx){mx=fabs(A->m[i][k]);mx_r=i;}
        if (mx_r!=k) {
            for (j=0;j<n;j++){double t=A->m[k][j];A->m[k][j]=A->m[mx_r][j];A->m[mx_r][j]=t;}
            { int t2=piv[k]; piv[k]=piv[mx_r]; piv[mx_r]=t2; }
            det=-det;
        }
        if (A->m[k][k]==0.0) return 0.0;
        det*=A->m[k][k];
        for (i=k+1;i<n;i++) {
            A->m[i][k]/=A->m[k][k];
            for (j=k+1;j<n;j++) A->m[i][j]-=A->m[i][k]*A->m[k][j];
        }
    }
    return det;
}
TT_INLINE void tt_mat_lu_solve(const TT_Mat *LU, const int *piv, const double *b, double *x, int n) {
    int i,k; double y[TT_MAT_MAX];
    for (i=0;i<n;i++){y[i]=b[piv[i]]; for(k=0;k<i;k++) y[i]-=LU->m[i][k]*y[k];}
    for (i=n-1;i>=0;i--){x[i]=y[i]; for(k=i+1;k<n;k++) x[i]-=LU->m[i][k]*x[k]; x[i]/=LU->m[i][i];}
}
TT_INLINE double tt_mat_det(const TT_Mat *A) {
    TT_Mat tmp=*A; int piv[TT_MAT_MAX]; return tt_mat_lu(&tmp, piv);
}
TT_INLINE void tt_mat_print(const TT_Mat *A) {
    int i,j;
    for (i=0;i<A->rows;i++){
        for (j=0;j<A->cols;j++) printf("%10.4f ", A->m[i][j]);
        printf("\n");
    }
}


typedef struct { double re, im; } TT_Cplx;

TT_INLINE TT_Cplx tt_cplx(double re, double im) { TT_Cplx c; c.re=re; c.im=im; return c; }
TT_INLINE TT_Cplx tt_cplx_add(TT_Cplx a, TT_Cplx b) { return tt_cplx(a.re+b.re, a.im+b.im); }
TT_INLINE TT_Cplx tt_cplx_sub(TT_Cplx a, TT_Cplx b) { return tt_cplx(a.re-b.re, a.im-b.im); }
TT_INLINE TT_Cplx tt_cplx_mul(TT_Cplx a, TT_Cplx b) { return tt_cplx(a.re*b.re-a.im*b.im, a.re*b.im+a.im*b.re); }
TT_INLINE TT_Cplx tt_cplx_div(TT_Cplx a, TT_Cplx b) {
    double d=b.re*b.re+b.im*b.im;
    return d==0.0 ? tt_cplx(0,0) : tt_cplx((a.re*b.re+a.im*b.im)/d,(a.im*b.re-a.re*b.im)/d);
}
TT_INLINE TT_Cplx tt_cplx_conj(TT_Cplx a)  { return tt_cplx(a.re,-a.im); }
TT_INLINE double  tt_cplx_abs(TT_Cplx a)   { return sqrt(a.re*a.re+a.im*a.im); }
TT_INLINE double  tt_cplx_arg(TT_Cplx a)   { return atan2(a.im,a.re); }
TT_INLINE TT_Cplx tt_cplx_exp(TT_Cplx a)   { double e=exp(a.re); return tt_cplx(e*cos(a.im),e*sin(a.im)); }
TT_INLINE TT_Cplx tt_cplx_log(TT_Cplx a)   { return tt_cplx(log(tt_cplx_abs(a)), tt_cplx_arg(a)); }
TT_INLINE TT_Cplx tt_cplx_pow(TT_Cplx a, TT_Cplx b) { return tt_cplx_exp(tt_cplx_mul(b, tt_cplx_log(a))); }
TT_INLINE TT_Cplx tt_cplx_sqrt(TT_Cplx a)  {
    double r=tt_cplx_abs(a), th=tt_cplx_arg(a);
    return tt_cplx(sqrt(r)*cos(th/2.0), sqrt(r)*sin(th/2.0));
}
TT_INLINE TT_Cplx tt_cplx_from_polar(double r, double th) { return tt_cplx(r*cos(th), r*sin(th)); }


typedef struct { double x,y;       } TT_V2;
typedef struct { double x,y,z;     } TT_V3;
typedef struct { double x,y,z,w;   } TT_V4;

TT_INLINE TT_V2 tt_v2(double x,double y){TT_V2 v;v.x=x;v.y=y;return v;}
TT_INLINE TT_V2 tt_v2_add(TT_V2 a,TT_V2 b){return tt_v2(a.x+b.x,a.y+b.y);}
TT_INLINE TT_V2 tt_v2_sub(TT_V2 a,TT_V2 b){return tt_v2(a.x-b.x,a.y-b.y);}
TT_INLINE TT_V2 tt_v2_scale(TT_V2 a,double s){return tt_v2(a.x*s,a.y*s);}
TT_INLINE double tt_v2_dot(TT_V2 a,TT_V2 b){return a.x*b.x+a.y*b.y;}
TT_INLINE double tt_v2_len(TT_V2 a){return sqrt(a.x*a.x+a.y*a.y);}
TT_INLINE TT_V2  tt_v2_norm(TT_V2 a){double l=tt_v2_len(a);return l?tt_v2_scale(a,1.0/l):a;}
TT_INLINE TT_V2  tt_v2_perp(TT_V2 a){return tt_v2(-a.y,a.x);}
TT_INLINE double tt_v2_cross(TT_V2 a,TT_V2 b){return a.x*b.y-a.y*b.x;}
TT_INLINE TT_V2  tt_v2_lerp(TT_V2 a,TT_V2 b,double t){return tt_v2(a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t);}
TT_INLINE double tt_v2_dist(TT_V2 a,TT_V2 b){return tt_v2_len(tt_v2_sub(b,a));}

TT_INLINE TT_V3 tt_v3(double x,double y,double z){TT_V3 v;v.x=x;v.y=y;v.z=z;return v;}
TT_INLINE TT_V3 tt_v3_add(TT_V3 a,TT_V3 b){return tt_v3(a.x+b.x,a.y+b.y,a.z+b.z);}
TT_INLINE TT_V3 tt_v3_sub(TT_V3 a,TT_V3 b){return tt_v3(a.x-b.x,a.y-b.y,a.z-b.z);}
TT_INLINE TT_V3 tt_v3_scale(TT_V3 a,double s){return tt_v3(a.x*s,a.y*s,a.z*s);}
TT_INLINE double tt_v3_dot(TT_V3 a,TT_V3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
TT_INLINE double tt_v3_len(TT_V3 a){return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);}
TT_INLINE TT_V3  tt_v3_norm(TT_V3 a){double l=tt_v3_len(a);return l?tt_v3_scale(a,1.0/l):a;}
TT_INLINE TT_V3  tt_v3_cross(TT_V3 a,TT_V3 b){return tt_v3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
TT_INLINE TT_V3  tt_v3_lerp(TT_V3 a,TT_V3 b,double t){return tt_v3(a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t);}
TT_INLINE double tt_v3_dist(TT_V3 a,TT_V3 b){return tt_v3_len(tt_v3_sub(b,a));}
TT_INLINE TT_V3  tt_v3_reflect(TT_V3 v,TT_V3 n){return tt_v3_sub(v,tt_v3_scale(n,2.0*tt_v3_dot(v,n)));}

TT_INLINE TT_V4 tt_v4(double x,double y,double z,double w){TT_V4 v;v.x=x;v.y=y;v.z=z;v.w=w;return v;}
TT_INLINE TT_V4 tt_v4_add(TT_V4 a,TT_V4 b){return tt_v4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);}
TT_INLINE double tt_v4_dot(TT_V4 a,TT_V4 b){return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;}


typedef struct { double x,y,z,w; } TT_Quat;

TT_INLINE TT_Quat tt_quat(double x,double y,double z,double w){TT_Quat q;q.x=x;q.y=y;q.z=z;q.w=w;return q;}
TT_INLINE TT_Quat tt_quat_identity(void){return tt_quat(0,0,0,1);}
TT_INLINE TT_Quat tt_quat_mul(TT_Quat a,TT_Quat b){
    return tt_quat(
        a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y,
        a.w*b.y-a.x*b.z+a.y*b.w+a.z*b.x,
        a.w*b.z+a.x*b.y-a.y*b.x+a.z*b.w,
        a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z);
}
TT_INLINE TT_Quat tt_quat_conj(TT_Quat q){return tt_quat(-q.x,-q.y,-q.z,q.w);}
TT_INLINE double  tt_quat_len(TT_Quat q){return sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w);}
TT_INLINE TT_Quat tt_quat_norm(TT_Quat q){double l=tt_quat_len(q);return l?tt_quat(q.x/l,q.y/l,q.z/l,q.w/l):q;}
TT_INLINE TT_Quat tt_quat_from_axis_angle(TT_V3 axis, double angle) {
    double s=sin(angle/2.0);
    return tt_quat_norm(tt_quat(axis.x*s,axis.y*s,axis.z*s,cos(angle/2.0)));
}
TT_INLINE TT_V3   tt_quat_rotate(TT_Quat q, TT_V3 v) {
    TT_Quat vq=tt_quat(v.x,v.y,v.z,0);
    TT_Quat r=tt_quat_mul(tt_quat_mul(q,vq),tt_quat_conj(q));
    return tt_v3(r.x,r.y,r.z);
}
TT_INLINE TT_Quat tt_quat_slerp(TT_Quat a, TT_Quat b, double t) {
    double dot=a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
    double theta, sinT, sa, sb;
    if (dot<0){dot=-dot;b=tt_quat(-b.x,-b.y,-b.z,-b.w);}
    if (dot>0.9995){
        TT_Quat r=tt_quat(a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t,a.z+(b.z-a.z)*t,a.w+(b.w-a.w)*t);
        return tt_quat_norm(r);
    }
    theta=acos(dot); sinT=sin(theta);
    sa=sin((1.0-t)*theta)/sinT; sb=sin(t*theta)/sinT;
    return tt_quat(a.x*sa+b.x*sb,a.y*sa+b.y*sb,a.z*sa+b.z*sb,a.w*sa+b.w*sb);
}


typedef struct { tt_u8 r,g,b,a; } TT_Color;

TT_INLINE TT_Color tt_rgba(tt_u8 r,tt_u8 g,tt_u8 b,tt_u8 a){TT_Color c;c.r=r;c.g=g;c.b=b;c.a=a;return c;}
TT_INLINE TT_Color tt_rgb(tt_u8 r,tt_u8 g,tt_u8 b){return tt_rgba(r,g,b,255);}
TT_INLINE TT_Color tt_color_from_hex(tt_u32 hex){
    return tt_rgba((tt_u8)((hex>>16)&0xFF),(tt_u8)((hex>>8)&0xFF),(tt_u8)(hex&0xFF),255);
}
TT_INLINE tt_u32 tt_color_to_hex(TT_Color c){return ((tt_u32)c.r<<16)|((tt_u32)c.g<<8)|c.b;}
TT_INLINE TT_Color tt_color_lerp(TT_Color a, TT_Color b, double t) {
    return tt_rgba(
        (tt_u8)(a.r+(b.r-a.r)*t), (tt_u8)(a.g+(b.g-a.g)*t),
        (tt_u8)(a.b+(b.b-a.b)*t), (tt_u8)(a.a+(b.a-a.a)*t));
}
TT_INLINE TT_Color tt_color_alpha_blend(TT_Color src, TT_Color dst) {
    double sa=src.a/255.0, da=dst.a/255.0*(1.0-sa);
    double oa=sa+da;
    return oa<1e-9 ? tt_rgba(0,0,0,0) :
        tt_rgba((tt_u8)((src.r*sa+dst.r*da)/oa),
                (tt_u8)((src.g*sa+dst.g*da)/oa),
                (tt_u8)((src.b*sa+dst.b*da)/oa),
                (tt_u8)(oa*255.0));
}
#define TT_RED     tt_rgb(255,  0,  0)
#define TT_GREEN   tt_rgb(  0,255,  0)
#define TT_BLUE    tt_rgb(  0,  0,255)
#define TT_WHITE   tt_rgb(255,255,255)
#define TT_BLACK   tt_rgb(  0,  0,  0)
#define TT_YELLOW  tt_rgb(255,255,  0)
#define TT_CYAN    tt_rgb(  0,255,255)
#define TT_MAGENTA tt_rgb(255,  0,255)
#define TT_ORANGE  tt_rgb(255,165,  0)
#define TT_PURPLE  tt_rgb(128,  0,128)
TT_INLINE int tt_color_to_ansi256(TT_Color c) {
    if (c.r==c.g && c.g==c.b) {
        if (c.r<8)   return 16;
        if (c.r>247) return 231;
        return (int)((c.r-8)/247.0*24)+232;
    }
    return 16 + 36*(c.r/51) + 6*(c.g/51) + (c.b/51);
}


typedef struct {
    void   *buf;
    int    *free_list;
    int     cap;
    int     block_size;
    int     head;
} TT_Pool;

TT_INLINE int tt_pool_init(TT_Pool *p, int cap, int block_size) {
    int i;
    p->cap        = cap;
    p->block_size = block_size;
    p->buf        = malloc((size_t)cap * (size_t)block_size);
    p->free_list  = (int*)malloc((size_t)cap * sizeof(int));
    if (!p->buf || !p->free_list) { free(p->buf); free(p->free_list); return 0; }
    for (i=0; i<cap; i++) p->free_list[i] = i;
    p->head = cap - 1;
    return 1;
}

TT_INLINE void *tt_pool_alloc(TT_Pool *p) {
    if (p->head < 0) return NULL;
    return (tt_u8*)p->buf + (size_t)p->free_list[p->head--] * (size_t)p->block_size;
}

TT_INLINE void tt_pool_free_ptr(TT_Pool *p, void *ptr) {
    int idx = (int)(((tt_u8*)ptr - (tt_u8*)p->buf) / (size_t)p->block_size);
    if (p->head < p->cap - 1) p->free_list[++p->head] = idx;
}

TT_INLINE void tt_pool_destroy(TT_Pool *p) {
    free(p->buf); free(p->free_list);
    p->buf = NULL; p->free_list = NULL;
}


#define TT_TRIE_ALPHA 128            

typedef struct TT_TrieNode {
    struct TT_TrieNode *children[TT_TRIE_ALPHA];
    int is_end;
    void *data;
} TT_TrieNode;

TT_INLINE TT_TrieNode *tt_trie_new(void) {
    return (TT_TrieNode*)calloc(1, sizeof(TT_TrieNode));
}

TT_INLINE void tt_trie_insert(TT_TrieNode *root, const char *key, void *data) {
    for (; *key; key++) {
        unsigned char c = (unsigned char)*key % TT_TRIE_ALPHA;
        if (!root->children[c]) root->children[c] = tt_trie_new();
        root = root->children[c];
    }
    root->is_end = 1; root->data = data;
}

TT_INLINE TT_TrieNode *tt_trie_search(TT_TrieNode *root, const char *key) {
    for (; *key; key++) {
        unsigned char c = (unsigned char)*key % TT_TRIE_ALPHA;
        if (!root->children[c]) return NULL;
        root = root->children[c];
    }
    return root->is_end ? root : NULL;
}

TT_INLINE void *tt_trie_get(TT_TrieNode *root, const char *key) {
    TT_TrieNode *n = tt_trie_search(root, key);
    return n ? n->data : NULL;
}

static void tt__trie_free(TT_TrieNode *node) {
    int i;
    if (!node) return;
    for (i=0;i<TT_TRIE_ALPHA;i++) tt__trie_free(node->children[i]);
    free(node);
}
TT_INLINE void tt_trie_free(TT_TrieNode *root) { tt__trie_free(root); }


typedef int (*TT_HeapCmp)(const void*, const void*);

typedef struct {
    void     **data;
    int        size;
    int        cap;
    TT_HeapCmp cmp;
} TT_Heap;

TT_INLINE int tt_heap_init(TT_Heap *h, int cap, TT_HeapCmp cmp) {
    h->data = (void**)malloc((size_t)cap * sizeof(void*));
    h->size = 0; h->cap = cap; h->cmp = cmp;
    return h->data != NULL;
}
TT_INLINE void tt_heap_free(TT_Heap *h) { free(h->data); h->data=NULL; }

TT_INLINE void tt_heap_push(TT_Heap *h, void *item) {
    int i;
    if (h->size >= h->cap) {
        void **p = (void**)realloc(h->data, (size_t)(h->cap*2)*sizeof(void*));
        if (!p) return;
        h->data = p; h->cap *= 2;
    }
    h->data[h->size++] = item;
    i = h->size - 1;
    while (i > 0) {
        int parent = (i-1)/2;
        if (h->cmp(h->data[i], h->data[parent]) < 0) {
            void *t = h->data[i]; h->data[i]=h->data[parent]; h->data[parent]=t;
            i = parent;
        } else break;
    }
}

TT_INLINE void *tt_heap_pop(TT_Heap *h) {
    void *top; int i=0;
    if (!h->size) return NULL;
    top = h->data[0];
    h->data[0] = h->data[--h->size];
    while (1) {
        int l=2*i+1, r=2*i+2, best=i;
        if (l<h->size && h->cmp(h->data[l],h->data[best])<0) best=l;
        if (r<h->size && h->cmp(h->data[r],h->data[best])<0) best=r;
        if (best==i) break;
        { void *t=h->data[i]; h->data[i]=h->data[best]; h->data[best]=t; }
        i=best;
    }
    return top;
}

TT_INLINE void *tt_heap_peek(const TT_Heap *h) { return h->size ? h->data[0] : NULL; }


TT_INLINE void tt_isort(void *base, int n, int size, int (*cmp)(const void*,const void*)) {
    int i,j;
    char *b = (char*)base;
    char *tmp = (char*)malloc((size_t)size);
    if (!tmp) return;
    for (i=1;i<n;i++) {
        memcpy(tmp, b+i*size, (size_t)size);
        for (j=i-1; j>=0 && cmp(b+j*size,tmp)>0; j--)
            memcpy(b+(j+1)*size, b+j*size, (size_t)size);
        memcpy(b+(j+1)*size, tmp, (size_t)size);
    }
    free(tmp);
}

TT_INLINE int tt_bsearch(const void *key, const void *base, int n, int size,
                          int (*cmp)(const void*,const void*)) {
    int lo=0, hi=n-1;
    const char *b = (const char*)base;
    while (lo<=hi) {
        int mid=(lo+hi)/2;
        int c=cmp(key, b+mid*size);
        if (c==0) return mid;
        if (c<0)  hi=mid-1;
        else      lo=mid+1;
    }
    return -1;
}

TT_INLINE int tt_lower_bound(const void *key, const void *base, int n, int size,
                              int (*cmp)(const void*,const void*)) {
    int lo=0, hi=n;
    const char *b = (const char*)base;
    while (lo<hi) {
        int mid=(lo+hi)/2;
        if (cmp(b+mid*size, key)<0) lo=mid+1; else hi=mid;
    }
    return lo;
}

TT_INLINE int tt_upper_bound(const void *key, const void *base, int n, int size,
                              int (*cmp)(const void*,const void*)) {
    int lo=0, hi=n;
    const char *b = (const char*)base;
    while (lo<hi) {
        int mid=(lo+hi)/2;
        if (cmp(b+mid*size, key)<=0) lo=mid+1; else hi=mid;
    }
    return lo;
}


typedef struct { const char *ptr; size_t len; } TT_Sv;

TT_INLINE TT_Sv   tt_sv(const char *s)           { TT_Sv v; v.ptr=s; v.len=strlen(s); return v; }
TT_INLINE TT_Sv   tt_sv_n(const char *s, size_t n){ TT_Sv v; v.ptr=s; v.len=n; return v; }
TT_INLINE int     tt_sv_eq(TT_Sv a, TT_Sv b)     { return a.len==b.len&&memcmp(a.ptr,b.ptr,a.len)==0; }
TT_INLINE int     tt_sv_starts(TT_Sv s, TT_Sv p) { return s.len>=p.len&&memcmp(s.ptr,p.ptr,p.len)==0; }
TT_INLINE int     tt_sv_ends(TT_Sv s, TT_Sv p)   { return s.len>=p.len&&memcmp(s.ptr+s.len-p.len,p.ptr,p.len)==0; }
TT_INLINE TT_Sv   tt_sv_trim(TT_Sv s) {
    while (s.len && isspace((unsigned char)*s.ptr)) { s.ptr++; s.len--; }
    while (s.len && isspace((unsigned char)s.ptr[s.len-1])) s.len--;
    return s;
}
TT_INLINE TT_Sv   tt_sv_slice(TT_Sv s, size_t lo, size_t hi) {
    if (lo>s.len) { lo=s.len; } if (hi>s.len) { hi=s.len; }
    return tt_sv_n(s.ptr+lo, hi-lo);
}
TT_INLINE int     tt_sv_find(TT_Sv s, TT_Sv pat) {
    size_t i;
    if (pat.len==0) return 0;
    for (i=0; i+pat.len<=s.len; i++)
        if (memcmp(s.ptr+i, pat.ptr, pat.len)==0) return (int)i;
    return -1;
}
TT_INLINE char   *tt_sv_dup(TT_Sv s) { return tt_strndup(s.ptr, s.len); }


typedef struct { char *buf; size_t len; size_t cap; } TT_SB;

TT_INLINE void tt_sb_init(TT_SB *sb, size_t initial_cap) {
    sb->buf = (char*)malloc(initial_cap ? initial_cap : 64);
    sb->len = 0; sb->cap = initial_cap ? initial_cap : 64;
    if (sb->buf) sb->buf[0] = '\0';
}
TT_INLINE void tt_sb_free(TT_SB *sb) { free(sb->buf); sb->buf=NULL; sb->len=sb->cap=0; }

TT_INLINE int tt_sb_grow(TT_SB *sb, size_t needed) {
    if (sb->len + needed < sb->cap) return 1;
    size_t ncap = sb->cap * 2;
    char *p;
    while (ncap < sb->len + needed + 1) ncap *= 2;
    p = (char*)realloc(sb->buf, ncap);
    if (!p) return 0;
    sb->buf = p; sb->cap = ncap;
    return 1;
}

TT_INLINE void tt_sb_append(TT_SB *sb, const char *s, size_t n) {
    if (!tt_sb_grow(sb, n)) return;
    memcpy(sb->buf + sb->len, s, n);
    sb->len += n;
    sb->buf[sb->len] = '\0';
}
TT_INLINE void tt_sb_appends(TT_SB *sb, const char *s) { tt_sb_append(sb, s, strlen(s)); }
TT_INLINE void tt_sb_appendc(TT_SB *sb, char c)        { tt_sb_append(sb, &c, 1); }
TT_INLINE void tt_sb_appendf(TT_SB *sb, const char *fmt, ...) {
    char tmp[1024]; va_list ap;
    va_start(ap, fmt); tt_vsnprintf(tmp, sizeof(tmp), fmt, ap); va_end(ap);
    tt_sb_appends(sb, tmp);
}
TT_INLINE void tt_sb_clear(TT_SB *sb) { sb->len=0; if(sb->buf) sb->buf[0]='\0'; }
TT_INLINE char *tt_sb_finish(TT_SB *sb) { return sb->buf; }


#define TT_SHA256_DIGEST_LEN 32

typedef struct {
    tt_u32 state[8];
    tt_u64 count;
    tt_u8  buf[64];
    int    buflen;
} TT_Sha256;

static const tt_u32 tt__sha256_k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define TT_SHA256_ROTR(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define TT_SHA256_CH(x,y,z)  (((x)&(y))^(~(x)&(z)))
#define TT_SHA256_MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))
#define TT_SHA256_S0(x)  (TT_SHA256_ROTR(x,2)^TT_SHA256_ROTR(x,13)^TT_SHA256_ROTR(x,22))
#define TT_SHA256_S1(x)  (TT_SHA256_ROTR(x,6)^TT_SHA256_ROTR(x,11)^TT_SHA256_ROTR(x,25))
#define TT_SHA256_G0(x)  (TT_SHA256_ROTR(x,7)^TT_SHA256_ROTR(x,18)^((x)>>3))
#define TT_SHA256_G1(x)  (TT_SHA256_ROTR(x,17)^TT_SHA256_ROTR(x,19)^((x)>>10))

static void tt__sha256_transform(TT_Sha256 *ctx, const tt_u8 *block) {
    tt_u32 w[64], a,b,c,d,e,f,g,h,t1,t2;
    int i;
    for (i=0;i<16;i++) w[i]=((tt_u32)block[i*4]<<24)|((tt_u32)block[i*4+1]<<16)|((tt_u32)block[i*4+2]<<8)|block[i*4+3];
    for (i=16;i<64;i++) w[i]=TT_SHA256_G1(w[i-2])+w[i-7]+TT_SHA256_G0(w[i-15])+w[i-16];
    a=ctx->state[0];b=ctx->state[1];c=ctx->state[2];d=ctx->state[3];
    e=ctx->state[4];f=ctx->state[5];g=ctx->state[6];h=ctx->state[7];
    for (i=0;i<64;i++){
        t1=h+TT_SHA256_S1(e)+TT_SHA256_CH(e,f,g)+tt__sha256_k[i]+w[i];
        t2=TT_SHA256_S0(a)+TT_SHA256_MAJ(a,b,c);
        h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;
    }
    ctx->state[0]+=a;ctx->state[1]+=b;ctx->state[2]+=c;ctx->state[3]+=d;
    ctx->state[4]+=e;ctx->state[5]+=f;ctx->state[6]+=g;ctx->state[7]+=h;
}

TT_INLINE void tt_sha256_init(TT_Sha256 *ctx) {
    ctx->state[0]=0x6a09e667; ctx->state[1]=0xbb67ae85;
    ctx->state[2]=0x3c6ef372; ctx->state[3]=0xa54ff53a;
    ctx->state[4]=0x510e527f; ctx->state[5]=0x9b05688c;
    ctx->state[6]=0x1f83d9ab; ctx->state[7]=0x5be0cd19;
    ctx->count=0; ctx->buflen=0;
}

TT_INLINE void tt_sha256_update(TT_Sha256 *ctx, const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    while (len > 0) {
        int space = 64 - ctx->buflen;
        int take  = (int)len < space ? (int)len : space;
        memcpy(ctx->buf + ctx->buflen, p, (size_t)take);
        ctx->buflen += take; p += take; len -= (size_t)take;
        ctx->count  += (tt_u64)take * 8;
        if (ctx->buflen == 64) { tt__sha256_transform(ctx, ctx->buf); ctx->buflen=0; }
    }
}

TT_INLINE void tt_sha256_final(TT_Sha256 *ctx, tt_u8 digest[TT_SHA256_DIGEST_LEN]) {
    int i; tt_u64 bc = ctx->count;
    ctx->buf[ctx->buflen++] = 0x80;
    if (ctx->buflen > 56) { memset(ctx->buf+ctx->buflen,0,(size_t)(64-ctx->buflen)); tt__sha256_transform(ctx,ctx->buf); ctx->buflen=0; }
    memset(ctx->buf+ctx->buflen,0,(size_t)(56-ctx->buflen));
    for (i=0;i<8;i++) ctx->buf[56+i]=(tt_u8)(bc>>(56-i*8));
    tt__sha256_transform(ctx,ctx->buf);
    for (i=0;i<8;i++){digest[i*4]=(tt_u8)(ctx->state[i]>>24);digest[i*4+1]=(tt_u8)(ctx->state[i]>>16);digest[i*4+2]=(tt_u8)(ctx->state[i]>>8);digest[i*4+3]=(tt_u8)ctx->state[i];}
}

TT_INLINE void tt_sha256(const void *data, size_t len, tt_u8 digest[TT_SHA256_DIGEST_LEN]) {
    TT_Sha256 ctx; tt_sha256_init(&ctx); tt_sha256_update(&ctx,data,len); tt_sha256_final(&ctx,digest);
}

TT_INLINE void tt_sha256_hex(const void *data, size_t len, char out[65]) {
    tt_u8 d[32]; int i; tt_sha256(data,len,d);
    for (i=0;i<32;i++) tt_snprintf(out+i*2,3,"%02x",d[i]);
    out[64]='\0';
}


#define TT_MD5_DIGEST_LEN 16

typedef struct { tt_u32 state[4]; tt_u64 count; tt_u8 buf[64]; int buflen; } TT_Md5;

static const tt_u32 tt__md5_t[64] = {
    0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
    0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
    0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
    0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
    0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
    0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
    0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
    0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
};
static const tt_u8 tt__md5_s[64] = {
    7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
    5, 9,14,20,5, 9,14,20,5, 9,14,20,5, 9,14,20,
    4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
    6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
};

#define TT_MD5_ROTL(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define TT_MD5_LE32(b) ((tt_u32)(b)[0]|((tt_u32)(b)[1]<<8)|((tt_u32)(b)[2]<<16)|((tt_u32)(b)[3]<<24))

static void tt__md5_transform(TT_Md5 *ctx, const tt_u8 *block) {
    tt_u32 a=ctx->state[0],b=ctx->state[1],c=ctx->state[2],d=ctx->state[3],f,g,tmp;
    tt_u32 m[16]; int i;
    for (i=0;i<16;i++) m[i]=TT_MD5_LE32(block+i*4);
    for (i=0;i<64;i++){
        if (i<16){f=(b&c)|(~b&d);g=(tt_u32)i;}
        else if (i<32){f=(d&b)|(~d&c);g=(tt_u32)(5*i+1)%16;}
        else if (i<48){f=b^c^d;g=(tt_u32)(3*i+5)%16;}
        else{f=c^(b|~d);g=(tt_u32)(7*i)%16;}
        tmp=d;d=c;c=b;b=b+TT_MD5_ROTL(a+f+tt__md5_t[i]+m[g],(int)tt__md5_s[i]);a=tmp;
    }
    ctx->state[0]+=a;ctx->state[1]+=b;ctx->state[2]+=c;ctx->state[3]+=d;
}

TT_INLINE void tt_md5_init(TT_Md5 *c){c->state[0]=0x67452301;c->state[1]=0xefcdab89;c->state[2]=0x98badcfe;c->state[3]=0x10325476;c->count=0;c->buflen=0;}

TT_INLINE void tt_md5_update(TT_Md5 *ctx, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while (len>0){
        int sp=64-ctx->buflen, tk=(int)len<sp?(int)len:sp;
        memcpy(ctx->buf+ctx->buflen,p,(size_t)tk);
        ctx->buflen+=tk;p+=tk;len-=(size_t)tk;ctx->count+=(tt_u64)tk*8;
        if(ctx->buflen==64){tt__md5_transform(ctx,ctx->buf);ctx->buflen=0;}
    }
}

TT_INLINE void tt_md5_final(TT_Md5 *ctx, tt_u8 digest[TT_MD5_DIGEST_LEN]) {
    int i; tt_u64 bc=ctx->count;
    ctx->buf[ctx->buflen++]=0x80;
    if(ctx->buflen>56){memset(ctx->buf+ctx->buflen,0,(size_t)(64-ctx->buflen));tt__md5_transform(ctx,ctx->buf);ctx->buflen=0;}
    memset(ctx->buf+ctx->buflen,0,(size_t)(56-ctx->buflen));
    for(i=0;i<8;i++) ctx->buf[56+i]=(tt_u8)(bc>>(i*8));
    tt__md5_transform(ctx,ctx->buf);
    for(i=0;i<4;i++){digest[i*4]=(tt_u8)ctx->state[i];digest[i*4+1]=(tt_u8)(ctx->state[i]>>8);digest[i*4+2]=(tt_u8)(ctx->state[i]>>16);digest[i*4+3]=(tt_u8)(ctx->state[i]>>24);}
}

TT_INLINE void tt_md5(const void *data, size_t len, tt_u8 digest[TT_MD5_DIGEST_LEN]) {
    TT_Md5 c; tt_md5_init(&c); tt_md5_update(&c,data,len); tt_md5_final(&c,digest);
}

TT_INLINE void tt_md5_hex(const void *data, size_t len, char out[33]) {
    tt_u8 d[16]; int i; tt_md5(data,len,d);
    for(i=0;i<16;i++) tt_snprintf(out+i*2,3,"%02x",d[i]);
    out[32]='\0';
}


TT_INLINE void tt_url_encode(const char *in, char *out, size_t out_sz) {
    static const char hex[]="0123456789ABCDEF";
    size_t j=0;
    while (*in && j+3 < out_sz) {
        unsigned char c=(unsigned char)*in++;
        if (isalnum(c)||c=='-'||c=='_'||c=='.'||c=='~') { out[j++]=(char)c; }
        else { out[j++]='%'; out[j++]=hex[c>>4]; out[j++]=hex[c&0xF]; }
    }
    out[j]='\0';
}

TT_INLINE void tt_url_decode(const char *in, char *out, size_t out_sz) {
    size_t j=0;
    while (*in && j+1 < out_sz) {
        if (*in=='%'&&in[1]&&in[2]) {
            char hi=in[1], lo=in[2];
            int h = isdigit((unsigned char)hi)?hi-'0':tolower((unsigned char)hi)-'a'+10;
            int l = isdigit((unsigned char)lo)?lo-'0':tolower((unsigned char)lo)-'a'+10;
            out[j++]=(char)((h<<4)|l); in+=3;
        } else if (*in=='+') { out[j++]=' '; in++; }
        else { out[j++]=*in++; }
    }
    out[j]='\0';
}


TT_INLINE int tt_fnmatch(const char *pat, const char *str) {
    while (*pat && *str) {
        if (*pat == '*') {
            while (*pat=='*') pat++;
            if (!*pat) return 1;
            while (*str) { if (tt_fnmatch(pat,str)) return 1; str++; }
            return 0;
        } else if (*pat == '?') { pat++; str++; }
        else if (*pat == '[') {
            int neg=0, matched=0;
            pat++;
            if (*pat=='!'){neg=1;pat++;}
            while (*pat && *pat!=']') {
                if (*(pat+1)=='-'&&*(pat+2)&&*(pat+2)!=']'){
                    if (*str>=(unsigned char)*pat&&*str<=(unsigned char)*(pat+2)) matched=1;
                    pat+=3;
                } else { if (*str==(unsigned char)*pat) matched=1; pat++; }
            }
            if (*pat==']') pat++;
            if (matched==neg) return 0;
            str++;
        } else { if (*pat!=*str) return 0; pat++; str++; }
    }
    while (*pat=='*') pat++;
    return !*pat && !*str;
}


typedef struct {
    int    optind;
    int    optopt;
    char  *optarg;
    int    _sp;                                                       
} TT_Getopt;

TT_INLINE void tt_getopt_init(TT_Getopt *g) { g->optind=1; g->optopt=0; g->optarg=NULL; g->_sp=1; }

TT_INLINE int tt_getopt(TT_Getopt *g, int argc, char *const argv[], const char *opts) {
    const char *oli;
    if (g->optind >= argc || !argv[g->optind]) return -1;
    if (g->_sp == 1) {
        if (argv[g->optind][0]!='-'||argv[g->optind][1]=='\0') return -1;
        if (argv[g->optind][1]=='-'&&argv[g->optind][2]=='\0') { g->optind++; return -1; }
    }
    g->optopt = (int)(unsigned char)argv[g->optind][g->_sp];
    oli = strchr(opts, (char)g->optopt);
    if (!oli) { if (!argv[g->optind][++g->_sp]){g->optind++;g->_sp=1;} return '?'; }
    if (*(oli+1)==':') {
        if (argv[g->optind][g->_sp+1]) g->optarg=argv[g->optind++]+g->_sp+1;
        else if (g->optind+1 >= argc) { if (*opts!=':'){} g->optarg=NULL; g->optind++; g->_sp=1; return ':'; }
        else g->optarg=argv[++g->optind];
        g->_sp=1; g->optind++;
    } else { if (!argv[g->optind][++g->_sp]){g->optind++;g->_sp=1;} g->optarg=NULL; }
    return g->optopt;
}


#define TT_ARGS_MAX 64

typedef struct {
    const char *keys[TT_ARGS_MAX];
    const char *vals[TT_ARGS_MAX];                               
    const char *positional[TT_ARGS_MAX];
    int         nkeys;
    int         npositional;
} TT_Args;

TT_INLINE void tt_args_parse(TT_Args *a, int argc, char *const argv[]) {
    int i;
    memset(a, 0, sizeof(*a));
    for (i=1; i<argc; i++) {
        const char *s = argv[i];
        const char *eq;
        if (s[0]=='-'&&s[1]=='-') {
            s+=2; eq=strchr(s,'=');
            if (eq && a->nkeys<TT_ARGS_MAX) {
                a->keys[a->nkeys]   = s;
                a->vals[a->nkeys++] = eq+1;
            } else if (a->nkeys<TT_ARGS_MAX) {
                a->keys[a->nkeys]   = s;
                a->vals[a->nkeys++] = NULL;
            }
        } else if (s[0]=='-'&&s[1]) {
            s++;
            if (a->nkeys<TT_ARGS_MAX) { a->keys[a->nkeys]=s; a->vals[a->nkeys++]=NULL; }
        } else {
            if (a->npositional<TT_ARGS_MAX) a->positional[a->npositional++]=s;
        }
    }
}

TT_INLINE const char *tt_args_get(const TT_Args *a, const char *key) {
    int i;
    for (i=0;i<a->nkeys;i++)
        if (tt_strcasecmp(a->keys[i],key)==0) return a->vals[i] ? a->vals[i] : "";
    return NULL;
}

TT_INLINE int tt_args_flag(const TT_Args *a, const char *key) { return tt_args_get(a,key)!=NULL; }


TT_INLINE int tt_is_little_endian(void) {
    tt_u16 v = 1; return *(tt_u8*)&v == 1;
}

TT_INLINE tt_u16 tt_bswap16(tt_u16 x) { return (tt_u16)((x>>8)|(x<<8)); }
TT_INLINE tt_u32 tt_bswap32(tt_u32 x) {
    return ((x>>24)&0xFF)|((x>>8)&0xFF00)|((x<<8)&0xFF0000)|((x<<24)&0xFF000000u);
}
TT_INLINE tt_u64 tt_bswap64(tt_u64 x) {
    return ((tt_u64)tt_bswap32((tt_u32)x)<<32)|tt_bswap32((tt_u32)(x>>32));
}

TT_INLINE tt_u16 tt_htole16(tt_u16 x) { return tt_is_little_endian() ? x : tt_bswap16(x); }
TT_INLINE tt_u32 tt_htole32(tt_u32 x) { return tt_is_little_endian() ? x : tt_bswap32(x); }
TT_INLINE tt_u16 tt_htobe16(tt_u16 x) { return tt_is_little_endian() ? tt_bswap16(x) : x; }
TT_INLINE tt_u32 tt_htobe32(tt_u32 x) { return tt_is_little_endian() ? tt_bswap32(x) : x; }
TT_INLINE tt_u16 tt_le16toh(tt_u16 x) { return tt_htole16(x); }
TT_INLINE tt_u32 tt_le32toh(tt_u32 x) { return tt_htole32(x); }
TT_INLINE tt_u16 tt_be16toh(tt_u16 x) { return tt_htobe16(x); }
TT_INLINE tt_u32 tt_be32toh(tt_u32 x) { return tt_htobe32(x); }


typedef struct { int year,month,day,hour,min,sec; } TT_DateTime;

TT_INLINE TT_DateTime tt_datetime_now(void) {
    TT_DateTime dt; time_t t=time(NULL); struct tm *tm=localtime(&t);
    dt.year=tm->tm_year+1900; dt.month=tm->tm_mon+1; dt.day=tm->tm_mday;
    dt.hour=tm->tm_hour; dt.min=tm->tm_min; dt.sec=tm->tm_sec;
    return dt;
}

TT_INLINE void tt_datetime_to_iso8601(TT_DateTime dt, char *buf, int bufsz) {
    tt_snprintf(buf, (size_t)bufsz, "%04d-%02d-%02dT%02d:%02d:%02d",
                dt.year,dt.month,dt.day,dt.hour,dt.min,dt.sec);
}

TT_INLINE int tt_is_leap_year(int y) { return (y%4==0&&y%100!=0)||(y%400==0); }

TT_INLINE int tt_days_in_month(int m, int y) {
    static const int days[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if (m==2&&tt_is_leap_year(y)) return 29;
    return days[(m-1)%12];
}

TT_INLINE int tt_date_to_days(int y, int m, int d) {
    int days=(y-1970)*365+(y-1969)/4-(y-1601)/100+(y-1601)/400;
    static const int md[]={0,31,59,90,120,151,181,212,243,273,304,334};
    days+=md[m-1]+d-1;
    if (m>2&&tt_is_leap_year(y)) days++;
    return days;
}


TT_INLINE void tt_sparkline(int x, int y, const double *data, int count) {
    static const char *bars[] = {" ","_","\xe2\x96\x81","\xe2\x96\x82","\xe2\x96\x83",
                                  "\xe2\x96\x84","\xe2\x96\x85","\xe2\x96\x86","\xe2\x96\x87","\xe2\x96\x88"};
    double mn=data[0],mx=data[0];
    int i;
    for (i=1;i<count;i++){if(data[i]<mn)mn=data[i];if(data[i]>mx)mx=data[i];}
    tt_goto(x, y);
    for (i=0;i<count;i++){
        int idx = (mx>mn) ? (int)((data[i]-mn)/(mx-mn)*9.0) : 4;
        if (idx<0) { idx=0; } if (idx>9) { idx=9; }
        tt_write(bars[idx]);
    }
}

TT_INLINE void tt_heatmap(int x, int y, const double *data, int rows, int cols) {
    int i, j;
    double mn=data[0],mx=data[0];
    for (i=0;i<rows*cols;i++){if(data[i]<mn)mn=data[i];if(data[i]>mx)mx=data[i];}
    for (j=0;j<rows;j++) for (i=0;i<cols;i++) {
        double v = (mx>mn)?(data[j*cols+i]-mn)/(mx-mn):0.5;
        int ci = 16 + (int)(v * 215.0);                     
        char col[24];
        tt_bg(col, sizeof(col), ci);
        tt_goto(x+i, y+j);
        tt_write(col);
        tt_write("  ");
        tt_write(TT_RESET);
    }
}

TT_INLINE void tt_pie_chart(int cx, int cy, int radius,
                             const double *slices, int count) {
    static const char *chars[] = {"*","#","@","+","=","-","~","&","%","$"};
    int dy, dx;
    for (dy=-radius;dy<=radius;dy++) for (dx=-radius*2;dx<=radius*2;dx++) {
        double px=dx/2.0, py=dy;
        if (px*px+py*py <= (double)radius*radius) {
            double angle=atan2(py,px);
            if (angle<0) angle+=2.0*TT_PI;
            double accum=0.0, total=0.0; int k, si=0;
            for (k=0;k<count;k++) total+=slices[k];
            if (total<=0) total=1;
            for (k=0;k<count;k++){
                accum+=slices[k]/total*2.0*TT_PI;
                if (angle<=accum){si=k;break;}
            }
            tt_draw(cx+dx, cy+dy, chars[si%10]);
        }
    }
}


#define TT_MULTIBAR_MAX 16

typedef struct {
    const char *label;
    double      value;                   
    int         color;
} TT_BarRow;

typedef struct {
    TT_BarRow rows[TT_MULTIBAR_MAX];
    int       count;
    int       x, y, bar_width;
} TT_MultiBar;

TT_INLINE void tt_multibar_init(TT_MultiBar *mb, int x, int y, int bar_w) {
    mb->count=0; mb->x=x; mb->y=y; mb->bar_width=bar_w;
}

TT_INLINE void tt_multibar_set(TT_MultiBar *mb, int idx,
                                const char *label, double v, int color) {
    if (idx<0||idx>=TT_MULTIBAR_MAX) return;
    mb->rows[idx].label=label; mb->rows[idx].value=v; mb->rows[idx].color=color;
    if (idx>=mb->count) mb->count=idx+1;
}

TT_INLINE void tt_multibar_draw(const TT_MultiBar *mb) {
    int i;
    for (i=0;i<mb->count;i++){
        char col[24]; int pct=(int)(mb->rows[i].value*100.0);
        char pct_str[8];
        tt_snprintf(pct_str, sizeof(pct_str), " %3d%%", pct);
        tt_goto(mb->x, mb->y+i);
        tt_fg(col, sizeof(col), mb->rows[i].color);
        tt_write(col);
        tt_writef("%-12s ", mb->rows[i].label ? mb->rows[i].label : "");
        tt_write(TT_RESET);
        tt_progress(mb->x+14, mb->y+i, mb->bar_width,
                    mb->rows[i].value, 1.0, '#', '.');
        tt_draw(mb->x+14+mb->bar_width, mb->y+i, pct_str);
    }
}


TT_INLINE float tt_synth_sine(double phase)     { return (float)sin(phase * 2.0 * TT_PI); }
TT_INLINE float tt_synth_square(double phase)   { return (phase - (int)phase) < 0.5f ? 1.0f : -1.0f; }
TT_INLINE float tt_synth_saw(double phase)      { return (float)(2.0 * (phase - (int)phase) - 1.0); }
TT_INLINE float tt_synth_tri(double phase)      {
    double p = phase - (int)phase;
    return (float)(p < 0.5 ? 4.0*p - 1.0 : 3.0 - 4.0*p);
}
TT_INLINE float tt_synth_noise(void)            { return (float)(tt_randf() * 2.0 - 1.0); }

typedef struct { double attack, decay, sustain, release; } TT_ADSR;
TT_INLINE double tt_adsr_sample(const TT_ADSR *e, double t, double duration) {
    double note_off = duration - e->release;
    if (t < e->attack)              return t / e->attack;
    if (t < e->attack + e->decay)  return 1.0 - (1.0-e->sustain)*(t-e->attack)/e->decay;
    if (t < note_off)               return e->sustain;
    if (t < duration)               return e->sustain * (1.0 - (t-note_off)/e->release);
    return 0.0;
}

TT_INLINE float tt_synth_fm(double t, double carrier_hz, double mod_hz, double mod_depth) {
    double mod   = sin(2.0*TT_PI*mod_hz*t) * mod_depth;
    return (float)sin(2.0*TT_PI*(carrier_hz+mod)*t);
}

TT_INLINE void tt_synth_tone(TT_Wav *out, double freq, double duration,
                              const TT_ADSR *env, tt_u32 sample_rate) {
    tt_u32 n = (tt_u32)(duration * sample_rate), i;
    out->sample_rate=sample_rate; out->channels=1;
    out->bits_per_sample=16; out->num_samples=n;
    out->data=(float*)malloc(n*sizeof(float));
    if (!out->data) return;
    for (i=0;i<n;i++){
        double t=(double)i/(double)sample_rate;
        double phase=freq*t;
        double amp = env ? tt_adsr_sample(env,t,duration) : 1.0;
        out->data[i]=tt_synth_sine(phase)*(float)amp;
    }
}

TT_INLINE void tt_synth_mix(TT_Wav *dst, const TT_Wav *src, float gain) {
    tt_u32 i, n = dst->num_samples < src->num_samples ? dst->num_samples : src->num_samples;
    for (i=0;i<n;i++) {
        dst->data[i] = tt_clamp_f((double)(dst->data[i] + src->data[i]*gain), -1.0, 1.0);
    }
}

#ifdef TT_ENABLE_SOCKETS

#ifdef TT_WINDOWS
   typedef SOCKET TT_Socket;
#  define TT_INVALID_SOCKET INVALID_SOCKET
#  define TT_SOCKET_ERROR   SOCKET_ERROR
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0501
#  endif
#  if defined(__MINGW32__) && !defined(socklen_t)
     typedef int socklen_t;
#  endif
#else
   typedef int TT_Socket;
#  define TT_INVALID_SOCKET (-1)
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  define TT_SOCKET_ERROR   (-1)
#endif

TT_INLINE int tt_socket_init(void) {
#ifdef TT_WINDOWS
    WSADATA wsa; return WSAStartup(MAKEWORD(2,2),&wsa)==0;
#else
    return 1;
#endif
}

TT_INLINE void tt_socket_cleanup(void) {
#ifdef TT_WINDOWS
    WSACleanup();
#endif
}

TT_INLINE TT_Socket tt_socket_connect(const char *host, int port) {
    struct addrinfo hints, *res;
    char port_s[16];
    TT_Socket fd;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC; hints.ai_socktype=SOCK_STREAM;
    tt_snprintf(port_s, sizeof(port_s), "%d", port);
    if (getaddrinfo(host, port_s, &hints, &res)!=0) return TT_INVALID_SOCKET;
    fd=(TT_Socket)socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (fd==TT_INVALID_SOCKET){freeaddrinfo(res);return TT_INVALID_SOCKET;}
    if (connect(fd, res->ai_addr, (int)res->ai_addrlen)!=0){
#ifdef TT_WINDOWS
        closesocket(fd);
#else
        close(fd);
#endif
        freeaddrinfo(res); return TT_INVALID_SOCKET;
    }
    freeaddrinfo(res); return fd;
}

TT_INLINE TT_Socket tt_socket_listen(int port) {
    struct sockaddr_in addr;
    TT_Socket fd=(TT_Socket)socket(AF_INET,SOCK_STREAM,0);
    int opt=1;
    if (fd==TT_INVALID_SOCKET) return fd;
#ifdef TT_WINDOWS
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const char*)&opt,sizeof(opt));
#else
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
#endif
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET; addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=tt_htobe16((tt_u16)port);
    if (bind(fd,(struct sockaddr*)&addr,sizeof(addr))!=0||listen(fd,8)!=0){
#ifdef TT_WINDOWS
        closesocket(fd);
#else
        close(fd);
#endif
        return TT_INVALID_SOCKET;
    }
    return fd;
}

TT_INLINE TT_Socket tt_socket_accept(TT_Socket srv) {
    struct sockaddr_in ca; int cl=sizeof(ca);
    return (TT_Socket)accept(srv,(struct sockaddr*)&ca,(socklen_t*)&cl);
}

TT_INLINE int tt_socket_send(TT_Socket fd, const void *data, int len) {
#ifdef TT_WINDOWS
    return send(fd,(const char*)data,len,0);
#else
    return (int)send(fd,data,(size_t)len,0);
#endif
}

TT_INLINE int tt_socket_recv(TT_Socket fd, void *buf, int len) {
#ifdef TT_WINDOWS
    return recv(fd,(char*)buf,len,0);
#else
    return (int)recv(fd,buf,(size_t)len,0);
#endif
}

TT_INLINE void tt_socket_close(TT_Socket fd) {
#ifdef TT_WINDOWS
    closesocket(fd);
#else
    close(fd);
#endif
}

#endif

typedef volatile int       TT_AtomicInt;
typedef volatile tt_u32    TT_AtomicU32;
typedef volatile long      TT_AtomicLong;

TT_INLINE int      tt_atomic_load_i(const TT_AtomicInt  *p) { return *p; }
TT_INLINE tt_u32   tt_atomic_load_u(const TT_AtomicU32  *p) { return *p; }
TT_INLINE long     tt_atomic_load_l(const TT_AtomicLong *p) { return *p; }

TT_INLINE void tt_atomic_store_i(TT_AtomicInt  *p, int v)    { *p = v; }
TT_INLINE void tt_atomic_store_u(TT_AtomicU32  *p, tt_u32 v) { *p = v; }
TT_INLINE void tt_atomic_store_l(TT_AtomicLong *p, long v)   { *p = v; }

TT_INLINE int  tt_atomic_inc_i(TT_AtomicInt *p)  { return ++(*p); }
TT_INLINE int  tt_atomic_dec_i(TT_AtomicInt *p)  { return --(*p); }

#if defined(__GNUC__) || defined(__clang__)
#  define TT_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
#elif defined(_MSC_VER)
#  define TT_COMPILER_BARRIER() _ReadWriteBarrier()
#else
#  define TT_COMPILER_BARRIER() ((void)0)
#endif


#define TT_PROF_MAX 64

typedef struct {
    const char *name;
    double      start;
    double      total;
    int         calls;
    int         parent;
} TT_ProfZone;

typedef struct {
    TT_ProfZone zones[TT_PROF_MAX];
    int         count;
    int         cur;                                       
} TT_Profiler;

TT_INLINE void tt_prof_init(TT_Profiler *p) {
    memset(p, 0, sizeof(*p)); p->count=0; p->cur=-1;
}

TT_INLINE void tt_prof_begin(TT_Profiler *p, const char *name) {
    int i;
    for (i=0;i<p->count;i++) if (p->zones[i].name==name) break;
    if (i==p->count && p->count<TT_PROF_MAX) {
        p->zones[i].name=name; p->zones[i].total=0; p->zones[i].calls=0;
        p->zones[i].parent=p->cur;
        p->count++;
    }
    p->zones[i].start=tt_time(); p->zones[i].calls++;
    p->cur=i;
}

TT_INLINE void tt_prof_end(TT_Profiler *p) {
    if (p->cur<0||p->cur>=p->count) return;
    p->zones[p->cur].total += tt_time()-p->zones[p->cur].start;
    p->cur = p->zones[p->cur].parent;
}

TT_INLINE void tt_prof_print(const TT_Profiler *p) {
    int i;
    printf("%-28s %8s %12s %12s\n","Zone","Calls","Total(ms)","Avg(ms)");
    printf("%-28s %8s %12s %12s\n","----","-----","---------","-------");
    for (i=0;i<p->count;i++) {
        double ms=p->zones[i].total*1000.0;
        double avg=p->zones[i].calls?ms/p->zones[i].calls:0.0;
        printf("%-28s %8d %12.3f %12.3f\n",
               p->zones[i].name, p->zones[i].calls, ms, avg);
    }
}


#define TT_STATIC_ASSERT(cond, msg) \
    typedef char tt__static_assert_##msg[(cond)?1:-1]

TT_STATIC_ASSERT(sizeof(tt_u8)==1,  u8_is_1_byte);
TT_STATIC_ASSERT(sizeof(tt_u16)==2, u16_is_2_bytes);
TT_STATIC_ASSERT(sizeof(tt_u32)==4, u32_is_4_bytes);
TT_STATIC_ASSERT(sizeof(tt_u64)==8, u64_is_8_bytes);


typedef enum {
    TT_ANSI_NONE, TT_ANSI_CURSOR_POS, TT_ANSI_CURSOR_UP,
    TT_ANSI_CURSOR_DOWN, TT_ANSI_CURSOR_RIGHT, TT_ANSI_CURSOR_LEFT,
    TT_ANSI_KEY_F1, TT_ANSI_KEY_F2, TT_ANSI_KEY_F3, TT_ANSI_KEY_F4,
    TT_ANSI_KEY_HOME, TT_ANSI_KEY_END, TT_ANSI_KEY_INS, TT_ANSI_KEY_DEL,
    TT_ANSI_KEY_PGUP, TT_ANSI_KEY_PGDN, TT_ANSI_MOUSE,
    TT_ANSI_UNKNOWN
} TT_AnsiEvType;

typedef struct {
    TT_AnsiEvType type;
    int p1, p2;                                                                       
} TT_AnsiEvent;

TT_INLINE int tt_ansi_parse(const char *seq, int len, TT_AnsiEvent *ev) {
    int n=1, p1=0, p2=0;
    memset(ev,0,sizeof(*ev)); ev->type=TT_ANSI_UNKNOWN;
    if (len<2) return 0;
    if (seq[0]=='[') {
        const char *s=seq+1; n=2;
        while (n<len && ((*s>='0'&&*s<='9')||*s==';')) {
            if (*s==';'){p1=p2;p2=0;}
            else { p2=p2*10+(*s-'0'); }
            s++; n++;
        }
        if (n>=len) return 0;
        switch (*s) {
            case 'A': ev->type=TT_ANSI_CURSOR_UP;    ev->p1=p2?p2:1; break;
            case 'B': ev->type=TT_ANSI_CURSOR_DOWN;  ev->p1=p2?p2:1; break;
            case 'C': ev->type=TT_ANSI_CURSOR_RIGHT; ev->p1=p2?p2:1; break;
            case 'D': ev->type=TT_ANSI_CURSOR_LEFT;  ev->p1=p2?p2:1; break;
            case 'H': ev->type=TT_ANSI_CURSOR_POS;   ev->p1=p1;ev->p2=p2; break;
            case 'F': ev->type=TT_ANSI_KEY_END;   break;
            case '~':
                switch (p1?p1:p2) {
                    case 1: case 7: ev->type=TT_ANSI_KEY_HOME; break;
                    case 2: ev->type=TT_ANSI_KEY_INS;  break;
                    case 3: ev->type=TT_ANSI_KEY_DEL;  break;
                    case 5: ev->type=TT_ANSI_KEY_PGUP; break;
                    case 6: ev->type=TT_ANSI_KEY_PGDN; break;
                    case 11:ev->type=TT_ANSI_KEY_F1;   break;
                    case 12:ev->type=TT_ANSI_KEY_F2;   break;
                    case 13:ev->type=TT_ANSI_KEY_F3;   break;
                    case 14:ev->type=TT_ANSI_KEY_F4;   break;
                }
                break;
            case 'M':                
                ev->type=TT_ANSI_MOUSE;
                if (n+2<len){ev->p1=(unsigned char)s[1]-32;ev->p2=(unsigned char)s[2]-32;}
                n+=3; break;
        }
        n++;
    }
    return n;
}


#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
#  if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  else
#    define bool  int
#    define true  1
#    define false 0
#    define __bool_true_false_are_defined 1
#  endif
#endif

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
#  define restrict
#endif

#define TT_FAM 1                                                         




#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#elif defined(__GNUC__)
#  define __func__ __FUNCTION__
#else
#  define __func__ "<unknown>"
#endif


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define TT_ALIGNOF(T) _Alignof(T)
#elif defined(__GNUC__)
#  define TT_ALIGNOF(T) __alignof__(T)
#else
#  define TT_ALIGNOF(T) sizeof(T)                    
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define TT_ALIGNAS(n) _Alignas(n)
#elif defined(__GNUC__)
#  define TT_ALIGNAS(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER)
#  define TT_ALIGNAS(n) __declspec(align(n))
#else
#  define TT_ALIGNAS(n)                  
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#  define TT_NODISCARD  [[nodiscard]]
#  define TT_NORETURN   [[noreturn]]
#elif defined(__GNUC__)
#  define TT_NODISCARD  __attribute__((warn_unused_result))
#  define TT_NORETURN   __attribute__((noreturn))
#elif defined(_MSC_VER)
#  define TT_NODISCARD  _Check_return_
#  define TT_NORETURN   __declspec(noreturn)
#else
#  define TT_NODISCARD
#  define TT_NORETURN
#endif

#if defined(__GNUC__)
#  define TT_LIKELY(x)   __builtin_expect(!!(x),1)
#  define TT_UNLIKELY(x) __builtin_expect(!!(x),0)
#else
#  define TT_LIKELY(x)   (x)
#  define TT_UNLIKELY(x) (x)
#endif

#if defined(__GNUC__)
#  define TT_ATTR_PURE     __attribute__((pure))
#  define TT_ATTR_CONST    __attribute__((const))
#  define TT_ATTR_MALLOC   __attribute__((malloc))
#  define TT_ATTR_FORMAT(f,a,b) __attribute__((format(printf,f,b)))
#else
#  define TT_ATTR_PURE
#  define TT_ATTR_CONST
#  define TT_ATTR_MALLOC
#  define TT_ATTR_FORMAT(f,a,b)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define TT_IS_INT(x)    _Generic((x), int:1, long:1, short:1, default:0)
#  define TT_IS_FLOAT(x)  _Generic((x), float:1, double:1, long double:1, default:0)
#else
#  define TT_IS_INT(x)    (sizeof(x)==sizeof(int)||sizeof(x)==sizeof(long))
#  define TT_IS_FLOAT(x)  (sizeof(x)==sizeof(double))
#endif


TT_INLINE size_t tt_strnlen(const char *s, size_t maxlen) {
    size_t i; for (i=0;i<maxlen&&s[i];i++); return i;
}

TT_INLINE void *tt_memchr(const void *s, int c, size_t n) { return (void*)memchr(s,c,n); }

TT_INLINE void *tt_memmem(const void *haystack, size_t hlen,
                            const void *needle,   size_t nlen) {
    const tt_u8 *h = (const tt_u8*)haystack;
    const tt_u8 *n = (const tt_u8*)needle;
    size_t i;
    if (!nlen) return (void*)haystack;
    if (nlen > hlen) return NULL;
    for (i=0; i<=hlen-nlen; i++)
        if (memcmp(h+i, n, nlen)==0) return (void*)(h+i);
    return NULL;
}

TT_INLINE const char *tt_strnchr_set(const char *s, size_t n, const char *chars) {
    size_t i;
    for (i=0;i<n&&s[i];i++) if (strchr(chars,s[i])) return s+i;
    return NULL;
}

TT_INLINE char *tt_strrepeat(const char *s, int n) {
    size_t sl=strlen(s), i;
    char *out=(char*)malloc(sl*(size_t)n+1);
    if (!out) return NULL;
    for (i=0;i<(size_t)n;i++) memcpy(out+i*sl, s, sl);
    out[sl*(size_t)n]='\0';
    return out;
}

TT_INLINE char *tt_strupr(char *s) { char *p=s; while(*p){*p=(char)toupper((unsigned char)*p);p++;} return s; }
TT_INLINE char *tt_strlwr(char *s) { char *p=s; while(*p){*p=(char)tolower((unsigned char)*p);p++;} return s; }

TT_INLINE void tt_strrev(char *s) {
    int l=0,r=(int)strlen(s)-1; char c;
    while(l<r){c=s[l];s[l++]=s[r];s[r--]=c;}
}

TT_INLINE int tt_strcount(const char *haystack, const char *needle) {
    int c=0; size_t nl=strlen(needle);
    if (!nl) return 0;
    while ((haystack=strstr(haystack,needle))) { c++; haystack+=nl; }
    return c;
}

TT_INLINE int tt_parse_kvpairs(const char *s,
                                char keys[][64], char vals[][256], int max_pairs) {
    int n=0; char *buf=tt_strdup(s), *p=buf, *tok, *buf2;
    if (!buf) return 0;
    tok = tt_strtok_r(p," \t\n",&buf2);
    while (tok&&n<max_pairs){
        char *eq=strchr(tok,'=');
        if (eq){
            int kl=(int)(eq-tok); if(kl>63)kl=63;
            strncpy(keys[n],tok,(size_t)kl); keys[n][kl]='\0';
            strncpy(vals[n],eq+1,255); vals[n][255]='\0';
        } else {
            strncpy(keys[n],tok,63); keys[n][63]='\0'; vals[n][0]='\0';
        }
        n++; tok=tt_strtok_r(NULL," \t\n",&buf2);
    }
    free(buf); return n;
}


TT_INLINE int tt_ilog2(tt_u32 v) {
    int r=0; while(v>1){v>>=1;r++;} return r;
}

TT_INLINE int tt_ilog10(tt_u64 v) {
    int r=0; while(v>=10){v/=10;r++;} return r;
}

TT_INLINE tt_u32 tt_next_pow2(tt_u32 v) {
    if (!v) return 1;
    v--;v|=v>>1;v|=v>>2;v|=v>>4;v|=v>>8;v|=v>>16;
    return v+1;
}

TT_INLINE tt_u32 tt_isqrt32(tt_u32 n) {
    tt_u32 x=n, y=(x+1)/2;
    while(y<x){x=y;y=(x+n/x)/2;}
    return x;
}

TT_INLINE int tt_popcount32(tt_u32 v) {
    v -= (v>>1) & 0x55555555u;
    v  = ((v>>2) & 0x33333333u) + (v & 0x33333333u);
    v  = ((v>>4) + v) & 0x0f0f0f0fu;
    v  = (v * 0x01010101u) >> 24;
    return (int)v;
}
TT_INLINE int tt_popcount64(tt_u64 v) {
    return tt_popcount32((tt_u32)v)+tt_popcount32((tt_u32)(v>>32));
}

TT_INLINE int tt_ctz32(tt_u32 v) {
    int c=0; if(!v) return 32;
    while(!(v&1)){v>>=1;c++;} return c;
}

TT_INLINE int tt_clz32(tt_u32 v) {
    int c=0; if(!v) return 32;
    while(!(v&0x80000000u)){v<<=1;c++;} return c;
}

TT_INLINE tt_u32 tt_bitrev32(tt_u32 v) {
    v=((v>>1)&0x55555555u)|((v&0x55555555u)<<1);
    v=((v>>2)&0x33333333u)|((v&0x33333333u)<<2);
    v=((v>>4)&0x0f0f0f0fu)|((v&0x0f0f0f0fu)<<4);
    v=((v>>8)&0x00ff00ffu)|((v&0x00ff00ffu)<<8);
    return (v>>16)|(v<<16);
}

TT_INLINE tt_i64 tt_modinv(tt_i64 a, tt_i64 m) {
    tt_i64 g=m, x=0, y=1, tmp;
    if (m==1) return 0;
    while (a>1){
        tt_i64 q=a/g; tmp=g; g=a%g; a=tmp; tmp=x; x=y-q*x; y=tmp;
    }
    return y<0?y+m:y;
}

TT_INLINE tt_u64 tt_ipow(tt_u64 base, tt_u64 exp) {
    tt_u64 r=1;
    while(exp>0){if(exp&1)r*=base;base*=base;exp>>=1;}
    return r;
}


TT_INLINE void tt_mempattern(void *dst, size_t n, const void *pattern, size_t plen) {
    tt_u8 *d=(tt_u8*)dst; size_t i;
    for (i=0;i<n;i++) d[i]=((const tt_u8*)pattern)[i%plen];
}

TT_INLINE int tt_membyte_count(const void *data, size_t len, tt_u8 byte) {
    const tt_u8 *p=(const tt_u8*)data; size_t i; int c=0;
    for(i=0;i<len;i++) if(p[i]==byte) c++;
    return c;
}

TT_INLINE void tt_memxor(void *dst, const void *a, const void *b, size_t n) {
    tt_u8 *d=(tt_u8*)dst;
    const tt_u8 *pa=(const tt_u8*)a, *pb=(const tt_u8*)b;
    size_t i; for(i=0;i<n;i++) d[i]=pa[i]^pb[i];
}

TT_INLINE int tt_memcmp_safe(const void *a, const void *b, size_t n) {
    const tt_u8 *pa=(const tt_u8*)a, *pb=(const tt_u8*)b;
    tt_u8 diff=0; size_t i;
    for(i=0;i<n;i++) diff|=pa[i]^pb[i];
    return diff!=0;
}


typedef enum {
    TT_JSON_NONE, TT_JSON_OBJ_BEGIN, TT_JSON_OBJ_END,
    TT_JSON_ARR_BEGIN, TT_JSON_ARR_END,
    TT_JSON_STRING, TT_JSON_NUMBER, TT_JSON_TRUE, TT_JSON_FALSE,
    TT_JSON_NULL, TT_JSON_COLON, TT_JSON_COMMA, TT_JSON_END, TT_JSON_ERROR
} TT_JsonTokType;

typedef struct {
    TT_JsonTokType type;
    const char    *start;
    int            len;
} TT_JsonTok;

typedef struct { const char *p; const char *end; } TT_JsonIter;

TT_INLINE void tt_json_iter_init(TT_JsonIter *it, const char *json, size_t len) {
    it->p = json; it->end = json + len;
}

TT_INLINE TT_JsonTok tt_json_next(TT_JsonIter *it) {
    TT_JsonTok tok; tok.type=TT_JSON_NONE; tok.start=it->p; tok.len=0;
    while (it->p<it->end && isspace((unsigned char)*it->p)) it->p++;
    if (it->p>=it->end){tok.type=TT_JSON_END;return tok;}
    tok.start=it->p;
    switch (*it->p) {
        case '{': tok.type=TT_JSON_OBJ_BEGIN; it->p++; break;
        case '}': tok.type=TT_JSON_OBJ_END;   it->p++; break;
        case '[': tok.type=TT_JSON_ARR_BEGIN;  it->p++; break;
        case ']': tok.type=TT_JSON_ARR_END;   it->p++; break;
        case ':': tok.type=TT_JSON_COLON;     it->p++; break;
        case ',': tok.type=TT_JSON_COMMA;     it->p++; break;
        case '"': {
            it->p++;
            tok.start=it->p;
            while(it->p<it->end&&*it->p!='"'){
                if(*it->p=='\\')it->p++;
                it->p++;
            }
            tok.len=(int)(it->p-tok.start);
            tok.type=TT_JSON_STRING;
            if(it->p<it->end)it->p++;
            break;
        }
        case 't':
            tok.type=TT_JSON_TRUE;
            while(it->p<it->end&&isalpha((unsigned char)*it->p))it->p++;
            break;
        case 'f':
            tok.type=TT_JSON_FALSE;
            while(it->p<it->end&&isalpha((unsigned char)*it->p))it->p++;
            break;
        case 'n':
            tok.type=TT_JSON_NULL;
            while(it->p<it->end&&isalpha((unsigned char)*it->p))it->p++;
            break;
        default:
            if(*it->p=='-'||isdigit((unsigned char)*it->p)){
                tok.type=TT_JSON_NUMBER;
                while(it->p<it->end&&(isdigit((unsigned char)*it->p)||
                      *it->p=='.'||*it->p=='e'||*it->p=='E'||
                      *it->p=='+'||*it->p=='-'))it->p++;
            } else { tok.type=TT_JSON_ERROR; it->p++; }
            break;
    }
    if (!tok.len) tok.len=(int)(it->p-tok.start);
    return tok;
}


static int tt__re_match(const char *pat, const char *s);
static int tt__re_match_bracket(const char **pat, char c) {
    int neg=0, matched=0;
    (*pat)++;               
    if (**pat=='^'){neg=1;(*pat)++;}
    while (**pat&&**pat!=']'){
        if (*((*pat)+1)=='-'&&*((*pat)+2)&&*((*pat)+2)!=']'){
            if(c>=(unsigned char)**pat&&c<=(unsigned char)*((*pat)+2)) matched=1;
            (*pat)+=3;
        } else { if(c==(unsigned char)**pat) matched=1; (*pat)++; }
    }
    if (**pat==']') (*pat)++;
    return matched!=neg;
}

static int tt__re_match(const char *pat, const char *s) {
    if (!*pat) return 1;
    if (*pat=='$'&&!*(pat+1)) return !*s;
    if (*pat=='^') return tt__re_match(pat+1, s);
    if (*pat=='[') {
        const char *p2=pat;
        int matched2 = *s ? tt__re_match_bracket(&p2, (unsigned char)*s) : 0;
        if (*(p2)=='*') {
            if (tt__re_match(p2+1,s)) return 1;
            if (matched2&&tt__re_match(pat,s+1)) return 1;
            return 0;
        }
        if (*(p2)=='+') {
            if (!matched2) return 0;
            return tt__re_match(p2+1,s+1)||tt__re_match(pat,s+1);
        }
        if (*(p2)=='?') {
            if (matched2&&tt__re_match(p2+1,s+1)) return 1;
            return tt__re_match(p2+1,s);
        }
        return matched2&&tt__re_match(p2,s+1);
    }
    int dot_match = *s && (*pat=='.'||*pat==(unsigned char)*s);
    if (*(pat+1)=='*') {
        if (tt__re_match(pat+2,s)) return 1;
        if (dot_match&&tt__re_match(pat,s+1)) return 1;
        return 0;
    }
    if (*(pat+1)=='+') {
        if (!dot_match) return 0;
        return tt__re_match(pat+2,s+1)||tt__re_match(pat,s+1);
    }
    if (*(pat+1)=='?') {
        if (dot_match&&tt__re_match(pat+2,s+1)) return 1;
        return tt__re_match(pat+2,s);
    }
    return dot_match&&tt__re_match(pat+1,s+1);
}

TT_INLINE int tt_regex_match(const char *pattern, const char *s) {
    if (*pattern=='^') return tt__re_match(pattern,s);
    do { if (tt__re_match(pattern,s)) return 1; } while (*s++);
    return 0;
}


TT_INLINE void tt_scanlines(int x, int y, int w, int h) {
    int j;
    for (j=y; j<y+h; j+=2) tt_fill(x, j, w, 1, '\xb7');                 
}

TT_INLINE void tt_matrix_rain(int col_count) {
    static char tt__mrain_pos[256];
    static int  tt__mrain_init = 0;
    int i;
    TT_Size sz = tt_term_size();
    if (!tt__mrain_init) {
        for (i=0;i<256;i++) tt__mrain_pos[i]=(char)tt_rand_range(0,sz.h);
        tt__mrain_init=1;
    }
    for (i=0;i<col_count&&i<sz.w;i++){
        char ch[2];
        ch[0]=(char)tt_rand_range('!','~'); ch[1]='\0';
        tt_draw(i+1, (int)(unsigned char)tt__mrain_pos[i], "\033[32m");
        tt_write(ch);
        tt_write(TT_RESET);
        tt__mrain_pos[i]=(char)((tt__mrain_pos[i]+1)%sz.h);
    }
}

TT_INLINE void tt_crt_cursor(int x, int y, int visible) {
    tt_draw(x, y, visible ? "\033[32m_\033[0m" : " ");
}


TT_INLINE void tt_print_sysinfo(void) {
    const char *platform =
#if defined(TT_WINDOWS)
        "Windows";
#elif defined(TT_LINUX)
        "Linux";
#elif defined(TT_MACOS)
        "macOS";
#elif defined(TT_BSD)
        "BSD";
#elif defined(TT_DOS)
        "DOS";
#else
        "Unknown";
#endif
    printf("termtools.h extended -- build info:\n");
#if defined(__GNUC__)
    printf("  Compiler : GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    printf("  Compiler : MSVC %d\n", _MSC_VER);
#elif defined(__clang__)
    printf("  Compiler : Clang %d.%d\n", __clang_major__, __clang_minor__);
#endif
#if defined(__STDC_VERSION__)
    printf("  C std    : %ld\n", (long)__STDC_VERSION__);
#else
    printf("  C std    : C89\n");
#endif
    printf("  Platform : %s\n", platform);
    printf("  Endian   : %s\n", tt_is_little_endian() ? "little" : "big");
    printf("  sizeof(int): %d  sizeof(long): %d  sizeof(void*): %d\n",
           (int)sizeof(int), (int)sizeof(long), (int)sizeof(void*));
}

#endif                  

#ifdef TERMTOOLS_IMPLEMENTATION
#endif
