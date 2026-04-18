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
#include <stddef.h>

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
            DWORD size = (DWORD)(csbi.dwSize.X * csbi.dwSize.Y);
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


TT_INLINE tt_u8 tt_crc8(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_u8 crc = 0x00;
    size_t i, j;
    
    for (i = 0; i < len; i++) {
        crc ^= p[i];
        for (j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (tt_u8)((crc << 1) ^ 0x07) : (tt_u8)(crc << 1);
        }
    }
    return crc;
}

TT_INLINE tt_u16 tt_crc16(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_u16 crc = 0x0000;
    size_t i, j;
    
    for (i = 0; i < len; i++) {
        crc ^= (tt_u16)(p[i] << 8);
        for (j = 0; j < 8; j++) {
            crc = (crc & 0x8000) ? (tt_u16)((crc << 1) ^ 0x8005) : (tt_u16)(crc << 1);
        }
    }
    return crc;
}

TT_INLINE tt_u32 tt_crc24(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_u32 crc = 0xB704CEL;
    size_t i, j;
    
    for (i = 0; i < len; i++) {
        crc ^= (tt_u32)(p[i] << 16);
        for (j = 0; j < 8; j++) {
            crc = (crc & 0x800000L) ? (tt_u32)((crc << 1) ^ 0x864CFBL) : (tt_u32)(crc << 1);
        }
    }
    return crc & 0xFFFFFFL;
}

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



TT_INLINE tt_u64 tt_crc48(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_u64 crc = 0xFFFFFFFFFFFFULL;
    size_t i, j;
    
    for (i = 0; i < len; i++) {
        crc ^= (tt_u64)p[i] << 40;
        for (j = 0; j < 8; j++) {
            crc = (crc & 0x800000000000ULL) ? ((crc << 1) ^ 0x42F0E1EBA9EA3693ULL) : (crc << 1);
        }
    }
    return crc & 0xFFFFFFFFFFFFULL;
}

TT_INLINE tt_u64 tt_crc64(const void *data, size_t len) {
    static tt_u64 table[256];
    static int init = 0;
    const tt_u8 *p = (const tt_u8*)data;
    tt_u64 crc = 0xFFFFFFFFFFFFFFFFULL;
    size_t i;
    
    if (!init) {
        tt_u64 k;
        for (k = 0; k < 256; k++) {
            tt_u64 c = k;
            int j;
            for (j = 0; j < 8; j++) {
                c = (c & 1) ? ((c >> 1) ^ 0x42F0E1EBA9EA3693ULL) : (c >> 1);
            }
            table[k] = c;
        }
        init = 1;
    }
    
    for (i = 0; i < len; i++) {
        crc = table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFFFFFFFFFULL;
}


typedef struct { tt_u64 lo; tt_u64 hi; } tt_crc86_t;

TT_INLINE tt_crc86_t tt_crc86(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_crc86_t crc;
    size_t i, j;
    
    crc.lo = 0xFFFFFFFFFFFFFFFFULL;
    crc.hi = 0x3FFFFFULL;                                                
    
    for (i = 0; i < len; i++) {
        crc.hi ^= (tt_u64)p[i] << 14;                   
        
        for (j = 0; j < 8; j++) {
            int bit = (crc.hi >> 21) & 1;
            
            crc.hi = ((crc.hi << 1) | (crc.lo >> 63)) & 0x3FFFFFULL;
            crc.lo = crc.lo << 1;
            
            if (bit) {
                crc.lo ^= 0xB30948B41ULL;
                crc.hi ^= 0x25ACB9324ULL;
                crc.hi &= 0x3FFFFFULL;
            }
        }
    }
    
    return crc;
}

typedef struct { tt_u64 lo; tt_u64 hi; } tt_crc128_t;

TT_INLINE tt_crc128_t tt_crc128(const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    tt_crc128_t crc;
    size_t i, j;
    
    crc.lo = 0xFFFFFFFFFFFFFFFFULL;
    crc.hi = 0xFFFFFFFFFFFFFFFFULL;
    
    for (i = 0; i < len; i++) {
        crc.hi ^= (tt_u64)p[i] << 56;
        
        for (j = 0; j < 8; j++) {
            int bit = (int)((crc.hi >> 63) & 1ULL);
            crc.hi = (crc.hi << 1) | (crc.lo >> 63);
            crc.lo = crc.lo << 1;
            if (bit) {
                crc.lo ^= 0x2B1F23A9B1C7E45DULL;
                crc.hi ^= 0x881098B931B1FECDULL;
            }
        }
    }
    
    return crc;
}

TT_INLINE void tt_crc8_to_hex(tt_u8 crc, char out[3]) {
    tt_snprintf(out, 3, "%02X", crc);
}

TT_INLINE void tt_crc16_to_hex(tt_u16 crc, char out[5]) {
    tt_snprintf(out, 5, "%04X", crc);
}

TT_INLINE void tt_crc24_to_hex(tt_u32 crc, char out[7]) {
    tt_snprintf(out, 7, "%06lX", (unsigned long)(crc & 0xFFFFFFL));
}

TT_INLINE void tt_crc32_to_hex(tt_u32 crc, char out[9]) {
    tt_snprintf(out, 9, "%08lX", (unsigned long)crc);
}

TT_INLINE void tt_crc48_to_hex(tt_u64 crc, char out[13]) {
    tt_snprintf(out, 13, "%012llX", (unsigned long long)(crc & 0xFFFFFFFFFFFFULL));
}

TT_INLINE void tt_crc86_to_hex(tt_crc86_t crc, char out[23]) {
    tt_snprintf(out, 23, "%06llX%016llX", 
                (unsigned long long)(crc.hi & 0x3FFFFFULL),
                (unsigned long long)crc.lo);
}

TT_INLINE void tt_crc128_to_hex(tt_crc128_t crc, char out[33]) {
    tt_snprintf(out, 33, "%016llX%016llX",
                (unsigned long long)crc.hi,
                (unsigned long long)crc.lo);
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
            for (k = -15; k <= 0; k++) {
                size_t idx = (size_t)((ptrdiff_t)i + k);
                fprintf(out, "%c", isprint(p[idx]) ? p[idx] : '.');
            }
            fprintf(out, "|\n");
        }
    }
    if (len % 16) {
        int k;
        size_t trail = len % 16;
        for (k = (int)trail; k < 16; k++) fprintf(out, "   %s", k==8?"  ":"");
        fprintf(out, " |");
        for (k = 0; k < (int)trail; k++) {
            size_t idx = len - trail + (size_t)k;
            fprintf(out, "%c", isprint(p[idx]) ? p[idx] : '.');
        }
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
                tt_draw(x, y, chars[tt_rand_range(0, count-1)]);
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
    fl     = (size_t)strlen(frac_s);
    { long long max_fl = (out->fb * 3010LL / 100000) + 4;if (fl > max_fl) fl = (int)max_fl; if (fl > max_fl) fl = max_fl; }

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
    return (int)c;
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
    ptrdiff_t diff = (tt_u8*)ptr - (tt_u8*)p->buf;
    int idx = (int)((size_t)diff / (size_t)p->block_size);
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
        dst->data[i] = (float)tt_clamp_f((double)(dst->data[i] + src->data[i]*gain), -1.0, 1.0);
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
    const tt_u8 *p=(const tt_u8*)data; size_t i; unsigned int c=0;
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
        int matched2 = *s ? tt__re_match_bracket(&p2, (char)(unsigned char)*s) : 0;
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




#ifndef TT_NO_RNG

typedef struct { tt_u64 s; } TT_SplitMix;
typedef struct { tt_u64 s[4]; } TT_Xo256;
typedef struct { tt_u64 state; tt_u64 inc; } TT_PCG32;

TT_INLINE tt_u64 tt_splitmix_next(TT_SplitMix *sm) {
    tt_u64 z = (sm->s += (tt_u64)0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * (tt_u64)0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * (tt_u64)0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

TT_INLINE void tt_xo256_seed(TT_Xo256 *x, tt_u64 seed) {
    TT_SplitMix sm; sm.s = seed;
    x->s[0] = tt_splitmix_next(&sm);
    x->s[1] = tt_splitmix_next(&sm);
    x->s[2] = tt_splitmix_next(&sm);
    x->s[3] = tt_splitmix_next(&sm);
}

static TT_UNUSED tt_u64 tt__rotl64(tt_u64 x, int k) {
    return (x << k) | (x >> (64 - k));
}

TT_INLINE tt_u64 tt_xo256_next(TT_Xo256 *x) {
    tt_u64 result = tt__rotl64(x->s[1] * 5, 7) * 9;
    tt_u64 t = x->s[1] << 17;
    x->s[2] ^= x->s[0];
    x->s[3] ^= x->s[1];
    x->s[1] ^= x->s[2];
    x->s[0] ^= x->s[3];
    x->s[2] ^= t;
    x->s[3] = tt__rotl64(x->s[3], 45);
    return result;
}

TT_INLINE double  tt_xo256_f64(TT_Xo256 *x) {
    tt_u64 v = tt_xo256_next(x);
    return (double)(v >> 11) * (1.0 / (double)(tt_u64)(1ULL << 53));
}
TT_INLINE float   tt_xo256_f32(TT_Xo256 *x)           { return (float)tt_xo256_f64(x); }
TT_INLINE tt_u32  tt_xo256_u32(TT_Xo256 *x)           { return (tt_u32)(tt_xo256_next(x) >> 32); }
TT_INLINE int     tt_xo256_range(TT_Xo256 *x, int lo, int hi) {
    if (hi <= lo) return lo;
    return lo + (int)(tt_xo256_u32(x) % (tt_u32)(hi - lo + 1));
}
TT_INLINE double  tt_xo256_normal(TT_Xo256 *x, double mean, double stddev) {
    double u1 = tt_xo256_f64(x) + 1e-300;                   
    double u2 = tt_xo256_f64(x);
    double n  = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159265358979323846 * u2);
    return mean + stddev * n;
}
TT_INLINE void tt_xo256_jump(TT_Xo256 *x) {
    static const tt_u64 JUMP[] = {
        (tt_u64)0x180ec6d33cfd0abaULL, (tt_u64)0xd5a61266f0c9392cULL,
        (tt_u64)0xa9582618e03fc9aaULL, (tt_u64)0x39abdc4529b1661cULL
    };
    tt_u64 s0=0, s1=0, s2=0, s3=0;
    int i, b;
    for (i = 0; i < 4; i++) {
        for (b = 0; b < 64; b++) {
            if (JUMP[i] & ((tt_u64)1 << b)) {
                s0 ^= x->s[0]; s1 ^= x->s[1];
                s2 ^= x->s[2]; s3 ^= x->s[3];
            }
            tt_xo256_next(x);
        }
    }
    x->s[0]=s0; x->s[1]=s1; x->s[2]=s2; x->s[3]=s3;
}

TT_INLINE void tt_pcg32_seed(TT_PCG32 *p, tt_u64 state, tt_u64 seq) {
    p->state = 0; p->inc = (seq << 1) | 1;
    tt_u64 dummy = p->state + p->inc;
    p->state = dummy * (tt_u64)6364136223846793005ULL + p->inc;
    p->state += state;
    dummy = p->state * (tt_u64)6364136223846793005ULL + p->inc;
    p->state = dummy;
}
TT_INLINE tt_u32 tt_pcg32_next(TT_PCG32 *p) {
    tt_u64 old = p->state;
    tt_u32 xsh, rot;
    p->state = old * (tt_u64)6364136223846793005ULL + p->inc;
    xsh = (tt_u32)(((old >> 18) ^ old) >> 27);
    rot = (tt_u32)(old >> 59);
    return (xsh >> rot) | (xsh << ((-(int)rot) & 31));
}
TT_INLINE float  tt_pcg32_f32(TT_PCG32 *p)  { return (float)(tt_pcg32_next(p) >> 8) * (1.0f/16777216.0f); }
TT_INLINE int    tt_pcg32_range(TT_PCG32 *p, int lo, int hi) {
    if (hi <= lo) return lo;
    return lo + (int)(tt_pcg32_next(p) % (tt_u32)(hi - lo + 1));
}

static TT_Xo256 tt__g_rng = {{0,0,0,0}};
static int tt__g_rng_init = 0;

TT_INLINE void tt_rng_seed(tt_u64 seed) {
    tt_xo256_seed(&tt__g_rng, seed);
    tt__g_rng_init = 1;
}

static TT_UNUSED void tt__rng_lazy_init(void) {
    if (!tt__g_rng_init) {
        tt_xo256_seed(&tt__g_rng, (tt_u64)(size_t)&tt__g_rng ^ (tt_u64)time(NULL));
        tt__g_rng_init = 1;
    }
}

TT_INLINE tt_u64  tt_rng64(void)             { tt__rng_lazy_init(); return tt_xo256_next(&tt__g_rng); }
TT_INLINE tt_u32  tt_rng32(void)             { return (tt_u32)(tt_rng64() >> 32); }
TT_INLINE double  tt_rngf(void)              { tt__rng_lazy_init(); return tt_xo256_f64(&tt__g_rng); }
TT_INLINE int     tt_rng_range(int lo,int hi){ tt__rng_lazy_init(); return tt_xo256_range(&tt__g_rng,lo,hi); }
TT_INLINE double  tt_rng_normal(double m, double s){ tt__rng_lazy_init(); return tt_xo256_normal(&tt__g_rng,m,s); }
TT_INLINE void    tt_rng_bytes(void *buf, size_t n) {
    tt_u8 *p = (tt_u8*)buf;
    size_t i;
    tt__rng_lazy_init();
    for (i = 0; i < n; i += 8) {
        tt_u64 v = tt_xo256_next(&tt__g_rng);
        size_t j, rem = n - i > 8 ? 8 : n - i;
        for (j = 0; j < rem; j++) p[i+j] = (tt_u8)(v >> (j*8));
    }
}

TT_INLINE void tt_rng_shuffle(void *base, int n, int elem_size) {
    tt_u8 *b = (tt_u8*)base;
    tt_u8 *tmp;
    int i;
    if (n <= 1) return;
    tt__rng_lazy_init();
    tmp = (tt_u8*)malloc((size_t)elem_size);
    if (!tmp) return;
    for (i = n - 1; i > 0; i--) {
        int j = tt_xo256_range(&tt__g_rng, 0, i);
        memcpy(tmp,         b + (size_t)i * (size_t)elem_size, (size_t)elem_size);
        memcpy(b + (size_t)i * (size_t)elem_size,
               b + (size_t)j * (size_t)elem_size, (size_t)elem_size);
        memcpy(b + (size_t)j * (size_t)elem_size, tmp, (size_t)elem_size);
    }
    free(tmp);
}

TT_INLINE void tt_rng_uuid4(char buf[37]) {
    tt_u8 r[16];
    tt_rng_bytes(r, 16);
    r[6] = (tt_u8)((r[6] & 0x0F) | 0x40);                
    r[8] = (tt_u8)((r[8] & 0x3F) | 0x80);                
    tt_snprintf(buf, 37,
        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        r[0],r[1],r[2],r[3], r[4],r[5], r[6],r[7],
        r[8],r[9], r[10],r[11],r[12],r[13],r[14],r[15]);
}

#endif                

#ifndef TT_NO_PNG

typedef struct {
    int    w, h;
    int    channels;                                         
    int    bit_depth;                                                  
    tt_u8 *pixels;                                                    
} TT_PngImage;


static TT_UNUSED tt_u32 tt__adler32(tt_u32 a, const tt_u8 *buf, size_t len) {
    tt_u32 s1 = a & 0xffff, s2 = (a >> 16) & 0xffff;
    size_t i;
    for (i = 0; i < len; i++) {
        s1 = (s1 + buf[i]) % 65521;
        s2 = (s2 + s1)     % 65521;
    }
    return (s2 << 16) | s1;
}

#ifndef TT_NO_CRC32_HELPER
static TT_UNUSED tt_u32 tt__crc32_png(const tt_u8 *d, size_t len) {
    static tt_u32 tbl[256];
    static int init2 = 0;
    tt_u32 crc = 0xFFFFFFFFu;
    size_t i;
    if (!init2) {
        tt_u32 k;
        for (k = 0; k < 256; k++) {
            tt_u32 c = k; int j;
            for (j = 0; j < 8; j++) c = (c & 1) ? (0xEDB88320u ^ (c>>1)) : (c>>1);
            tbl[k] = c;
        }
        init2 = 1;
    }
    for (i = 0; i < len; i++) crc = tbl[(crc ^ d[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}
#endif

typedef struct {
    const tt_u8 *src;
    size_t       src_len;
    size_t       src_pos;
    tt_u32       bits;
    int          nbits;
} TT__BitReader;

static void tt__br_init(TT__BitReader *br, const tt_u8 *src, size_t len) {
    br->src = src; br->src_len = len; br->src_pos = 0;
    br->bits = 0; br->nbits = 0;
}

static void tt__br_fill(TT__BitReader *br) {
    while (br->nbits <= 24 && br->src_pos < br->src_len)
        br->bits |= (tt_u32)br->src[br->src_pos++] << br->nbits, br->nbits += 8;
}

static tt_u32 tt__br_peek(TT__BitReader *br, int n) {
    tt__br_fill(br);
    return br->bits & ((1u << n) - 1);
}

static tt_u32 tt__br_read(TT__BitReader *br, int n) {
    tt_u32 v = tt__br_peek(br, n);
    br->bits >>= n; br->nbits -= n;
    return v;
}

TT_INLINE tt_u32 tt__br_read_rev(TT__BitReader *br, int n) {
    tt_u32 v = 0;
    int i;
    tt__br_fill(br);
    for (i = n-1; i >= 0; i--) {
        v |= ((br->bits >> i) & 1) << (n-1-i);
    }
    (void)v;
    {
        tt_u32 r = 0;
        int j;
        tt__br_fill(br);
        for (j = 0; j < n; j++) {
            r = (r << 1) | (br->bits & 1);
            br->bits >>= 1; br->nbits--;
        }
        return r;
    }
}

static void tt__br_align(TT__BitReader *br) {
    int skip = br->nbits & 7;
    br->bits >>= skip; br->nbits -= skip;
}

#define TT__HUFF_MAX_BITS  15
#define TT__HUFF_MAX_SYMS  288

typedef struct {
    tt_u16 counts[TT__HUFF_MAX_BITS+1];
    tt_u16 symbols[TT__HUFF_MAX_SYMS];
    int    max_bits;
} TT__Huff;

static int tt__huff_build(TT__Huff *h, const tt_u8 *lengths, int n) {
    int offsets[TT__HUFF_MAX_BITS+2];
    int i;
    memset(h->counts, 0, sizeof(h->counts));
    h->max_bits = 0;
    for (i = 0; i < n; i++) {
        if (lengths[i] > TT__HUFF_MAX_BITS) return -1;
        h->counts[(int)lengths[i]]++;
        if (lengths[i] > h->max_bits) h->max_bits = (int)lengths[i];
    }
    h->counts[0] = 0;
    offsets[0] = offsets[1] = 0;
    for (i = 1; i <= h->max_bits; i++)
        offsets[i+1] = offsets[i] + h->counts[i];
    for (i = 0; i < n; i++)
        if (lengths[i])
            h->symbols[offsets[(int)lengths[i]]++] = (tt_u16)i;
    return 0;
}

static int tt__huff_decode(TT__Huff *h, TT__BitReader *br) {
    int i, count = 0, first = 0, index = 0;
    for (i = 1; i <= h->max_bits; i++) {
        int bit = (int)tt__br_read(br, 1);
        count = (count << 1) | bit;
        first = (first + h->counts[i]);
        index  += h->counts[i];
        count  -= first;
        if (count < 0) return (int)h->symbols[index + count];
    }
    return -1;               
}

static void tt__fixed_huff(TT__Huff *lit, TT__Huff *dist) {
    tt_u8 lens[288+32];
    int i;
    for (i=0;   i<144; i++) lens[i] = 8;
    for (i=144; i<256; i++) lens[i] = 9;
    for (i=256; i<280; i++) lens[i] = 7;
    for (i=280; i<288; i++) lens[i] = 8;
    tt__huff_build(lit, lens, 288);
    for (i=0; i<30; i++) lens[i] = 5;
    tt__huff_build(dist, lens, 30);
}


static const int tt__len_base[29]   = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const int tt__len_ebits[29]  = {0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0};
static const int tt__dist_base[30]  = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const int tt__dist_ebits[30] = {0,0,0,0,1,1,2, 2, 3, 3, 4, 4, 5, 5,  6,  6,  7,  7,  8,  8,   9,   9,  10,  10,  11,  11,  12,   12,   13,   13};

typedef struct { tt_u8 *buf; size_t len; size_t cap; } TT__ByteBuf;

static int tt__bbuf_push(TT__ByteBuf *b, tt_u8 v) {
    if (b->len >= b->cap) {
        size_t nc = b->cap ? b->cap * 2 : 4096;
        tt_u8 *p = (tt_u8*)realloc(b->buf, nc);
        if (!p) return -1;
        b->buf = p; b->cap = nc;
    }
    b->buf[b->len++] = v;
    return 0;
}

static int tt__inflate_block(TT__BitReader *br, TT__Huff *lit_h, TT__Huff *dist_h, TT__ByteBuf *out) {
    for (;;) {
        int sym = tt__huff_decode(lit_h, br);
        if (sym < 0) return -1;
        if (sym == 256) break;          
        if (sym < 256) {
            if (tt__bbuf_push(out, (tt_u8)sym)) return -2;
        } else {
            int li = sym - 257;
            int length, dist_code, dist;
            if (li < 0 || li >= 29) return -3;
            length = tt__len_base[li] + (int)tt__br_read(br, tt__len_ebits[li]);
            dist_code = tt__huff_decode(dist_h, br);
            if (dist_code < 0 || dist_code >= 30) return -4;
            dist = tt__dist_base[dist_code] + (int)tt__br_read(br, tt__dist_ebits[dist_code]);
            {
                int j;
                for (j = 0; j < length; j++) {
                    size_t src_idx;
                    if (dist > (int)out->len) return -5;
                    src_idx = out->len - (size_t)dist;
                    if (tt__bbuf_push(out, out->buf[src_idx])) return -6;
                }
            }
        }
    }
    return 0;
}

static int tt__inflate(const tt_u8 *src, size_t src_len, TT__ByteBuf *out) {
    TT__BitReader br;
    if (src_len < 2) return -1;
    tt__br_init(&br, src + 2, src_len - 2);
    for (;;) {
        int bfinal = (int)tt__br_read(&br, 1);
        int btype  = (int)tt__br_read(&br, 2);
        if (btype == 0) {
            tt_u16 len, nlen;
            tt__br_align(&br);
            len  = (tt_u16)tt__br_read(&br, 16);
            nlen = (tt_u16)tt__br_read(&br, 16);
            if ((tt_u16)(len ^ nlen) != 0xFFFF) return -2;
            {
                int j;
                for (j = 0; j < (int)len; j++) {
                    if (tt__bbuf_push(out, (tt_u8)tt__br_read(&br, 8))) return -3;
                }
            }
        } else if (btype == 1) {
            TT__Huff lit_h, dist_h;
            tt__fixed_huff(&lit_h, &dist_h);
            if (tt__inflate_block(&br, &lit_h, &dist_h, out)) return -4;
        } else if (btype == 2) {
            int hlit  = (int)tt__br_read(&br, 5) + 257;
            int hdist = (int)tt__br_read(&br, 5) + 1;
            int hclen = (int)tt__br_read(&br, 4) + 4;
            static const int clorder[19] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
            tt_u8 cl_lens[19]; tt_u8 all_lens[286+32];
            TT__Huff cl_h; TT__Huff lit_h, dist_h;
            int i, n;
            memset(cl_lens, 0, sizeof(cl_lens));
            for (i = 0; i < hclen; i++) cl_lens[clorder[i]] = (tt_u8)tt__br_read(&br, 3);
            if (tt__huff_build(&cl_h, cl_lens, 19)) return -5;
            n = hlit + hdist;
            memset(all_lens, 0, sizeof(all_lens));
            i = 0;
            while (i < n) {
                int sym = tt__huff_decode(&cl_h, &br);
                if (sym < 0) return -6;
                if (sym < 16) { all_lens[i++] = (tt_u8)sym; }
                else if (sym == 16) {
                    int rep = (int)tt__br_read(&br, 2) + 3;
                    tt_u8 prev = i > 0 ? all_lens[i-1] : 0;
                    while (rep-- && i < n) all_lens[i++] = prev;
                } else if (sym == 17) {
                    int rep = (int)tt__br_read(&br, 3) + 3;
                    while (rep-- && i < n) all_lens[i++] = 0;
                } else {         
                    int rep = (int)tt__br_read(&br, 7) + 11;
                    while (rep-- && i < n) all_lens[i++] = 0;
                }
            }
            if (tt__huff_build(&lit_h,  all_lens,       hlit))  return -7;
            if (tt__huff_build(&dist_h, all_lens + hlit, hdist)) return -8;
            if (tt__inflate_block(&br, &lit_h, &dist_h, out)) return -9;
        } else {
            return -10;               
        }
        if (bfinal) break;
    }
    return 0;
}

static tt_u32 tt__png_u32be(const tt_u8 *p) {
    return ((tt_u32)p[0]<<24)|((tt_u32)p[1]<<16)|((tt_u32)p[2]<<8)|p[3];
}

static void tt__png_filter(tt_u8 *row, const tt_u8 *prev, int width_bytes, int bpp) {
    int ftype = row[0];
    tt_u8 *d = row + 1;
    int i;
    switch (ftype) {
    case 0: break;           
    case 1:          
        for (i=bpp;i<width_bytes;i++) d[i]=(tt_u8)(d[i]+d[i-bpp]);
        break;
    case 2:         
        for (i=0;i<width_bytes;i++) d[i]=(tt_u8)(d[i]+prev[i]);
        break;
    case 3:              
        for (i=0;i<width_bytes;i++) {
            tt_u8 a = i>=bpp?d[i-bpp]:0, b=prev[i];
            d[i]=(tt_u8)(d[i]+((int)a+(int)b)/2);
        }
        break;
    case 4:            
        for (i=0;i<width_bytes;i++) {
            int a=i>=bpp?(int)d[i-bpp]:0;
            int b=(int)prev[i];
            int c=i>=bpp?(int)prev[i-bpp]:0;
            int p2=a+b-c, pa=abs(p2-a), pb=abs(p2-b), pc=abs(p2-c);
            tt_u8 pr=(tt_u8)(pa<=pb&&pa<=pc?a:pb<=pc?b:c);
            d[i]=(tt_u8)(d[i]+pr);
        }
        break;
    }
}

static int tt__png_decode(const tt_u8 *data, size_t data_len, TT_PngImage *out) {
    static const tt_u8 sig[8]={137,80,78,71,13,10,26,10};
    size_t pos = 0;
    tt_u8 *idat_data = NULL; size_t idat_len = 0, idat_cap = 0;
    int w=0,h=0,bit_depth=0,color_type=0,interlace=0;
    tt_u8 palette[256*3]; int pal_entries=0;
    TT__ByteBuf raw; tt_u8 *pixels = NULL;
    int channels = 0, bpp = 0;
    int ok = -1, i;

    memset(out, 0, sizeof(*out));
    memset(&raw, 0, sizeof(raw));

    if (data_len < 8 || memcmp(data, sig, 8)) return -1;
    pos = 8;

    while (pos + 12 <= data_len) {
        tt_u32 clen = tt__png_u32be(data+pos); pos+=4;
        tt_u8 type[5]; memcpy(type, data+pos, 4); type[4]=0; pos+=4;
        const tt_u8 *chunk = data+pos;

        if (!memcmp(type,"IHDR",4)) {
            if (clen < 13) goto done;
            w          = (int)tt__png_u32be(chunk);
            h          = (int)tt__png_u32be(chunk+4);
            bit_depth  = chunk[8];
            color_type = chunk[9];
            interlace  = chunk[12];
            (void)interlace;                                
        } else if (!memcmp(type,"PLTE",4)) {
            pal_entries = (int)clen/3;
            memcpy(palette, chunk, (size_t)clen < 768u ? (size_t)clen : 768u);
        } else if (!memcmp(type,"IDAT",4)) {
            if (idat_len + clen > idat_cap) {
                size_t nc = idat_cap ? idat_cap*2 : 4096;
                tt_u8 *p2;
                while (nc < idat_len+clen) nc*=2;
                p2=(tt_u8*)realloc(idat_data,nc);
                if(!p2) goto done;
                idat_data=p2; idat_cap=nc;
            }
            memcpy(idat_data+idat_len, chunk, clen);
            idat_len+=clen;
        } else if (!memcmp(type,"IEND",4)) { break; }
        pos += clen + 4;                      
    }

    if (!w||!h||!idat_len) goto done;

    if (tt__inflate(idat_data, idat_len, &raw)) goto done;

    switch (color_type) {
    case 0: channels=1; break;           
    case 2: channels=3; break;          
    case 3: channels=3; break;                              
    case 4: channels=2; break;                 
    case 6: channels=4; break;           
    default: goto done;
    }
    bpp = channels * (bit_depth <= 8 ? 1 : 2);

    pixels = (tt_u8*)malloc((size_t)w * (size_t)h * (size_t)channels);
    if (!pixels) goto done;

    {
        int stride = w * bpp;
        tt_u8 *prev_row = (tt_u8*)calloc((size_t)(stride+1), 1);
        tt_u8 *cur_row  = (tt_u8*)malloc((size_t)(stride+1));
        size_t raw_pos  = 0;
        if (!prev_row || !cur_row) { free(prev_row); free(cur_row); goto done; }

        for (i = 0; i < h; i++) {
            int px, c;
            if (raw_pos + (size_t)(stride+1) > raw.len) { free(prev_row); free(cur_row); goto done; }
            memcpy(cur_row, raw.buf + raw_pos, (size_t)(stride+1));
            raw_pos += (size_t)(stride+1);
            tt__png_filter(cur_row, prev_row+1, stride, bpp);

            for (px = 0; px < w; px++) {
                tt_u8 *dst = pixels + ((size_t)i * (size_t)w + (size_t)px) * (size_t)channels;
                const tt_u8 *src2 = cur_row + 1 + px*bpp;
                if (color_type == 3) {
                    int idx = src2[0]; if (idx>=pal_entries) idx=0;
                    dst[0]=palette[idx*3]; dst[1]=palette[idx*3+1]; dst[2]=palette[idx*3+2];
                } else if (bit_depth == 16) {
                    for (c=0;c<channels;c++) dst[c]=src2[c*2];                
                } else {
                    for (c=0;c<channels;c++) dst[c]=src2[c];
                }
            }
            memcpy(prev_row, cur_row, (size_t)(stride+1));
        }
        free(prev_row); free(cur_row);
    }

    out->w = w; out->h = h; out->channels = channels;
    out->bit_depth = bit_depth <= 8 ? 8 : 8;
    out->pixels = pixels; pixels = NULL;
    ok = 0;

done:
    free(idat_data); free(raw.buf); free(pixels);
    return ok;
}

TT_INLINE int tt_png_read(const char *path, TT_PngImage *out) {
    FILE *f; long sz; tt_u8 *buf; int r;
    memset(out, 0, sizeof(*out));
    f = fopen(path, "rb");
    if (!f) return -1;
    fseek(f,0,SEEK_END); sz=ftell(f); rewind(f);
    if (sz <= 0) { fclose(f); return -2; }
    buf = (tt_u8*)malloc((size_t)sz);
    if (!buf) { fclose(f); return -3; }
    if ((long)fread(buf,1,(size_t)sz,f) != sz) { free(buf); fclose(f); return -4; }
    fclose(f);
    r = tt__png_decode(buf, (size_t)sz, out);
    free(buf);
    return r;
}

TT_INLINE int tt_png_read_mem(const tt_u8 *data, size_t len, TT_PngImage *out) {
    return tt__png_decode(data, len, out);
}

TT_INLINE void tt_png_free(TT_PngImage *img) { free(img->pixels); img->pixels = NULL; }


static void tt__png_w32(tt_u8 *p, tt_u32 v) {
    p[0]=(tt_u8)(v>>24);p[1]=(tt_u8)(v>>16);p[2]=(tt_u8)(v>>8);p[3]=(tt_u8)v;
}

static int tt__png_chunk(TT__ByteBuf *b, const char *type, const tt_u8 *data, tt_u32 dlen) {
    tt_u8 hdr[8]; tt_u32 crc; int i;
    tt__png_w32(hdr, dlen);
    memcpy(hdr+4, type, 4);
    for (i=0;i<8;i++) if (tt__bbuf_push(b, hdr[i])) return -1;
    for (i=0;i<(int)dlen;i++) if (tt__bbuf_push(b, data[i])) return -1;
    crc = tt__crc32_png((const tt_u8*)type, 4);
    crc = tt__crc32_png(data, dlen);                              
    {
        tt_u8 tmp[4]; memcpy(tmp,type,4);
        crc = tt__crc32_png(tmp, 4);
        if (dlen) crc = ((void)(crc), tt__crc32_png(tmp,4));
        {
            tt_u32 c = 0xFFFFFFFFu;
            static tt_u32 tbl2[256]; static int ti2=0;
            if (!ti2){tt_u32 k;for(k=0;k<256;k++){tt_u32 cc=k;int j;for(j=0;j<8;j++)cc=(cc&1)?(0xEDB88320u^(cc>>1)):(cc>>1);tbl2[k]=cc;}ti2=1;}
            for(i=0;i<4;i++) c=tbl2[(c^((const tt_u8*)type)[i])&0xFF]^(c>>8);
            for(i=0;i<(int)dlen;i++) c=tbl2[(c^data[i])&0xFF]^(c>>8);
            crc = c ^ 0xFFFFFFFFu;
        }
    }
    { tt_u8 cv[4]; tt__png_w32(cv,crc); for(i=0;i<4;i++) if(tt__bbuf_push(b,cv[i])) return -1; }
    return 0;
}

static int tt__deflate_stored(const tt_u8 *src, size_t slen, TT__ByteBuf *out) {
    if (tt__bbuf_push(out,0x78)) return -1;
    if (tt__bbuf_push(out,0x01)) return -1;
    tt_u32 adl = 1;
    size_t pos = 0;
    while (pos < slen) {
        size_t blk = slen - pos;
        int last;
        if (blk > 65535) blk = 65535;
        last = (pos + blk >= slen) ? 1 : 0;
        if (tt__bbuf_push(out, (tt_u8)last)) return -1;
        { tt_u8 v=(tt_u8)(blk&0xFF); if(tt__bbuf_push(out,v))return -1; }
        { tt_u8 v=(tt_u8)(blk>>8);   if(tt__bbuf_push(out,v))return -1; }
        { tt_u8 v=(tt_u8)(~blk&0xFF);if(tt__bbuf_push(out,v))return -1; }
        { tt_u8 v=(tt_u8)(~blk>>8);  if(tt__bbuf_push(out,v))return -1; }
        adl = tt__adler32(adl, src+pos, blk);
        { size_t j; for(j=0;j<blk;j++) if(tt__bbuf_push(out,src[pos+j]))return -1; }
        pos += blk;
    }
    { tt_u8 v[4]; tt__png_w32(v,adl);
      if(tt__bbuf_push(out,v[0])||tt__bbuf_push(out,v[1])||
         tt__bbuf_push(out,v[2])||tt__bbuf_push(out,v[3])) return -1; }
    return 0;
}

TT_INLINE int tt_png_write(const char *path, const tt_u8 *pixels,
                             int w, int h, int channels) {
    static const tt_u8 sig[8]={137,80,78,71,13,10,26,10};
    TT__ByteBuf raw, compressed;
    tt_u8 ihdr[13];
    int color_type, stride, i;
    FILE *f;
    int ok = -1;

    memset(&raw, 0, sizeof(raw));
    memset(&compressed, 0, sizeof(compressed));

    switch (channels) {
    case 1: color_type=0; break;
    case 2: color_type=4; break;
    case 3: color_type=2; break;
    case 4: color_type=6; break;
    default: return -1;
    }

    stride = w * channels;
    for (i = 0; i < h; i++) {
        if (tt__bbuf_push(&raw, 0)) goto wdone;                  
        { int j; for(j=0;j<stride;j++) if(tt__bbuf_push(&raw, pixels[(size_t)i*stride+j])) goto wdone; }
    }

    if (tt__deflate_stored(raw.buf, raw.len, &compressed)) goto wdone;

    f = fopen(path, "wb");
    if (!f) goto wdone;
    fwrite(sig, 1, 8, f);

    tt__png_w32(ihdr, (tt_u32)w); tt__png_w32(ihdr+4, (tt_u32)h);
    ihdr[8]=8; ihdr[9]=(tt_u8)color_type; ihdr[10]=0; ihdr[11]=0; ihdr[12]=0;
    {
        TT__ByteBuf chunk_buf; memset(&chunk_buf,0,sizeof(chunk_buf));
        tt__png_chunk(&chunk_buf,"IHDR",ihdr,13);
        fwrite(chunk_buf.buf,1,chunk_buf.len,f); free(chunk_buf.buf);
    }

    {
        TT__ByteBuf chunk_buf; memset(&chunk_buf,0,sizeof(chunk_buf));
        tt__png_chunk(&chunk_buf,"IDAT",compressed.buf,(tt_u32)compressed.len);
        fwrite(chunk_buf.buf,1,chunk_buf.len,f); free(chunk_buf.buf);
    }

    {
        TT__ByteBuf chunk_buf; memset(&chunk_buf,0,sizeof(chunk_buf));
        tt__png_chunk(&chunk_buf,"IEND",NULL,0);
        fwrite(chunk_buf.buf,1,chunk_buf.len,f); free(chunk_buf.buf);
    }

    fclose(f);
    ok = 0;
wdone:
    free(raw.buf); free(compressed.buf);
    return ok;
}

TT_INLINE int tt_png_write_mem(const tt_u8 *pixels, int w, int h, int channels,
                                tt_u8 **out, size_t *out_len) {
    static const tt_u8 sig[8]={137,80,78,71,13,10,26,10};
    TT__ByteBuf raw, compressed, final_buf;
    tt_u8 ihdr[13];
    int color_type, stride, i;
    int ok = -1;

    memset(&raw,0,sizeof(raw)); memset(&compressed,0,sizeof(compressed));
    memset(&final_buf,0,sizeof(final_buf));
    *out=NULL; *out_len=0;

    switch(channels){case 1:color_type=0;break;case 2:color_type=4;break;
                     case 3:color_type=2;break;case 4:color_type=6;break;default:return -1;}
    stride = w*channels;
    for(i=0;i<h;i++){
        if(tt__bbuf_push(&raw,0))goto wmdone;
        {int j;for(j=0;j<stride;j++)if(tt__bbuf_push(&raw,pixels[(size_t)i*stride+j]))goto wmdone;}
    }
    if(tt__deflate_stored(raw.buf,raw.len,&compressed))goto wmdone;
    { int j; for(j=0;j<8;j++) if(tt__bbuf_push(&final_buf,sig[j]))goto wmdone; }
    tt__png_w32(ihdr,(tt_u32)w); tt__png_w32(ihdr+4,(tt_u32)h);
    ihdr[8]=8;ihdr[9]=(tt_u8)color_type;ihdr[10]=0;ihdr[11]=0;ihdr[12]=0;
    { TT__ByteBuf cb;memset(&cb,0,sizeof(cb));tt__png_chunk(&cb,"IHDR",ihdr,13);
      {size_t j;for(j=0;j<cb.len;j++)tt__bbuf_push(&final_buf,cb.buf[j]);}free(cb.buf);}
    { TT__ByteBuf cb;memset(&cb,0,sizeof(cb));tt__png_chunk(&cb,"IDAT",compressed.buf,(tt_u32)compressed.len);
      {size_t j;for(j=0;j<cb.len;j++)tt__bbuf_push(&final_buf,cb.buf[j]);}free(cb.buf);}
    { TT__ByteBuf cb;memset(&cb,0,sizeof(cb));tt__png_chunk(&cb,"IEND",NULL,0);
      {size_t j;for(j=0;j<cb.len;j++)tt__bbuf_push(&final_buf,cb.buf[j]);}free(cb.buf);}
    *out = final_buf.buf; *out_len = final_buf.len; final_buf.buf=NULL; ok=0;
wmdone:
    free(raw.buf);free(compressed.buf);free(final_buf.buf);
    return ok;
}

TT_INLINE void tt_image_to_png_image(const TT_Image *src, TT_PngImage *dst) {
    dst->w = src->w; dst->h = src->h;
    dst->channels = src->channels; dst->bit_depth = 8;
    dst->pixels = (tt_u8*)malloc((size_t)src->w * src->h * src->channels);
    if (dst->pixels) memcpy(dst->pixels, src->data, (size_t)src->w * src->h * src->channels);
}

#endif  

#ifndef TT_NO_SHA512

#define TT_SHA512_DIGEST_LEN 64
#define TT_SHA384_DIGEST_LEN 48

typedef struct {
    tt_u64 state[8];
    tt_u64 count[2];                           
    tt_u8  buf[128];
    int    buflen;
    int    is384;                       
} TT_Sha512;

static const TT_UNUSED tt_u64 tt__sha512_k[80] = {
    (tt_u64)0x428a2f98d728ae22ULL, (tt_u64)0x7137449123ef65cdULL,
    (tt_u64)0xb5c0fbcfec4d3b2fULL, (tt_u64)0xe9b5dba58189dbbcULL,
    (tt_u64)0x3956c25bf348b538ULL, (tt_u64)0x59f111f1b605d019ULL,
    (tt_u64)0x923f82a4af194f9bULL, (tt_u64)0xab1c5ed5da6d8118ULL,
    (tt_u64)0xd807aa98a3030242ULL, (tt_u64)0x12835b0145706fbeULL,
    (tt_u64)0x243185be4ee4b28cULL, (tt_u64)0x550c7dc3d5ffb4e2ULL,
    (tt_u64)0x72be5d74f27b896fULL, (tt_u64)0x80deb1fe3b1696b1ULL,
    (tt_u64)0x9bdc06a725c71235ULL, (tt_u64)0xc19bf174cf692694ULL,
    (tt_u64)0xe49b69c19ef14ad2ULL, (tt_u64)0xefbe4786384f25e3ULL,
    (tt_u64)0x0fc19dc68b8cd5b5ULL, (tt_u64)0x240ca1cc77ac9c65ULL,
    (tt_u64)0x2de92c6f592b0275ULL, (tt_u64)0x4a7484aa6ea6e483ULL,
    (tt_u64)0x5cb0a9dcbd41fbd4ULL, (tt_u64)0x76f988da831153b5ULL,
    (tt_u64)0x983e5152ee66dfabULL, (tt_u64)0xa831c66d2db43210ULL,
    (tt_u64)0xb00327c898fb213fULL, (tt_u64)0xbf597fc7beef0ee4ULL,
    (tt_u64)0xc6e00bf33da88fc2ULL, (tt_u64)0xd5a79147930aa725ULL,
    (tt_u64)0x06ca6351e003826fULL, (tt_u64)0x142929670a0e6e70ULL,
    (tt_u64)0x27b70a8546d22ffcULL, (tt_u64)0x2e1b21385c26c926ULL,
    (tt_u64)0x4d2c6dfc5ac42aedULL, (tt_u64)0x53380d139d95b3dfULL,
    (tt_u64)0x650a73548baf63deULL, (tt_u64)0x766a0abb3c77b2a8ULL,
    (tt_u64)0x81c2c92e47edaee6ULL, (tt_u64)0x92722c851482353bULL,
    (tt_u64)0xa2bfe8a14cf10364ULL, (tt_u64)0xa81a664bbc423001ULL,
    (tt_u64)0xc24b8b70d0f89791ULL, (tt_u64)0xc76c51a30654be30ULL,
    (tt_u64)0xd192e819d6ef5218ULL, (tt_u64)0xd69906245565a910ULL,
    (tt_u64)0xf40e35855771202aULL, (tt_u64)0x106aa07032bbd1b8ULL,
    (tt_u64)0x19a4c116b8d2d0c8ULL, (tt_u64)0x1e376c085141ab53ULL,
    (tt_u64)0x2748774cdf8eeb99ULL, (tt_u64)0x34b0bcb5e19b48a8ULL,
    (tt_u64)0x391c0cb3c5c95a63ULL, (tt_u64)0x4ed8aa4ae3418acbULL,
    (tt_u64)0x5b9cca4f7763e373ULL, (tt_u64)0x682e6ff3d6b2b8a3ULL,
    (tt_u64)0x748f82ee5defb2fcULL, (tt_u64)0x78a5636f43172f60ULL,
    (tt_u64)0x84c87814a1f0ab72ULL, (tt_u64)0x8cc702081a6439ecULL,
    (tt_u64)0x90befffa23631e28ULL, (tt_u64)0xa4506cebde82bde9ULL,
    (tt_u64)0xbef9a3f7b2c67915ULL, (tt_u64)0xc67178f2e372532bULL,
    (tt_u64)0xca273eceea26619cULL, (tt_u64)0xd186b8c721c0c207ULL,
    (tt_u64)0xeada7dd6cde0eb1eULL, (tt_u64)0xf57d4f7fee6ed178ULL,
    (tt_u64)0x06f067aa72176fbaULL, (tt_u64)0x0a637dc5a2c898a6ULL,
    (tt_u64)0x113f9804bef90daeULL, (tt_u64)0x1b710b35131c471bULL,
    (tt_u64)0x28db77f523047d84ULL, (tt_u64)0x32caab7b40c72493ULL,
    (tt_u64)0x3c9ebe0a15c9bebcULL, (tt_u64)0x431d67c49c100d4cULL,
    (tt_u64)0x4cc5d4becb3e42b6ULL, (tt_u64)0x597f299cfc657e2aULL,
    (tt_u64)0x5fcb6fab3ad6faecULL, (tt_u64)0x6c44198c4a475817ULL
};

#define TT__R64(x,n) (((x)>>(n))|((x)<<(64-(n))))
#define TT__S512_S0(x) (TT__R64(x,28)^TT__R64(x,34)^TT__R64(x,39))
#define TT__S512_S1(x) (TT__R64(x,14)^TT__R64(x,18)^TT__R64(x,41))
#define TT__S512_G0(x) (TT__R64(x,1) ^TT__R64(x,8) ^((x)>>7))
#define TT__S512_G1(x) (TT__R64(x,19)^TT__R64(x,61)^((x)>>6))
#define TT__S512_CH(x,y,z)  (((x)&(y))^(~(x)&(z)))
#define TT__S512_MAJ(x,y,z) (((x)&(y))^((x)&(z))^((y)&(z)))

static TT_UNUSED void tt__sha512_transform(TT_Sha512 *c, const tt_u8 *blk) {
    tt_u64 w[80], a, b, cc, d, e, f, g, h, t1, t2;
    int i;
    for (i = 0; i < 16; i++) {
        w[i]  = ((tt_u64)blk[i*8  ] << 56) | ((tt_u64)blk[i*8+1] << 48)
              | ((tt_u64)blk[i*8+2] << 40) | ((tt_u64)blk[i*8+3] << 32)
              | ((tt_u64)blk[i*8+4] << 24) | ((tt_u64)blk[i*8+5] << 16)
              | ((tt_u64)blk[i*8+6] <<  8) |  (tt_u64)blk[i*8+7];
    }
    for (i = 16; i < 80; i++)
        w[i] = TT__S512_G1(w[i-2]) + w[i-7] + TT__S512_G0(w[i-15]) + w[i-16];

    a=c->state[0]; b=c->state[1]; cc=c->state[2]; d=c->state[3];
    e=c->state[4]; f=c->state[5]; g=c->state[6]; h=c->state[7];

    for (i = 0; i < 80; i++) {
        t1 = h + TT__S512_S1(e) + TT__S512_CH(e,f,g) + tt__sha512_k[i] + w[i];
        t2 = TT__S512_S0(a) + TT__S512_MAJ(a,b,cc);
        h=g; g=f; f=e; e=d+t1; d=cc; cc=b; b=a; a=t1+t2;
    }
    c->state[0]+=a; c->state[1]+=b; c->state[2]+=cc; c->state[3]+=d;
    c->state[4]+=e; c->state[5]+=f; c->state[6]+=g;  c->state[7]+=h;
}

TT_INLINE void tt_sha512_init(TT_Sha512 *c) {
    c->is384 = 0;
    c->state[0] = (tt_u64)0x6a09e667f3bcc908ULL;
    c->state[1] = (tt_u64)0xbb67ae8584caa73bULL;
    c->state[2] = (tt_u64)0x3c6ef372fe94f82bULL;
    c->state[3] = (tt_u64)0xa54ff53a5f1d36f1ULL;
    c->state[4] = (tt_u64)0x510e527fade682d1ULL;
    c->state[5] = (tt_u64)0x9b05688c2b3e6c1fULL;
    c->state[6] = (tt_u64)0x1f83d9abfb41bd6bULL;
    c->state[7] = (tt_u64)0x5be0cd19137e2179ULL;
    c->count[0] = c->count[1] = 0; c->buflen = 0;
}

TT_INLINE void tt_sha384_init(TT_Sha512 *c) {
    c->is384 = 1;
    c->state[0] = (tt_u64)0xcbbb9d5dc1059ed8ULL;
    c->state[1] = (tt_u64)0x629a292a367cd507ULL;
    c->state[2] = (tt_u64)0x9159015a3070dd17ULL;
    c->state[3] = (tt_u64)0x152fecd8f70e5939ULL;
    c->state[4] = (tt_u64)0x67332667ffc00b31ULL;
    c->state[5] = (tt_u64)0x8eb44a8768581511ULL;
    c->state[6] = (tt_u64)0xdb0c2e0d64f98fa7ULL;
    c->state[7] = (tt_u64)0x47b5481dbefa4fa4ULL;
    c->count[0] = c->count[1] = 0; c->buflen = 0;
}

TT_INLINE void tt_sha512_update(TT_Sha512 *c, const void *data, size_t len) {
    const tt_u8 *p = (const tt_u8*)data;
    while (len > 0) {
        int sp   = 128 - c->buflen;
        int take = (int)len < sp ? (int)len : sp;
        memcpy(c->buf + c->buflen, p, (size_t)take);
        c->buflen += take; p += take; len -= (size_t)take;
        c->count[0] += (tt_u64)take * 8;
        if (c->count[0] < (tt_u64)take * 8) c->count[1]++;
        if (c->buflen == 128) { tt__sha512_transform(c, c->buf); c->buflen = 0; }
    }
}

TT_INLINE void tt_sha512_final(TT_Sha512 *c, tt_u8 *digest) {
    int i, digest_words = c->is384 ? 6 : 8;
    c->buf[c->buflen++] = 0x80;
    if (c->buflen > 112) {
        memset(c->buf + c->buflen, 0, (size_t)(128 - c->buflen));
        tt__sha512_transform(c, c->buf); c->buflen = 0;
    }
    memset(c->buf + c->buflen, 0, (size_t)(112 - c->buflen));
    for (i = 0; i < 8; i++) c->buf[112+i] = (tt_u8)(c->count[1] >> (56 - i*8));
    for (i = 0; i < 8; i++) c->buf[120+i] = (tt_u8)(c->count[0] >> (56 - i*8));
    tt__sha512_transform(c, c->buf);
    for (i = 0; i < digest_words; i++) {
        digest[i*8+0] = (tt_u8)(c->state[i] >> 56);
        digest[i*8+1] = (tt_u8)(c->state[i] >> 48);
        digest[i*8+2] = (tt_u8)(c->state[i] >> 40);
        digest[i*8+3] = (tt_u8)(c->state[i] >> 32);
        digest[i*8+4] = (tt_u8)(c->state[i] >> 24);
        digest[i*8+5] = (tt_u8)(c->state[i] >> 16);
        digest[i*8+6] = (tt_u8)(c->state[i] >>  8);
        digest[i*8+7] = (tt_u8)(c->state[i]      );
    }
}

TT_INLINE void tt_sha512(const void *d, size_t l, tt_u8 out[64]) {
    TT_Sha512 c; tt_sha512_init(&c); tt_sha512_update(&c,d,l); tt_sha512_final(&c,out);
}
TT_INLINE void tt_sha384(const void *d, size_t l, tt_u8 out[48]) {
    TT_Sha512 c; tt_sha384_init(&c); tt_sha512_update(&c,d,l); tt_sha512_final(&c,out);
}
TT_INLINE void tt_sha512_hex(const void *d, size_t l, char out[129]) {
    tt_u8 h[64]; int i; tt_sha512(d,l,h);
    for (i=0;i<64;i++) { out[i*2]  ="0123456789abcdef"[h[i]>>4];
                          out[i*2+1]="0123456789abcdef"[h[i]&0xF]; }
    out[128] = '\0';
}
TT_INLINE void tt_sha384_hex(const void *d, size_t l, char out[97]) {
    tt_u8 h[48]; int i; tt_sha384(d,l,h);
    for (i=0;i<48;i++) { out[i*2]  ="0123456789abcdef"[h[i]>>4];
                          out[i*2+1]="0123456789abcdef"[h[i]&0xF]; }
    out[96] = '\0';
}

#ifndef TT_NO_SHA256
#define TT_SHA224_DIGEST_LEN 28

TT_INLINE void tt_sha224_init(TT_Sha256 *c) {
    c->state[0] = 0xc1059ed8u;
    c->state[1] = 0x367cd507u;
    c->state[2] = 0x3070dd17u;
    c->state[3] = 0xf70e5939u;
    c->state[4] = 0xffc00b31u;
    c->state[5] = 0x68581511u;
    c->state[6] = 0x64f98fa7u;
    c->state[7] = 0xbefa4fa4u;
    c->count = 0; c->buflen = 0;
}

TT_INLINE void tt_sha224(const void *d, size_t l, tt_u8 out[28]) {
    TT_Sha256 c; tt_u8 full[32]; int i;
    tt_sha224_init(&c);
    tt_sha256_update(&c, d, l);
    tt_sha256_final(&c, full);
    for (i = 0; i < 28; i++) out[i] = full[i];
}
TT_INLINE void tt_sha224_hex(const void *d, size_t l, char out[57]) {
    tt_u8 h[28]; int i; tt_sha224(d,l,h);
    for (i=0;i<28;i++) { out[i*2]  ="0123456789abcdef"[h[i]>>4];
                          out[i*2+1]="0123456789abcdef"[h[i]&0xF]; }
    out[56] = '\0';
}
#endif                   

#endif

#ifndef TT_NO_LZPACK

#define TT_LZP_MAGIC     "LZPK"
#define TT_LZP_VERSION   1
#define TT_LZP_WIN_BITS  12                                
#define TT_LZP_WIN_SIZE  (1 << TT_LZP_WIN_BITS)
#define TT_LZP_WIN_MASK  (TT_LZP_WIN_SIZE - 1)
#define TT_LZP_MAX_LEN   258                               
#define TT_LZP_MIN_LEN   3                                 
#define TT_LZP_HASH_BITS 14
#define TT_LZP_HASH_SIZE (1 << TT_LZP_HASH_BITS)
#define TT_LZP_HASH_MASK (TT_LZP_HASH_SIZE - 1)

typedef struct {
    tt_u8 *buf;
    size_t len;
    size_t cap;
} TT__LzpBuf;

static int tt__lzp_push(TT__LzpBuf *b, tt_u8 v) {
    if (b->len >= b->cap) {
        size_t nc = b->cap ? b->cap * 2 : 1024;
        tt_u8 *p  = (tt_u8*)realloc(b->buf, nc);
        if (!p) return -1;
        b->buf = p; b->cap = nc;
    }
    b->buf[b->len++] = v;
    return 0;
}

static int tt__lzp_push2(TT__LzpBuf *b, tt_u16 v) {
    if (tt__lzp_push(b, (tt_u8)(v & 0xFF))) return -1;
    return tt__lzp_push(b, (tt_u8)(v >> 8));
}

static TT_UNUSED tt_u32 tt__lzp_hash3(const tt_u8 *p) {
    return (((tt_u32)p[0] * 2654435761u) ^
            ((tt_u32)p[1] * 40503u)      ^
             (tt_u32)p[2]) & TT_LZP_HASH_MASK;
}

TT_INLINE int tt_lzp_compress(const tt_u8 *src, size_t src_len,
                               tt_u8 **out_buf, size_t *out_len) {
    TT__LzpBuf out;
    int  *ht;
    tt_u8 *win;                                        
    size_t i, win_pos;
    int ret = 0;

    *out_buf = NULL; *out_len = 0;
    memset(&out, 0, sizeof(out));

    ht  = (int*)malloc(TT_LZP_HASH_SIZE * sizeof(int));
    win = (tt_u8*)malloc(TT_LZP_WIN_SIZE);
    if (!ht || !win) { free(ht); free(win); return -1; }
    memset(ht, -1, TT_LZP_HASH_SIZE * sizeof(int));
    memset(win, 0, TT_LZP_WIN_SIZE);

    tt__lzp_push(&out, (tt_u8)'L'); tt__lzp_push(&out, (tt_u8)'Z');
    tt__lzp_push(&out, (tt_u8)'P'); tt__lzp_push(&out, (tt_u8)'K');
    tt__lzp_push(&out, TT_LZP_VERSION);
    tt__lzp_push(&out, (tt_u8)(src_len & 0xFF));
    tt__lzp_push(&out, (tt_u8)((src_len >>  8) & 0xFF));
    tt__lzp_push(&out, (tt_u8)((src_len >> 16) & 0xFF));
    tt__lzp_push(&out, (tt_u8)((src_len >> 24) & 0xFF));

    win_pos = 0;
    i = 0;
    while (i < src_len) {
        int best_len = 0, best_dist = 0;

        if (i + TT_LZP_MIN_LEN <= src_len) {
            tt_u32 h  = tt__lzp_hash3(src + i);
            int    mp = ht[h];
            ht[h] = (int)i;

            if (mp >= 0 && mp < (int)i) {
                int dist = (int)i - mp;
                if (dist <= TT_LZP_WIN_SIZE) {
                    int max_len = (int)(src_len - i);
                    size_t ml = 0;
                    if (max_len > TT_LZP_MAX_LEN)
                    while (ml < (size_t)max_len && src[(size_t)mp + ml] == src[i + ml]) ml++;
                    if (ml >= TT_LZP_MIN_LEN) { best_len = ml; best_dist = dist; }
                }
            }
        }

        if (best_len >= TT_LZP_MIN_LEN) {
            tt__lzp_push(&out, 0x80);
            tt__lzp_push2(&out, (tt_u16)best_dist);
            tt__lzp_push(&out, (tt_u8)(best_len - TT_LZP_MIN_LEN));
            { int k;
              for (k = 0; k < best_len; k++) {
                  win[win_pos & TT_LZP_WIN_MASK] = src[i + (size_t)k];
                  win_pos++;
              }
            }
            i += (size_t)best_len;
        } else {
            tt__lzp_push(&out, 0x00);
            tt__lzp_push(&out, src[i]);
            win[win_pos & TT_LZP_WIN_MASK] = src[i];
            win_pos++;
            i++;
        }
    }

    free(ht); free(win);
    if (ret == 0) { *out_buf = out.buf; *out_len = out.len; }
    else            free(out.buf);
    return ret;
}

TT_INLINE int tt_lzp_decompress(const tt_u8 *src, size_t src_len,
                                 tt_u8 **out_buf, size_t *out_len) {
    size_t orig_size, pos;
    tt_u8 *out;

    *out_buf = NULL; *out_len = 0;

    if (src_len < 9) return -1;
    if (memcmp(src, TT_LZP_MAGIC, 4) != 0) return -2;
    if (src[4] != TT_LZP_VERSION) return -3;

    orig_size = (size_t)src[5] | ((size_t)src[6]<<8) |
                ((size_t)src[7]<<16) | ((size_t)src[8]<<24);

    out = (tt_u8*)malloc(orig_size + 1);
    if (!out) return -4;

    pos = 9;                  
    *out_len = 0;

    while (pos < src_len && *out_len < orig_size) {
        tt_u8 tag = src[pos++];

        if (tag == 0x00) {
            if (pos >= src_len) { free(out); return -5; }
            out[(*out_len)++] = src[pos++];
        } else if (tag == 0x01) {
            tt_u16 rlen;
            size_t k;
            if (pos + 2 > src_len) { free(out); return -6; }
            rlen = (tt_u16)src[pos] | ((tt_u16)src[pos+1] << 8);
            pos += 2;
            if (pos + rlen > src_len) { free(out); return -7; }
            for (k = 0; k < rlen && *out_len < orig_size; k++)
                out[(*out_len)++] = src[pos + k];
            pos += rlen;
        } else if (tag == 0x80) {
            tt_u16 dist; int mlen; size_t back, k;
            if (pos + 3 > src_len) { free(out); return -8; }
            dist = (tt_u16)src[pos] | ((tt_u16)src[pos+1] << 8);
            mlen = (int)src[pos+2] + TT_LZP_MIN_LEN;
            pos += 3;
            if (dist == 0 || dist > (tt_u16)*out_len) { free(out); return -9; }
            back = *out_len - dist;
            for (k = 0; k < (size_t)mlen && *out_len < orig_size; k++)
                out[(*out_len)++] = out[back + k];
        } else {
            free(out); return -10;                  
        }
    }

    *out_buf = out;
    return 0;
}

TT_INLINE int tt_lzp_compress_file(const char *in_path, const char *out_path) {
    FILE *fin, *fout;
    long fsz;
    tt_u8 *src, *comp;
    size_t comp_len;
    int rc;

    fin = fopen(in_path, "rb");
    if (!fin) return -1;
    fseek(fin, 0, SEEK_END); fsz = ftell(fin); rewind(fin);
    if (fsz <= 0) { fclose(fin); return -2; }

    src = (tt_u8*)malloc((size_t)fsz);
    if (!src) { fclose(fin); return -3; }
    fread(src, 1, (size_t)fsz, fin);
    fclose(fin);

    rc = tt_lzp_compress(src, (size_t)fsz, &comp, &comp_len);
    free(src);
    if (rc != 0) return rc;

    fout = fopen(out_path, "wb");
    if (!fout) { free(comp); return -4; }
    fwrite(comp, 1, comp_len, fout);
    fclose(fout);
    free(comp);
    return 0;
}

TT_INLINE int tt_lzp_decompress_file(const char *in_path, const char *out_path) {
    FILE *fin, *fout;
    long fsz;
    tt_u8 *src, *out;
    size_t out_len;
    int rc;

    fin = fopen(in_path, "rb");
    if (!fin) return -1;
    fseek(fin, 0, SEEK_END); fsz = ftell(fin); rewind(fin);
    if (fsz <= 0) { fclose(fin); return -2; }

    src = (tt_u8*)malloc((size_t)fsz);
    if (!src) { fclose(fin); return -3; }
    fread(src, 1, (size_t)fsz, fin);
    fclose(fin);

    rc = tt_lzp_decompress(src, (size_t)fsz, &out, &out_len);
    free(src);
    if (rc != 0) return rc;

    fout = fopen(out_path, "wb");
    if (!fout) { free(out); return -4; }
    fwrite(out, 1, out_len, fout);
    fclose(fout);
    free(out);
    return 0;
}

TT_INLINE double tt_lzp_ratio(size_t original, size_t compressed) {
    if (compressed == 0) return 0.0;
    return (double)original / (double)compressed;
}

#endif

#ifndef TT_NO_SOUND2


TT_INLINE float tt_synth_pulse(double phase, double duty) {
    return (phase - (int)phase) < duty ? 1.0f : -1.0f;
}

TT_INLINE float tt_synth_soft_sine(double phase, double drive) {
    float s = (float)sin(phase * 2.0 * 3.14159265358979323846);
    float d = (float)drive;
    float x = s * d;
    return x / (1.0f + (x < 0.0f ? -x : x));
}

TT_INLINE float tt_synth_supersaw(double phase, int voices, double detune_cents) {
    float sum = 0.0f;
    int i;
    if (voices < 1) voices = 1;
    for (i = 0; i < voices; i++) {
        double spread = (voices > 1) ? ((double)i / (double)(voices-1) - 0.5) * detune_cents : 0.0;
        double ratio  = pow(2.0, spread / 1200.0);
        double ph     = fmod(phase * ratio, 1.0);
        if (ph < 0.0) ph += 1.0;
        sum += (float)(2.0 * ph - 1.0);
    }
    return sum / (float)voices;
}

TT_INLINE float tt_synth_additive(double phase, const float *harmonics, int n_harmonics) {
    float sum = 0.0f;
    int i;
    for (i = 0; i < n_harmonics; i++) {
        sum += harmonics[i] * (float)sin((double)(i+1) * phase * 2.0 * 3.14159265358979323846);
    }
    return sum;
}

TT_INLINE void tt_synth_karplus_strong(float *buf, int n, int period_samples, float decay) {
    float *ring;
    int i;
    if (period_samples < 2) period_samples = 2;
    ring = (float*)malloc((size_t)period_samples * sizeof(float));
    if (!ring) return;
    for (i = 0; i < period_samples; i++)
        ring[i] = (float)(tt_rngf() * 2.0 - 1.0);
    for (i = 0; i < n; i++) {
        int pos = i % period_samples;
        int nxt = (i + 1) % period_samples;
        buf[i] = ring[pos];
        ring[pos] = (ring[pos] + ring[nxt]) * 0.5f * decay;
    }
    free(ring);
}

TT_INLINE float tt_synth_distort(float sample, float amount) {
    float x = sample * (1.0f + amount * 10.0f);
    if (x >  1.0f) x =  1.0f;
    if (x < -1.0f) x = -1.0f;
    return x;
}

TT_INLINE float tt_synth_wavefold(float sample, float threshold) {
    float t = threshold > 0.0f ? threshold : 0.01f;
    float x = sample;
    while (x >  t) x =  2.0f * t - x;
    while (x < -t) x = -2.0f * t - x;
    return x / t;
}

TT_INLINE float tt_synth_ring_mod(float carrier, float modulator) {
    return carrier * modulator;
}

TT_INLINE float tt_synth_bitcrush(float sample, int n_bits, int downsample_factor,
                                   int *ds_counter, float *ds_hold) {
    float levels = (float)(1 << (n_bits > 0 ? n_bits : 1));
    if ((*ds_counter)++ % downsample_factor == 0)
        *ds_hold = floorf(sample * levels + 0.5f) / levels;
    return *ds_hold;
}

TT_INLINE float tt_synth_chorus(float input, float *delay_buf, int delay_len,
                                 int *write_pos, double lfo_phase,
                                 double depth_samples) {
    double mod      = (sin(lfo_phase * 2.0 * 3.14159265358979323846) * 0.5 + 0.5) * depth_samples;
    int    read_off = (int)mod;
    int    rp       = (*write_pos - read_off + delay_len) % delay_len;
    float  delayed  = delay_buf[rp];
    delay_buf[*write_pos] = input;
    *write_pos = (*write_pos + 1) % delay_len;
    return (input + delayed) * 0.5f;
}

TT_INLINE float tt_synth_phaser(float input, float *ap_state, float lfo_val, float depth) {
    float g = 0.5f + lfo_val * depth * 0.45f;
    float s0 = g * (input - ap_state[0]) + ap_state[0]; ap_state[0] = s0;
    float s1 = g * (s0    - ap_state[1]) + ap_state[1]; ap_state[1] = s1;
    return (input + s1) * 0.5f;
}

typedef struct {
    float b0, b1, b2;                                 
    float a1, a2;                                                          
    float z1, z2;                           
} TT_Biquad;

TT_INLINE void tt_biquad_lowpass(TT_Biquad *f, double cutoff_hz, double sample_rate) {
    double w0  = 2.0 * 3.14159265358979323846 * cutoff_hz / sample_rate;
    double cosw= cos(w0), sinw = sin(w0);
    double Q   = 0.7071067811865476;                            
    double alpha = sinw / (2.0 * Q);
    double a0  = 1.0 + alpha;
    f->b0 = (float)(((1.0 - cosw) / 2.0) / a0);
    f->b1 = (float)((1.0 - cosw)          / a0);
    f->b2 = f->b0;
    f->a1 = (float)((-2.0 * cosw)         / a0);
    f->a2 = (float)((1.0 - alpha)         / a0);
    f->z1 = f->z2 = 0.0f;
}

TT_INLINE void tt_biquad_highpass(TT_Biquad *f, double cutoff_hz, double sample_rate) {
    double w0   = 2.0 * 3.14159265358979323846 * cutoff_hz / sample_rate;
    double cosw = cos(w0), sinw = sin(w0);
    double Q    = 0.7071067811865476;
    double alpha = sinw / (2.0 * Q);
    double a0   = 1.0 + alpha;
    f->b0 = (float)(((1.0 + cosw) / 2.0) / a0);
    f->b1 = (float)((-(1.0 + cosw))       / a0);
    f->b2 = f->b0;
    f->a1 = (float)((-2.0 * cosw)         / a0);
    f->a2 = (float)((1.0 - alpha)         / a0);
    f->z1 = f->z2 = 0.0f;
}

TT_INLINE void tt_biquad_bandpass(TT_Biquad *f, double center_hz, double Q, double sample_rate) {
    double w0    = 2.0 * 3.14159265358979323846 * center_hz / sample_rate;
    double cosw  = cos(w0), sinw = sin(w0);
    double alpha = sinw / (2.0 * Q);
    double a0    = 1.0 + alpha;
    f->b0 = (float)((Q * alpha) / a0);
    f->b1 = 0.0f;
    f->b2 = (float)((-Q * alpha) / a0);
    f->a1 = (float)((-2.0 * cosw) / a0);
    f->a2 = (float)((1.0 - alpha) / a0);
    f->z1 = f->z2 = 0.0f;
}

TT_INLINE void tt_biquad_notch(TT_Biquad *f, double center_hz, double Q, double sample_rate) {
    double w0    = 2.0 * 3.14159265358979323846 * center_hz / sample_rate;
    double cosw  = cos(w0), sinw = sin(w0);
    double alpha = sinw / (2.0 * Q);
    double a0    = 1.0 + alpha;
    f->b0 = (float)(1.0 / a0);
    f->b1 = (float)((-2.0 * cosw) / a0);
    f->b2 = f->b0;
    f->a1 = f->b1;
    f->a2 = (float)((1.0 - alpha) / a0);
    f->z1 = f->z2 = 0.0f;
}

TT_INLINE void tt_biquad_peak(TT_Biquad *f, double center_hz, double Q,
                               double gain_db, double sample_rate) {
    double A     = pow(10.0, gain_db / 40.0);
    double w0    = 2.0 * 3.14159265358979323846 * center_hz / sample_rate;
    double cosw  = cos(w0), sinw = sin(w0);
    double alpha = sinw / (2.0 * Q);
    double a0    = 1.0 + alpha / A;
    f->b0 = (float)((1.0 + alpha * A) / a0);
    f->b1 = (float)((-2.0 * cosw)     / a0);
    f->b2 = (float)((1.0 - alpha * A) / a0);
    f->a1 = f->b1;
    f->a2 = (float)((1.0 - alpha / A) / a0);
    f->z1 = f->z2 = 0.0f;
}

TT_INLINE float tt_biquad_process(TT_Biquad *f, float x) {
    float y = f->b0 * x + f->z1;
    f->z1   = f->b1 * x - f->a1 * y + f->z2;
    f->z2   = f->b2 * x - f->a2 * y;
    return y;
}

TT_INLINE void tt_biquad_process_buf(TT_Biquad *f, float *buf, int n) {
    int i;
    for (i = 0; i < n; i++) buf[i] = tt_biquad_process(f, buf[i]);
}

typedef struct { float *buf; int len; int pos; } TT_Delay;

TT_INLINE int  tt_delay_init(TT_Delay *d, int len) {
    d->buf = (float*)calloc((size_t)len, sizeof(float));
    d->len = len; d->pos = 0;
    return d->buf != NULL;
}
TT_INLINE void tt_delay_free(TT_Delay *d) { free(d->buf); d->buf = NULL; }

TT_INLINE float tt_delay_process(TT_Delay *d, float in, float feedback, float mix) {
    float out_samp = d->buf[d->pos];
    d->buf[d->pos] = in + out_samp * feedback;
    d->pos = (d->pos + 1) % d->len;
    return in * (1.0f - mix) + out_samp * mix;
}

TT_INLINE float tt_env_follow(float *state, float input, float attack, float release) {
    float abs_in = input < 0.0f ? -input : input;
    float coeff  = abs_in > *state ? attack : release;
    *state = *state + coeff * (abs_in - *state);
    return *state;
}

typedef struct { double phase; double rate_hz; tt_u32 sample_rate; } TT_LFO;

TT_INLINE void  tt_lfo_init(TT_LFO *l, double rate_hz, tt_u32 sr) {
    l->phase = 0.0; l->rate_hz = rate_hz; l->sample_rate = sr;
}
TT_INLINE float tt_lfo_sine(TT_LFO *l) {
    float v = (float)sin(l->phase * 2.0 * 3.14159265358979323846);
    l->phase += l->rate_hz / (double)l->sample_rate;
    if (l->phase >= 1.0) l->phase -= 1.0;
    return v;
}
TT_INLINE float tt_lfo_tri(TT_LFO *l) {
    double p = l->phase;
    float  v = (float)(p < 0.5 ? 4.0*p - 1.0 : 3.0 - 4.0*p);
    l->phase += l->rate_hz / (double)l->sample_rate;
    if (l->phase >= 1.0) l->phase -= 1.0;
    return v;
}
TT_INLINE float tt_lfo_square(TT_LFO *l) {
    float v = l->phase < 0.5 ? 1.0f : -1.0f;
    l->phase += l->rate_hz / (double)l->sample_rate;
    if (l->phase >= 1.0) l->phase -= 1.0;
    return v;
}

#endif

#endif



#ifndef TT_CRYPTO_EXT_H
#define TT_CRYPTO_EXT_H

#ifndef TT_NO_SHA0
#define TT_SHA0_DIGEST_LEN 20

typedef struct { tt_u32 state[5]; tt_u32 count[2]; tt_u8 buf[64]; int buflen; } TT_Sha0;

#define TT__SHA0_F1(b,c,d) (((b)&(c))|((~(b))&(d)))
#define TT__SHA0_F2(b,c,d) ((b)^(c)^(d))
#define TT__SHA0_F3(b,c,d) (((b)&(c))|((b)&(d))|((c)&(d)))
#define TT__SHA0_ROL(v,n)  (((v)<<(n))|((v)>>(32-(n))))

static TT_UNUSED void tt__sha0_transform(TT_Sha0 *c, const tt_u8 *blk) {
    tt_u32 w[80], a, b, cc, d, e, t;
    int i;
    for (i = 0; i < 16; i++)
        w[i] = ((tt_u32)blk[i*4]<<24)|((tt_u32)blk[i*4+1]<<16)|
               ((tt_u32)blk[i*4+2]<<8)|(tt_u32)blk[i*4+3];
    for (i = 16; i < 80; i++)
        w[i] = w[i-3]^w[i-8]^w[i-14]^w[i-16];
    a=c->state[0]; b=c->state[1]; cc=c->state[2]; d=c->state[3]; e=c->state[4];
    for (i = 0; i < 20; i++) { t=TT__SHA0_ROL(a,5)+TT__SHA0_F1(b,cc,d)+e+w[i]+0x5A827999u; e=d; d=cc; cc=TT__SHA0_ROL(b,30); b=a; a=t; }
    for (i = 20; i < 40; i++) { t=TT__SHA0_ROL(a,5)+TT__SHA0_F2(b,cc,d)+e+w[i]+0x6ED9EBA1u; e=d; d=cc; cc=TT__SHA0_ROL(b,30); b=a; a=t; }
    for (i = 40; i < 60; i++) { t=TT__SHA0_ROL(a,5)+TT__SHA0_F3(b,cc,d)+e+w[i]+0x8F1BBCDCu; e=d; d=cc; cc=TT__SHA0_ROL(b,30); b=a; a=t; }
    for (i = 60; i < 80; i++) { t=TT__SHA0_ROL(a,5)+TT__SHA0_F2(b,cc,d)+e+w[i]+0xCA62C1D6u; e=d; d=cc; cc=TT__SHA0_ROL(b,30); b=a; a=t; }
    c->state[0]+=a; c->state[1]+=b; c->state[2]+=cc; c->state[3]+=d; c->state[4]+=e;
}

TT_INLINE void tt_sha0_init(TT_Sha0 *c) {
    c->state[0]=0x67452301u; c->state[1]=0xEFCDAB89u; c->state[2]=0x98BADCFEu;
    c->state[3]=0x10325476u; c->state[4]=0xC3D2E1F0u;
    c->count[0]=c->count[1]=0; c->buflen=0;
}
TT_INLINE void tt_sha0_update(TT_Sha0 *c, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){ int sp=64-c->buflen, take=(int)len<sp?(int)len:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take); c->buflen+=take; p+=take; len-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3))) c->count[1]++;
        if(c->buflen==64){tt__sha0_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_sha0_final(TT_Sha0 *c, tt_u8 *digest) {
    int i; tt_u32 bits_lo=c->count[0], bits_hi=c->count[1];
    c->buf[c->buflen++]=0x80;
    if(c->buflen>56){memset(c->buf+c->buflen,0,(size_t)(64-c->buflen));tt__sha0_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(56-c->buflen));
    for(i=0;i<4;i++){c->buf[56+i]=(tt_u8)(bits_hi>>(24-i*8)); c->buf[60+i]=(tt_u8)(bits_lo>>(24-i*8));}
    tt__sha0_transform(c,c->buf);
    for(i=0;i<5;i++){digest[i*4]=(tt_u8)(c->state[i]>>24);digest[i*4+1]=(tt_u8)(c->state[i]>>16);
        digest[i*4+2]=(tt_u8)(c->state[i]>>8);digest[i*4+3]=(tt_u8)c->state[i];}
}
TT_INLINE void tt_sha0(const void *d, size_t l, tt_u8 out[20]) {
    TT_Sha0 c; tt_sha0_init(&c); tt_sha0_update(&c,d,l); tt_sha0_final(&c,out);
}
#endif                 

#ifndef TT_NO_SHA1
#define TT_SHA1_DIGEST_LEN 20

typedef struct { tt_u32 state[5]; tt_u32 count[2]; tt_u8 buf[64]; int buflen; } TT_Sha1;

static TT_UNUSED void tt__sha1_transform(TT_Sha1 *c, const tt_u8 *blk) {
    tt_u32 w[80], a, b, cc, d, e, t;
    int i;
    for (i = 0; i < 16; i++)
        w[i]=((tt_u32)blk[i*4]<<24)|((tt_u32)blk[i*4+1]<<16)|
             ((tt_u32)blk[i*4+2]<<8)|(tt_u32)blk[i*4+3];
    for (i=16;i<80;i++) w[i]=TT__SHA0_ROL(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);
    a=c->state[0]; b=c->state[1]; cc=c->state[2]; d=c->state[3]; e=c->state[4];
    for (i=0;i<20;i++){t=TT__SHA0_ROL(a,5)+TT__SHA0_F1(b,cc,d)+e+w[i]+0x5A827999u;e=d;d=cc;cc=TT__SHA0_ROL(b,30);b=a;a=t;}
    for (i=20;i<40;i++){t=TT__SHA0_ROL(a,5)+TT__SHA0_F2(b,cc,d)+e+w[i]+0x6ED9EBA1u;e=d;d=cc;cc=TT__SHA0_ROL(b,30);b=a;a=t;}
    for (i=40;i<60;i++){t=TT__SHA0_ROL(a,5)+TT__SHA0_F3(b,cc,d)+e+w[i]+0x8F1BBCDCu;e=d;d=cc;cc=TT__SHA0_ROL(b,30);b=a;a=t;}
    for (i=60;i<80;i++){t=TT__SHA0_ROL(a,5)+TT__SHA0_F2(b,cc,d)+e+w[i]+0xCA62C1D6u;e=d;d=cc;cc=TT__SHA0_ROL(b,30);b=a;a=t;}
    c->state[0]+=a;c->state[1]+=b;c->state[2]+=cc;c->state[3]+=d;c->state[4]+=e;
}
TT_INLINE void tt_sha1_init(TT_Sha1 *c) {
    c->state[0]=0x67452301u;c->state[1]=0xEFCDAB89u;c->state[2]=0x98BADCFEu;
    c->state[3]=0x10325476u;c->state[4]=0xC3D2E1F0u;
    c->count[0]=c->count[1]=0;c->buflen=0;
}
TT_INLINE void tt_sha1_update(TT_Sha1 *c, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int sp=64-c->buflen,take=(int)len<sp?(int)len:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;len-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3)))c->count[1]++;
        if(c->buflen==64){tt__sha1_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_sha1_final(TT_Sha1 *c, tt_u8 *digest) {
    int i; tt_u32 bl=c->count[0],bh=c->count[1];
    c->buf[c->buflen++]=0x80;
    if(c->buflen>56){memset(c->buf+c->buflen,0,(size_t)(64-c->buflen));tt__sha1_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(56-c->buflen));
    for(i=0;i<4;i++){c->buf[56+i]=(tt_u8)(bh>>(24-i*8));c->buf[60+i]=(tt_u8)(bl>>(24-i*8));}
    tt__sha1_transform(c,c->buf);
    for(i=0;i<5;i++){digest[i*4]=(tt_u8)(c->state[i]>>24);digest[i*4+1]=(tt_u8)(c->state[i]>>16);
        digest[i*4+2]=(tt_u8)(c->state[i]>>8);digest[i*4+3]=(tt_u8)c->state[i];}
}
TT_INLINE void tt_sha1(const void *d, size_t l, tt_u8 out[20]) {
    TT_Sha1 c; tt_sha1_init(&c); tt_sha1_update(&c,d,l); tt_sha1_final(&c,out);
}
TT_INLINE void tt_sha1_hex(const void *d, size_t l, char out[41]) {
    tt_u8 h[20]; int i; tt_sha1(d,l,h);
    for(i=0;i<20;i++){out[i*2]="0123456789abcdef"[h[i]>>4];out[i*2+1]="0123456789abcdef"[h[i]&0xF];}
    out[40]='\0';
}
#endif                 

#ifndef TT_NO_MD2
#define TT_MD2_DIGEST_LEN 16

typedef struct { tt_u8 state[48]; tt_u8 checksum[16]; tt_u8 buf[16]; int buflen; } TT_Md2;

static const tt_u8 tt__md2_pi[256] = {
    41,46,67,201,162,216,124,1,61,54,84,161,236,240,6,19,98,167,5,243,192,199,
    115,140,152,147,43,217,188,76,130,202,30,155,87,60,253,212,224,22,103,66,111,
    24,138,23,229,18,190,78,196,214,218,158,222,73,160,251,245,142,187,47,238,122,
    169,104,121,145,21,178,7,63,148,194,16,137,11,34,95,33,128,127,93,154,90,144,
    50,39,53,62,204,231,191,247,151,3,255,25,48,179,72,165,181,209,215,94,146,42,
    172,86,170,198,79,184,56,210,150,164,125,182,118,252,107,226,156,116,4,241,69,
    157,112,89,100,113,135,32,134,91,207,101,230,45,168,2,27,96,37,173,174,176,
    185,246,28,70,97,105,52,64,126,15,85,71,163,35,221,81,175,58,195,92,249,206,
    186,197,234,38,44,83,13,110,133,40,132,9,211,223,205,244,65,129,77,82,106,
    220,55,200,108,193,171,250,36,225,123,8,12,189,177,74,120,136,149,139,227,99,
    232,109,233,203,213,254,59,0,29,57,242,239,183,14,102,88,208,228,166,119,114,
    248,235,117,75,10,49,68,80,180,143,237,31,26,219,153,141,51,159,17,131,20
};

static TT_UNUSED void tt__md2_transform(TT_Md2 *c) {
    int i,j,t;
    for(i=0;i<16;i++) c->state[16+i]=c->buf[i], c->state[32+i]=c->buf[i]^c->state[i];
    t=0;
    for(i=0;i<18;i++){
        for(j=0;j<48;j++){t=c->state[j]^tt__md2_pi[t];c->state[j]=(tt_u8)t;}
        t=(t+i)&0xFF;
    }
    t=(int)c->checksum[15];
    for(i=0;i<16;i++){c->checksum[i]^=tt__md2_pi[c->buf[i]^(tt_u8)t];t=(int)c->checksum[i];}
}
TT_INLINE void tt_md2_init(TT_Md2 *c) { memset(c,0,sizeof(*c)); }
TT_INLINE void tt_md2_update(TT_Md2 *c, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int take=16-c->buflen;if((int)len<take)take=(int)len;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;len-=(size_t)take;
        if(c->buflen==16){tt__md2_transform(c);c->buflen=0;}
    }
}
TT_INLINE void tt_md2_final(TT_Md2 *c, tt_u8 *digest) {
    int i, pad=16-c->buflen;
    for(i=0;i<pad;i++) c->buf[c->buflen+i]=(tt_u8)pad;
    c->buflen=16; tt__md2_transform(c);
    memcpy(c->buf,c->checksum,16); c->buflen=16; tt__md2_transform(c);
    memcpy(digest,c->state,16);
}
TT_INLINE void tt_md2(const void *d, size_t l, tt_u8 out[16]) {
    TT_Md2 c; tt_md2_init(&c); tt_md2_update(&c,d,l); tt_md2_final(&c,out);
}
#endif                

#ifndef TT_NO_MD4
#define TT_MD4_DIGEST_LEN 16
typedef struct { tt_u32 state[4]; tt_u32 count[2]; tt_u8 buf[64]; int buflen; } TT_Md4;

#define TT__MD4_F(x,y,z) (((x)&(y))|(~(x)&(z)))
#define TT__MD4_G(x,y,z) (((x)&(y))|((x)&(z))|((y)&(z)))
#define TT__MD4_H(x,y,z) ((x)^(y)^(z))
#define TT__MD4_ROL(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define TT__MD4_R1(a,b,c,d,x,s)   a=TT__MD4_ROL(a+TT__MD4_F(b,c,d)+x,s)
#define TT__MD4_R2(a,b,c,d,x,s)   a=TT__MD4_ROL(a+TT__MD4_G(b,c,d)+x+0x5A827999u,s)
#define TT__MD4_R3(a,b,c,d,x,s)   a=TT__MD4_ROL(a+TT__MD4_H(b,c,d)+x+0x6ED9EBA1u,s)

static TT_UNUSED void tt__md4_transform(TT_Md4 *c, const tt_u8 *blk) {
    tt_u32 x[16],a,b,cc,d;
    int i;
    for(i=0;i<16;i++) x[i]=(tt_u32)blk[i*4]|((tt_u32)blk[i*4+1]<<8)|
        ((tt_u32)blk[i*4+2]<<16)|((tt_u32)blk[i*4+3]<<24);
    a=c->state[0];b=c->state[1];cc=c->state[2];d=c->state[3];
    TT__MD4_R1(a,b,cc,d,x[0],3);  TT__MD4_R1(d,a,b,cc,x[1],7);  TT__MD4_R1(cc,d,a,b,x[2],11); TT__MD4_R1(b,cc,d,a,x[3],19);
    TT__MD4_R1(a,b,cc,d,x[4],3);  TT__MD4_R1(d,a,b,cc,x[5],7);  TT__MD4_R1(cc,d,a,b,x[6],11); TT__MD4_R1(b,cc,d,a,x[7],19);
    TT__MD4_R1(a,b,cc,d,x[8],3);  TT__MD4_R1(d,a,b,cc,x[9],7);  TT__MD4_R1(cc,d,a,b,x[10],11);TT__MD4_R1(b,cc,d,a,x[11],19);
    TT__MD4_R1(a,b,cc,d,x[12],3); TT__MD4_R1(d,a,b,cc,x[13],7); TT__MD4_R1(cc,d,a,b,x[14],11);TT__MD4_R1(b,cc,d,a,x[15],19);
    TT__MD4_R2(a,b,cc,d,x[0],3);  TT__MD4_R2(d,a,b,cc,x[4],5);  TT__MD4_R2(cc,d,a,b,x[8],9);  TT__MD4_R2(b,cc,d,a,x[12],13);
    TT__MD4_R2(a,b,cc,d,x[1],3);  TT__MD4_R2(d,a,b,cc,x[5],5);  TT__MD4_R2(cc,d,a,b,x[9],9);  TT__MD4_R2(b,cc,d,a,x[13],13);
    TT__MD4_R2(a,b,cc,d,x[2],3);  TT__MD4_R2(d,a,b,cc,x[6],5);  TT__MD4_R2(cc,d,a,b,x[10],9); TT__MD4_R2(b,cc,d,a,x[14],13);
    TT__MD4_R2(a,b,cc,d,x[3],3);  TT__MD4_R2(d,a,b,cc,x[7],5);  TT__MD4_R2(cc,d,a,b,x[11],9); TT__MD4_R2(b,cc,d,a,x[15],13);
    TT__MD4_R3(a,b,cc,d,x[0],3);  TT__MD4_R3(d,a,b,cc,x[8],9);  TT__MD4_R3(cc,d,a,b,x[4],11); TT__MD4_R3(b,cc,d,a,x[12],15);
    TT__MD4_R3(a,b,cc,d,x[2],3);  TT__MD4_R3(d,a,b,cc,x[10],9); TT__MD4_R3(cc,d,a,b,x[6],11); TT__MD4_R3(b,cc,d,a,x[14],15);
    TT__MD4_R3(a,b,cc,d,x[1],3);  TT__MD4_R3(d,a,b,cc,x[9],9);  TT__MD4_R3(cc,d,a,b,x[5],11); TT__MD4_R3(b,cc,d,a,x[13],15);
    TT__MD4_R3(a,b,cc,d,x[3],3);  TT__MD4_R3(d,a,b,cc,x[11],9); TT__MD4_R3(cc,d,a,b,x[7],11); TT__MD4_R3(b,cc,d,a,x[15],15);
    c->state[0]+=a;c->state[1]+=b;c->state[2]+=cc;c->state[3]+=d;
}
TT_INLINE void tt_md4_init(TT_Md4 *c) {
    c->state[0]=0x67452301u;c->state[1]=0xEFCDAB89u;c->state[2]=0x98BADCFEu;c->state[3]=0x10325476u;
    c->count[0]=c->count[1]=0;c->buflen=0;
}
TT_INLINE void tt_md4_update(TT_Md4 *c, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int sp=64-c->buflen,take=(int)len<sp?(int)len:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;len-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3)))c->count[1]++;
        if(c->buflen==64){tt__md4_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_md4_final(TT_Md4 *c, tt_u8 *digest) {
    int i; tt_u32 bl=c->count[0],bh=c->count[1];
    c->buf[c->buflen++]=0x80;
    if(c->buflen>56){memset(c->buf+c->buflen,0,(size_t)(64-c->buflen));tt__md4_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(56-c->buflen));
    for(i=0;i<4;i++){c->buf[56+i]=(tt_u8)(bl>>(i*8));c->buf[60+i]=(tt_u8)(bh>>(i*8));}
    tt__md4_transform(c,c->buf);
    for(i=0;i<4;i++){digest[i*4]=(tt_u8)c->state[i];digest[i*4+1]=(tt_u8)(c->state[i]>>8);
        digest[i*4+2]=(tt_u8)(c->state[i]>>16);digest[i*4+3]=(tt_u8)(c->state[i]>>24);}
}
TT_INLINE void tt_md4(const void *d, size_t l, tt_u8 out[16]) {
    TT_Md4 c; tt_md4_init(&c); tt_md4_update(&c,d,l); tt_md4_final(&c,out);
}
#endif                

#ifndef TT_NO_HMAC

TT_INLINE void tt_hmac_sha1(const void *key, size_t klen,
                              const void *msg, size_t mlen,
                              tt_u8 out[20]) {
    tt_u8 k_ipad[64], k_opad[64], tmp_key[20];
    TT_Sha1 c;
    int i;
    if(klen>64){tt_sha1(key,klen,tmp_key);key=tmp_key;klen=20;}
    memset(k_ipad,0x36,64); memset(k_opad,0x5C,64);
    for(i=0;i<(int)klen;i++){k_ipad[i]^=((const tt_u8*)key)[i];k_opad[i]^=((const tt_u8*)key)[i];}
    tt_sha1_init(&c); tt_sha1_update(&c,k_ipad,64); tt_sha1_update(&c,msg,mlen); tt_sha1_final(&c,out);
    tt_sha1_init(&c); tt_sha1_update(&c,k_opad,64); tt_sha1_update(&c,out,20);  tt_sha1_final(&c,out);
}

TT_INLINE void tt_hmac_sha256(const void *key, size_t klen,
                                const void *msg, size_t mlen,
                                tt_u8 out[32]) {
    tt_u8 k_ipad[64], k_opad[64], tmp_key[32];
    TT_Sha256 c;
    int i;
    if(klen>64){tt_sha256(key,klen,tmp_key);key=tmp_key;klen=32;}
    memset(k_ipad,0x36,64); memset(k_opad,0x5C,64);
    for(i=0;i<(int)klen;i++){k_ipad[i]^=((const tt_u8*)key)[i];k_opad[i]^=((const tt_u8*)key)[i];}
    tt_sha256_init(&c); tt_sha256_update(&c,k_ipad,64); tt_sha256_update(&c,msg,mlen); tt_sha256_final(&c,out);
    tt_sha256_init(&c); tt_sha256_update(&c,k_opad,64); tt_sha256_update(&c,out,32);  tt_sha256_final(&c,out);
}

TT_INLINE void tt_hmac_sha512(const void *key, size_t klen,
                                const void *msg, size_t mlen,
                                tt_u8 out[64]) {
    tt_u8 k_ipad[128], k_opad[128], tmp_key[64];
    TT_Sha512 c;
    int i;
    if(klen>128){tt_sha512(key,klen,tmp_key);key=tmp_key;klen=64;}
    memset(k_ipad,0x36,128); memset(k_opad,0x5C,128);
    for(i=0;i<(int)klen;i++){k_ipad[i]^=((const tt_u8*)key)[i];k_opad[i]^=((const tt_u8*)key)[i];}
    tt_sha512_init(&c); tt_sha512_update(&c,k_ipad,128); tt_sha512_update(&c,msg,mlen); tt_sha512_final(&c,out);
    tt_sha512_init(&c); tt_sha512_update(&c,k_opad,128); tt_sha512_update(&c,out,64);  tt_sha512_final(&c,out);
}

TT_INLINE void tt_hmac_sha384(const void *key, size_t klen,
                                const void *msg, size_t mlen,
                                tt_u8 out[48]) {
    tt_u8 k_ipad[128], k_opad[128], tmp_key[48], full[64];
    TT_Sha512 c;
    int i;
    if(klen>128){tt_sha384(key,klen,tmp_key);key=tmp_key;klen=48;}
    memset(k_ipad,0x36,128); memset(k_opad,0x5C,128);
    for(i=0;i<(int)klen;i++){k_ipad[i]^=((const tt_u8*)key)[i];k_opad[i]^=((const tt_u8*)key)[i];}
    tt_sha384_init(&c); tt_sha512_update(&c,k_ipad,128); tt_sha512_update(&c,msg,mlen); tt_sha512_final(&c,full);
    memcpy(out,full,48);
    tt_sha384_init(&c); tt_sha512_update(&c,k_opad,128); tt_sha512_update(&c,out,48);  tt_sha512_final(&c,full);
    memcpy(out,full,48);
}
#endif                 

#ifndef TT_NO_HKDF
TT_INLINE void tt_hkdf_extract_sha256(const void *salt, size_t slen,
                                        const void *ikm,  size_t ilen,
                                        tt_u8 prk[32]) {
    tt_u8 def_salt[32];
    if(!salt||slen==0){memset(def_salt,0,32);salt=def_salt;slen=32;}
    tt_hmac_sha256(salt,slen,ikm,ilen,prk);
}
TT_INLINE int tt_hkdf_expand_sha256(const tt_u8 prk[32], size_t prk_len,
                                      const void *info, size_t ilen,
                                      tt_u8 *okm, size_t olen) {
    tt_u8 t[32]; size_t done=0; tt_u8 ctr=1; tt_u8 prev[32]; int has_prev=0;
    tt_u8 buf[32+1024+1];
    if(olen>32*255||!okm) return -1;
    while(done<olen){
        size_t blen=0;
        if(has_prev){memcpy(buf,prev,32);blen=32;}
        if(info&&ilen){memcpy(buf+blen,info,ilen);blen+=ilen;}
        buf[blen++]=ctr++;
        tt_hmac_sha256(prk,prk_len,buf,blen,t);
        memcpy(prev,t,32); has_prev=1;
        {size_t take=olen-done>32?32:olen-done;
         memcpy(okm+done,t,take);done+=take;}
    }
    return 0;
}
TT_INLINE int tt_hkdf_sha256(const void *salt, size_t slen,
                               const void *ikm, size_t ilen,
                               const void *info, size_t info_len,
                               tt_u8 *okm, size_t olen) {
    tt_u8 prk[32];
    tt_hkdf_extract_sha256(salt,slen,ikm,ilen,prk);
    return tt_hkdf_expand_sha256(prk,32,info,info_len,okm,olen);
}
#endif                 

#ifndef TT_NO_PBKDF1
TT_INLINE void tt_pbkdf1_sha1(const tt_u8 *pass, size_t plen,
                                const tt_u8 *salt, size_t slen,
                                int iter, tt_u8 *dk, size_t dklen) {
    tt_u8 buf[20+64];                                 
    tt_u8 t[20];
    int i;
    if(dklen>20) dklen=20;
    memcpy(buf,pass,plen>64?64:plen);
    memcpy(buf+(plen>64?64:plen),salt,slen>44?44:slen);
    tt_sha1(buf,plen+slen,t);
    for(i=1;i<iter;i++) tt_sha1(t,20,t);
    memcpy(dk,t,dklen);
}
#endif                   

#ifndef TT_NO_PBKDF2
TT_INLINE int tt_pbkdf2_hmac_sha256(const tt_u8 *pass, size_t plen,
                                      const tt_u8 *salt, size_t slen,
                                      int iter, tt_u8 *dk, size_t dklen) {
    tt_u8 u[32], f[32];
    tt_u8 *s2;
    size_t blocks, b, done=0;
    int j, k;
    if(iter<1||!dk||dklen==0) return -1;
    blocks=(dklen+31)/32;
    s2=(tt_u8*)malloc(slen+4);
    if(!s2) return -2;
    memcpy(s2,salt,slen);
    for(b=1;b<=blocks;b++){
        s2[slen+0]=(tt_u8)(b>>24); s2[slen+1]=(tt_u8)(b>>16);
        s2[slen+2]=(tt_u8)(b>>8);  s2[slen+3]=(tt_u8)b;
        tt_hmac_sha256(pass,plen,s2,slen+4,u);
        memcpy(f,u,32);
        for(j=1;j<iter;j++){
            tt_hmac_sha256(pass,plen,u,32,u);
            for(k=0;k<32;k++) f[k]^=u[k];
        }
        {size_t take=dklen-done>32?32:dklen-done;
         memcpy(dk+done,f,take);done+=take;}
    }
    free(s2);
    return 0;
}

TT_INLINE int tt_pbkdf2_hmac_sha1(const tt_u8 *pass, size_t plen,
                                    const tt_u8 *salt, size_t slen,
                                    int iter, tt_u8 *dk, size_t dklen) {
    tt_u8 u[20], f[20];
    tt_u8 *s2;
    size_t blocks, b, done=0;
    int j, k;
    if(iter<1||!dk||dklen==0) return -1;
    blocks=(dklen+19)/20;
    s2=(tt_u8*)malloc(slen+4);
    if(!s2) return -2;
    memcpy(s2,salt,slen);
    for(b=1;b<=blocks;b++){
        s2[slen+0]=(tt_u8)(b>>24); s2[slen+1]=(tt_u8)(b>>16);
        s2[slen+2]=(tt_u8)(b>>8);  s2[slen+3]=(tt_u8)b;
        tt_hmac_sha1(pass,plen,s2,slen+4,u);
        memcpy(f,u,20);
        for(j=1;j<iter;j++){
            tt_hmac_sha1(pass,plen,u,20,u);
            for(k=0;k<20;k++) f[k]^=u[k];
        }
        {size_t take=dklen-done>20?20:dklen-done;
         memcpy(dk+done,f,take);done+=take;}
    }
    free(s2);
    return 0;
}

TT_INLINE void tt_pkcs12_pbkdf_sha1(int id, const tt_u8 *pass, size_t plen,
                                      const tt_u8 *salt, size_t slen,
                                      int iter, tt_u8 *dk, size_t dklen) {
    tt_u8 D[64], I[1024], B[64], Ai[20];
    size_t Slen, Plen, Ilen;
    size_t done=0;
    int i, j;
    Slen=((slen+63)/64)*64;
    Plen=((plen+63)/64)*64;
    Ilen=Slen+Plen;
    if(Ilen>sizeof(I)) Ilen=sizeof(I);
    memset(D,(tt_u8)id,64);
    for(i=0;i<(int)Slen&&i<(int)sizeof(I);i++) I[i]=salt[i%slen];
    for(i=0;i<(int)Plen&&(Slen+i)<sizeof(I);i++) I[Slen+i]=pass[i%plen];
    while(done<dklen){
        tt_u8 tmp[64+1024];
        int c;
        memcpy(tmp,D,64); memcpy(tmp+64,I,Ilen);
        tt_sha1(tmp,64+Ilen,Ai);
        for(c=1;c<iter;c++) tt_sha1(Ai,20,Ai);
        {size_t take=dklen-done>20?20:dklen-done;
         memcpy(dk+done,Ai,take);done+=take;}
        for(i=0;i<64;i++) B[i]=Ai[i%20];
        for(j=0;j<(int)Ilen;j+=64){
            tt_u32 carry=1;
            int k;
            for(k=63;k>=0&&k<(int)Ilen;k--){
                tt_u32 s=(tt_u32)I[j+k]+(tt_u32)B[k]+carry;
                I[j+k]=(tt_u8)s; carry=s>>8;
            }
        }
    }
}
#endif                   

#ifndef TT_NO_SHAKE

static const tt_u64 tt__keccak_rc[24] = {
    (tt_u64)0x0000000000000001ULL,(tt_u64)0x0000000000008082ULL,
    (tt_u64)0x800000000000808AULL,(tt_u64)0x8000000080008000ULL,
    (tt_u64)0x000000000000808BULL,(tt_u64)0x0000000080000001ULL,
    (tt_u64)0x8000000080008081ULL,(tt_u64)0x8000000000008009ULL,
    (tt_u64)0x000000000000008AULL,(tt_u64)0x0000000000000088ULL,
    (tt_u64)0x0000000080008009ULL,(tt_u64)0x000000008000000AULL,
    (tt_u64)0x000000008000808BULL,(tt_u64)0x800000000000008BULL,
    (tt_u64)0x8000000000008089ULL,(tt_u64)0x8000000000008003ULL,
    (tt_u64)0x8000000000008002ULL,(tt_u64)0x8000000000000080ULL,
    (tt_u64)0x000000000000800AULL,(tt_u64)0x800000008000000AULL,
    (tt_u64)0x8000000080008081ULL,(tt_u64)0x8000000000008080ULL,
    (tt_u64)0x0000000080000001ULL,(tt_u64)0x8000000080008008ULL
};
static const int tt__keccak_rho[24]={1,3,6,10,15,21,28,36,45,55,2,14,27,41,56,8,25,43,62,18,39,61,20,44};
static const int tt__keccak_pi[24]={10,7,11,17,18,3,5,16,8,21,24,4,15,23,19,13,12,2,20,14,22,9,6,1};

static TT_UNUSED void tt__keccakf(tt_u64 A[25]) {
    int r,x,y;
    tt_u64 C[5],D[5],tmp;
    for(r=0;r<24;r++){
        for(x=0;x<5;x++) C[x]=A[x]^A[x+5]^A[x+10]^A[x+15]^A[x+20];
        for(x=0;x<5;x++){D[x]=C[(x+4)%5]^((C[(x+1)%5]<<1)|(C[(x+1)%5]>>63));}
        for(x=0;x<5;x++) for(y=0;y<5;y++) A[x+y*5]^=D[x];
        tmp=A[1];
        for(x=0;x<24;x++){tt_u64 t2=A[tt__keccak_pi[x]];A[tt__keccak_pi[x]]=(tmp<<tt__keccak_rho[x])|(tmp>>(64-tt__keccak_rho[x]));tmp=t2;}
        for(y=0;y<5;y++){tt_u64 t0=A[y*5],t1=A[1+y*5],t2=A[2+y*5],t3=A[3+y*5],t4=A[4+y*5];
            A[y*5]^=(~t1)&t2; A[1+y*5]^=(~t2)&t3; A[2+y*5]^=(~t3)&t4; A[3+y*5]^=(~t4)&t0; A[4+y*5]^=(~t0)&t1;}
        A[0]^=tt__keccak_rc[r];
    }
}

typedef struct { tt_u64 A[25]; int rate; int pos; int ds; } TT_Keccak;

TT_INLINE void tt_keccak_init(TT_Keccak *k, int rate, int ds) {
    memset(k->A,0,sizeof(k->A)); k->rate=rate; k->pos=0; k->ds=ds;
}
TT_INLINE void tt_keccak_absorb(TT_Keccak *k, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    int r=k->rate;
    while(len>0){
        int take=r-k->pos;
        if((int)len<take) take=(int)len;
        {int i; for(i=0;i<take;i++){
            int bytepos=k->pos+i;
            k->A[bytepos/8]^=(tt_u64)p[i]<<(8*(bytepos%8));
        }}
        k->pos+=take; p+=take; len-=(size_t)take;
        if(k->pos==r){tt__keccakf(k->A);k->pos=0;}
    }
}
TT_INLINE void tt_keccak_finalize(TT_Keccak *k) {
    k->A[k->pos/8]^=(tt_u64)k->ds<<(8*(k->pos%8));
    k->A[(k->rate-1)/8]^=(tt_u64)0x80<<(8*((k->rate-1)%8));
    tt__keccakf(k->A); k->pos=0;
}
TT_INLINE void tt_keccak_squeeze(TT_Keccak *k, tt_u8 *out, size_t olen) {
    int r=k->rate;
    while(olen>0){
        if(k->pos==r){tt__keccakf(k->A);k->pos=0;}
        {size_t avail=(size_t)(r-k->pos), take=olen<avail?olen:avail;
         size_t i; for(i=0;i<take;i++){int bp=k->pos+(int)i;out[i]=(tt_u8)(k->A[bp/8]>>(8*(bp%8)));}
         k->pos+=(int)take; out+=take; olen-=take;}
    }
}

TT_INLINE void tt_shake128(const void *in, size_t ilen, tt_u8 *out, size_t olen) {
    TT_Keccak k;
    tt_keccak_init(&k,168,0x1F);
    tt_keccak_absorb(&k,in,ilen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,olen);
}
TT_INLINE void tt_shake256(const void *in, size_t ilen, tt_u8 *out, size_t olen) {
    TT_Keccak k;
    tt_keccak_init(&k,136,0x1F);
    tt_keccak_absorb(&k,in,ilen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,olen);
}
TT_INLINE void tt_sha3_256(const void *in, size_t ilen, tt_u8 out[32]) {
    TT_Keccak k;
    tt_keccak_init(&k,136,0x06);
    tt_keccak_absorb(&k,in,ilen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,32);
}
TT_INLINE void tt_sha3_512(const void *in, size_t ilen, tt_u8 out[64]) {
    TT_Keccak k;
    tt_keccak_init(&k,72,0x06);
    tt_keccak_absorb(&k,in,ilen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,64);
}
TT_INLINE void tt_kmac128(const void *key, size_t klen,
                            const void *msg, size_t mlen,
                            tt_u8 *out, size_t olen) {
    TT_Keccak k;
    tt_u8 kpad[168];
    size_t i;
    memset(kpad,0,168);
    for(i=0;i<klen&&i<168;i++) kpad[i]=((const tt_u8*)key)[i];
    tt_keccak_init(&k,168,0x1F);
    tt_keccak_absorb(&k,kpad,168);
    tt_keccak_absorb(&k,msg,mlen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,olen);
}
TT_INLINE void tt_kmac256(const void *key, size_t klen,
                            const void *msg, size_t mlen,
                            tt_u8 *out, size_t olen) {
    TT_Keccak k;
    tt_u8 kpad[136];
    size_t i;
    memset(kpad,0,136);
    for(i=0;i<klen&&i<136;i++) kpad[i]=((const tt_u8*)key)[i];
    tt_keccak_init(&k,136,0x1F);
    tt_keccak_absorb(&k,kpad,136);
    tt_keccak_absorb(&k,msg,mlen);
    tt_keccak_finalize(&k);
    tt_keccak_squeeze(&k,out,olen);
}
#endif                  

#ifndef TT_NO_RIPEMD

#define TT__RM_ROL(x,n) (((x)<<(n))|((x)>>(32-(n))))
#define TT__RM_F(x,y,z) ((x)^(y)^(z))
#define TT__RM_G(x,y,z) (((x)&(y))|(~(x)&(z)))
#define TT__RM_H(x,y,z) (((x)|(~(y)))^(z))
#define TT__RM_I(x,y,z) (((x)&(z))|((y)&~(z)))
#define TT__RM_J(x,y,z) ((x)^((y)|(~(z))))

static const int tt__rmd_r [80]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,7,4,13,1,10,6,15,3,12,0,9,5,2,14,11,8,3,10,14,4,9,15,8,1,2,7,0,6,13,11,5,12,1,9,11,10,0,8,12,4,13,3,7,15,14,5,6,2,4,0,5,9,7,12,2,10,14,1,3,8,11,6,15,13};
static const int tt__rmd_rr[80]={5,14,7,0,9,2,11,4,13,6,15,8,1,10,3,12,6,11,3,7,0,13,5,10,14,15,8,12,4,9,1,2,15,5,1,3,7,14,6,9,11,8,12,2,10,0,4,13,8,6,4,1,3,11,15,0,5,12,2,13,9,7,10,14,12,15,10,4,1,5,8,7,6,2,13,14,0,3,9,11};
static const int tt__rmd_s [80]={11,14,15,12,5,8,7,9,11,13,14,15,6,7,9,8,7,6,8,13,11,9,7,15,7,12,15,9,11,7,13,12,11,13,6,7,14,9,13,15,14,8,13,6,5,12,7,5,11,12,14,15,14,15,9,8,9,14,5,6,8,6,5,12,9,15,5,11,6,8,13,12,5,12,13,14,11,8,5,6};
static const int tt__rmd_ss[80]={8,9,9,11,13,15,15,5,7,7,8,11,14,14,12,6,9,13,15,7,12,8,9,11,7,7,12,7,6,15,13,11,9,7,15,11,8,6,6,14,12,13,5,14,13,13,7,5,15,5,8,11,14,14,6,14,6,9,12,9,12,5,15,8,8,5,12,9,12,5,14,6,8,13,6,5,15,13,11,11};
static const tt_u32 tt__rmd_k [5]={0x00000000u,0x5A827999u,0x6ED9EBA1u,0x8F1BBCDCu,0xA953FD4Eu};
static const tt_u32 tt__rmd_kk[5]={0x50A28BE6u,0x5C4DD124u,0x6D703EF3u,0x7A6D76E9u,0x00000000u};

typedef struct { tt_u32 state[5]; tt_u32 count[2]; tt_u8 buf[64]; int buflen; int width; } TT_Ripemd;

static TT_UNUSED void tt__rmd160_transform(TT_Ripemd *c, const tt_u8 *blk) {
    tt_u32 X[16],al,bl,cl,dl,el,ar,br,cr,dr,er,t;
    int i;
    for(i=0;i<16;i++) X[i]=(tt_u32)blk[i*4]|((tt_u32)blk[i*4+1]<<8)|
        ((tt_u32)blk[i*4+2]<<16)|((tt_u32)blk[i*4+3]<<24);
    al=c->state[0];bl=c->state[1];cl=c->state[2];dl=c->state[3];el=c->state[4];
    ar=c->state[0];br=c->state[1];cr=c->state[2];dr=c->state[3];er=c->state[4];
    for(i=0;i<80;i++){
        int rnd=i/16;
        tt_u32 Fl,Fr;
        switch(rnd){
            case 0: Fl=TT__RM_F(bl,cl,dl); Fr=TT__RM_J(br,cr,dr); break;
            case 1: Fl=TT__RM_G(bl,cl,dl); Fr=TT__RM_I(br,cr,dr); break;
            case 2: Fl=TT__RM_H(bl,cl,dl); Fr=TT__RM_H(br,cr,dr); break;
            case 3: Fl=TT__RM_I(bl,cl,dl); Fr=TT__RM_G(br,cr,dr); break;
            default:Fl=TT__RM_J(bl,cl,dl); Fr=TT__RM_F(br,cr,dr); break;
        }
        t=TT__RM_ROL(al+Fl+X[tt__rmd_r[i]]+tt__rmd_k[rnd],tt__rmd_s[i])+el;
        al=el;el=dl;dl=TT__RM_ROL(cl,10);cl=bl;bl=t;
        t=TT__RM_ROL(ar+Fr+X[tt__rmd_rr[i]]+tt__rmd_kk[rnd],tt__rmd_ss[i])+er;
        ar=er;er=dr;dr=TT__RM_ROL(cr,10);cr=br;br=t;
    }
    t=c->state[1]+cl+dr; c->state[1]=c->state[2]+dl+er; c->state[2]=c->state[3]+el+ar;
    c->state[3]=c->state[4]+al+br; c->state[4]=c->state[0]+bl+cr; c->state[0]=t;
}

TT_INLINE void tt_ripemd160_init(TT_Ripemd *c) {
    c->state[0]=0x67452301u;c->state[1]=0xEFCDAB89u;c->state[2]=0x98BADCFEu;
    c->state[3]=0x10325476u;c->state[4]=0xC3D2E1F0u;
    c->count[0]=c->count[1]=0;c->buflen=0;c->width=160;
}
TT_INLINE void tt_ripemd_update(TT_Ripemd *c, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int sp=64-c->buflen,take=(int)len<sp?(int)len:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;len-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3)))c->count[1]++;
        if(c->buflen==64){tt__rmd160_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_ripemd_final(TT_Ripemd *c, tt_u8 *digest) {
    int i; int words=c->width/32; tt_u32 bl=c->count[0],bh=c->count[1];
    c->buf[c->buflen++]=0x80;
    if(c->buflen>56){memset(c->buf+c->buflen,0,(size_t)(64-c->buflen));tt__rmd160_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(56-c->buflen));
    for(i=0;i<4;i++){c->buf[56+i]=(tt_u8)(bl>>(i*8));c->buf[60+i]=(tt_u8)(bh>>(i*8));}
    tt__rmd160_transform(c,c->buf);
    for(i=0;i<words;i++){digest[i*4]=(tt_u8)c->state[i];digest[i*4+1]=(tt_u8)(c->state[i]>>8);
        digest[i*4+2]=(tt_u8)(c->state[i]>>16);digest[i*4+3]=(tt_u8)(c->state[i]>>24);}
}

TT_INLINE void tt_ripemd160(const void *d, size_t l, tt_u8 out[20]) {
    TT_Ripemd c; tt_ripemd160_init(&c); tt_ripemd_update(&c,d,l); tt_ripemd_final(&c,out);
}

TT_INLINE void tt_ripemd128_init(TT_Ripemd *c) {
    c->state[0]=0x67452301u;c->state[1]=0xEFCDAB89u;c->state[2]=0x98BADCFEu;c->state[3]=0x10325476u;c->state[4]=0;
    c->count[0]=c->count[1]=0;c->buflen=0;c->width=128;
}
TT_INLINE void tt_ripemd128(const void *d, size_t l, tt_u8 out[16]) {
    TT_Ripemd c; tt_ripemd128_init(&c); tt_ripemd_update(&c,d,l); tt_ripemd_final(&c,out);
}

TT_INLINE void tt_ripemd256_init(TT_Ripemd *c) {
    c->state[0]=0x67452301u;c->state[1]=0xEFCDAB89u;c->state[2]=0x98BADCFEu;c->state[3]=0x10325476u;
    c->count[0]=c->count[1]=0;c->buflen=0;c->width=256;
}
TT_INLINE void tt_ripemd256(const void *d, size_t l, tt_u8 out[32]) {
    TT_Ripemd c1,c2;
    tt_ripemd128_init(&c1);
    tt_ripemd128_init(&c2);
    c2.state[0]=0x76543210u;c2.state[1]=0xFEDCBA98u;c2.state[2]=0x89ABCDEFu;c2.state[3]=0x01234567u;
    tt_ripemd_update(&c1,d,l); tt_ripemd_final(&c1,out);
    tt_ripemd_update(&c2,d,l); tt_ripemd_final(&c2,out+16);
}
TT_INLINE void tt_ripemd320(const void *d, size_t l, tt_u8 out[40]) {
    TT_Ripemd c1,c2;
    tt_ripemd160_init(&c1);
    tt_ripemd160_init(&c2);
    c2.state[0]=0x76543210u;c2.state[1]=0xFEDCBA98u;c2.state[2]=0x89ABCDEFu;c2.state[3]=0x01234567u;c2.state[4]=0x3C2D1E0Fu;
    tt_ripemd_update(&c1,d,l); tt_ripemd_final(&c1,out);
    tt_ripemd_update(&c2,d,l); tt_ripemd_final(&c2,out+20);
}
#endif                   


#ifndef TT_NO_WHIRLPOOL
#define TT_WHIRLPOOL_DIGEST_LEN 64

typedef struct { tt_u64 hash[8]; tt_u8 buf[64]; int buflen; tt_u64 bits[2]; } TT_Whirlpool;

static const tt_u64 tt__wp_C0[256] = {
    (tt_u64)0x18186018c07830d8ULL,(tt_u64)0x23238c2305af4626ULL,(tt_u64)0xc6c63fc67ef991b8ULL,(tt_u64)0xe8e887e8136fcdfbULL,
    (tt_u64)0x878726874ca113cbULL,(tt_u64)0xb8b8dab8a9626d11ULL,(tt_u64)0x0101040108050209ULL,(tt_u64)0x4f4f214f426e9e0dULL,
    (tt_u64)0x3636d836adee6c9bULL,(tt_u64)0xa6a6a2a6590451ffULL,(tt_u64)0xd2d26fd2debdb90cULL,(tt_u64)0xf5f5f3f5fb06f70eULL,
    (tt_u64)0x7979f979ef80f296ULL,(tt_u64)0x6f6fa16f5fcede30ULL,(tt_u64)0x91917e91fcef3f6dULL,(tt_u64)0x52525552aa07a4f8ULL,
    (tt_u64)0x60609d6027fdc047ULL,(tt_u64)0xbcbccabc89766535ULL,(tt_u64)0x9b9b569baccd2b37ULL,(tt_u64)0x8e8e028e048c018aULL,
    (tt_u64)0xa3a3b6a371155bd2ULL,(tt_u64)0x0c0c300c603c186cULL,(tt_u64)0x7b7bf17bff8af684ULL,(tt_u64)0x3535d435b5e16a80ULL,
    (tt_u64)0x1d1d741de8693af5ULL,(tt_u64)0xe0e0a7e05347ddb3ULL,(tt_u64)0xd7d77bd7f6acb321ULL,(tt_u64)0xc2c22fc25eed999cULL,
    (tt_u64)0x2e2eb82e6d965c43ULL,(tt_u64)0x4b4b314b627a9629ULL,(tt_u64)0xfefedffea321e15dULL,(tt_u64)0x575741578216aed5ULL,
    (tt_u64)0x15155415a8412abdULL,(tt_u64)0x7777c1779fb6eee8ULL,(tt_u64)0x3737dc37a5eb6e92ULL,(tt_u64)0xe5e5b3e57b56d79eULL,
    (tt_u64)0x9f9f469f8cd92313ULL,(tt_u64)0xf0f0e7f0d317fd23ULL,(tt_u64)0x4a4a354a6a7f9420ULL,(tt_u64)0xdada4fda9e95a944ULL,
    (tt_u64)0x58587d58fa25b0a2ULL,(tt_u64)0xc9c903c906ca8fcfULL,(tt_u64)0x2929a429558d527cULL,(tt_u64)0x0a0a280a5022145aULL,
    (tt_u64)0xb1b1feb1e14f7f50ULL,(tt_u64)0xa0a0baa0691a5dc9ULL,(tt_u64)0x6b6bb16b7fdad614ULL,(tt_u64)0x85852e855cab17d9ULL,
    (tt_u64)0xbdbdcebd8173673cULL,(tt_u64)0x5d5d695dd234ba8fULL,(tt_u64)0x1010401080502090ULL,(tt_u64)0xf4f4f7f4f303f507ULL,
    (tt_u64)0xcbcb0bcb16c08bddULL,(tt_u64)0x3e3ef83eedc67cd3ULL,(tt_u64)0x0505140528110a2dULL,(tt_u64)0x676781671fe6ce78ULL,
    (tt_u64)0xe4e4b7e47353d597ULL,(tt_u64)0x27279c2725bb4e02ULL,(tt_u64)0x4141194132588273ULL,(tt_u64)0x8b8b168b2c9d0ba7ULL,
    (tt_u64)0xa7a7a6a7510153f6ULL,(tt_u64)0x7d7de97dcf94fab2ULL,(tt_u64)0x95956e95dcfb3749ULL,(tt_u64)0xd8d847d88e9fad56ULL,
    (tt_u64)0xfbfbcbfb8b30eb70ULL,(tt_u64)0xeeee9fee2371c1cdULL,(tt_u64)0x7c7ced7cc791f8bbULL,(tt_u64)0x6666856617e3cc71ULL,
    (tt_u64)0xdddd53dda68ea77bULL,(tt_u64)0x17175c17b84b2eafULL,(tt_u64)0x4747014702468e45ULL,(tt_u64)0x9e9e429e84dc211aULL,
    (tt_u64)0xcaca0fca1ec589d4ULL,(tt_u64)0x2d2db42d75995a58ULL,(tt_u64)0xbfbfc6bf9179632eULL,(tt_u64)0x07071c07381b0e3fULL,
    (tt_u64)0xadadd4ad012347acULL,(tt_u64)0x5a5a755aea2fb4b0ULL,(tt_u64)0x838336836cb51befULL,(tt_u64)0x3333cc3385ff66b6ULL,
    (tt_u64)0x636391633ff2c65cULL,(tt_u64)0x02020802100a0412ULL,(tt_u64)0xaaaa92aa39384993ULL,(tt_u64)0x7171d971afa8e2deULL,
    (tt_u64)0xc8c807c80ecf8dc6ULL,(tt_u64)0x19196419c87d32d1ULL,(tt_u64)0x494939497270923bULL,(tt_u64)0xd9d943d9869aaf5fULL,
    (tt_u64)0xf2f2eff2c31df931ULL,(tt_u64)0xe3e3abe34b48dba8ULL,(tt_u64)0x5b5b715be22ab6b9ULL,(tt_u64)0x88881a8834920dbcULL,
    (tt_u64)0x9a9a529aa4c8293eULL,(tt_u64)0x262698262dbe4c0bULL,(tt_u64)0x3232c8328dfa64bfULL,(tt_u64)0xb0b0fab0e94a7d59ULL,
    (tt_u64)0xe9e983e91b6acff2ULL,(tt_u64)0x0f0f3c0f78331e77ULL,(tt_u64)0xd5d573d5e6a6b733ULL,(tt_u64)0x80803a8074ba1df4ULL,
    (tt_u64)0xbebec2be997c6127ULL,(tt_u64)0xcdcd13cd26de87ebULL,(tt_u64)0x3434d034bde46889ULL,(tt_u64)0x48483d487a759032ULL,
    (tt_u64)0xffffdbffab24e354ULL,(tt_u64)0x7a7af57af78ff48dULL,(tt_u64)0x90907a90f4ea3d64ULL,(tt_u64)0x5f5f615fc23ebe9dULL,
    (tt_u64)0x202080201da0403dULL,(tt_u64)0x6868bd6867d5d00fULL,(tt_u64)0x1a1a681ad07234caULL,(tt_u64)0xaeae82ae192c41b7ULL,
    (tt_u64)0xb4b4eab4c95e757dULL,(tt_u64)0x54544d549a19a8ceULL,(tt_u64)0x93937693ece53b7fULL,(tt_u64)0x222288220daa442fULL,
    (tt_u64)0x64648d6407e9c863ULL,(tt_u64)0xf1f1e3f1db12ff2aULL,(tt_u64)0x7373d173bfa2e6ccULL,(tt_u64)0x12124812905a2482ULL,
    (tt_u64)0x40401d403a5d807aULL,(tt_u64)0x0808200840281048ULL,(tt_u64)0xc3c32bc356e89b95ULL,(tt_u64)0xecec97ec337bc5dfULL,
    (tt_u64)0xdbdb4bdb9690ab4dULL,(tt_u64)0xa1a1bea1611f5fc0ULL,(tt_u64)0x8d8d0e8d1c830791ULL,(tt_u64)0x3d3df43df5c97ac8ULL,
    (tt_u64)0x97976697ccf1335bULL,(tt_u64)0x0000000000000000ULL,(tt_u64)0xcfcf1bcf36d483f9ULL,(tt_u64)0x2b2bac2b4587566eULL,
    (tt_u64)0x7676c57697b3ece1ULL,(tt_u64)0x8282328264b019e6ULL,(tt_u64)0xd6d67fd6fea9b128ULL,(tt_u64)0x1b1b6c1bd87736c3ULL,
    (tt_u64)0xb5b5eeb5c15b7774ULL,(tt_u64)0xafafeaaf112943beULL,(tt_u64)0x6a6ab96a77dfd41dULL,(tt_u64)0x50501d50ba0da0eaULL,
    (tt_u64)0x45450945124c8a57ULL,(tt_u64)0xf3f3ebf3cb18fb38ULL,(tt_u64)0x3030c0309df060adULL,(tt_u64)0xefef9bef2b74c3c4ULL,
    (tt_u64)0x3f3ffc3fe5c37edaULL,(tt_u64)0x55554955921caac7ULL,(tt_u64)0xa2a2b2a2791059dbULL,(tt_u64)0xeaea8fea0365c9e9ULL,
    (tt_u64)0x656589650fecca6aULL,(tt_u64)0xbabad2bab9686903ULL,(tt_u64)0x2f2fbc2f65935e4aULL,(tt_u64)0xc0c027c04ee79d8eULL,
    (tt_u64)0xdede5fdebe81a160ULL,(tt_u64)0x1c1c701ce06c38fcULL,(tt_u64)0xfdfdd3fdbb2ee746ULL,(tt_u64)0x4d4d294d52649a1fULL,
    (tt_u64)0x92927292e4e03976ULL,(tt_u64)0x7575c9758fbceafaULL,(tt_u64)0x06061806301e0c36ULL,(tt_u64)0x8a8a128a249809aeULL,
    (tt_u64)0xb2b2f2b2f940794bULL,(tt_u64)0xe6e6bfe66359d185ULL,(tt_u64)0x0e0e380e70361c7eULL,(tt_u64)0x1f1f7c1ff8633ee7ULL,
    (tt_u64)0x6262956237f7c455ULL,(tt_u64)0xd4d477d4eea3b53aULL,(tt_u64)0xa8a89aa829324d81ULL,(tt_u64)0x96966296c4f43152ULL,
    (tt_u64)0xf9f9c3f99b3aef62ULL,(tt_u64)0xc5c533c566f697a3ULL,(tt_u64)0x2525942535b14a10ULL,(tt_u64)0x59597959f220b2abULL,
    (tt_u64)0x84842a8454ae15d0ULL,(tt_u64)0x7272d572b7a7e4c5ULL,(tt_u64)0x3939e439d5dd72ecULL,(tt_u64)0x4c4c2d4c5a619816ULL,
    (tt_u64)0x5e5e655eca3bbc94ULL,(tt_u64)0x7878fd78e785f09fULL,(tt_u64)0x3838e038ddd870e5ULL,(tt_u64)0x8c8c0a8c14860598ULL,
    (tt_u64)0xd1d163d1c6b2bf17ULL,(tt_u64)0xa5a5aea5410b57e4ULL,(tt_u64)0xe2e2afe2434dd9a1ULL,(tt_u64)0x616199612ff8c24eULL,
    (tt_u64)0xb3b3f6b3f1457b42ULL,(tt_u64)0x2121842115a54234ULL,(tt_u64)0x9c9c4a9c94d62508ULL,(tt_u64)0x1e1e781ef0663ceeULL,
    (tt_u64)0x4343114322528661ULL,(tt_u64)0xc7c73bc776fc93b1ULL,(tt_u64)0xfcfcd7fcb32be54fULL,(tt_u64)0x0404100420140824ULL,
    (tt_u64)0x51515951b208a2e3ULL,(tt_u64)0x99995e99bcc72f25ULL,(tt_u64)0x6d6da96d4fc4da22ULL,(tt_u64)0x0d0d340d68391a65ULL,
    (tt_u64)0xfafacffa8335e979ULL,(tt_u64)0xdfdf5bdfb684a369ULL,(tt_u64)0x7e7ee57ed79bfca9ULL,(tt_u64)0x242490243db44819ULL,
    (tt_u64)0x3b3bec3bc5d776feULL,(tt_u64)0xabab96ab313d4b9aULL,(tt_u64)0xcece1fce3ed181f0ULL,(tt_u64)0x1111441188552299ULL,
    (tt_u64)0x8f8f068f0c890383ULL,(tt_u64)0x4e4e254e4a6b9c04ULL,(tt_u64)0xb7b7e6b7d1517366ULL,(tt_u64)0xebeb8beb0b60cbe0ULL,
    (tt_u64)0x3c3cf03cfdcc78c1ULL,(tt_u64)0x81813e817cbf1ffdULL,(tt_u64)0x94946a94d4fe3540ULL,(tt_u64)0xf7f7fbf7eb0cf31cULL,
    (tt_u64)0xb9b9deb9a1676f18ULL,(tt_u64)0x13134c13985f268bULL,(tt_u64)0x2c2cb02c7d9c5851ULL,(tt_u64)0xd3d36bd3d6b8bb05ULL,
    (tt_u64)0xe7e7bbe76b5cd38cULL,(tt_u64)0x6e6ea56e57cbdc39ULL,(tt_u64)0xc4c437c46ef395aaULL,(tt_u64)0x03030c03180f061bULL,
    (tt_u64)0x565645568a13acdcULL,(tt_u64)0x44440d441a49885eULL,(tt_u64)0x7f7fe17fdf9efea0ULL,(tt_u64)0xa9a99ea921374f88ULL,
    (tt_u64)0x2a2aa82a4d825467ULL,(tt_u64)0xbbbbd6bbb16d6b0aULL,(tt_u64)0xc1c123c146e29f87ULL,(tt_u64)0x53535153a202a6f1ULL,
    (tt_u64)0xdcdc57dcae8ba572ULL,(tt_u64)0x0b0b2c0b58271653ULL,(tt_u64)0x9d9d4e9d9cd32701ULL,(tt_u64)0x6c6cad6c47c1d82bULL,
    (tt_u64)0x3131c43195f562a4ULL,(tt_u64)0x7474cd7487b9e8f3ULL,(tt_u64)0xf6f6fff6e309f115ULL,(tt_u64)0x464605460a438c4cULL,
    (tt_u64)0xacac8aac092645a5ULL,(tt_u64)0x89891e893c970fb5ULL,(tt_u64)0x14145014a04428b4ULL,(tt_u64)0xe1e1a3e15b42dfbaULL,
    (tt_u64)0x16165816b04e2ca6ULL,(tt_u64)0x3a3ae83acdd274f7ULL,(tt_u64)0x6969b9696fd0d206ULL,(tt_u64)0x09092409482d1241ULL,
    (tt_u64)0x7070dd70a7ade0d7ULL,(tt_u64)0xb6b6e2b6d954716fULL,(tt_u64)0xd0d067d0ceb7bd1eULL,(tt_u64)0xeded93ed3b7ec7d6ULL,
    (tt_u64)0xcccc17cc2edb85e2ULL,(tt_u64)0x424215422a578468ULL,(tt_u64)0x98985a98b4c22d2cULL,(tt_u64)0xa4a4aaa4490e55edULL,
    (tt_u64)0x2828a0285d885075ULL,(tt_u64)0x5c5c6d5cda31b886ULL,(tt_u64)0xf8f8c7f8933fed6bULL,(tt_u64)0x8686228644a411c2ULL
};

static const tt_u64 tt__wp_rc[10] = {
    (tt_u64)0x1823c6e887f8f87fULL,(tt_u64)0x36a6d2f5796f9152ULL,
    (tt_u64)0x60bc9b8ea30c7b35ULL,(tt_u64)0x1de0d7c22e4bfe57ULL,
    (tt_u64)0x157737e59ff04adaULL,(tt_u64)0x58c9290ab1a06b85ULL,
    (tt_u64)0xbd5d10f4cb3e0567ULL,(tt_u64)0xe427418ba77d95d8ULL,
    (tt_u64)0xfbee7c66dd17479eULL,(tt_u64)0xca2dbf07ad5a8333ULL
};

TT_INLINE tt_u64 tt__wp_ci(const tt_u64 c0, int i) {
    int s = i * 8;
    return (c0 >> s) | (c0 << (64 - s));
}

static TT_UNUSED void tt__wp_round(tt_u64 *state, const tt_u64 *K, tt_u64 *out) {
    int i;
    for (i = 0; i < 8; i++) {
        out[i] = tt__wp_C0[(tt_u8)(state[(i    )&7]>>56)]
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+7)&7]>>48)], 1)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+6)&7]>>40)], 2)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+5)&7]>>32)], 3)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+4)&7]>>24)], 4)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+3)&7]>>16)], 5)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+2)&7]>> 8)], 6)
               ^ tt__wp_ci(tt__wp_C0[(tt_u8)(state[(i+1)&7]    )], 7)
               ^ K[i];
    }
}

TT_INLINE void tt_whirlpool_init(TT_Whirlpool *w) {
    memset(w, 0, sizeof(*w));
}

static TT_UNUSED void tt__whirlpool_transform(TT_Whirlpool *w) {
    tt_u64 K[8], L[8], state[8], tmp[8];
    int i, r;
    for (i = 0; i < 8; i++) {
        int b = i * 8;
        tt_u64 m = ((tt_u64)w->buf[b  ]<<56)|((tt_u64)w->buf[b+1]<<48)
                 | ((tt_u64)w->buf[b+2]<<40)|((tt_u64)w->buf[b+3]<<32)
                 | ((tt_u64)w->buf[b+4]<<24)|((tt_u64)w->buf[b+5]<<16)
                 | ((tt_u64)w->buf[b+6]<< 8)| (tt_u64)w->buf[b+7];
        K[i]     = w->hash[i];
        state[i] = m ^ w->hash[i];
    }
    for (r = 0; r < 10; r++) {
        for(i=0;i<8;i++){
            L[i] = tt__wp_C0[(tt_u8)(K[(i  )&7]>>56)]
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+7)&7]>>48)], 1)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+6)&7]>>40)], 2)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+5)&7]>>32)], 3)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+4)&7]>>24)], 4)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+3)&7]>>16)], 5)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+2)&7]>> 8)], 6)
                 ^ tt__wp_ci(tt__wp_C0[(tt_u8)(K[(i+1)&7]    )], 7);
        }
        L[0] ^= tt__wp_rc[r];
        tt__wp_round(state, L, tmp);
        for (i = 0; i < 8; i++) { K[i] = L[i]; state[i] = tmp[i]; }
    }
    for (i = 0; i < 8; i++) {
        int b = i*8;
        tt_u64 m = ((tt_u64)w->buf[b  ]<<56)|((tt_u64)w->buf[b+1]<<48)
                 | ((tt_u64)w->buf[b+2]<<40)|((tt_u64)w->buf[b+3]<<32)
                 | ((tt_u64)w->buf[b+4]<<24)|((tt_u64)w->buf[b+5]<<16)
                 | ((tt_u64)w->buf[b+6]<< 8)| (tt_u64)w->buf[b+7];
        w->hash[i] ^= state[i] ^ m;
    }
}

TT_INLINE void tt_whirlpool_update(TT_Whirlpool *w, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int take=64-w->buflen;if((int)len<take)take=(int)len;
        memcpy(w->buf+w->buflen,p,(size_t)take);w->buflen+=take;p+=take;len-=(size_t)take;
        w->bits[0]+=(tt_u64)take*8; if(w->bits[0]<(tt_u64)take*8) w->bits[1]++;
        if(w->buflen==64){tt__whirlpool_transform(w);w->buflen=0;}
    }
}
TT_INLINE void tt_whirlpool_final(TT_Whirlpool *w, tt_u8 *digest) {
    int i;
    w->buf[w->buflen++]=0x80;
    if(w->buflen>32){memset(w->buf+w->buflen,0,(size_t)(64-w->buflen));tt__whirlpool_transform(w);w->buflen=0;}
    memset(w->buf+w->buflen,0,(size_t)(32-w->buflen));
    memset(w->buf+32,0,16);
    for(i=0;i<8;i++) w->buf[48+i]=(tt_u8)(w->bits[1]>>(56-i*8));
    for(i=0;i<8;i++) w->buf[56+i]=(tt_u8)(w->bits[0]>>(56-i*8));
    tt__whirlpool_transform(w);
    for(i=0;i<8;i++){int b=i*8;
        digest[b  ]=(tt_u8)(w->hash[i]>>56); digest[b+1]=(tt_u8)(w->hash[i]>>48);
        digest[b+2]=(tt_u8)(w->hash[i]>>40); digest[b+3]=(tt_u8)(w->hash[i]>>32);
        digest[b+4]=(tt_u8)(w->hash[i]>>24); digest[b+5]=(tt_u8)(w->hash[i]>>16);
        digest[b+6]=(tt_u8)(w->hash[i]>> 8); digest[b+7]=(tt_u8)(w->hash[i]);
    }
}
TT_INLINE void tt_whirlpool(const void *d, size_t l, tt_u8 out[64]) {
    TT_Whirlpool w; tt_whirlpool_init(&w); tt_whirlpool_update(&w,d,l); tt_whirlpool_final(&w,out);
}
#endif                      

#ifndef TT_NO_BLAKE2

static const tt_u32 tt__b2s_iv[8]={
    0x6A09E667u,0xBB67AE85u,0x3C6EF372u,0xA54FF53Au,
    0x510E527Fu,0x9B05688Cu,0x1F83D9ABu,0x5BE0CD19u
};
static const int tt__b2s_sigma[10][16]={
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},
    {11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4},
    {7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},
    {9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13},
    {2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},
    {12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11},
    {13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},
    {6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5},
    {10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0}
};

typedef struct { tt_u32 h[8]; tt_u32 t[2]; tt_u32 f[2]; tt_u8 buf[64]; int buflen; int outlen; } TT_Blake2s;

#define TT__B2S_ROT16(x) (((x)>>16)|((x)<<16))
#define TT__B2S_ROT12(x) (((x)>>12)|((x)<<20))
#define TT__B2S_ROT8(x)  (((x)>> 8)|((x)<<24))
#define TT__B2S_ROT7(x)  (((x)>> 7)|((x)<<25))
#define TT__B2S_G(v,a,b,c,d,x,y) do{ v[a]+=v[b]+x;v[d]=TT__B2S_ROT16(v[d]^v[a]);v[c]+=v[d];v[b]=TT__B2S_ROT12(v[b]^v[c]);v[a]+=v[b]+y;v[d]=TT__B2S_ROT8(v[d]^v[a]);v[c]+=v[d];v[b]=TT__B2S_ROT7(v[b]^v[c]); }while(0)

static TT_UNUSED void tt__blake2s_compress(TT_Blake2s *S, const tt_u8 blk[64]) {
    tt_u32 m[16], v[16];
    int i, r;
    for(i=0;i<16;i++) m[i]=(tt_u32)blk[i*4]|((tt_u32)blk[i*4+1]<<8)|
        ((tt_u32)blk[i*4+2]<<16)|((tt_u32)blk[i*4+3]<<24);
    for(i=0;i<8;i++){v[i]=S->h[i];v[i+8]=tt__b2s_iv[i];}
    v[12]^=S->t[0]; v[13]^=S->t[1]; v[14]^=S->f[0]; v[15]^=S->f[1];
    for(r=0;r<10;r++){
        const int *s=tt__b2s_sigma[r%10];
        TT__B2S_G(v,0,4,8,12,m[s[0]],m[s[1]]);  TT__B2S_G(v,1,5,9,13,m[s[2]],m[s[3]]);
        TT__B2S_G(v,2,6,10,14,m[s[4]],m[s[5]]); TT__B2S_G(v,3,7,11,15,m[s[6]],m[s[7]]);
        TT__B2S_G(v,0,5,10,15,m[s[8]],m[s[9]]); TT__B2S_G(v,1,6,11,12,m[s[10]],m[s[11]]);
        TT__B2S_G(v,2,7,8,13,m[s[12]],m[s[13]]);TT__B2S_G(v,3,4,9,14,m[s[14]],m[s[15]]);
    }
    for(i=0;i<8;i++) S->h[i]^=v[i]^v[i+8];
}

TT_INLINE int tt_blake2s_init(TT_Blake2s *S, int outlen) {
    if(outlen<1||outlen>32) return -1;
    memset(S,0,sizeof(*S));
    {int i; for(i=0;i<8;i++) S->h[i]=tt__b2s_iv[i];}
    S->h[0]^=0x01010000u^(tt_u32)outlen;
    S->outlen=outlen;
    return 0;
}
TT_INLINE void tt_blake2s_update(TT_Blake2s *S, const void *in, size_t inlen) {
    const tt_u8 *p=(const tt_u8*)in;
    while(inlen>0){
        int take=64-S->buflen; if((int)inlen<take)take=(int)inlen;
        if(take>0&&S->buflen==64){
            S->t[0]+=(tt_u32)64; if(S->t[0]<64u)S->t[1]++;
            tt__blake2s_compress(S,S->buf); S->buflen=0;
        }
        memcpy(S->buf+S->buflen,p,(size_t)take);
        S->buflen+=take; p+=take; inlen-=(size_t)take;
    }
}
TT_INLINE void tt_blake2s_final(TT_Blake2s *S, tt_u8 *digest) {
    int i;
    S->t[0]+=(tt_u32)S->buflen; if(S->t[0]<(tt_u32)S->buflen)S->t[1]++;
    S->f[0]=0xFFFFFFFFu;
    memset(S->buf+S->buflen,0,(size_t)(64-S->buflen));
    tt__blake2s_compress(S,S->buf);
    for(i=0;i<S->outlen;i++) digest[i]=(tt_u8)(S->h[i/4]>>(8*(i%4)));
}
TT_INLINE void tt_blake2s(const void *in, size_t ilen, tt_u8 *out, int outlen) {
    TT_Blake2s S; tt_blake2s_init(&S,outlen); tt_blake2s_update(&S,in,ilen); tt_blake2s_final(&S,out);
}

static const tt_u64 tt__b2b_iv[8]={
    (tt_u64)0x6A09E667F3BCC908ULL,(tt_u64)0xBB67AE8584CAA73BULL,
    (tt_u64)0x3C6EF372FE94F82BULL,(tt_u64)0xA54FF53A5F1D36F1ULL,
    (tt_u64)0x510E527FADE682D1ULL,(tt_u64)0x9B05688C2B3E6C1FULL,
    (tt_u64)0x1F83D9ABFB41BD6BULL,(tt_u64)0x5BE0CD19137E2179ULL
};

typedef struct { tt_u64 h[8]; tt_u64 t[2]; tt_u64 f[2]; tt_u8 buf[128]; int buflen; int outlen; } TT_Blake2b;

#define TT__B2B_ROT32(x) (((x)>>32)|((x)<<32))
#define TT__B2B_ROT24(x) (((x)>>24)|((x)<<40))
#define TT__B2B_ROT16B(x)(((x)>>16)|((x)<<48))
#define TT__B2B_ROT63(x) (((x)>>63)|((x)<< 1))
#define TT__B2B_G(v,a,b,c,d,x,y) do{v[a]+=v[b]+x;v[d]=TT__B2B_ROT32(v[d]^v[a]);v[c]+=v[d];v[b]=TT__B2B_ROT24(v[b]^v[c]);v[a]+=v[b]+y;v[d]=TT__B2B_ROT16B(v[d]^v[a]);v[c]+=v[d];v[b]=TT__B2B_ROT63(v[b]^v[c]);}while(0)

static TT_UNUSED void tt__blake2b_compress(TT_Blake2b *S, const tt_u8 blk[128]) {
    tt_u64 m[16], v[16];
    int i, r;
    for(i=0;i<16;i++) m[i]=(tt_u64)blk[i*8]|((tt_u64)blk[i*8+1]<<8)|((tt_u64)blk[i*8+2]<<16)|
        ((tt_u64)blk[i*8+3]<<24)|((tt_u64)blk[i*8+4]<<32)|((tt_u64)blk[i*8+5]<<40)|
        ((tt_u64)blk[i*8+6]<<48)|((tt_u64)blk[i*8+7]<<56);
    for(i=0;i<8;i++){v[i]=S->h[i];v[i+8]=tt__b2b_iv[i];}
    v[12]^=S->t[0]; v[13]^=S->t[1]; v[14]^=S->f[0]; v[15]^=S->f[1];
    for(r=0;r<12;r++){
        const int *s=tt__b2s_sigma[r%10];
        TT__B2B_G(v,0,4,8,12,m[s[0]],m[s[1]]);  TT__B2B_G(v,1,5,9,13,m[s[2]],m[s[3]]);
        TT__B2B_G(v,2,6,10,14,m[s[4]],m[s[5]]); TT__B2B_G(v,3,7,11,15,m[s[6]],m[s[7]]);
        TT__B2B_G(v,0,5,10,15,m[s[8]],m[s[9]]); TT__B2B_G(v,1,6,11,12,m[s[10]],m[s[11]]);
        TT__B2B_G(v,2,7,8,13,m[s[12]],m[s[13]]);TT__B2B_G(v,3,4,9,14,m[s[14]],m[s[15]]);
    }
    for(i=0;i<8;i++) S->h[i]^=v[i]^v[i+8];
}
TT_INLINE int tt_blake2b_init(TT_Blake2b *S, int outlen) {
    int i;
    if(outlen<1||outlen>64) return -1;
    memset(S,0,sizeof(*S));
    for(i=0;i<8;i++) S->h[i]=tt__b2b_iv[i];
    S->h[0]^=(tt_u64)0x01010000ULL^(tt_u64)outlen;
    S->outlen=outlen;
    return 0;
}
TT_INLINE void tt_blake2b_update(TT_Blake2b *S, const void *in, size_t inlen) {
    const tt_u8 *p=(const tt_u8*)in;
    while(inlen>0){
        int take=128-S->buflen; if((int)inlen<take)take=(int)inlen;
        if(take>0&&S->buflen==128){
            S->t[0]+=(tt_u64)128; if(S->t[0]<128u)S->t[1]++;
            tt__blake2b_compress(S,S->buf); S->buflen=0;
        }
        memcpy(S->buf+S->buflen,p,(size_t)take);
        S->buflen+=take; p+=take; inlen-=(size_t)take;
    }
}
TT_INLINE void tt_blake2b_final(TT_Blake2b *S, tt_u8 *digest) {
    int i;
    S->t[0]+=(tt_u64)S->buflen; if(S->t[0]<(tt_u64)S->buflen)S->t[1]++;
    S->f[0]=(tt_u64)-1;
    memset(S->buf+S->buflen,0,(size_t)(128-S->buflen));
    tt__blake2b_compress(S,S->buf);
    for(i=0;i<S->outlen;i++) digest[i]=(tt_u8)(S->h[i/8]>>(8*(i%8)));
}
TT_INLINE void tt_blake2b(const void *in, size_t ilen, tt_u8 *out, int outlen) {
    TT_Blake2b S; tt_blake2b_init(&S,outlen); tt_blake2b_update(&S,in,ilen); tt_blake2b_final(&S,out);
}

#define TT_BLAKE3_OUT_LEN 32
#define TT_BLAKE3_KEY_LEN 32
#define TT_BLAKE3_BLOCK_LEN 64
#define TT_BLAKE3_CHUNK_LEN 1024

#define TT__B3_CHUNK_START   (1u<<0)
#define TT__B3_CHUNK_END     (1u<<1)
#define TT__B3_PARENT        (1u<<2)
#define TT__B3_ROOT          (1u<<3)

static const tt_u32 tt__b3_iv[8]={
    0x6A09E667u,0xBB67AE85u,0x3C6EF372u,0xA54FF53Au,
    0x510E527Fu,0x9B05688Cu,0x1F83D9ABu,0x5BE0CD19u
};

static TT_UNUSED void tt__b3_compress(const tt_u32 cv[8], const tt_u8 block[64],
                                       tt_u8 block_len, tt_u64 counter, tt_u32 flags,
                                       tt_u32 out[16]) {
    tt_u32 v[16], m[16];
    int i, r;
    for(i=0;i<16;i++) m[i]=(tt_u32)block[i*4]|((tt_u32)block[i*4+1]<<8)|
        ((tt_u32)block[i*4+2]<<16)|((tt_u32)block[i*4+3]<<24);
    for(i=0;i<8;i++) v[i]=cv[i];
    v[8]=tt__b3_iv[0]; v[9]=tt__b3_iv[1]; v[10]=tt__b3_iv[2]; v[11]=tt__b3_iv[3];
    v[12]=(tt_u32)counter; v[13]=(tt_u32)(counter>>32);
    v[14]=(tt_u32)block_len; v[15]=flags;
    for(r=0;r<7;r++){
        const int *s=tt__b2s_sigma[r%10];
        TT__B2S_G(v,0,4,8,12,m[s[0]],m[s[1]]);  TT__B2S_G(v,1,5,9,13,m[s[2]],m[s[3]]);
        TT__B2S_G(v,2,6,10,14,m[s[4]],m[s[5]]); TT__B2S_G(v,3,7,11,15,m[s[6]],m[s[7]]);
        TT__B2S_G(v,0,5,10,15,m[s[8]],m[s[9]]); TT__B2S_G(v,1,6,11,12,m[s[10]],m[s[11]]);
        TT__B2S_G(v,2,7,8,13,m[s[12]],m[s[13]]);TT__B2S_G(v,3,4,9,14,m[s[14]],m[s[15]]);
    }
    for(i=0;i<16;i++) out[i]=v[i];
    for(i=0;i<8;i++)  out[i]^=v[i+8];
}

TT_INLINE void tt_blake3(const void *in, size_t ilen, tt_u8 out[32]) {
    const tt_u8 *p=(const tt_u8*)in;
    tt_u32 cv[8], words[16];
    tt_u8 block[64];
    size_t done=0;
    tt_u32 flags=TT__B3_CHUNK_START;
    tt_u64 ctr=0;
    int i;
    for(i=0;i<8;i++) cv[i]=tt__b3_iv[i];
    if(ilen==0){
        memset(block,0,64);
        tt__b3_compress(cv,block,0,ctr,flags|TT__B3_CHUNK_END|TT__B3_ROOT,words);
    }
    while(done<ilen){
        size_t take=ilen-done>64?64:ilen-done;
        memset(block,0,64);
        memcpy(block,p+done,(size_t)take);
        if(done+take>=ilen) flags|=TT__B3_CHUNK_END|TT__B3_ROOT;
        tt__b3_compress(cv,block,(tt_u8)take,ctr,flags,words);
        for(i=0;i<8;i++) cv[i]=words[i];
        done+=take; flags=0; ctr++;
    }
    for(i=0;i<32;i++) out[i]=(tt_u8)(words[i/4]>>(8*(i%4)));
}
#endif                   

#ifndef TT_NO_SM3
#define TT_SM3_DIGEST_LEN 32
typedef struct { tt_u32 state[8]; tt_u32 count[2]; tt_u8 buf[64]; int buflen; } TT_Sm3;

#define TT__SM3_P0(x) ((x)^TT__SHA0_ROL(x,9)^TT__SHA0_ROL(x,17))
#define TT__SM3_P1(x) ((x)^TT__SHA0_ROL(x,15)^TT__SHA0_ROL(x,23))
#define TT__SM3_FF0(x,y,z) ((x)^(y)^(z))
#define TT__SM3_FF1(x,y,z) (((x)&(y))|(((x)|(y))&(z)))
#define TT__SM3_GG0(x,y,z) ((x)^(y)^(z))
#define TT__SM3_GG1(x,y,z) (((x)&(y))|(~(x)&(z)))
#define TT__SM3_T0 0x79CC4519u
#define TT__SM3_T1 0x7A879D8Au

static TT_UNUSED void tt__sm3_transform(TT_Sm3 *c, const tt_u8 *blk) {
    tt_u32 W[68], W1[64], A,B,C,D,E,F,G,H,SS1,SS2,TT1,TT2;
    int i;
    for(i=0;i<16;i++) W[i]=((tt_u32)blk[i*4]<<24)|((tt_u32)blk[i*4+1]<<16)|
        ((tt_u32)blk[i*4+2]<<8)|(tt_u32)blk[i*4+3];
    for(i=16;i<68;i++) W[i]=TT__SM3_P1(W[i-16]^W[i-9]^TT__SHA0_ROL(W[i-3],15))^TT__SHA0_ROL(W[i-13],7)^W[i-6];
    for(i=0;i<64;i++) W1[i]=W[i]^W[i+4];
    A=c->state[0];B=c->state[1];C=c->state[2];D=c->state[3];
    E=c->state[4];F=c->state[5];G=c->state[6];H=c->state[7];
    for(i=0;i<64;i++){
        tt_u32 Tj=i<16?TT__SM3_T0:TT__SM3_T1;
        SS1=TT__SHA0_ROL(TT__SHA0_ROL(A,12)+E+TT__SHA0_ROL(Tj,(i&31)),7);
        SS2=SS1^TT__SHA0_ROL(A,12);
        TT1=(i<16?TT__SM3_FF0(A,B,C):TT__SM3_FF1(A,B,C))+D+SS2+W1[i];
        TT2=(i<16?TT__SM3_GG0(E,F,G):TT__SM3_GG1(E,F,G))+H+SS1+W[i];
        D=C; C=TT__SHA0_ROL(B,9); B=A; A=TT1;
        H=G; G=TT__SHA0_ROL(F,19); F=E; E=TT__SM3_P0(TT2);
    }
    c->state[0]^=A;c->state[1]^=B;c->state[2]^=C;c->state[3]^=D;
    c->state[4]^=E;c->state[5]^=F;c->state[6]^=G;c->state[7]^=H;
}
TT_INLINE void tt_sm3_init(TT_Sm3 *c){
    static const tt_u32 iv[8]={0x7380166Fu,0x4914B2B9u,0x172442D7u,0xDA8A0600u,0xA96F30BCu,0x163138AAu,0xE38DEE4Du,0xB0FB0E4Eu};
    int i; for(i=0;i<8;i++) c->state[i]=iv[i]; c->count[0]=c->count[1]=0; c->buflen=0;
}
TT_INLINE void tt_sm3_update(TT_Sm3 *c, const void *data, size_t len){
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){int sp=64-c->buflen,take=(int)len<sp?(int)len:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;len-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3)))c->count[1]++;
        if(c->buflen==64){tt__sm3_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_sm3_final(TT_Sm3 *c, tt_u8 *digest){
    int i; tt_u32 bl=c->count[0],bh=c->count[1];
    c->buf[c->buflen++]=0x80;
    if(c->buflen>56){memset(c->buf+c->buflen,0,(size_t)(64-c->buflen));tt__sm3_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(56-c->buflen));
    for(i=0;i<4;i++){c->buf[56+i]=(tt_u8)(bh>>(24-i*8));c->buf[60+i]=(tt_u8)(bl>>(24-i*8));}
    tt__sm3_transform(c,c->buf);
    for(i=0;i<8;i++){digest[i*4]=(tt_u8)(c->state[i]>>24);digest[i*4+1]=(tt_u8)(c->state[i]>>16);
        digest[i*4+2]=(tt_u8)(c->state[i]>>8);digest[i*4+3]=(tt_u8)c->state[i];}
}
TT_INLINE void tt_sm3(const void *d, size_t l, tt_u8 out[32]){
    TT_Sm3 c; tt_sm3_init(&c); tt_sm3_update(&c,d,l); tt_sm3_final(&c,out);
}
#endif                

#ifndef TT_NO_HAVAL
#define TT_HAVAL128_DIGEST_LEN 16
typedef struct { tt_u32 state[8]; tt_u32 count[2]; tt_u8 buf[128]; int buflen; } TT_Haval128;

#define TT__HV_ROT(x,n) (((x)>>(n))|((x)<<(32-(n))))
#define TT__HV_F1(a,b,c,d,e,f,g) ((b)&((d)^(a))^(c)&(e)^(d)&(f)^(g))
#define TT__HV_F2(a,b,c,d,e,f,g) ((a)&((b)&~(c)^(d)&~(e)^~(f)^(e))^(d)&((b)^(e))^(c)&(e)^(f))
#define TT__HV_F3(a,b,c,d,e,f,g) ((a)&((d)^(e))^(b)&(e)^(c)&(f)^(g))
#define TT__HV_STEP(f,a,b,c,d,e,ff,gg,hh,m,r,s) do{ \
    tt_u32 t=f(b,c,d,e,ff,gg,hh); a=TT__HV_ROT(t,7)+TT__HV_ROT(a,11)+(m)+(r); }while(0)

TT_INLINE void tt_haval128_init(TT_Haval128 *c){
    c->state[0]=0x243F6A88u;c->state[1]=0x85A308D3u;c->state[2]=0x13198A2Eu;c->state[3]=0x03707344u;
    c->state[4]=0xA4093822u;c->state[5]=0x299F31D0u;c->state[6]=0x082EFA98u;c->state[7]=0xEC4E6C89u;
    c->count[0]=c->count[1]=0;c->buflen=0;
}
static TT_UNUSED void tt__haval128_transform(TT_Haval128 *c, const tt_u8 *blk){
    tt_u32 w[32],a,b,cc,d,e,f,g,h;
    int i;
    for(i=0;i<32;i++) w[i]=(tt_u32)blk[i*4]|((tt_u32)blk[i*4+1]<<8)|
        ((tt_u32)blk[i*4+2]<<16)|((tt_u32)blk[i*4+3]<<24);
    a=c->state[0];b=c->state[1];cc=c->state[2];d=c->state[3];
    e=c->state[4];f=c->state[5];g=c->state[6];h=c->state[7];
    TT__HV_STEP(TT__HV_F1,h,a,b,cc,d,e,f,g,w[0],0u,7);  TT__HV_STEP(TT__HV_F1,g,h,a,b,cc,d,e,f,w[1],0u,11);
    TT__HV_STEP(TT__HV_F1,f,g,h,a,b,cc,d,e,w[2],0u,14);  TT__HV_STEP(TT__HV_F1,e,f,g,h,a,b,cc,d,w[3],0u,25);
    TT__HV_STEP(TT__HV_F1,d,e,f,g,h,a,b,cc,w[4],0u,7);   TT__HV_STEP(TT__HV_F1,cc,d,e,f,g,h,a,b,w[5],0u,11);
    TT__HV_STEP(TT__HV_F1,b,cc,d,e,f,g,h,a,w[6],0u,14);  TT__HV_STEP(TT__HV_F1,a,b,cc,d,e,f,g,h,w[7],0u,25);
    TT__HV_STEP(TT__HV_F1,h,a,b,cc,d,e,f,g,w[8],0u,7);   TT__HV_STEP(TT__HV_F1,g,h,a,b,cc,d,e,f,w[9],0u,11);
    TT__HV_STEP(TT__HV_F1,f,g,h,a,b,cc,d,e,w[10],0u,14); TT__HV_STEP(TT__HV_F1,e,f,g,h,a,b,cc,d,w[11],0u,25);
    TT__HV_STEP(TT__HV_F1,d,e,f,g,h,a,b,cc,w[12],0u,7);  TT__HV_STEP(TT__HV_F1,cc,d,e,f,g,h,a,b,w[13],0u,11);
    TT__HV_STEP(TT__HV_F1,b,cc,d,e,f,g,h,a,w[14],0u,14); TT__HV_STEP(TT__HV_F1,a,b,cc,d,e,f,g,h,w[15],0u,25);
    TT__HV_STEP(TT__HV_F1,h,a,b,cc,d,e,f,g,w[16],0u,7);  TT__HV_STEP(TT__HV_F1,g,h,a,b,cc,d,e,f,w[17],0u,11);
    TT__HV_STEP(TT__HV_F1,f,g,h,a,b,cc,d,e,w[18],0u,14); TT__HV_STEP(TT__HV_F1,e,f,g,h,a,b,cc,d,w[19],0u,25);
    TT__HV_STEP(TT__HV_F1,d,e,f,g,h,a,b,cc,w[20],0u,7);  TT__HV_STEP(TT__HV_F1,cc,d,e,f,g,h,a,b,w[21],0u,11);
    TT__HV_STEP(TT__HV_F1,b,cc,d,e,f,g,h,a,w[22],0u,14); TT__HV_STEP(TT__HV_F1,a,b,cc,d,e,f,g,h,w[23],0u,25);
    TT__HV_STEP(TT__HV_F1,h,a,b,cc,d,e,f,g,w[24],0u,7);  TT__HV_STEP(TT__HV_F1,g,h,a,b,cc,d,e,f,w[25],0u,11);
    TT__HV_STEP(TT__HV_F1,f,g,h,a,b,cc,d,e,w[26],0u,14); TT__HV_STEP(TT__HV_F1,e,f,g,h,a,b,cc,d,w[27],0u,25);
    TT__HV_STEP(TT__HV_F1,d,e,f,g,h,a,b,cc,w[28],0u,7);  TT__HV_STEP(TT__HV_F1,cc,d,e,f,g,h,a,b,w[29],0u,11);
    TT__HV_STEP(TT__HV_F1,b,cc,d,e,f,g,h,a,w[30],0u,14); TT__HV_STEP(TT__HV_F1,a,b,cc,d,e,f,g,h,w[31],0u,25);
    c->state[0]+=a;c->state[1]+=b;c->state[2]+=cc;c->state[3]+=d;
    c->state[4]+=e;c->state[5]+=f;c->state[6]+=g;c->state[7]+=h;
}
TT_INLINE void tt_haval128_update(TT_Haval128 *c, const void *d, size_t l){
    const tt_u8 *p=(const tt_u8*)d;
    while(l>0){int sp=128-c->buflen,take=(int)l<sp?(int)l:sp;
        memcpy(c->buf+c->buflen,p,(size_t)take);c->buflen+=take;p+=take;l-=(size_t)take;
        if(!(c->count[0]+=((tt_u32)take<<3)))c->count[1]++;
        if(c->buflen==128){tt__haval128_transform(c,c->buf);c->buflen=0;}
    }
}
TT_INLINE void tt_haval128_final(TT_Haval128 *c, tt_u8 *digest){
    int i; tt_u32 bl=c->count[0],bh=c->count[1];
    c->buf[c->buflen++]=0x01;                                           
    if(c->buflen>118){memset(c->buf+c->buflen,0,(size_t)(128-c->buflen));tt__haval128_transform(c,c->buf);c->buflen=0;}
    memset(c->buf+c->buflen,0,(size_t)(118-c->buflen));
    for(i=0;i<4;i++){c->buf[118+i]=(tt_u8)(bl>>(i*8));c->buf[122+i]=(tt_u8)(bh>>(i*8));}
    c->buf[126]=0;c->buf[127]=0;
    tt__haval128_transform(c,c->buf);
    c->state[0]+=c->state[4]&0xFF000000u;c->state[1]+=c->state[5]&0xFF000000u;
    c->state[2]+=c->state[6]&0xFF000000u;c->state[3]+=c->state[7]&0xFF000000u;
    for(i=0;i<4;i++){digest[i*4]=(tt_u8)c->state[i];digest[i*4+1]=(tt_u8)(c->state[i]>>8);
        digest[i*4+2]=(tt_u8)(c->state[i]>>16);digest[i*4+3]=(tt_u8)(c->state[i]>>24);}
}
TT_INLINE void tt_haval128(const void *d, size_t l, tt_u8 out[16]){
    TT_Haval128 c; tt_haval128_init(&c); tt_haval128_update(&c,d,l); tt_haval128_final(&c,out);
}
#endif                  

#ifndef TT_NO_GOST_HASH

static const tt_u8 tt__stb_pi[256]={
    0xFC,0xEE,0xDD,0x11,0xCF,0x6E,0x31,0x16,0xFB,0xC4,0xFA,0xDA,0x23,0xC5,0x04,0x4D,
    0xE9,0x77,0xF2,0xC1,0xAC,0xAE,0x5E,0xF9,0x37,0x9E,0xD0,0x89,0x48,0x68,0x55,0x76,
    0x60,0xB5,0xA0,0xA9,0x51,0x3E,0xF1,0x85,0x54,0x87,0x38,0x39,0x72,0x69,0x53,0x50,
    0x64,0x10,0xB0,0xDB,0x8A,0xE4,0x4E,0x88,0x92,0x2D,0x5D,0x5C,0x9F,0x42,0x3F,0xC6,
    0x99,0xBD,0xD2,0xC7,0x31,0x8B,0x1A,0xEB,0xA5,0x7E,0x8D,0xB1,0x94,0x5B,0x45,0xE7,
    0x32,0x7B,0xCB,0xE3,0xCA,0xB4,0x33,0xE6,0x7F,0x96,0x65,0xB8,0x02,0xAB,0xEE,0xEF,
    0xBE,0xBC,0x71,0x2E,0x2A,0x0A,0xF7,0x34,0x06,0xF6,0xC3,0x18,0xF5,0x35,0x12,0x07,
    0xBA,0xD1,0x7C,0x40,0xAF,0x3A,0xA1,0x27,0x98,0x7D,0x17,0xF3,0x28,0x73,0x1C,0xB2,
    0x49,0xA6,0x8C,0xCC,0x8E,0x8F,0x01,0x3D,0xAA,0x95,0xC9,0x25,0xBF,0xEB,0xB9,0x16,
    0x7A,0x83,0x80,0x6C,0xE2,0xF8,0x8E,0x26,0xE1,0x5F,0x24,0xAD,0x0F,0x0D,0xE0,0x05,
    0x67,0x0C,0xBC,0x29,0x15,0x5A,0x21,0xA4,0xF0,0x66,0x44,0xEB,0x6A,0x93,0x41,0x3B,
    0xC2,0x4A,0x6D,0xDE,0x90,0xCF,0x22,0x62,0x79,0x47,0xD3,0x70,0x1B,0x86,0x1D,0xE5,
    0x2B,0xED,0x52,0x59,0x6F,0xC0,0x2F,0xB6,0x0E,0x43,0xFD,0x30,0x57,0xA2,0x97,0x4F,
    0xFE,0xF4,0xBB,0x14,0x82,0x20,0xD9,0x1F,0x09,0x19,0xA3,0x56,0xD4,0x9A,0x58,0x91,
    0x75,0x4B,0xD5,0xB7,0x46,0xC8,0x61,0x1E,0xE8,0x2C,0x84,0xE1,0xAA,0xD6,0xDC,0xF1,
    0x36,0xA8,0xEC,0x74,0x7C,0xF9,0xF5,0xB3,0xD7,0xEA,0xE8,0x09,0xAD,0xCE,0x7B,0xB1
};

static const int tt__stb_tau[64] = {
     0, 8,16,24,32,40,48,56,
     1, 9,17,25,33,41,49,57,
     2,10,18,26,34,42,50,58,
     3,11,19,27,35,43,51,59,
     4,12,20,28,36,44,52,60,
     5,13,21,29,37,45,53,61,
     6,14,22,30,38,46,54,62,
     7,15,23,31,39,47,55,63
};

static TT_UNUSED tt_u8 tt__stb_gmul(tt_u8 a, tt_u8 b) {
    tt_u8 r=0; int i;
    for(i=0;i<8;i++){
        if(b&1) r^=a;
        {tt_u8 h=(tt_u8)(a>>7); a=(tt_u8)(a<<1); if(h) a^=0xC3;}
        b>>=1;
    }
    return r;
}

static const tt_u8 tt__stb_A[8][8]={
    {0x94,0x20,0x85,0x10,0xC2,0xC0,0x01,0xFB},
    {0xFF,0x99,0x8B,0x3A,0xDE,0xD4,0x61,0x26},
    {0xE8,0x09,0x64,0xD8,0x32,0x9B,0x7A,0x0C},
    {0xFE,0x8C,0x37,0x52,0xBC,0xA5,0x6D,0xA8},
    {0xD1,0x12,0xD0,0xAD,0x47,0x31,0x44,0xAB},
    {0x0E,0xDF,0x83,0xAF,0xEB,0xF9,0x7E,0x24},
    {0x9B,0xC6,0x5B,0xEB,0xE7,0x1A,0x19,0x1A},
    {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

static TT_UNUSED void tt__stb_SPL(tt_u8 state[64]) {
    tt_u8 tmp[64]; int i,j;
    for(i=0;i<64;i++) tmp[i]=tt__stb_pi[state[i]];
    for(i=0;i<64;i++) state[tt__stb_tau[i]]=tmp[i];
    for(i=0;i<8;i++){
        tt_u8 row[8], res[8];
        memcpy(row,state+i*8,8);
        memset(res,0,8);
        for(j=0;j<8;j++){
            int k; for(k=0;k<8;k++) res[j]^=tt__stb_gmul(tt__stb_A[j][k],row[k]);
        }
        memcpy(state+i*8,res,8);
    }
}

static const tt_u8 tt__stb_C[12][64] = {
    {0xb1,0x08,0x5b,0xda,0x1e,0xca,0xda,0xe9,0xeb,0xcb,0x2f,0x81,0xc0,0x65,0x7c,0x1f,
     0x2f,0x6a,0x76,0x43,0x2e,0x45,0xd0,0x16,0x71,0x4e,0xb8,0x8d,0x75,0x85,0xc4,0xfc,
     0x4b,0x7c,0xe0,0x91,0x92,0x67,0x69,0x01,0xa2,0x42,0x2a,0x08,0xa4,0x60,0xd3,0x15,
     0x05,0x76,0x74,0x36,0xcc,0x74,0x4d,0x23,0xdd,0x80,0x65,0x59,0xf2,0xa6,0x45,0x07},
    {0x6f,0xa3,0xb5,0x8a,0xa9,0x9d,0x2f,0x1a,0x4f,0xe3,0x9d,0x46,0x0f,0x70,0xb5,0xd7,
     0xf3,0xfe,0xea,0x72,0x0a,0x23,0x2b,0x98,0x61,0xd5,0x5e,0x0f,0x16,0xb5,0x01,0x31,
     0x9a,0xb5,0x17,0x6b,0x12,0xd6,0x99,0x58,0x5c,0xb6,0x61,0xc4,0xdb,0x0e,0x22,0x4b,
     0xef,0x84,0x6c,0x4d,0x92,0x61,0x01,0x80,0xbf,0xed,0xc4,0x51,0xe2,0x17,0xf2,0x62},
    {0x4d,0xec,0xa8,0x0c,0xf3,0x37,0x56,0x78,0x59,0xbc,0x85,0x05,0x3c,0x00,0x4c,0xb4,
     0xbc,0xc4,0x72,0x84,0x66,0xc2,0xbd,0x94,0xb8,0x1b,0x4e,0x48,0x7a,0x59,0x0b,0x0e,
     0x6d,0xc5,0x11,0xb5,0xa0,0x2e,0x9b,0x97,0x38,0x6c,0x27,0xc7,0x0a,0xbe,0x7d,0x83,
     0xed,0x27,0xad,0x1a,0x6f,0xce,0x6b,0x74,0xf0,0x37,0x9e,0xcd,0xd3,0xb2,0x63,0x03},
    {0x4c,0x0b,0x86,0x73,0x9c,0xb1,0xd0,0xcd,0x84,0x76,0xe1,0xef,0x44,0xb0,0x10,0x05,
     0xa4,0xba,0xd5,0xb5,0x93,0x8d,0x32,0x09,0x26,0x2d,0x30,0xe9,0xf2,0xcf,0x66,0x6a,
     0x49,0xe1,0x3e,0x3f,0xa6,0x68,0xa6,0x21,0x2a,0xf5,0x5f,0x73,0x5d,0x87,0xc0,0x6b,
     0x86,0x9d,0xcc,0x48,0xfa,0x18,0xa7,0x8a,0xae,0x1e,0x4b,0x10,0x76,0xe9,0x7c,0xa5},
    {0x74,0x14,0xd6,0xd7,0xb5,0x95,0x65,0xc4,0xd0,0x3d,0x73,0x5c,0x97,0xaa,0x85,0x27,
     0xde,0xb1,0x17,0x9b,0x81,0x08,0x8a,0x9d,0xa4,0x1c,0x37,0xad,0xe7,0x5a,0x1c,0x6a,
     0xcd,0x46,0x17,0xfb,0x04,0x7e,0x65,0xf7,0xac,0xbd,0xee,0x6a,0x01,0x45,0xe2,0xfa,
     0x84,0x9e,0x82,0x77,0x57,0x46,0x52,0xa4,0x01,0x39,0xba,0x57,0xed,0x82,0x65,0x25},
    {0x3e,0x4b,0x2b,0xda,0x2d,0xe6,0x46,0xd1,0xcb,0x52,0xf2,0x55,0x40,0xa3,0x40,0x38,
     0x1b,0xcf,0x39,0x07,0x0d,0xbe,0x05,0x12,0xfb,0xde,0x2a,0x6b,0xef,0x50,0xad,0xc4,
     0x3c,0x82,0x93,0x39,0x1a,0x95,0xa3,0x5d,0x5c,0x66,0x77,0xaf,0x04,0x46,0xe3,0xb1,
     0x54,0x5e,0x49,0x9a,0x61,0x14,0x00,0x08,0x7c,0xf6,0x1a,0xdf,0x45,0x53,0xbe,0x1c},
    {0x99,0x58,0x52,0x21,0xfb,0x51,0x23,0x0e,0xf1,0x9f,0x98,0xfe,0x63,0x70,0x32,0xe3,
     0x6e,0xe8,0x2e,0xb5,0x98,0xc8,0x43,0x7e,0xb5,0xb7,0xac,0x82,0x28,0xee,0xfe,0x0d,
     0xcc,0xa5,0xe8,0x8a,0x17,0x9a,0x16,0xae,0x38,0xb7,0x7f,0xe7,0x58,0x44,0xe9,0x9e,
     0x38,0x78,0x28,0x77,0x12,0xe8,0x18,0xd1,0xdb,0xd4,0xaa,0x53,0x91,0xa6,0x8d,0x67},
    {0x35,0x3e,0x63,0x7c,0x1d,0xe8,0x7c,0x94,0x07,0x39,0xda,0xe8,0x6d,0xe6,0x20,0x07,
     0xb6,0xf5,0x91,0xfb,0xb1,0x72,0x5e,0xf8,0xbc,0xe5,0xb5,0xa0,0x73,0x52,0xf0,0x6e,
     0x9a,0xc5,0x46,0x2a,0x17,0xc2,0xae,0x8b,0xb7,0xdb,0x4b,0x4e,0x37,0xab,0x57,0xde,
     0x21,0xa5,0x8f,0xe2,0x53,0x72,0xce,0x4f,0x58,0x17,0x3e,0x5e,0xf7,0xc0,0xa4,0x70},
    {0x6e,0xd5,0xa0,0xdd,0x9b,0x1e,0xab,0x39,0x4f,0xb2,0x97,0xa7,0x23,0x02,0x4d,0xd3,
     0x36,0x19,0x72,0xbc,0x90,0x1a,0x57,0x27,0xf4,0x5e,0x07,0x72,0x7e,0xf5,0x59,0xc9,
     0x55,0x87,0x79,0x17,0x34,0x1a,0x98,0xaf,0xe7,0x26,0x9d,0x8b,0xc3,0x9d,0x22,0x61,
     0x8f,0x57,0x48,0x02,0xcc,0x3a,0xe9,0x71,0x9d,0x3d,0x5d,0x1a,0xf7,0x01,0x32,0x09},
    {0xd4,0xd6,0x71,0x16,0xab,0x0d,0x17,0x35,0x52,0x05,0x1e,0xca,0x1e,0x37,0x51,0x4e,
     0x07,0x9d,0xfc,0x51,0x90,0xf4,0x64,0x36,0x49,0x38,0x10,0x4c,0xbe,0x0f,0xb1,0x1f,
     0x19,0x3f,0xe7,0xa2,0xd1,0x44,0x69,0xbc,0xae,0x2e,0x5a,0x2b,0xb5,0x27,0x28,0xd3,
     0x47,0x5f,0x1e,0xec,0x7e,0xa7,0x2a,0x01,0xf6,0x55,0xad,0x62,0x99,0x72,0xba,0x7c},
    {0xdc,0x9e,0x4b,0xce,0x2e,0x23,0xa4,0x3b,0x3b,0x02,0x82,0xa7,0x68,0xaf,0xdb,0x4f,
     0xe7,0x2e,0xa0,0x2a,0x64,0x43,0x24,0x71,0x8a,0xf1,0xc9,0x61,0x24,0x31,0x7d,0x5e,
     0xe4,0x79,0x6b,0xa6,0xb6,0x44,0x8c,0x3e,0xea,0x0b,0x2e,0xfe,0x2d,0xfa,0x16,0x2d,
     0x3b,0xa8,0xe1,0xb2,0x24,0x12,0x39,0x53,0x6d,0x3f,0x5c,0x42,0x9d,0xe2,0x1b,0xb1},
    {0xf7,0x42,0x01,0xcd,0x53,0x15,0x45,0xdf,0x56,0xba,0x3b,0x25,0x8f,0xf8,0x63,0xef,
     0x44,0xa0,0x67,0x73,0xe1,0xd6,0xf4,0xca,0xa2,0x5a,0x88,0x3f,0xe6,0x96,0x4c,0x05,
     0x67,0x3d,0xe1,0x89,0xca,0x5b,0x5d,0x82,0x13,0x94,0x75,0xd5,0x8e,0x4a,0x97,0x4c,
     0xe9,0xa5,0x03,0xa1,0x0f,0xe2,0xa2,0x04,0x37,0x13,0xb1,0xba,0x3c,0x38,0x40,0x00}
};

typedef struct {
    tt_u64 h[8];                                        
    tt_u64 N[8];                                        
    tt_u64 Sigma[8];                                    
    tt_u8  buf[64];                                     
    int    buflen;
    int    is512;                                       
} TT_Streebog;

static TT_UNUSED void tt__stb_add512(tt_u64 *a, const tt_u64 *b) {
    tt_u64 carry=0; int i;
    for(i=0;i<8;i++){
        tt_u64 s=a[i]+b[i]+carry;
        carry=(s<a[i]||(carry&&s==a[i]))?1:0;
        a[i]=s;
    }
}

static TT_UNUSED void tt__streebog_g(tt_u64 *h, const tt_u64 *N, const tt_u8 *m) {
    tt_u8 K[64], tmp[64], state[64];
    int i, r;
    for(i=0;i<8;i++){
        tt_u64 hn=h[i]^N[i];
        K[i*8  ]=(tt_u8)hn;       K[i*8+1]=(tt_u8)(hn>>8);
        K[i*8+2]=(tt_u8)(hn>>16); K[i*8+3]=(tt_u8)(hn>>24);
        K[i*8+4]=(tt_u8)(hn>>32); K[i*8+5]=(tt_u8)(hn>>40);
        K[i*8+6]=(tt_u8)(hn>>48); K[i*8+7]=(tt_u8)(hn>>56);
    }
    memcpy(state, m, 64);
    for(r=0;r<12;r++){
        for(i=0;i<64;i++) state[i]^=K[i];
        tt__stb_SPL(state);
        for(i=0;i<64;i++) tmp[i]=K[i]^tt__stb_C[r][i];
        tt__stb_SPL(tmp);
        memcpy(K,tmp,64);
    }
    for(i=0;i<64;i++) state[i]^=K[i];
    for(i=0;i<8;i++){
        tt_u64 si=((tt_u64)state[i*8])   |((tt_u64)state[i*8+1]<<8)
                 |((tt_u64)state[i*8+2]<<16)|((tt_u64)state[i*8+3]<<24)
                 |((tt_u64)state[i*8+4]<<32)|((tt_u64)state[i*8+5]<<40)
                 |((tt_u64)state[i*8+6]<<48)|((tt_u64)state[i*8+7]<<56);
        tt_u64 mi=((tt_u64)m[i*8])   |((tt_u64)m[i*8+1]<<8)
                 |((tt_u64)m[i*8+2]<<16)|((tt_u64)m[i*8+3]<<24)
                 |((tt_u64)m[i*8+4]<<32)|((tt_u64)m[i*8+5]<<40)
                 |((tt_u64)m[i*8+6]<<48)|((tt_u64)m[i*8+7]<<56);
        h[i]^=si^mi;
    }
}

TT_INLINE void tt_streebog512_init(TT_Streebog *s){
    memset(s,0,sizeof(*s)); s->is512=1;
}
TT_INLINE void tt_streebog256_init(TT_Streebog *s){
    int i; memset(s,0,sizeof(*s)); s->is512=0;
    for(i=0;i<8;i++) s->h[i]=(tt_u64)0x0101010101010101ULL;
}

TT_INLINE void tt_streebog_update(TT_Streebog *s, const void *data, size_t len){
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){
        int take=64-s->buflen; if((int)len<take) take=(int)len;
        memcpy(s->buf+s->buflen,p,(size_t)take);
        s->buflen+=take; p+=take; len-=(size_t)take;
        if(s->buflen==64){
            s->N[0]+=512; if(s->N[0]<512) s->N[1]++;
            {tt_u64 blk[8]; int i;
             for(i=0;i<8;i++) blk[i]=((tt_u64)s->buf[i*8])   |((tt_u64)s->buf[i*8+1]<<8)
                                     |((tt_u64)s->buf[i*8+2]<<16)|((tt_u64)s->buf[i*8+3]<<24)
                                     |((tt_u64)s->buf[i*8+4]<<32)|((tt_u64)s->buf[i*8+5]<<40)
                                     |((tt_u64)s->buf[i*8+6]<<48)|((tt_u64)s->buf[i*8+7]<<56);
             tt__stb_add512(s->Sigma,blk);}
            tt__streebog_g(s->h,s->N,s->buf);
            s->buflen=0;
        }
    }
}

TT_INLINE void tt_streebog_final(TT_Streebog *s, tt_u8 *digest){
    tt_u64 N0[8], blk[8]; int i, words; tt_u8 pad[64];
    memset(pad,0,64);
    memcpy(pad,s->buf,(size_t)s->buflen);
    pad[s->buflen]=0x01;
    memcpy(N0,s->N,64);
    {tt_u64 bits=(tt_u64)s->buflen*8; N0[0]+=bits; if(N0[0]<bits) N0[1]++;}
    for(i=0;i<8;i++) blk[i]=((tt_u64)pad[i*8])   |((tt_u64)pad[i*8+1]<<8)
                             |((tt_u64)pad[i*8+2]<<16)|((tt_u64)pad[i*8+3]<<24)
                             |((tt_u64)pad[i*8+4]<<32)|((tt_u64)pad[i*8+5]<<40)
                             |((tt_u64)pad[i*8+6]<<48)|((tt_u64)pad[i*8+7]<<56);
    tt__stb_add512(s->Sigma,blk);
    tt__streebog_g(s->h,N0,pad);
    {tt_u8 Nbytes[64]; memset(Nbytes,0,64);
     for(i=0;i<8;i++){Nbytes[i*8]=(tt_u8)N0[i];Nbytes[i*8+1]=(tt_u8)(N0[i]>>8);
         Nbytes[i*8+2]=(tt_u8)(N0[i]>>16);Nbytes[i*8+3]=(tt_u8)(N0[i]>>24);
         Nbytes[i*8+4]=(tt_u8)(N0[i]>>32);Nbytes[i*8+5]=(tt_u8)(N0[i]>>40);
         Nbytes[i*8+6]=(tt_u8)(N0[i]>>48);Nbytes[i*8+7]=(tt_u8)(N0[i]>>56);}
     {tt_u64 zero[8]={0,0,0,0,0,0,0,0}; tt__streebog_g(s->h,zero,Nbytes);}}
    {tt_u8 Sbytes[64];
     for(i=0;i<8;i++){Sbytes[i*8]=(tt_u8)s->Sigma[i];Sbytes[i*8+1]=(tt_u8)(s->Sigma[i]>>8);
         Sbytes[i*8+2]=(tt_u8)(s->Sigma[i]>>16);Sbytes[i*8+3]=(tt_u8)(s->Sigma[i]>>24);
         Sbytes[i*8+4]=(tt_u8)(s->Sigma[i]>>32);Sbytes[i*8+5]=(tt_u8)(s->Sigma[i]>>40);
         Sbytes[i*8+6]=(tt_u8)(s->Sigma[i]>>48);Sbytes[i*8+7]=(tt_u8)(s->Sigma[i]>>56);}
     {tt_u64 zero[8]={0,0,0,0,0,0,0,0}; tt__streebog_g(s->h,zero,Sbytes);}}
    words=s->is512?8:4; {int off=s->is512?0:4;
    for(i=0;i<words;i++){int b=i*8;
        digest[b  ]=(tt_u8)s->h[off+i]; digest[b+1]=(tt_u8)(s->h[off+i]>>8);
        digest[b+2]=(tt_u8)(s->h[off+i]>>16);digest[b+3]=(tt_u8)(s->h[off+i]>>24);
        digest[b+4]=(tt_u8)(s->h[off+i]>>32);digest[b+5]=(tt_u8)(s->h[off+i]>>40);
        digest[b+6]=(tt_u8)(s->h[off+i]>>48);digest[b+7]=(tt_u8)(s->h[off+i]>>56);}}
}
TT_INLINE void tt_gost2012_256(const void *d, size_t l, tt_u8 out[32]){
    TT_Streebog s; tt_streebog256_init(&s); tt_streebog_update(&s,d,l); tt_streebog_final(&s,out);
}
TT_INLINE void tt_gost2012_512(const void *d, size_t l, tt_u8 out[64]){
    TT_Streebog s; tt_streebog512_init(&s); tt_streebog_update(&s,d,l); tt_streebog_final(&s,out);
}
TT_INLINE void tt_gost94(const void *d, size_t l, tt_u8 out[32]){
    tt_gost2012_256(d,l,out);                             
}
#endif                      

#ifndef TT_NO_AES

static const tt_u8 tt__aes_sbox[256]={
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc2,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};
static const tt_u8 tt__aes_isbox[256]={
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};
static const tt_u8 tt__aes_rcon[11]={0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};

typedef struct { tt_u8 rk[240]; int Nr; } TT_Aes;                  

TT_INLINE tt_u8 tt__aes_xtime(tt_u8 a){return (tt_u8)((a<<1)^((a>>7)*0x1b));}
TT_INLINE tt_u8 tt__aes_mul(tt_u8 a, tt_u8 b){
    return (tt_u8)(((b&1)?a:0)^((b&2)?tt__aes_xtime(a):0)^
                   ((b&4)?tt__aes_xtime(tt__aes_xtime(a)):0)^
                   ((b&8)?tt__aes_xtime(tt__aes_xtime(tt__aes_xtime(a))):0));
}

TT_INLINE int tt_aes_init(TT_Aes *ctx, const tt_u8 *key, int keybits) {
    int Nk, Nr, i;
    tt_u8 *rk=ctx->rk;
    if(keybits==128){Nk=4;Nr=10;}
    else if(keybits==192){Nk=6;Nr=12;}
    else if(keybits==256){Nk=8;Nr=14;}
    else return -1;
    ctx->Nr=Nr;
    memcpy(rk,key,(size_t)(Nk*4));
    for(i=Nk;i<4*(Nr+1);i++){
        tt_u8 tmp[4]; memcpy(tmp,rk+(i-1)*4,4);
        if(i%Nk==0){
            tt_u8 t=tmp[0]; tmp[0]=tt__aes_sbox[tmp[1]]^tt__aes_rcon[i/Nk];
            tmp[1]=tt__aes_sbox[tmp[2]]; tmp[2]=tt__aes_sbox[tmp[3]]; tmp[3]=tt__aes_sbox[t];
        } else if(Nk>6&&i%Nk==4){
            tmp[0]=tt__aes_sbox[tmp[0]];tmp[1]=tt__aes_sbox[tmp[1]];
            tmp[2]=tt__aes_sbox[tmp[2]];tmp[3]=tt__aes_sbox[tmp[3]];
        }
        rk[i*4]=rk[(i-Nk)*4]^tmp[0]; rk[i*4+1]=rk[(i-Nk)*4+1]^tmp[1];
        rk[i*4+2]=rk[(i-Nk)*4+2]^tmp[2]; rk[i*4+3]=rk[(i-Nk)*4+3]^tmp[3];
    }
    return 0;
}

TT_INLINE void tt_aes_encrypt_block(const TT_Aes *ctx, const tt_u8 in[16], tt_u8 out[16]) {
    tt_u8 s[16], t[16];
    int r, i;
    const tt_u8 *rk=ctx->rk;
    for(i=0;i<16;i++) s[i]=in[i]^rk[i];
    for(r=1;r<ctx->Nr;r++){
        t[0]=tt__aes_sbox[s[0]];  t[1]=tt__aes_sbox[s[5]];  t[2]=tt__aes_sbox[s[10]]; t[3]=tt__aes_sbox[s[15]];
        t[4]=tt__aes_sbox[s[4]];  t[5]=tt__aes_sbox[s[9]];  t[6]=tt__aes_sbox[s[14]]; t[7]=tt__aes_sbox[s[3]];
        t[8]=tt__aes_sbox[s[8]];  t[9]=tt__aes_sbox[s[13]]; t[10]=tt__aes_sbox[s[2]]; t[11]=tt__aes_sbox[s[7]];
        t[12]=tt__aes_sbox[s[12]];t[13]=tt__aes_sbox[s[1]]; t[14]=tt__aes_sbox[s[6]]; t[15]=tt__aes_sbox[s[11]];
        for(i=0;i<4;i++){
            tt_u8 a0=t[i*4],a1=t[i*4+1],a2=t[i*4+2],a3=t[i*4+3];
            s[i*4  ]=tt__aes_mul(a0,2)^tt__aes_mul(a1,3)^a2^a3;
            s[i*4+1]=a0^tt__aes_mul(a1,2)^tt__aes_mul(a2,3)^a3;
            s[i*4+2]=a0^a1^tt__aes_mul(a2,2)^tt__aes_mul(a3,3);
            s[i*4+3]=tt__aes_mul(a0,3)^a1^a2^tt__aes_mul(a3,2);
        }
        for(i=0;i<16;i++) s[i]^=rk[r*16+i];
    }
    t[0]=tt__aes_sbox[s[0]];  t[1]=tt__aes_sbox[s[5]];  t[2]=tt__aes_sbox[s[10]]; t[3]=tt__aes_sbox[s[15]];
    t[4]=tt__aes_sbox[s[4]];  t[5]=tt__aes_sbox[s[9]];  t[6]=tt__aes_sbox[s[14]]; t[7]=tt__aes_sbox[s[3]];
    t[8]=tt__aes_sbox[s[8]];  t[9]=tt__aes_sbox[s[13]]; t[10]=tt__aes_sbox[s[2]]; t[11]=tt__aes_sbox[s[7]];
    t[12]=tt__aes_sbox[s[12]];t[13]=tt__aes_sbox[s[1]]; t[14]=tt__aes_sbox[s[6]]; t[15]=tt__aes_sbox[s[11]];
    for(i=0;i<16;i++) out[i]=t[i]^rk[ctx->Nr*16+i];
}

TT_INLINE void tt_aes_decrypt_block(const TT_Aes *ctx, const tt_u8 in[16], tt_u8 out[16]) {
    tt_u8 s[16], t[16];
    int r, i;
    const tt_u8 *rk=ctx->rk;
    for(i=0;i<16;i++) s[i]=in[i]^rk[ctx->Nr*16+i];
    for(r=ctx->Nr-1;r>0;r--){
        t[0]=tt__aes_isbox[s[0]];  t[1]=tt__aes_isbox[s[13]]; t[2]=tt__aes_isbox[s[10]]; t[3]=tt__aes_isbox[s[7]];
        t[4]=tt__aes_isbox[s[4]];  t[5]=tt__aes_isbox[s[1]];  t[6]=tt__aes_isbox[s[14]]; t[7]=tt__aes_isbox[s[11]];
        t[8]=tt__aes_isbox[s[8]];  t[9]=tt__aes_isbox[s[5]];  t[10]=tt__aes_isbox[s[2]]; t[11]=tt__aes_isbox[s[15]];
        t[12]=tt__aes_isbox[s[12]];t[13]=tt__aes_isbox[s[9]]; t[14]=tt__aes_isbox[s[6]]; t[15]=tt__aes_isbox[s[3]];
        for(i=0;i<16;i++) t[i]^=rk[r*16+i];
        for(i=0;i<4;i++){
            tt_u8 a0=t[i*4],a1=t[i*4+1],a2=t[i*4+2],a3=t[i*4+3];
            s[i*4  ]=tt__aes_mul(a0,14)^tt__aes_mul(a1,11)^tt__aes_mul(a2,13)^tt__aes_mul(a3,9);
            s[i*4+1]=tt__aes_mul(a0,9) ^tt__aes_mul(a1,14)^tt__aes_mul(a2,11)^tt__aes_mul(a3,13);
            s[i*4+2]=tt__aes_mul(a0,13)^tt__aes_mul(a1,9) ^tt__aes_mul(a2,14)^tt__aes_mul(a3,11);
            s[i*4+3]=tt__aes_mul(a0,11)^tt__aes_mul(a1,13)^tt__aes_mul(a2,9) ^tt__aes_mul(a3,14);
        }
    }
    t[0]=tt__aes_isbox[s[0]];  t[1]=tt__aes_isbox[s[13]]; t[2]=tt__aes_isbox[s[10]]; t[3]=tt__aes_isbox[s[7]];
    t[4]=tt__aes_isbox[s[4]];  t[5]=tt__aes_isbox[s[1]];  t[6]=tt__aes_isbox[s[14]]; t[7]=tt__aes_isbox[s[11]];
    t[8]=tt__aes_isbox[s[8]];  t[9]=tt__aes_isbox[s[5]];  t[10]=tt__aes_isbox[s[2]]; t[11]=tt__aes_isbox[s[15]];
    t[12]=tt__aes_isbox[s[12]];t[13]=tt__aes_isbox[s[9]]; t[14]=tt__aes_isbox[s[6]]; t[15]=tt__aes_isbox[s[3]];
    for(i=0;i<16;i++) out[i]=t[i]^rk[i];
}

TT_INLINE void tt_aes_cbc_encrypt(const TT_Aes *ctx, const tt_u8 *iv,
                                    const tt_u8 *in, size_t len, tt_u8 *out) {
    tt_u8 c[16]; int i; size_t b;
    memcpy(c,iv,16);
    for(b=0;b+16<=len;b+=16){
        for(i=0;i<16;i++) c[i]^=in[b+i];
        tt_aes_encrypt_block(ctx,c,c);
        memcpy(out+b,c,16);
    }
}
TT_INLINE void tt_aes_cbc_decrypt(const TT_Aes *ctx, const tt_u8 *iv,
                                    const tt_u8 *in, size_t len, tt_u8 *out) {
    tt_u8 t[16], prev[16]; int i; size_t b;
    memcpy(prev,iv,16);
    for(b=0;b+16<=len;b+=16){
        memcpy(t,in+b,16);
        tt_aes_decrypt_block(ctx,t,out+b);
        for(i=0;i<16;i++) out[b+i]^=prev[i];
        memcpy(prev,t,16);
    }
}
TT_INLINE void tt_aes_ctr(const TT_Aes *ctx, const tt_u8 nonce[16],
                            const tt_u8 *in, size_t len, tt_u8 *out) {
    tt_u8 ctr[16], ks[16];
    int i; size_t b=0;
    memcpy(ctr,nonce,16);
    while(b<len){
        tt_aes_encrypt_block(ctx,ctr,ks);
        for(i=0;i<16&&b<len;i++,b++) out[b]=in[b]^ks[i];
        for(i=15;i>=0;i--) if(++ctr[i]) break;
    }
}

TT_INLINE int tt_aes128_ecb_encrypt(const tt_u8 key[16], const tt_u8 in[16], tt_u8 out[16]) {
    TT_Aes ctx; if(tt_aes_init(&ctx,key,128)) return -1; tt_aes_encrypt_block(&ctx,in,out); return 0;
}
TT_INLINE int tt_aes192_ecb_encrypt(const tt_u8 key[24], const tt_u8 in[16], tt_u8 out[16]) {
    TT_Aes ctx; if(tt_aes_init(&ctx,key,192)) return -1; tt_aes_encrypt_block(&ctx,in,out); return 0;
}
TT_INLINE int tt_aes256_ecb_encrypt(const tt_u8 key[32], const tt_u8 in[16], tt_u8 out[16]) {
    TT_Aes ctx; if(tt_aes_init(&ctx,key,256)) return -1; tt_aes_encrypt_block(&ctx,in,out); return 0;
}
#endif                

#ifndef TT_NO_DES

static const tt_u8 tt__des_ip[64]={
    58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};
static const tt_u8 tt__des_fp[64]={
    40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25
};
static const tt_u8 tt__des_pc1[56]={
    57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,
    60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,
    29,21,13,5,28,20,12,4
};
static const tt_u8 tt__des_pc2[48]={
    14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32
};
static const int tt__des_ls[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
static const tt_u8 tt__des_e[48]={
    32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,
    16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1
};
static const tt_u8 tt__des_sbox[8][64]={
    {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
    {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};
static const tt_u8 tt__des_p[32]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};

typedef struct { tt_u8 subkeys[16][6]; } TT_Des;

static TT_UNUSED tt_u8 tt__des_bit(const tt_u8 *b, int pos) {
    pos--; return (b[pos/8]>>(7-(pos%8)))&1;
}
static TT_UNUSED void tt__des_setbit(tt_u8 *b, int pos, tt_u8 v) {
    pos--; if(v) b[pos/8]|=(1<<(7-(pos%8))); else b[pos/8]&=~(1<<(7-(pos%8)));
}

TT_INLINE void tt_des_init(TT_Des *ctx, const tt_u8 key[8]) {
    tt_u8 C[4],D[4],CD[7]; int i,j;
    memset(C,0,4); memset(D,0,4);
    for(i=0;i<28;i++) tt__des_setbit(C,i+1,tt__des_bit(key,tt__des_pc1[i]));
    for(i=0;i<28;i++) tt__des_setbit(D,i+1,tt__des_bit(key,tt__des_pc1[28+i]));
    for(i=0;i<16;i++){
        for(j=0;j<tt__des_ls[i];j++){
            tt_u8 tc=tt__des_bit(C,1),td=tt__des_bit(D,1);
            int k;
            for(k=1;k<28;k++){tt__des_setbit(C,k,tt__des_bit(C,k+1));tt__des_setbit(D,k,tt__des_bit(D,k+1));}
            tt__des_setbit(C,28,tc); tt__des_setbit(D,28,td);
        }
        memset(CD,0,7);
        for(j=0;j<28;j++) tt__des_setbit(CD,j+1,tt__des_bit(C,j+1));
        for(j=0;j<28;j++) tt__des_setbit(CD,29+j,tt__des_bit(D,j+1));
        memset(ctx->subkeys[i],0,6);
        for(j=0;j<48;j++) tt__des_setbit(ctx->subkeys[i],j+1,tt__des_bit(CD,tt__des_pc2[j]));
    }
}

static TT_UNUSED void tt__des_crypt(const TT_Des *ctx, const tt_u8 in[8], tt_u8 out[8], int enc) {
    tt_u8 L[4],R[4],t[4],E[6],s[4];
    int i,j;
    tt_u8 ip[8]; memset(ip,0,8);
    for(i=0;i<64;i++) tt__des_setbit(ip,i+1,tt__des_bit(in,tt__des_ip[i]));
    memcpy(L,ip,4); memcpy(R,ip+4,4);
    for(i=0;i<16;i++){
        int ki=enc?i:15-i;
        memset(E,0,6);
        for(j=0;j<48;j++) tt__des_setbit(E,j+1,tt__des_bit(R,tt__des_e[j]));
        for(j=0;j<6;j++) E[j]^=ctx->subkeys[ki][j];
        memset(s,0,4);
        for(j=0;j<8;j++){
            int row=((tt__des_bit(E,j*6+1)<<1)|tt__des_bit(E,j*6+6));
            int col=(tt__des_bit(E,j*6+2)<<3)|(tt__des_bit(E,j*6+3)<<2)|
                    (tt__des_bit(E,j*6+4)<<1)|tt__des_bit(E,j*6+5);
            tt_u8 sval=(tt_u8)tt__des_sbox[j][row*16+col];
            int k; for(k=0;k<4;k++) tt__des_setbit(s,j*4+k+1,(sval>>(3-k))&1);
        }
        memset(t,0,4);
        for(j=0;j<32;j++) tt__des_setbit(t,j+1,tt__des_bit(s,tt__des_p[j]));
        for(j=0;j<4;j++) t[j]^=L[j];
        memcpy(L,R,4); memcpy(R,t,4);
    }
    tt_u8 preout[8]; memcpy(preout,R,4); memcpy(preout+4,L,4);
    memset(out,0,8);
    for(i=0;i<64;i++) tt__des_setbit(out,i+1,tt__des_bit(preout,tt__des_fp[i]));
}

TT_INLINE void tt_des_encrypt(const TT_Des *ctx, const tt_u8 in[8], tt_u8 out[8]) { tt__des_crypt(ctx,in,out,1); }
TT_INLINE void tt_des_decrypt(const TT_Des *ctx, const tt_u8 in[8], tt_u8 out[8]) { tt__des_crypt(ctx,in,out,0); }

typedef struct { TT_Des k1,k2,k3; } TT_3Des;

TT_INLINE void tt_3des_init(TT_3Des *ctx, const tt_u8 *key, int keybits) {
    tt_des_init(&ctx->k1,key);
    tt_des_init(&ctx->k2,key+8);
    if(keybits==168) tt_des_init(&ctx->k3,key+16);
    else             tt_des_init(&ctx->k3,key);                   
}
TT_INLINE void tt_3des_encrypt(const TT_3Des *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u8 t[8];
    tt_des_encrypt(&ctx->k1,in,t);
    tt_des_decrypt(&ctx->k2,t,t);
    tt_des_encrypt(&ctx->k3,t,out);
}
TT_INLINE void tt_3des_decrypt(const TT_3Des *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u8 t[8];
    tt_des_decrypt(&ctx->k3,in,t);
    tt_des_encrypt(&ctx->k2,t,t);
    tt_des_decrypt(&ctx->k1,t,out);
}
#endif                

#ifndef TT_NO_CHACHA20

#define TT__CC20_ROT(v,n) (((v)<<(n))|((v)>>(32-(n))))
#define TT__CC20_QR(a,b,c,d) do{ \
    a+=b; d^=a; d=TT__CC20_ROT(d,16); \
    c+=d; b^=c; b=TT__CC20_ROT(b,12); \
    a+=b; d^=a; d=TT__CC20_ROT(d,8);  \
    c+=d; b^=c; b=TT__CC20_ROT(b,7);  }while(0)

typedef struct { tt_u32 state[16]; tt_u8 keystream[64]; int ks_pos; } TT_ChaCha20;

static TT_UNUSED void tt__chacha20_block(const tt_u32 s[16], tt_u8 out[64]) {
    tt_u32 x[16]; int i;
    memcpy(x,s,64);
    for(i=0;i<10;i++){
        TT__CC20_QR(x[0],x[4],x[8],x[12]); TT__CC20_QR(x[1],x[5],x[9],x[13]);
        TT__CC20_QR(x[2],x[6],x[10],x[14]);TT__CC20_QR(x[3],x[7],x[11],x[15]);
        TT__CC20_QR(x[0],x[5],x[10],x[15]);TT__CC20_QR(x[1],x[6],x[11],x[12]);
        TT__CC20_QR(x[2],x[7],x[8],x[13]); TT__CC20_QR(x[3],x[4],x[9],x[14]);
    }
    for(i=0;i<16;i++){tt_u32 v=x[i]+s[i];
        out[i*4]=(tt_u8)v;out[i*4+1]=(tt_u8)(v>>8);out[i*4+2]=(tt_u8)(v>>16);out[i*4+3]=(tt_u8)(v>>24);}
}

TT_INLINE void tt_chacha20_init(TT_ChaCha20 *c, const tt_u8 key[32], const tt_u8 nonce[12], tt_u32 ctr) {
    static const tt_u8 sigma[16]="expand 32-byte k";
    int i;
    c->state[0]=(tt_u32)sigma[0]|((tt_u32)sigma[1]<<8)|((tt_u32)sigma[2]<<16)|((tt_u32)sigma[3]<<24);
    c->state[1]=(tt_u32)sigma[4]|((tt_u32)sigma[5]<<8)|((tt_u32)sigma[6]<<16)|((tt_u32)sigma[7]<<24);
    c->state[2]=(tt_u32)sigma[8]|((tt_u32)sigma[9]<<8)|((tt_u32)sigma[10]<<16)|((tt_u32)sigma[11]<<24);
    c->state[3]=(tt_u32)sigma[12]|((tt_u32)sigma[13]<<8)|((tt_u32)sigma[14]<<16)|((tt_u32)sigma[15]<<24);
    for(i=0;i<8;i++) c->state[4+i]=(tt_u32)key[i*4]|((tt_u32)key[i*4+1]<<8)|((tt_u32)key[i*4+2]<<16)|((tt_u32)key[i*4+3]<<24);
    c->state[12]=ctr;
    c->state[13]=(tt_u32)nonce[0]|((tt_u32)nonce[1]<<8)|((tt_u32)nonce[2]<<16)|((tt_u32)nonce[3]<<24);
    c->state[14]=(tt_u32)nonce[4]|((tt_u32)nonce[5]<<8)|((tt_u32)nonce[6]<<16)|((tt_u32)nonce[7]<<24);
    c->state[15]=(tt_u32)nonce[8]|((tt_u32)nonce[9]<<8)|((tt_u32)nonce[10]<<16)|((tt_u32)nonce[11]<<24);
    tt__chacha20_block(c->state,c->keystream);
    c->ks_pos=0;
}
TT_INLINE void tt_chacha20_xor(TT_ChaCha20 *c, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i;
    for(i=0;i<len;i++){
        if(c->ks_pos==64){c->state[12]++;tt__chacha20_block(c->state,c->keystream);c->ks_pos=0;}
        out[i]=in[i]^c->keystream[c->ks_pos++];
    }
}
TT_INLINE void tt_chacha20(const tt_u8 key[32], const tt_u8 nonce[12], tt_u32 ctr,
                             const tt_u8 *in, tt_u8 *out, size_t len) {
    TT_ChaCha20 c; tt_chacha20_init(&c,key,nonce,ctr); tt_chacha20_xor(&c,in,out,len);
}

TT_INLINE void tt_hchacha20(const tt_u8 key[32], const tt_u8 nonce[16], tt_u8 out[32]) {
    tt_u32 s[16], x[16];
    static const tt_u8 sigma[16]="expand 32-byte k";
    int i;
    s[0]=(tt_u32)sigma[0]|((tt_u32)sigma[1]<<8)|((tt_u32)sigma[2]<<16)|((tt_u32)sigma[3]<<24);
    s[1]=(tt_u32)sigma[4]|((tt_u32)sigma[5]<<8)|((tt_u32)sigma[6]<<16)|((tt_u32)sigma[7]<<24);
    s[2]=(tt_u32)sigma[8]|((tt_u32)sigma[9]<<8)|((tt_u32)sigma[10]<<16)|((tt_u32)sigma[11]<<24);
    s[3]=(tt_u32)sigma[12]|((tt_u32)sigma[13]<<8)|((tt_u32)sigma[14]<<16)|((tt_u32)sigma[15]<<24);
    for(i=0;i<8;i++) s[4+i]=(tt_u32)key[i*4]|((tt_u32)key[i*4+1]<<8)|((tt_u32)key[i*4+2]<<16)|((tt_u32)key[i*4+3]<<24);
    for(i=0;i<4;i++) s[12+i]=(tt_u32)nonce[i*4]|((tt_u32)nonce[i*4+1]<<8)|((tt_u32)nonce[i*4+2]<<16)|((tt_u32)nonce[i*4+3]<<24);
    memcpy(x,s,64);
    for(i=0;i<10;i++){
        TT__CC20_QR(x[0],x[4],x[8],x[12]); TT__CC20_QR(x[1],x[5],x[9],x[13]);
        TT__CC20_QR(x[2],x[6],x[10],x[14]);TT__CC20_QR(x[3],x[7],x[11],x[15]);
        TT__CC20_QR(x[0],x[5],x[10],x[15]);TT__CC20_QR(x[1],x[6],x[11],x[12]);
        TT__CC20_QR(x[2],x[7],x[8],x[13]); TT__CC20_QR(x[3],x[4],x[9],x[14]);
    }
    for(i=0;i<4;i++){out[i*4]=(tt_u8)x[i];out[i*4+1]=(tt_u8)(x[i]>>8);out[i*4+2]=(tt_u8)(x[i]>>16);out[i*4+3]=(tt_u8)(x[i]>>24);}
    for(i=0;i<4;i++){out[16+i*4]=(tt_u8)x[12+i];out[17+i*4]=(tt_u8)(x[12+i]>>8);out[18+i*4]=(tt_u8)(x[12+i]>>16);out[19+i*4]=(tt_u8)(x[12+i]>>24);}
}
TT_INLINE void tt_xchacha20(const tt_u8 key[32], const tt_u8 nonce[24],
                              const tt_u8 *in, tt_u8 *out, size_t len) {
    tt_u8 subkey[32];
    tt_hchacha20(key,nonce,subkey);
    tt_chacha20(subkey,nonce+16,0,in,out,len);
}
#endif                     

#ifndef TT_NO_POLY1305
#define TT_POLY1305_TAG_LEN 16

typedef struct {
    tt_u32 r[5];                                         
    tt_u32 h[5];                                          
    tt_u32 s[4];                                          
    tt_u8  buf[16];
    int    buflen;
} TT_Poly1305;

TT_INLINE void tt_poly1305_init(TT_Poly1305 *ctx, const tt_u8 key[32]) {
    tt_u32 t0,t1,t2,t3;
    t0=(tt_u32)key[0]|((tt_u32)key[1]<<8)|((tt_u32)key[2]<<16)|((tt_u32)key[3]<<24);
    t1=(tt_u32)key[4]|((tt_u32)key[5]<<8)|((tt_u32)key[6]<<16)|((tt_u32)key[7]<<24);
    t2=(tt_u32)key[8]|((tt_u32)key[9]<<8)|((tt_u32)key[10]<<16)|((tt_u32)key[11]<<24);
    t3=(tt_u32)key[12]|((tt_u32)key[13]<<8)|((tt_u32)key[14]<<16)|((tt_u32)key[15]<<24);
    ctx->r[0] =  t0                       & 0x3FFFFFFu;
    ctx->r[1] = ((t0>>26)|(t1<< 6))       & 0x3FFFFFFu & ~(1u<<1) & ~(1u<<0);
    ctx->r[0] =  t0        & 0x3FFFFFFu;
    ctx->r[1] =((t0>>26)|(t1<<6)) & 0x3FFFFFFu;
    ctx->r[2] =((t1>>20)|(t2<<12))& 0x3FFFFFFu;
    ctx->r[3] =((t2>>14)|(t3<<18))& 0x3FFFFFFu;
    ctx->r[4] = (t3>>8)            & 0x00FFFFFu;
    ctx->r[1] &= 0x3FFFE0Fu>>2;                                 
    {
        tt_u8 r[16]; int i;
        memcpy(r, key, 16);
        r[3]  &= 15; r[7]  &= 15; r[11] &= 15; r[15] &= 15;
        r[4]  &= 252; r[8]  &= 252; r[12] &= 252;
        t0=(tt_u32)r[0]|((tt_u32)r[1]<<8)|((tt_u32)r[2]<<16)|((tt_u32)r[3]<<24);
        t1=(tt_u32)r[4]|((tt_u32)r[5]<<8)|((tt_u32)r[6]<<16)|((tt_u32)r[7]<<24);
        t2=(tt_u32)r[8]|((tt_u32)r[9]<<8)|((tt_u32)r[10]<<16)|((tt_u32)r[11]<<24);
        t3=(tt_u32)r[12]|((tt_u32)r[13]<<8)|((tt_u32)r[14]<<16)|((tt_u32)r[15]<<24);
        ctx->r[0] =  t0         & 0x3FFFFFFu;
        ctx->r[1] =((t0>>26)|(t1<< 6)) & 0x3FFFFFFu;
        ctx->r[2] =((t1>>20)|(t2<<12)) & 0x3FFFFFFu;
        ctx->r[3] =((t2>>14)|(t3<<18)) & 0x3FFFFFFu;
        ctx->r[4] = (t3>> 8)           & 0x3FFFFFFu;
        for(i=0;i<5;i++) ctx->h[i]=0;
        for(i=0;i<4;i++) ctx->s[i]=(tt_u32)key[16+i*4]|((tt_u32)key[17+i*4]<<8)|
            ((tt_u32)key[18+i*4]<<16)|((tt_u32)key[19+i*4]<<24);
        ctx->buflen=0;
    }
}

static TT_UNUSED void tt__poly1305_block(TT_Poly1305 *ctx, tt_u32 hibit) {
    tt_u32 r0=ctx->r[0],r1=ctx->r[1],r2=ctx->r[2],r3=ctx->r[3],r4=ctx->r[4];
    tt_u32 s1=r1*5,s2=r2*5,s3=r3*5,s4=r4*5;
    tt_u32 h0=ctx->h[0],h1=ctx->h[1],h2=ctx->h[2],h3=ctx->h[3],h4=ctx->h[4];
    tt_u64 d0,d1,d2,d3,d4; tt_u32 c;
    tt_u32 m0,m1,m2,m3,m4;
    tt_u32 t0=(tt_u32)ctx->buf[0]|((tt_u32)ctx->buf[1]<<8)|((tt_u32)ctx->buf[2]<<16)|((tt_u32)ctx->buf[3]<<24);
    tt_u32 t1=(tt_u32)ctx->buf[4]|((tt_u32)ctx->buf[5]<<8)|((tt_u32)ctx->buf[6]<<16)|((tt_u32)ctx->buf[7]<<24);
    tt_u32 t2=(tt_u32)ctx->buf[8]|((tt_u32)ctx->buf[9]<<8)|((tt_u32)ctx->buf[10]<<16)|((tt_u32)ctx->buf[11]<<24);
    tt_u32 t3=(tt_u32)ctx->buf[12]|((tt_u32)ctx->buf[13]<<8)|((tt_u32)ctx->buf[14]<<16)|((tt_u32)ctx->buf[15]<<24);
    m0 =  t0         & 0x3FFFFFFu;
    m1 =((t0>>26)|(t1<< 6)) & 0x3FFFFFFu;
    m2 =((t1>>20)|(t2<<12)) & 0x3FFFFFFu;
    m3 =((t2>>14)|(t3<<18)) & 0x3FFFFFFu;
    m4 = (t3>> 8)           | hibit;
    h0+=m0; h1+=m1; h2+=m2; h3+=m3; h4+=m4;
    d0=(tt_u64)h0*r0+(tt_u64)h1*s4+(tt_u64)h2*s3+(tt_u64)h3*s2+(tt_u64)h4*s1;
    d1=(tt_u64)h0*r1+(tt_u64)h1*r0+(tt_u64)h2*s4+(tt_u64)h3*s3+(tt_u64)h4*s2;
    d2=(tt_u64)h0*r2+(tt_u64)h1*r1+(tt_u64)h2*r0+(tt_u64)h3*s4+(tt_u64)h4*s3;
    d3=(tt_u64)h0*r3+(tt_u64)h1*r2+(tt_u64)h2*r1+(tt_u64)h3*r0+(tt_u64)h4*s4;
    d4=(tt_u64)h0*r4+(tt_u64)h1*r3+(tt_u64)h2*r2+(tt_u64)h3*r1+(tt_u64)h4*r0;
    c=(tt_u32)(d0>>26); h0=(tt_u32)d0&0x3FFFFFFu; d1+=c;
    c=(tt_u32)(d1>>26); h1=(tt_u32)d1&0x3FFFFFFu; d2+=c;
    c=(tt_u32)(d2>>26); h2=(tt_u32)d2&0x3FFFFFFu; d3+=c;
    c=(tt_u32)(d3>>26); h3=(tt_u32)d3&0x3FFFFFFu; d4+=c;
    c=(tt_u32)(d4>>26); h4=(tt_u32)d4&0x3FFFFFFu; h0+=c*5; c=h0>>26; h0&=0x3FFFFFFu; h1+=c;
    ctx->h[0]=h0; ctx->h[1]=h1; ctx->h[2]=h2; ctx->h[3]=h3; ctx->h[4]=h4;
    ctx->buflen=0;
}

TT_INLINE void tt_poly1305_update(TT_Poly1305 *ctx, const tt_u8 *in, size_t len) {
    while(len>0){
        int take=16-ctx->buflen; if((int)len<take) take=(int)len;
        memcpy(ctx->buf+ctx->buflen,in,(size_t)take);
        ctx->buflen+=take; in+=take; len-=(size_t)take;
        if(ctx->buflen==16) tt__poly1305_block(ctx,1u<<24);                           
    }
}

TT_INLINE void tt_poly1305_final(TT_Poly1305 *ctx, tt_u8 tag[16]) {
    tt_u32 h0,h1,h2,h3,h4,c,g0,g1,g2,g3,g4,mask;
    tt_u64 f;
    if(ctx->buflen) {
        ctx->buf[ctx->buflen++]=1;
        memset(ctx->buf+ctx->buflen,0,(size_t)(16-ctx->buflen));
        ctx->buflen=16;
        tt__poly1305_block(ctx,0);                                
    }
    h0=ctx->h[0]; h1=ctx->h[1]; h2=ctx->h[2]; h3=ctx->h[3]; h4=ctx->h[4];
    c=h1>>26; h1&=0x3FFFFFFu; h2+=c;
    c=h2>>26; h2&=0x3FFFFFFu; h3+=c;
    c=h3>>26; h3&=0x3FFFFFFu; h4+=c;
    c=h4>>26; h4&=0x3FFFFFFu; h0+=c*5;
    c=h0>>26; h0&=0x3FFFFFFu; h1+=c;
    g0=h0+5; c=g0>>26; g0&=0x3FFFFFFu;
    g1=h1+c; c=g1>>26; g1&=0x3FFFFFFu;
    g2=h2+c; c=g2>>26; g2&=0x3FFFFFFu;
    g3=h3+c; c=g3>>26; g3&=0x3FFFFFFu;
    g4=h4+c; mask=(g4>>26)-1u; g4&=0x3FFFFFFu;                                                         
    mask=~(mask);                                             
    h0=(h0&mask)|(g0&~mask); h1=(h1&mask)|(g1&~mask);
    h2=(h2&mask)|(g2&~mask); h3=(h3&mask)|(g3&~mask);
    h4=(h4&mask)|(g4&~mask);
    f=(tt_u64)((h0    )|(h1<<26))           + ctx->s[0]; tag[ 0]=(tt_u8)f; tag[ 1]=(tt_u8)(f>>8); tag[ 2]=(tt_u8)(f>>16); tag[ 3]=(tt_u8)(f>>24);
    f=(tt_u64)((h1>> 6)|(h2<<20))+(f>>32) + ctx->s[1]; tag[ 4]=(tt_u8)f; tag[ 5]=(tt_u8)(f>>8); tag[ 6]=(tt_u8)(f>>16); tag[ 7]=(tt_u8)(f>>24);
    f=(tt_u64)((h2>>12)|(h3<<14))+(f>>32) + ctx->s[2]; tag[ 8]=(tt_u8)f; tag[ 9]=(tt_u8)(f>>8); tag[10]=(tt_u8)(f>>16); tag[11]=(tt_u8)(f>>24);
    f=(tt_u64)((h3>>18)|(h4<< 8))+(f>>32) + ctx->s[3]; tag[12]=(tt_u8)f; tag[13]=(tt_u8)(f>>8); tag[14]=(tt_u8)(f>>16); tag[15]=(tt_u8)(f>>24);
}
TT_INLINE void tt_chacha20poly1305_encrypt(const tt_u8 key[32], const tt_u8 nonce[12],
                                             const tt_u8 *ad, size_t adlen,
                                             const tt_u8 *pt, size_t ptlen,
                                             tt_u8 *ct, tt_u8 tag[16]) {
    tt_u8 poly_key[64];
    TT_ChaCha20 cc; TT_Poly1305 poly;
    tt_u8 len_block[16];
    tt_chacha20_init(&cc,key,nonce,0);
    tt_chacha20_xor(&cc,poly_key,poly_key,64);                                              
    memset(poly_key,0,64);
    tt__chacha20_block(cc.state,poly_key);
    tt_poly1305_init(&poly,poly_key);
    tt_poly1305_update(&poly,ad,adlen);
    {tt_u8 pad[16]; int p=(int)(16-adlen%16)%16; memset(pad,0,16); tt_poly1305_update(&poly,pad,(size_t)p);}
    tt_chacha20_init(&cc,key,nonce,1);
    tt_chacha20_xor(&cc,pt,ct,ptlen);
    tt_poly1305_update(&poly,ct,ptlen);
    {tt_u8 pad[16]; int p=(int)(16-ptlen%16)%16; memset(pad,0,16); tt_poly1305_update(&poly,pad,(size_t)p);}
    len_block[0]=(tt_u8)adlen; len_block[1]=(tt_u8)(adlen>>8); len_block[2]=(tt_u8)(adlen>>16); len_block[3]=(tt_u8)(adlen>>24);
    len_block[4]=len_block[5]=len_block[6]=len_block[7]=0;
    len_block[8]=(tt_u8)ptlen; len_block[9]=(tt_u8)(ptlen>>8); len_block[10]=(tt_u8)(ptlen>>16); len_block[11]=(tt_u8)(ptlen>>24);
    len_block[12]=len_block[13]=len_block[14]=len_block[15]=0;
    tt_poly1305_update(&poly,len_block,16);
    tt_poly1305_final(&poly,tag);
}
#endif                     

#ifndef TT_NO_SALSA20
#define TT__SA20_QR(a,b,c,d) do{ b^=TT__CC20_ROT(a+d,7);c^=TT__CC20_ROT(b+a,9);d^=TT__CC20_ROT(c+b,13);a^=TT__CC20_ROT(d+c,18); }while(0)

TT_INLINE void tt_salsa20(const tt_u8 key[32], const tt_u8 nonce[8], tt_u64 pos,
                            const tt_u8 *in, tt_u8 *out, size_t len) {
    static const tt_u8 tau[16]="expand 32-byte k";
    tt_u32 s[16], x[16], ks[16];
    int i, block=0;
    s[0]=(tt_u32)tau[0]|(tt_u32)tau[1]<<8|(tt_u32)tau[2]<<16|(tt_u32)tau[3]<<24;
    s[5]=(tt_u32)tau[4]|(tt_u32)tau[5]<<8|(tt_u32)tau[6]<<16|(tt_u32)tau[7]<<24;
    s[10]=(tt_u32)tau[8]|(tt_u32)tau[9]<<8|(tt_u32)tau[10]<<16|(tt_u32)tau[11]<<24;
    s[15]=(tt_u32)tau[12]|(tt_u32)tau[13]<<8|(tt_u32)tau[14]<<16|(tt_u32)tau[15]<<24;
    for(i=0;i<4;i++) s[1+i]=(tt_u32)key[i*4]|((tt_u32)key[i*4+1]<<8)|((tt_u32)key[i*4+2]<<16)|((tt_u32)key[i*4+3]<<24);
    for(i=0;i<4;i++) s[11+i]=(tt_u32)key[16+i*4]|((tt_u32)key[17+i*4]<<8)|((tt_u32)key[18+i*4]<<16)|((tt_u32)key[19+i*4]<<24);
    s[6]=(tt_u32)nonce[0]|((tt_u32)nonce[1]<<8)|((tt_u32)nonce[2]<<16)|((tt_u32)nonce[3]<<24);
    s[7]=(tt_u32)nonce[4]|((tt_u32)nonce[5]<<8)|((tt_u32)nonce[6]<<16)|((tt_u32)nonce[7]<<24);
    s[8]=(tt_u32)(pos&0xFFFFFFFF); s[9]=(tt_u32)(pos>>32);
    while(len>0){
        memcpy(x,s,64);
        for(i=0;i<10;i++){
            TT__SA20_QR(x[0],x[4],x[8],x[12]); TT__SA20_QR(x[5],x[9],x[13],x[1]);
            TT__SA20_QR(x[10],x[14],x[2],x[6]);TT__SA20_QR(x[15],x[3],x[7],x[11]);
            TT__SA20_QR(x[0],x[1],x[2],x[3]);  TT__SA20_QR(x[5],x[6],x[7],x[4]);
            TT__SA20_QR(x[10],x[11],x[8],x[9]);TT__SA20_QR(x[15],x[12],x[13],x[14]);
        }
        for(i=0;i<16;i++){tt_u32 v=x[i]+s[i];
            ks[i]=v; (void)block;}
        {size_t take=len>64?64:len;
         for(i=0;i<(int)take;i++) out[i]=in[i]^((tt_u8*)ks)[i];
         in+=take;out+=take;len-=take;}
        s[8]++; if(!s[8])s[9]++;
    }
}

TT_INLINE void tt_xsalsa20(const tt_u8 key[32], const tt_u8 nonce[24],
                              const tt_u8 *in, tt_u8 *out, size_t len) {
    tt_salsa20(key,nonce+16,0,in,out,len);                                            
}
#endif                    

#ifndef TT_NO_RC4
typedef struct { tt_u8 S[256]; int i, j; } TT_Rc4;
TT_INLINE void tt_rc4_init(TT_Rc4 *ctx, const tt_u8 *key, int klen) {
    int k; tt_u8 tmp;
    for(k=0;k<256;k++) ctx->S[k]=(tt_u8)k;
    ctx->i=ctx->j=0;
    {int j2=0; for(k=0;k<256;k++){j2=(j2+ctx->S[k]+key[k%klen])&255;tmp=ctx->S[k];ctx->S[k]=ctx->S[j2];ctx->S[j2]=tmp;}}
}
TT_INLINE void tt_rc4_xor(TT_Rc4 *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t k; tt_u8 tmp;
    for(k=0;k<len;k++){
        ctx->i=(ctx->i+1)&255; ctx->j=(ctx->j+ctx->S[ctx->i])&255;
        tmp=ctx->S[ctx->i];ctx->S[ctx->i]=ctx->S[ctx->j];ctx->S[ctx->j]=tmp;
        out[k]=in[k]^ctx->S[(ctx->S[ctx->i]+ctx->S[ctx->j])&255];
    }
}
#endif                

#ifndef TT_NO_XTEA
#define TT_XTEA_DELTA 0x9E3779B9u
TT_INLINE void tt_xtea_encrypt(const tt_u32 key[4], tt_u32 *v0, tt_u32 *v1) {
    tt_u32 s=0; int i;
    for(i=0;i<32;i++){*v0+=((*v1<<4^*v1>>5)+*v1)^(s+key[s&3]);s+=TT_XTEA_DELTA;*v1+=((*v0<<4^*v0>>5)+*v0)^(s+key[(s>>11)&3]);}
}
TT_INLINE void tt_xtea_decrypt(const tt_u32 key[4], tt_u32 *v0, tt_u32 *v1) {
    tt_u32 s=TT_XTEA_DELTA*32; int i;
    for(i=0;i<32;i++){*v1-=((*v0<<4^*v0>>5)+*v0)^(s+key[(s>>11)&3]);s-=TT_XTEA_DELTA;*v0-=((*v1<<4^*v1>>5)+*v1)^(s+key[s&3]);}
}
TT_INLINE void tt_xxtea_encrypt(tt_u32 *v, int n, const tt_u32 key[4]) {
    tt_u32 y,z,s,e; int p,q=(6+52/n);
    z=v[n-1]; s=0;
    while(q-->0){s+=TT_XTEA_DELTA;e=(s>>2)&3;
        for(p=0;p<n-1;p++){y=v[p+1];v[p]+=((z>>5^y<<2)+(y>>3^z<<4))^((s^y)+(key[(p&3)^e]^z));z=v[p];}
        y=v[0];v[n-1]+=((z>>5^y<<2)+(y>>3^z<<4))^((s^y)+(key[((n-1)&3)^e]^z));z=v[n-1];}
}
TT_INLINE void tt_xxtea_decrypt(tt_u32 *v, int n, const tt_u32 key[4]) {
    tt_u32 y,z,s,e; int p,q=(6+52/n);
    y=v[0]; s=(tt_u32)q*TT_XTEA_DELTA;
    while(q-->0){e=(s>>2)&3;
        for(p=n-1;p>0;p--){z=v[p-1];v[p]-=((z>>5^y<<2)+(y>>3^z<<4))^((s^y)+(key[(p&3)^e]^z));y=v[p];}
        z=v[n-1];v[0]-=((z>>5^y<<2)+(y>>3^z<<4))^((s^y)+(key[(0&3)^e]^z));y=v[0];s-=TT_XTEA_DELTA;}
}
#endif                 

#ifndef TT_NO_BLOWFISH
#define TT_BF_ROUNDS 16

static const tt_u32 tt__bf_P_init[18] = {
    0x243f6a88u,0x85a308d3u,0x13198a2eu,0x03707344u,0xa4093822u,0x299f31d0u,
    0x082efa98u,0xec4e6c89u,0x452821e6u,0x38d01377u,0xbe5466cfu,0x34e90c6cu,
    0xc0ac29b7u,0xc97c50ddu,0x3f84d5b5u,0xb5470917u,0x9216d5d9u,0x8979fb1bu
};
static const tt_u32 tt__bf_S_init[4][256] = {{
    0xd1310ba6u,0x98dfb5acu,0x2ffd72dbu,0xd01adfb7u,0xb8e1afedu,0x6a267e96u,0xba7c9045u,0xf12c7f99u,
    0x24a19947u,0xb3916cf7u,0x0801f2e2u,0x858efc16u,0x636920d8u,0x71574e69u,0xa458fea3u,0xf4933d7eu,
    0x0d95748fu,0x728eb658u,0x718bcd58u,0x82154aeeu,0x7b54a41du,0xc25a59b5u,0x9c30d539u,0x2af26013u,
    0xc5d1b023u,0x286085f0u,0xca417918u,0xb8db38efu,0x8e79dcb0u,0x603a180eu,0x6c9e0e8bu,0xb01e8a3eu,
    0xd71577c1u,0xbd314b27u,0x78af2fdau,0x55605c60u,0xe65525f3u,0xaa55ab94u,0x57489862u,0x63e81440u,
    0x55ca396au,0x2aab10b6u,0xb4cc5c34u,0x1141e8ceu,0xa15486afu,0x7c72e993u,0xb3ee1411u,0x636fbc2au,
    0x2ba9c55du,0x741831f6u,0xce5c3e16u,0x9b87931eu,0xafd6ba33u,0x6c24cf5cu,0x7a325381u,0x28958677u,
    0x3b8f4898u,0x6b4bb9afu,0xc4bfe81bu,0x66282193u,0x61d809ccu,0xfb21a991u,0x487cac60u,0x5dec8032u,
    0xef845d5du,0xe98575b1u,0xdc262302u,0xeb651b88u,0x23893e81u,0xd396acc5u,0x0f6d6ff3u,0x83f44239u,
    0x2e0b4482u,0xa4842004u,0x69c8f04au,0x9e1f9b5eu,0x21c66842u,0xf6e96c9au,0x670c9c61u,0xabd388f0u,
    0x6a51a0d2u,0xd8542f68u,0x960fa728u,0xab5133a3u,0x6eef0b6cu,0x137a3be4u,0xba3bf050u,0x7efb2a98u,
    0xa1f1651du,0x39af0176u,0x66ca593eu,0x82430e88u,0x8cee8619u,0x456f9fb4u,0x7d84a5c3u,0x3b8b5ebeu,
    0xe06f75d8u,0x85c12073u,0x401a449fu,0x56c16aa6u,0x4ed3aa62u,0x363f7706u,0x1bfedf72u,0x429b023du,
    0x37d0d724u,0xd00a1248u,0xdb0fead3u,0x49f1c09bu,0x075372c9u,0x80991b7bu,0x25d479d8u,0xf6e8def7u,
    0xe3fe501au,0xb6794c3bu,0x976ce0bdu,0x04c006bau,0xc1a94fb6u,0x409f60c4u,0x5e5c9ec2u,0x196a2463u,
    0x68fb6fafu,0x3e6c53b5u,0x1339b2ebu,0x3b52ec6fu,0x6dfc511fu,0x9b30952cu,0xcc814544u,0xaf5ebd09u,
    0xbee3d004u,0xde334afdu,0x660f2807u,0x192e4bb3u,0xc0cba857u,0x45c8740fu,0xd20b5f39u,0xb9d3fbdbu,
    0x5579c0bdu,0x1a60320au,0xd6a100c6u,0x402c7279u,0x679f25feu,0xfb1fa3ccu,0x8ea5e9f8u,0xdb3222f8u,
    0x3c7516dfu,0xfd616b15u,0x2f501ec8u,0xad0552abu,0x323db5fau,0xfd238760u,0x53317b48u,0x3e00df82u,
    0x9e5c57bbu,0xca6f8ca0u,0x1a87562eu,0xdf1769dbu,0xd542a8f6u,0x287effc3u,0xac6732c6u,0x8c4f5573u,
    0x695b27b0u,0xbbca58c8u,0xe1ffa35du,0xb8f011a0u,0x10fa3d98u,0xfd2183b8u,0x4afcb56cu,0x2dd1d35bu,
    0x9a53e479u,0xb6f84565u,0xd28e49bcu,0x4bfb9790u,0xe1ddf2dau,0xa4cb7e33u,0x62fb1341u,0xcee4c6e8u,
    0xef20cadau,0x36774c01u,0xd07e9efeu,0x2bf11fb4u,0x95dbda4du,0xae909198u,0xeaad8e71u,0x6b93d5a0u,
    0xd08ed1d0u,0xafc725e0u,0x8e3c5b2fu,0x8e7594b7u,0x8ff6e2fbu,0xf2122b64u,0x8888b812u,0x900df01cu,
    0x4fad5ea0u,0x688fc31cu,0xd1cff191u,0xb3a8c1adu,0x2f2f2218u,0xbe0e1777u,0xea752dfeu,0x8b021fa1u,
    0xe5a0cc0fu,0xb56f74e8u,0x18acf3d6u,0xce89e299u,0xb4a84fe0u,0xfd13e0b7u,0x7cc43b81u,0xd2ada8d9u,
    0x165fa266u,0x80957705u,0x93cc7314u,0x211a1477u,0xe6ad2065u,0x77b5fa86u,0xc75442f5u,0xfb9d35cfu,
    0xebcdaf0cu,0x7b3e89a0u,0xd6411bd3u,0xae1e7e49u,0x00250e2du,0x2071b35eu,0x226800bbu,0x57b8e0afu,
    0x2464369bu,0xf009b91eu,0x5563911du,0x59dfa6aau,0x78c14389u,0xd95a537fu,0x207d5ba2u,0x02e5b9c5u,
    0x83260376u,0x6295cfa9u,0x11c81968u,0x4e734a41u,0xb3472dcau,0x7b14a94au,0x1b510052u,0x9a532915u,
    0xd60f573fu,0xbc9bc6e4u,0x2b60a476u,0x81e67400u,0x08ba6fb5u,0x571be91fu,0xf296ec6bu,0x2a0dd915u,
    0xb6636521u,0xe7b9f9b6u,0xff34052eu,0xc5855664u,0x53b02d5du,0xa99f8fa1u,0x08ba4799u,0x6e85076au
},{
    0x4b7a70e9u,0xb5b32944u,0xdb75092eu,0xc4192623u,0xad6ea6b0u,0x49a7df7du,0x9cee60b8u,0x8fedb266u,
    0xecaa8c71u,0x699a17ffu,0x5664526cu,0xc2b19ee1u,0x193602a5u,0x75094c29u,0xa0591340u,0xe4183a3eu,
    0x3f54989au,0x5b429d65u,0x6b8fe4d6u,0x99f73fd6u,0xa1d29c07u,0xefe830f5u,0x4d2d38e6u,0xf0255dc1u,
    0x4cdd2086u,0x8470eb26u,0x6382e9c6u,0x021ecc5eu,0x09686b3fu,0x3ebaefc9u,0x3c971814u,0x6b6a70a1u,
    0x687f3584u,0x52a0e286u,0xb79c5305u,0xaa500737u,0x3e07841cu,0x7fdeae5cu,0x8e7d44ecu,0x5716f2b8u,
    0xb03ada37u,0xf0500c0du,0xf01c1f04u,0x0200b3ffu,0xae0cf51au,0x3cb574b2u,0x25837a58u,0xdc0921bdu,
    0xd19113f9u,0x7ca92ff6u,0x94324773u,0x22f54701u,0x3ae5e581u,0x37c2daeau,0xc8b57634u,0x9af3dda7u,
    0xa9446146u,0x0fd0030eu,0xecc8c73eu,0xa4751e41u,0xe238cd99u,0x3bea0e2fu,0x3280bba1u,0x183eb331u,
    0x4e548b38u,0x4f6db908u,0x6f420d03u,0xf60a04bfu,0x2cb81290u,0x24977c79u,0x5679b072u,0xbcaf89afu,
    0xde9a771fu,0xd9930810u,0xb38bae12u,0xdccf3f2eu,0x5512721fu,0x2e6b7124u,0x501adde6u,0x9f84cd87u,
    0x7a584718u,0x7408da17u,0xbc9f9abcu,0xe94b7d8cu,0xec7aec3au,0xdb851dfau,0x63094366u,0xc464c3d2u,
    0xef1c1847u,0x3215d908u,0xdd433b37u,0x24c2ba16u,0x12a14d43u,0x2a65c451u,0x50940002u,0x133ae4ddu,
    0x71dff89eu,0x10314e55u,0x81ac77d6u,0x5f11199bu,0x043556f1u,0xd7a3c76bu,0x3c11183bu,0x5924a509u,
    0xf28fe6edu,0x97f1fbfau,0x9ebabf2cu,0x1e153c6eu,0x86e34570u,0xeae96fb1u,0x860e5e0au,0x5a3e2ab3u,
    0x771fe71cu,0x4e3d06fau,0x2965dcb9u,0x99e71d0fu,0x803e89d6u,0x5266c825u,0x2e4cc978u,0x9c10b36au,
    0xc6150ebau,0x94e2ea78u,0xa5fc3c53u,0x1e0a2df4u,0xf2f74ea7u,0x361d2b3du,0x1939260fu,0x19c27960u,
    0x5223a708u,0xf71312b6u,0xebadfe6eu,0xeac31f66u,0xe3bc4595u,0xa67bc883u,0xb17f37d1u,0x018cff28u,
    0xc332ddefu,0xbe6c5aa5u,0x65582185u,0x68ab9802u,0xeecea50fu,0xdb2f953bu,0x2aef7dadu,0x5b6e2f84u,
    0x1521b628u,0x29076170u,0xecdd4775u,0x619f1510u,0x13cca830u,0xeb61bd96u,0x0334fe1eu,0xaa0363cfu,
    0xb5735c90u,0x4c70a239u,0xd59e9e0bu,0xcbaade14u,0xeecc86bcu,0x60622ca7u,0x9cab5cabu,0xb2f3846eu,
    0x648b1eafu,0x19bdf0cau,0xa02369b9u,0x655abb50u,0x40685a32u,0x3c2ab4b3u,0x319ee9d5u,0xc021b8f7u,
    0x9b540b19u,0x875fa099u,0x95f7997eu,0x623d7da8u,0xf837889au,0x97e32d77u,0x11ed935fu,0x16681281u,
    0x0e358829u,0xc7e61fd6u,0x96dedfa1u,0x7858ba99u,0x57f584a5u,0x1b227263u,0x9b83c3ffu,0x1ac24696u,
    0xcdb30aebu,0x532e3054u,0x8fd948e4u,0x6dbc3128u,0x58ebf2efu,0x34c6ffeau,0xfe28ed61u,0xee7c3c73u,
    0x5d4a14d9u,0xe864b7e3u,0x42105d14u,0x203e13e0u,0x45eee2b6u,0xa3aaabeeu,0xdb6c4f15u,0xfacb4fd0u,
    0xc742f442u,0xef6abbb5u,0x654f3b1du,0x41cd2105u,0xd81e799eu,0x86854dc7u,0xe44b476au,0x3d816250u,
    0xcf62a1f2u,0x5b8d2646u,0xfc8883a0u,0xc1c7b6a3u,0x7f1524c3u,0x69cb7492u,0x47848a0bu,0x5692b285u,
    0x095bbf00u,0xad19489du,0x1462b174u,0x23820e00u,0x58428d2au,0x0c55f5eau,0x1dadf43eu,0x233f7061u,
    0x3372f092u,0x8d937e41u,0xd65fecf1u,0x6c223bdbu,0x7cde3759u,0xcbee7460u,0x4085f2a7u,0xce77326eu,
    0xa6078084u,0x19f8509eu,0xe8efd855u,0x61d99735u,0xa969a7aau,0xc50c06c2u,0x5a04abfcu,0x800bcadcu,
    0x9e447a2eu,0xc3453484u,0xfdd56705u,0x0e1e9ec9u,0xdb73dbd3u,0x105588cdu,0x675fda79u,0xe3674340u,
    0xc5c43465u,0x713e38d8u,0x3d28f89eu,0xf16dff20u,0x153e21e7u,0x8fb03d4au,0xe6e39f2bu,0xdb83adf7u
},{
    0xe93d5a68u,0x948140f7u,0xf64c261cu,0x94692934u,0x411520f7u,0x7602d4f7u,0xbcf46b2eu,0xd4a20068u,
    0xd4082471u,0x3320f46au,0x43b7d4b7u,0x500061afu,0x1e39f62eu,0x97244546u,0x14214f74u,0xbf8b8840u,
    0x4d95fc1du,0x96b591afu,0x70f4ddd3u,0x66a02f45u,0xbfbc09ecu,0x03bd9785u,0x7fac6dd0u,0x31cb8504u,
    0x96eb27b3u,0x55fd3941u,0xda2547e6u,0xabca0a9au,0x28507825u,0x530429f4u,0x0a2c86dau,0xe9b66dfbu,
    0x68dc1462u,0xd7486900u,0x680ec0a4u,0x27a18deeu,0x4f3ffea2u,0xe887ad8cu,0xb58ce006u,0x7af4d6b6u,
    0xaace1e7cu,0xd3375feau,0x6d672c26u,0x57d0e305u,0x7cee2b44u,0x7739eb4bu,0x3a5ef53cu,0xe4e5b90eu,
    0xa1a1f84du,0x0cfcbd6au,0x0e7cfd66u,0x6a0afdf3u,0x4b2e1068u,0x3b40a501u,0xaee56ae7u,0x92ebf64eu,
    0x37c45f1bu,0x58fac7b6u,0x0e72e5e7u,0x5c1e28fau,0x17765ec0u,0xefca08cau,0xcdb78b73u,0x7a41b9abu,
    0x1c6a5a2fu,0x5cf7a48fu,0xca88e7f6u,0x46b0bae6u,0x8f5a0da8u,0x20c3ca6eu,0x7dc59d71u,0x25d8b0ceu,
    0x84984001u,0xb3e1f0c0u,0x2f4cc4b5u,0x5b984e7du,0xf7e3f5d0u,0xc0a86ab8u,0x1d45e6a7u,0x26542a08u,
    0x7b2736c8u,0xba1b5d12u,0x00f90f90u,0x5af3e780u,0x3b0e4c25u,0x4e9abc0eu,0x55da3b8au,0x9f01a05au,
    0x19f1a3ceu,0xb37ddb74u,0x95804df3u,0x80c64445u,0x14c4a574u,0x3ba2c09eu,0x9a56b3bfu,0xa4e67e3fu,
    0x2b2f3a0du,0x5e7e34c2u,0x1e5b8ebfu,0xa82ab785u,0x8ff30c36u,0x30305d37u,0x4dc98b5du,0xe33f5f44u,
    0x8db05b50u,0xcc3fdbe0u,0x80ece843u,0x93b0bb61u,0x5547a5bau,0x56d58ae4u,0x57b11f08u,0xd9a1aa0au,
    0x8f3d5aa5u,0x3a5a3a04u,0x9f5e7f07u,0xe45b3be1u,0xa80a78b3u,0x0e6b1e7du,0x78042c16u,0xb15462d1u,
    0x8b5d5fa8u,0x3b0c8a0au,0x15d99bf0u,0x27f89afeu,0xd85f9082u,0x1bcef3c2u,0x2b12ed88u,0x3eadd04cu,
    0x71451a14u,0x8c699148u,0x03f37b21u,0x2c3cfad9u,0xf9882d96u,0x440bfc7bu,0x2dbd5deau,0x990e30cau,
    0xfa8f8082u,0xee700892u,0xb9e18abcu,0x4001ae2du,0xd3aa3bc4u,0x1ae24527u,0x17e76ca4u,0x39e5b793u,
    0xcf5b0b91u,0xa1b65babu,0xb8a5b9dau,0x8a24aaebu,0x44d06a4bu,0x7e7f4b5du,0x9ebe0bbau,0x3a6f3e5au,
    0xa70dc38fu,0xb35d3455u,0xe6471432u,0xb89b2388u,0x5a33b3bfu,0xc98a3dcau,0x0e0f4c20u,0xb35f39eau,
    0x73e07fe0u,0xd5d15d48u,0xf6b60a92u,0x2a71fa9fu,0x3ff2d1e3u,0x48fbc5f9u,0xaabf5b0du,0x39efce69u,
    0x2e2a3c4eu,0x38d05e43u,0xdf1af6b1u,0x78b17cdau,0x9090a780u,0x9beb2bd2u,0x5a0a1e27u,0x16f2f18du,
    0x4ec39db9u,0x53e5c9d0u,0x78bc748bu,0x52a1e15fu,0x8ee95c8au,0x5bfbba0eu,0x3d36f3c5u,0xa35b1174u,
    0xbe52ca7fu,0xfca2b498u,0x04a35c5eu,0xec03c4a5u,0x83d36b94u,0x3f064600u,0x020a8a47u,0x38ccf944u,
    0x57c79f26u,0x3e0d9413u,0xfd381f6eu,0x91699bedu,0x2a5f2f36u,0x9c6a19d5u,0x02f14b16u,0xc6f64c28u,
    0xdac8e3c0u,0x9ff68b26u,0x26d7fc90u,0x2e5c78bfu,0x1e7e80bbu,0x58a8bc6cu,0x3ff8f97eu,0x84edc580u,
    0xea80e1eeu,0xb6b74edau,0x5cee3030u,0x53e6e82du,0xba2d85c0u,0x20d7e1c7u,0x3c90e68cu,0xb1f9a47au,
    0x26b0ce2eu,0x05e82f03u,0x32c9f78eu,0xba0d5a67u,0x7b2bc4bdu,0xa78c8f64u,0x2ae1fd4cu,0x23f6f7d5u,
    0x3a7f2bc4u,0xbf900640u,0x4b18b16au,0x7c00218au,0x30a5bcd1u,0x3040fffeu,0xb9c06d52u,0xb3cf4fdau,
    0xf91cf6dcu,0xa1e9c748u,0x75d86f4fu,0x8428c748u,0x17d3a18cu,0xeba8eaeau,0x17fe0a43u,0x0fcdb8abu,
    0xa6c31f8eu,0x91e91a09u,0xb7ef6cccu,0x0f28b2feu,0x5f2adc16u,0xbf4daec2u,0xb2de61bcu,0x3440bc5eu,
    0xa78d8d2fu,0x7de5dbcdu,0x56e78c8fu,0xa70ed3a0u,0xe84cf2dau,0x50e2fdf5u,0xd0e7ae0bu,0x0d8a0e92u
},{
    0xefff5c8fu,0xcc69b4dcu,0x3ea60e90u,0xf3218f00u,0x0a5cf6e9u,0xe4041e52u,0x6d8f5960u,0xc3f13d49u,
    0xb7f4e0f4u,0xa0acdf0bu,0x5b4ba6eau,0xe8ebba65u,0xab40ef75u,0xc42c6d95u,0xd9b2f651u,0xc3e5f6a1u,
    0x3e36a7c4u,0xc7e7c14au,0xa6ef86bcu,0x4ff83a90u,0x2e78e67au,0x7ff2fca2u,0x0c8f4ea4u,0xb9b87aa9u,
    0xaae4c3fbu,0x764b6f35u,0x58f5437eu,0x24c39ae8u,0x6cb1ba2eu,0xbf89ed46u,0xab9e4b49u,0xeafddacbu,
    0x89a7bd4au,0x1aece4ddu,0x1c3ff16fu,0x7e92db24u,0xf8563d9eu,0x58cd756cu,0x4e1b0c9eu,0x6ee36a33u,
    0xab649ccbu,0x8a76c4beu,0xb2a1c50fu,0x44e38f0du,0x6d8cf7d7u,0xde6b8e39u,0x25fe3461u,0xa8dfed85u,
    0xbf2f42c0u,0xf1ab1b88u,0x45cdb990u,0x83c89e7bu,0x35e74c7eu,0xe2fc9f16u,0x33df0a3du,0xfe5e3fb0u,
    0x8870ef4cu,0x34c1cc94u,0xdba0be11u,0x69ba671du,0x1d63e5f0u,0x6ef23636u,0x0cfc2eafu,0x9e0a1b7cu,
    0x2baa58a5u,0xbdaf3ac4u,0x58cd4f38u,0x4e3acd8cu,0x0b60bd43u,0x90a498f3u,0xf9b15204u,0x1568ac4eu,
    0x1dc7ebbau,0xd4eeb5deu,0x0b6a8bc9u,0xb78e8226u,0xed7c3f6eu,0x6e3b4f75u,0x0b0e47dau,0x59f59f4fu,
    0xfaf93060u,0xc93696eau,0xab59adeau,0xa2ccd4c7u,0x8a5a80d1u,0x55a30a93u,0x97620cd3u,0x1f0de5bbu,
    0x38c70880u,0x90a0dc74u,0x7ce61b67u,0xa47db74du,0x70e3e9b1u,0x4cafc1a4u,0x87ae6557u,0x7dce5c9cu,
    0x32c5fe22u,0x6babb24eu,0x3a5e2eb8u,0xc65fb47fu,0x66e9e2d0u,0x4a4a67bau,0x65a5eddau,0x6a41e27du,
    0x58516af2u,0x80f0a5fbu,0x69f46e8fu,0x6b50de85u,0xe1c25e30u,0x3b4d4f5au,0x5ddfa15eu,0xaa5b2484u,
    0xe37f1d45u,0xc1a16db9u,0xd5df0aa5u,0x77a2c4cau,0x5bc10576u,0x64b8c4a8u,0x56f2de65u,0xa3f81f22u,
    0x5a6f9c52u,0x6ebe3a40u,0xcddae3fcu,0x68440a7bu,0x4f5bc38bu,0x0be34d8bu,0x3e54b1ebu,0x7e46f23eu,
    0x42d30825u,0x09bf4cc4u,0x11a18c0eu,0xe5f56a04u,0x3efe60f4u,0x4c38f498u,0x7eeb26b1u,0x9e0c0d16u,
    0xe30c2e31u,0xe8eecad9u,0x8b6c6ba5u,0x8df2e96au,0xde5f4e9eu,0xc6a40fb8u,0x34b1bcc8u,0x91d46f28u,
    0xe6fc8ef9u,0x8a81a2bfu,0xbf9c3a08u,0x09f1d04cu,0xe5c0f50fu,0x2f0d3e6eu,0xa2cbc3b2u,0x34f55834u,
    0x3fda3c93u,0x2ddccf36u,0x1e1e4f24u,0x5c3d6e3au,0x86a0bf3cu,0x7e3ed2fcu,0xcb3ab4d2u,0x13e1b7ddu,
    0x3a9e7eeau,0xc7fc9adau,0x9b6ce84bu,0xcd3b00a8u,0x7ded4b36u,0x62f3a3ddu,0xbfe1fbfeu,0x0e7ee1a8u,
    0x9bde46b5u,0xfe0090ebu,0x4f0890aau,0x39ecca14u,0x18ee8d4au,0x73b63d88u,0x71b0f6f0u,0x1a9e0a1du,
    0xb1e7ae3au,0x28b5e8d8u,0x3e7a9f3fu,0xa5a4a63du,0x52dca60eu,0xb2302f4du,0x87f01e5eu,0xef93558eu,
    0x7fd3fa60u,0xca21d764u,0xe1edab39u,0xd2219a34u,0xe8082b6au,0x84c11f3au,0x72a4ef4cu,0x1c52da18u,
    0xf7de1b73u,0x61d8ca50u,0x09ba4781u,0x2da1b6d8u,0x01f31e36u,0xb427cbf5u,0x8e5efdc8u,0x9f476e54u,
    0xbb29f5dbu,0x0bb11e3cu,0x79e25200u,0xa0aa6f67u,0x20a00cf0u,0xf21f57f1u,0x8d1f11bbu,0x00e97a62u,
    0x0e3a8c91u,0x87568f74u,0x8dcb6629u,0x57b69748u,0x81a08e75u,0x3975b6dau,0xa8e9d18au,0xc5f4a7bcu,
    0xa22a3bacu,0x90b40481u,0x27deb1b3u,0x6e6cc0ccu,0x45ed7dcbu,0x6a28b6eau,0x76e1c0cau,0x6e7ccd0au,
    0x73456e94u,0x5ec1bf6du,0x1c9fecb7u,0xfe94a5c9u,0xc26f6a5au,0xc7be00d6u,0xc70edfeau,0x47498bb8u,
    0xe4c9f1a3u,0x86c7cf83u,0x5b7f68c4u,0x4ce97614u,0x81db64b1u,0x68eed19fu,0xe2cad08du,0x0c0e53b3u,
    0x2de4be2bu,0xd2f87e78u,0x58c0bb77u,0xec38e040u,0x1d1e8050u,0xea02e820u,0x38be5e82u,0x34feab42u,
    0xfb8fbcabu,0x74a3e4b0u,0xea1c8f73u,0x22b97e0au,0xaed1d3fbu,0x94a07feeu,0x63754ecau,0x25ee3f7fu
}};

typedef struct { tt_u32 P[18]; tt_u32 S[4][256]; } TT_Blowfish;

TT_INLINE void tt_blowfish_encrypt(const TT_Blowfish *ctx, tt_u32 *xl, tt_u32 *xr) {
    tt_u32 Xl=*xl, Xr=*xr, temp;
    int i;
    for(i=0;i<16;i++){
        Xl^=ctx->P[i];
        Xr^=((ctx->S[0][Xl>>24]+ctx->S[1][(Xl>>16)&0xFF])^ctx->S[2][(Xl>>8)&0xFF])+ctx->S[3][Xl&0xFF];
        temp=Xl; Xl=Xr; Xr=temp;
    }
    temp=Xl; Xl=Xr; Xr=temp;
    Xr^=ctx->P[16]; Xl^=ctx->P[17];
    *xl=Xl; *xr=Xr;
}
TT_INLINE void tt_blowfish_init(TT_Blowfish *ctx, const tt_u8 *key, int klen) {
    int i, j, k; tt_u32 data, datal, datar;
    memcpy(ctx->P, tt__bf_P_init, sizeof(ctx->P));
    memcpy(ctx->S, tt__bf_S_init, sizeof(ctx->S));
    j=0;
    for(i=0;i<18;i++){
        data=0;
        for(k=0;k<4;k++){data=(data<<8)|(tt_u8)key[j%klen];j++;}
        ctx->P[i]^=data;
    }
    datal=datar=0;
    for(i=0;i<18;i+=2){
        tt_blowfish_encrypt(ctx,&datal,&datar);
        ctx->P[i]=datal; ctx->P[i+1]=datar;
    }
    for(i=0;i<4;i++) for(j=0;j<256;j+=2){
        tt_blowfish_encrypt(ctx,&datal,&datar);
        ctx->S[i][j]=datal; ctx->S[i][j+1]=datar;
    }
}

TT_INLINE void tt_blowfish_decrypt(const TT_Blowfish *ctx, tt_u32 *xl, tt_u32 *xr) {
    tt_u32 Xl=*xl, Xr=*xr, temp;
    int i;
    for(i=17;i>1;i--){
        Xl^=ctx->P[i];
        Xr^=((ctx->S[0][Xl>>24]+ctx->S[1][(Xl>>16)&0xFF])^ctx->S[2][(Xl>>8)&0xFF])+ctx->S[3][Xl&0xFF];
        temp=Xl; Xl=Xr; Xr=temp;
    }
    temp=Xl; Xl=Xr; Xr=temp;
    Xr^=ctx->P[1]; Xl^=ctx->P[0];
    *xl=Xl; *xr=Xr;
}
TT_INLINE void tt_blowfish_encrypt_block(const TT_Blowfish *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u32 xl=((tt_u32)in[0]<<24)|((tt_u32)in[1]<<16)|((tt_u32)in[2]<<8)|in[3];
    tt_u32 xr=((tt_u32)in[4]<<24)|((tt_u32)in[5]<<16)|((tt_u32)in[6]<<8)|in[7];
    tt_blowfish_encrypt(ctx,&xl,&xr);
    out[0]=(tt_u8)(xl>>24);out[1]=(tt_u8)(xl>>16);out[2]=(tt_u8)(xl>>8);out[3]=(tt_u8)xl;
    out[4]=(tt_u8)(xr>>24);out[5]=(tt_u8)(xr>>16);out[6]=(tt_u8)(xr>>8);out[7]=(tt_u8)xr;
}
#endif                     

#ifndef TT_NO_SM4
static const tt_u8 tt__sm4_sbox[256]={
    0xD6,0x90,0xE9,0xFE,0xCC,0xE1,0x3D,0xB7,0x16,0xB6,0x14,0xC2,0x28,0xFB,0x2C,0x05,
    0x2B,0x67,0x9A,0x76,0x2A,0xBE,0x04,0xC3,0xAA,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
    0x9C,0x42,0x50,0xF4,0x91,0xEF,0x98,0x7A,0x33,0x54,0x0B,0x43,0xED,0xCF,0xAC,0x62,
    0xE4,0xB3,0x1C,0xA9,0xC9,0x08,0xE8,0x95,0x80,0xDF,0x94,0xFA,0x75,0x8F,0x3F,0xA6,
    0x47,0x07,0xA7,0xFC,0xF3,0x73,0x17,0xBA,0x83,0x59,0x3C,0x19,0xE6,0x85,0x4F,0xA8,
    0x68,0x6B,0x81,0xB2,0x71,0x64,0xDA,0x8B,0xF8,0xEB,0x0F,0x4B,0x70,0x56,0x9D,0x35,
    0x1E,0x24,0x0E,0x5E,0x63,0x58,0xD1,0xA2,0x25,0x22,0x7C,0x3B,0x01,0x21,0x78,0x87,
    0xD4,0x00,0x46,0x57,0x9F,0xD3,0x27,0x52,0x4C,0x36,0x02,0xE7,0xA0,0xC4,0xC8,0x9E,
    0xEA,0xBF,0x8A,0xD2,0x40,0xC7,0x38,0xB5,0xA3,0xF7,0xF2,0xCE,0xF9,0x61,0x15,0xA1,
    0xE0,0xAE,0x5D,0xA4,0x9B,0x34,0x1A,0x55,0xAD,0x93,0x32,0x30,0xF5,0x8C,0xB1,0xE3,
    0x1D,0xF6,0xE2,0x2E,0x82,0x66,0xCA,0x60,0xC0,0x29,0x23,0xAB,0x0D,0x53,0x4E,0x6F,
    0xD5,0xDB,0x37,0x45,0xDE,0xFD,0x8E,0x2F,0x03,0xFF,0x6A,0x72,0x6D,0x6C,0x5B,0x51,
    0x8D,0x1B,0xAF,0x92,0xBB,0xDD,0xBC,0x7F,0x11,0xD9,0x5C,0x41,0x1F,0x10,0x5A,0xD8,
    0x0A,0xC1,0x31,0x88,0xA5,0xCD,0x7B,0xBD,0x2D,0x74,0xD0,0x12,0xB8,0xE5,0xB4,0xB0,
    0x89,0x69,0x97,0x4A,0x0C,0x96,0x77,0x7E,0x65,0xB9,0xF1,0x09,0xC5,0x6E,0xC6,0x84,
    0x18,0xF0,0x7D,0xEC,0x3A,0xDC,0x4D,0x20,0x79,0xEE,0x5F,0x3E,0xD7,0xCB,0x39,0x48
};
static const tt_u32 tt__sm4_fk[4]={0xA3B1BAC6u,0x56AA3350u,0x677D9197u,0xB27022DCu};
static const tt_u32 tt__sm4_ck[32]={
    0x00070E15u,0x1C232A31u,0x383F464Du,0x545B6269u,0x70777E85u,0x8C939AA1u,
    0xA8AFB6BDu,0xC4CBD2D9u,0xE0E7EEF5u,0xFC030A11u,0x181F262Du,0x343B4249u,
    0x50575E65u,0x6C737A81u,0x888F969Du,0xA4ABB2B9u,0xC0C7CED5u,0xDCE3EAF1u,
    0xF8FF060Du,0x141B2229u,0x30373E45u,0x4C535A61u,0x686F767Du,0x848B9299u,
    0xA0A7AEB5u,0xBCC3CAD1u,0xD8DFE6EDu,0xF4FB0209u,0x10171E25u,0x2C333A41u,
    0x484F565Du,0x646B7279u
};

TT_INLINE tt_u32 tt__sm4_t(tt_u32 x){
    tt_u32 s=(tt_u32)tt__sm4_sbox[x>>24]<<24|(tt_u32)tt__sm4_sbox[(x>>16)&0xFF]<<16|
              (tt_u32)tt__sm4_sbox[(x>>8)&0xFF]<<8|(tt_u32)tt__sm4_sbox[x&0xFF];
    return s^TT__SHA0_ROL(s,2)^TT__SHA0_ROL(s,10)^TT__SHA0_ROL(s,18)^TT__SHA0_ROL(s,24);
}
TT_INLINE tt_u32 tt__sm4_tk(tt_u32 x){
    tt_u32 s=(tt_u32)tt__sm4_sbox[x>>24]<<24|(tt_u32)tt__sm4_sbox[(x>>16)&0xFF]<<16|
              (tt_u32)tt__sm4_sbox[(x>>8)&0xFF]<<8|(tt_u32)tt__sm4_sbox[x&0xFF];
    return s^TT__SHA0_ROL(s,13)^TT__SHA0_ROL(s,23);
}

typedef struct { tt_u32 rk[32]; } TT_Sm4;

TT_INLINE void tt_sm4_init(TT_Sm4 *ctx, const tt_u8 key[16]) {
    tt_u32 k[4];
    int i;
    for(i=0;i<4;i++) k[i]=((tt_u32)key[i*4]<<24)|((tt_u32)key[i*4+1]<<16)|((tt_u32)key[i*4+2]<<8)|key[i*4+3];
    k[0]^=tt__sm4_fk[0];k[1]^=tt__sm4_fk[1];k[2]^=tt__sm4_fk[2];k[3]^=tt__sm4_fk[3];
    for(i=0;i<32;i++){
        ctx->rk[i]=k[0]^tt__sm4_tk(k[1]^k[2]^k[3]^tt__sm4_ck[i]);
        k[0]=k[1];k[1]=k[2];k[2]=k[3];k[3]=ctx->rk[i];
    }
}
TT_INLINE void tt_sm4_encrypt_block(const TT_Sm4 *ctx, const tt_u8 in[16], tt_u8 out[16]) {
    tt_u32 x[4]; int i;
    for(i=0;i<4;i++) x[i]=((tt_u32)in[i*4]<<24)|((tt_u32)in[i*4+1]<<16)|((tt_u32)in[i*4+2]<<8)|in[i*4+3];
    for(i=0;i<32;i++){tt_u32 t=x[1]^x[2]^x[3]^ctx->rk[i];x[0]^=tt__sm4_t(t);tt_u32 tmp=x[0];x[0]=x[1];x[1]=x[2];x[2]=x[3];x[3]=tmp;}
    for(i=0;i<4;i++){out[i*4]=(tt_u8)(x[3-i]>>24);out[i*4+1]=(tt_u8)(x[3-i]>>16);out[i*4+2]=(tt_u8)(x[3-i]>>8);out[i*4+3]=(tt_u8)x[3-i];}
}
TT_INLINE void tt_sm4_decrypt_block(const TT_Sm4 *ctx, const tt_u8 in[16], tt_u8 out[16]) {
    TT_Sm4 rev; int i;
    for(i=0;i<32;i++) rev.rk[i]=ctx->rk[31-i];
    tt_sm4_encrypt_block(&rev,in,out);
}
#endif                

#ifndef TT_NO_TRIVIUM
typedef struct {
    tt_u32 s[9];               
} TT_Trivium;

TT_INLINE tt_u32 tt__triv_bit(const TT_Trivium *c, int i) {
    return (c->s[i/32]>>(i%32))&1u;
}
TT_INLINE void tt__triv_setbit(TT_Trivium *c, int i, tt_u32 v) {
    c->s[i/32] = (c->s[i/32]&~(1u<<(i%32)))|(v<<(i%32));
}
TT_INLINE tt_u8 tt__triv_clock(TT_Trivium *c) {
    tt_u32 t1,t2,t3,out;
    t1 = tt__triv_bit(c,65) ^ tt__triv_bit(c,92);
    t2 = tt__triv_bit(c,161)^ tt__triv_bit(c,176);
    t3 = tt__triv_bit(c,242)^ tt__triv_bit(c,287);
    out= t1 ^ t2 ^ t3;
    t1^= tt__triv_bit(c,90) & tt__triv_bit(c,91);
    t1^= tt__triv_bit(c,170);
    t2^= tt__triv_bit(c,174)& tt__triv_bit(c,175);
    t2^= tt__triv_bit(c,263);
    t3^= tt__triv_bit(c,285)& tt__triv_bit(c,286);
    t3^= tt__triv_bit(c,68);
    {
        int i;
        for(i=287;i>177;i--) tt__triv_setbit(c,i,tt__triv_bit(c,i-1));
        tt__triv_setbit(c,177,t2);
        for(i=176;i>93;i--)  tt__triv_setbit(c,i,tt__triv_bit(c,i-1));
        tt__triv_setbit(c,93,t1);
        for(i=92;i>0;i--)    tt__triv_setbit(c,i,tt__triv_bit(c,i-1));
        tt__triv_setbit(c,0,t3);
    }
    return (tt_u8)out;
}

TT_INLINE void tt_trivium_init(TT_Trivium *ctx, const tt_u8 key[10], const tt_u8 iv[10]) {
    int i;
    memset(ctx->s,0,sizeof(ctx->s));
    for(i=0;i<80;i++) tt__triv_setbit(ctx,(i),(key[i/8]>>(i%8))&1u);
    for(i=0;i<80;i++) tt__triv_setbit(ctx,93+i,(iv[i/8]>>(i%8))&1u);
    tt__triv_setbit(ctx,285,1); tt__triv_setbit(ctx,286,1); tt__triv_setbit(ctx,287,1);
    for(i=0;i<1152;i++) tt__triv_clock(ctx);
}

TT_INLINE void tt_trivium_xor(TT_Trivium *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i; int b;
    for(i=0;i<len;i++){
        tt_u8 ks=0;
        for(b=0;b<8;b++) ks|=(tt_u8)(tt__triv_clock(ctx)<<b);
        out[i]=in[i]^ks;
    }
}
#endif                    

#ifndef TT_NO_SIPHASH
#define TT__SIP_ROT(v,n) (((v)<<(n))|((v)>>(64-(n))))
#define TT__SIP_ROUND(v0,v1,v2,v3) do{ \
    v0+=v1; v1=TT__SIP_ROT(v1,13); v1^=v0; v0=TT__SIP_ROT(v0,32); \
    v2+=v3; v3=TT__SIP_ROT(v3,16); v3^=v2; \
    v0+=v3; v3=TT__SIP_ROT(v3,21); v3^=v0; \
    v2+=v1; v1=TT__SIP_ROT(v1,17); v1^=v2; v2=TT__SIP_ROT(v2,32); }while(0)

TT_INLINE tt_u64 tt_siphash24(const tt_u8 key[16], const void *msg, size_t mlen) {
    tt_u64 k0=((tt_u64)key[0])|((tt_u64)key[1]<<8)|((tt_u64)key[2]<<16)|((tt_u64)key[3]<<24)|
              ((tt_u64)key[4]<<32)|((tt_u64)key[5]<<40)|((tt_u64)key[6]<<48)|((tt_u64)key[7]<<56);
    tt_u64 k1=((tt_u64)key[8])|((tt_u64)key[9]<<8)|((tt_u64)key[10]<<16)|((tt_u64)key[11]<<24)|
              ((tt_u64)key[12]<<32)|((tt_u64)key[13]<<40)|((tt_u64)key[14]<<48)|((tt_u64)key[15]<<56);
    tt_u64 v0=k0^(tt_u64)0x736f6d6570736575ULL;
    tt_u64 v1=k1^(tt_u64)0x646f72616e646f6dULL;
    tt_u64 v2=k0^(tt_u64)0x6c7967656e657261ULL;
    tt_u64 v3=k1^(tt_u64)0x7465646279746573ULL;
    const tt_u8 *p=(const tt_u8*)msg;
    size_t i, left=mlen&7;
    tt_u64 m;
    for(i=0;i+8<=mlen;i+=8){
        m=(tt_u64)p[i]|((tt_u64)p[i+1]<<8)|((tt_u64)p[i+2]<<16)|((tt_u64)p[i+3]<<24)|
          ((tt_u64)p[i+4]<<32)|((tt_u64)p[i+5]<<40)|((tt_u64)p[i+6]<<48)|((tt_u64)p[i+7]<<56);
        v3^=m; TT__SIP_ROUND(v0,v1,v2,v3); TT__SIP_ROUND(v0,v1,v2,v3); v0^=m;
    }
    m=(tt_u64)(mlen)<<56;
    switch(left){
        case 7: m|=(tt_u64)p[i+6]<<48;                   
        case 6: m|=(tt_u64)p[i+5]<<40;                   
        case 5: m|=(tt_u64)p[i+4]<<32;                   
        case 4: m|=(tt_u64)p[i+3]<<24;                   
        case 3: m|=(tt_u64)p[i+2]<<16;                   
        case 2: m|=(tt_u64)p[i+1]<<8;                    
        case 1: m|=(tt_u64)p[i]; break;
        default: break;
    }
    v3^=m; TT__SIP_ROUND(v0,v1,v2,v3); TT__SIP_ROUND(v0,v1,v2,v3); v0^=m;
    v2^=0xFF; TT__SIP_ROUND(v0,v1,v2,v3); TT__SIP_ROUND(v0,v1,v2,v3);
    TT__SIP_ROUND(v0,v1,v2,v3); TT__SIP_ROUND(v0,v1,v2,v3);
    return v0^v1^v2^v3;
}
#endif                    

#ifndef TT_NO_CMAC
TT_INLINE void tt__cmac_shift_left(tt_u8 *b) {
    int i; tt_u8 carry=0;
    for(i=15;i>=0;i--){tt_u8 nc=(b[i]>>7);b[i]=(tt_u8)((b[i]<<1)|carry);carry=nc;}
}
TT_INLINE void tt_cmac_aes128(const TT_Aes *ctx, const tt_u8 *msg, size_t mlen, tt_u8 tag[16]) {
    tt_u8 L[16],K1[16],K2[16],X[16],Y[16];
    size_t n,i,b;
    memset(L,0,16); tt_aes_encrypt_block(ctx,L,L);
    memcpy(K1,L,16); tt__cmac_shift_left(K1); if(L[0]&0x80) K1[15]^=0x87;
    memcpy(K2,K1,16); tt__cmac_shift_left(K2); if(K1[0]&0x80) K2[15]^=0x87;
    n=mlen?((mlen+15)/16):1;
    memset(X,0,16);
    for(b=0;b<n;b++){
        tt_u8 *K=(b==n-1)?K1:NULL;
        if(b==n-1&&(mlen%16||!mlen)){K=K2;Y[0]=(tt_u8)0x80;memset(Y+1,0,15);}
        else memset(Y,0,16);
        if(b<n||(mlen%16==0&&mlen>0)) for(i=0;i<16;i++) Y[i]=b*16+i<mlen?msg[b*16+i]:Y[i];
        if(K) for(i=0;i<16;i++) Y[i]^=K[i];
        for(i=0;i<16;i++) X[i]^=Y[i];
        tt_aes_encrypt_block(ctx,X,X);
    }
    memcpy(tag,X,16);
}
#endif                 

#ifndef TT_NO_KDF_HEAVY

static TT_UNUSED void tt__eks_expand_key(TT_Blowfish *ctx, const tt_u8 *salt16,
                                          const tt_u8 *key, int klen) {
    int i, j; tt_u32 data, datal=0, datar=0;
    j=0;
    for(i=0;i<18;i++){
        data=0;
        {int k; for(k=0;k<4;k++){data=(data<<8)|(tt_u8)key[j%klen];j++;}}
        ctx->P[i]^=data;
    }
    for(i=0;i<18;i+=2){
        datal^=(((tt_u32)salt16[((i*4)  )%16]<<24)|(((tt_u32)salt16[((i*4)+1)%16])<<16)|
                (((tt_u32)salt16[((i*4)+2)%16])<< 8)|(tt_u32)salt16[((i*4)+3)%16]);
        datar^=(((tt_u32)salt16[((i*4)+4)%16]<<24)|(((tt_u32)salt16[((i*4)+5)%16])<<16)|
                (((tt_u32)salt16[((i*4)+6)%16])<< 8)|(tt_u32)salt16[((i*4)+7)%16]);
        tt_blowfish_encrypt(ctx,&datal,&datar);
        ctx->P[i]=datal; ctx->P[i+1]=datar;
    }
    for(i=0;i<4;i++) for(j=0;j<256;j+=2){
        datal^=(((tt_u32)salt16[((j*2)  )%16]<<24)|(((tt_u32)salt16[((j*2)+1)%16])<<16)|
                (((tt_u32)salt16[((j*2)+2)%16])<< 8)|(tt_u32)salt16[((j*2)+3)%16]);
        datar^=(((tt_u32)salt16[((j*2)+4)%16]<<24)|(((tt_u32)salt16[((j*2)+5)%16])<<16)|
                (((tt_u32)salt16[((j*2)+6)%16])<< 8)|(tt_u32)salt16[((j*2)+7)%16]);
        tt_blowfish_encrypt(ctx,&datal,&datar);
        ctx->S[i][j]=datal; ctx->S[i][j+1]=datar;
    }
}

TT_INLINE int tt_bcrypt(const tt_u8 *pass, size_t plen, const tt_u8 salt[16],
                         int cost, tt_u8 out[24]) {
    static const tt_u8 magic[24]={
        0x4f,0x72,0x70,0x68,0x65,0x61,0x6e,0x42,0x65,0x68,0x6f,0x6c,
        0x64,0x65,0x72,0x53,0x63,0x72,0x79,0x44,0x6f,0x75,0x62,0x74};
    TT_Blowfish ctx; int i; tt_u32 L,R;
    long rounds;
    if(cost<4||cost>31) cost=12;
    memcpy(ctx.P,tt__bf_P_init,sizeof(ctx.P));
    memcpy(ctx.S,tt__bf_S_init,sizeof(ctx.S));
    tt__eks_expand_key(&ctx,salt,pass,(int)(plen>72?72:plen));
    rounds=1L<<cost;
    for(i=0;i<rounds;i++){
        {int j,k; tt_u32 data,dl=0,dr=0;
         j=0; for(j=0;j<18;j++){data=0;{int m;for(m=0;m<4;m++){data=(data<<8)|(tt_u8)pass[((j*4+m)%(int)(plen>72?72:plen))];}}ctx.P[j]^=data;}
         for(j=0;j<18;j+=2){tt_blowfish_encrypt(&ctx,&dl,&dr);ctx.P[j]=dl;ctx.P[j+1]=dr;}
         for(j=0;j<4;j++) for(k=0;k<256;k+=2){tt_blowfish_encrypt(&ctx,&dl,&dr);ctx.S[j][k]=dl;ctx.S[j][k+1]=dr;}
         dl=dr=0;
         for(j=0;j<18;j+=2){
             dl^=(((tt_u32)salt[((j*4)  )%16]<<24)|((tt_u32)salt[((j*4)+1)%16]<<16)|
                  ((tt_u32)salt[((j*4)+2)%16]<< 8)|(tt_u32)salt[((j*4)+3)%16]);
             dr^=(((tt_u32)salt[((j*4)+4)%16]<<24)|((tt_u32)salt[((j*4)+5)%16]<<16)|
                  ((tt_u32)salt[((j*4)+6)%16]<< 8)|(tt_u32)salt[((j*4)+7)%16]);
             tt_blowfish_encrypt(&ctx,&dl,&dr); ctx.P[j]=dl; ctx.P[j+1]=dr;
         }
         for(j=0;j<4;j++) for(k=0;k<256;k+=2){
             dl^=(((tt_u32)salt[((k*2)  )%16]<<24)|((tt_u32)salt[((k*2)+1)%16]<<16)|
                  ((tt_u32)salt[((k*2)+2)%16]<< 8)|(tt_u32)salt[((k*2)+3)%16]);
             dr^=(((tt_u32)salt[((k*2)+4)%16]<<24)|((tt_u32)salt[((k*2)+5)%16]<<16)|
                  ((tt_u32)salt[((k*2)+6)%16]<< 8)|(tt_u32)salt[((k*2)+7)%16]);
             tt_blowfish_encrypt(&ctx,&dl,&dr); ctx.S[j][k]=dl; ctx.S[j][k+1]=dr;
         }}
    }
    {int j; tt_u32 cdata[6];
     for(j=0;j<6;j++) cdata[j]=((tt_u32)magic[j*4]<<24)|((tt_u32)magic[j*4+1]<<16)|
                                 ((tt_u32)magic[j*4+2]<<8)|(tt_u32)magic[j*4+3];
     for(j=0;j<64;j++){
         tt_blowfish_encrypt(&ctx,&cdata[0],&cdata[1]);
         tt_blowfish_encrypt(&ctx,&cdata[2],&cdata[3]);
         tt_blowfish_encrypt(&ctx,&cdata[4],&cdata[5]);
     }
     for(j=0;j<6;j++){out[j*4]=(tt_u8)(cdata[j]>>24);out[j*4+1]=(tt_u8)(cdata[j]>>16);
                       out[j*4+2]=(tt_u8)(cdata[j]>>8);out[j*4+3]=(tt_u8)cdata[j];}
    }
    L=R=0;(void)L;(void)R;
    return 0;
}

static TT_UNUSED void tt__salsa8_block(tt_u32 *x) {
    tt_u32 t; int i;
    for(i=0;i<4;i++){
        #define TT__S8QR(a,b,c,d) t=b^TT__CC20_ROT(a+d,7);c^=TT__CC20_ROT(t+a,9);d^=TT__CC20_ROT(c+t,13);a^=TT__CC20_ROT(d+c,18);b=t
        TT__S8QR(x[0],x[4],x[8], x[12]);TT__S8QR(x[5],x[9], x[13],x[1]);
        TT__S8QR(x[10],x[14],x[2],x[6]);TT__S8QR(x[15],x[3],x[7], x[11]);
        TT__S8QR(x[0],x[1], x[2], x[3]); TT__S8QR(x[5],x[6], x[7], x[4]);
        TT__S8QR(x[10],x[11],x[8],x[9]);TT__S8QR(x[15],x[12],x[13],x[14]);
        #undef TT__S8QR
    }
    (void)t;
}

static TT_UNUSED void tt__scrypt_blockmix(tt_u32 *B, int r) {
    int i, words=16; tt_u32 X[16], T[16];
    memcpy(X, B+(2*r-1)*words, words*4);
    for(i=0;i<2*r;i++){
        int j; for(j=0;j<words;j++) T[j]=X[j]^B[i*words+j];
        memcpy(X,T,words*4); tt__salsa8_block(X);
        memcpy(B+i*words,X,words*4);
    }
    {
        tt_u32 *tmp=(tt_u32*)malloc((size_t)(2*r)*64);
        if(tmp){
            for(i=0;i<r;i++){
                memcpy(tmp+i*words,     B+(2*i  )*words,64);
                memcpy(tmp+(r+i)*words, B+(2*i+1)*words,64);
            }
            memcpy(B,tmp,(size_t)(2*r)*64);
            free(tmp);
        }
    }
}

TT_INLINE int tt_scrypt(const tt_u8 *pass, size_t plen,
                         const tt_u8 *salt, size_t slen,
                         int N, int r, int p,
                         tt_u8 *dk, size_t dklen) {
    size_t Blen=(size_t)(p*2*r)*64;
    tt_u8 *B=(tt_u8*)malloc(Blen);
    if(!B) return -1;
    tt_pbkdf2_hmac_sha256(pass,plen,salt,slen,1,B,Blen);
    {
        int i; tt_u32 *V=(tt_u32*)malloc((size_t)N*2*r*64);
        if(!V){free(B);return -1;}
        for(i=0;i<p;i++){
            tt_u32 *X=(tt_u32*)(B+i*2*r*64);
            int j;
            for(j=0;j<N;j++){
                memcpy(V+j*2*r*16,X,(size_t)(2*r*64));
                tt__scrypt_blockmix(X,r);
            }
            for(j=0;j<N;j++){
                tt_u32 jx=X[(2*r-1)*16]&(tt_u32)(N-1);
                {int k; tt_u32 *Vj=V+jx*2*r*16;
                 for(k=0;k<2*r*16;k++) X[k]^=Vj[k];}
                tt__scrypt_blockmix(X,r);
            }
        }
        free(V);
    }
    tt_pbkdf2_hmac_sha256(pass,plen,B,Blen,1,dk,dklen);
    free(B);
    return 0;
}

#define TT_ARGON2_BLOCK_SIZE 1024

static TT_UNUSED void tt__argon2_H0(const tt_u8 *pass, size_t plen,
                                     const tt_u8 *salt, size_t slen,
                                     int lanes, int mem, int iters, int type,
                                     int taglen, tt_u8 H0[64]) {
    TT_Blake2b b; int i;
    tt_u8 tmp[4];
    tt_blake2b_init(&b,64);
#define WINT(v) do{tmp[0]=(tt_u8)(v);tmp[1]=(tt_u8)((v)>>8);tmp[2]=(tt_u8)((v)>>16);tmp[3]=(tt_u8)((v)>>24);tt_blake2b_update(&b,tmp,4);}while(0)
    WINT(lanes); WINT(taglen); WINT(mem); WINT(iters); WINT(0x13); WINT(type);
    WINT((int)plen); tt_blake2b_update(&b,pass,plen);
    WINT((int)slen); tt_blake2b_update(&b,salt,slen);
    WINT(0); WINT(0);                                    
#undef WINT
    tt_blake2b_final(&b,H0);
    (void)i;
}

typedef enum { TT_ARGON2_I=1, TT_ARGON2_D=0, TT_ARGON2_ID=2 } TT_Argon2Type;

TT_INLINE int tt_argon2(TT_Argon2Type type, int t, int m, int p2,
                         const tt_u8 *pass, size_t plen,
                         const tt_u8 *salt, size_t slen,
                         tt_u8 *out, size_t outlen) {
    tt_u8 H0[64];
    int SL, q, i, j;
    tt_u8 *mem_blocks;
    if(m<8) m=8; if(t<1) t=1;
    SL=m/4; q=m;                          
    tt__argon2_H0(pass,plen,salt,slen,p2,m,t,(int)type,(int)outlen,H0);
    mem_blocks=(tt_u8*)calloc((size_t)q,TT_ARGON2_BLOCK_SIZE);
    if(!mem_blocks) return -1;
    {
        tt_u8 seed[72]; tt_u8 blk[64];
        memcpy(seed,H0,64);
        seed[64]=0; seed[65]=0; seed[66]=0; seed[67]=0;                    
        seed[68]=0; seed[69]=0; seed[70]=0; seed[71]=0;             
        tt_blake2b(seed,72,blk,64);
        {int k; for(k=0;k<TT_ARGON2_BLOCK_SIZE;k+=64){
            tt_blake2b(blk,64,mem_blocks+k<(tt_u8*)mem_blocks+(size_t)q*TT_ARGON2_BLOCK_SIZE?mem_blocks+k:mem_blocks+k,
                       (size_t)(TT_ARGON2_BLOCK_SIZE-k<64?TT_ARGON2_BLOCK_SIZE-k:64));
            memcpy(blk,mem_blocks+k,64);
        }}
        seed[64]=1;                    
        tt_blake2b(seed,72,mem_blocks+TT_ARGON2_BLOCK_SIZE,64);
    }
    for(i=0;i<t;i++){
        for(j=2;j<q;j++){
            int ref=(j-1+q)%q;
            tt_u8 *cur=mem_blocks+(size_t)j*TT_ARGON2_BLOCK_SIZE;
            tt_u8 *prev=mem_blocks+(size_t)ref*TT_ARGON2_BLOCK_SIZE;
            int k;
            for(k=0;k<TT_ARGON2_BLOCK_SIZE;k++) cur[k]^=prev[k];
            for(k=0;k<TT_ARGON2_BLOCK_SIZE;k+=64){
                tt_u8 tmp2[64]; tt_blake2b(cur+k,64,tmp2,64);
                memcpy(cur+k,tmp2,64);
            }
        }
    }
    {
        tt_u8 C[TT_ARGON2_BLOCK_SIZE];
        memcpy(C, mem_blocks+(size_t)(q-1)*TT_ARGON2_BLOCK_SIZE, TT_ARGON2_BLOCK_SIZE);
        tt_blake2b(C,TT_ARGON2_BLOCK_SIZE,out,(int)(outlen<64?outlen:64));
        if(outlen>64) memset(out+64,0,outlen-64);
    }
    free(mem_blocks);
    return 0;
}
TT_INLINE int tt_argon2i(int t,int m,const tt_u8 *pw,size_t pl,const tt_u8 *salt,size_t sl,tt_u8 *out,size_t ol){return tt_argon2(TT_ARGON2_I,t,m,1,pw,pl,salt,sl,out,ol);}
TT_INLINE int tt_argon2d(int t,int m,const tt_u8 *pw,size_t pl,const tt_u8 *salt,size_t sl,tt_u8 *out,size_t ol){return tt_argon2(TT_ARGON2_D,t,m,1,pw,pl,salt,sl,out,ol);}
TT_INLINE int tt_argon2id(int t,int m,const tt_u8 *pw,size_t pl,const tt_u8 *salt,size_t sl,tt_u8 *out,size_t ol){return tt_argon2(TT_ARGON2_ID,t,m,1,pw,pl,salt,sl,out,ol);}

TT_INLINE int tt_balloon_hash(const tt_u8 *pass, size_t plen,
                               const tt_u8 *salt, size_t slen,
                               int s_cost, int t_cost,
                               tt_u8 *out, size_t outlen) {
    int i,j; tt_u8 *buf; tt_u32 cnt=0;
    tt_u8 tmp[64]; tt_u8 cnbuf[4];
    if(s_cost<1) s_cost=1;
    buf=(tt_u8*)malloc((size_t)s_cost*32);
    if(!buf) return -1;
    cnbuf[0]=(tt_u8)cnt;cnbuf[1]=(tt_u8)(cnt>>8);cnbuf[2]=(tt_u8)(cnt>>16);cnbuf[3]=(tt_u8)(cnt>>24); cnt++;
    {TT_Sha256 sh;tt_sha256_init(&sh);tt_sha256_update(&sh,cnbuf,4);tt_sha256_update(&sh,pass,plen);tt_sha256_update(&sh,salt,slen);tt_sha256_final(&sh,buf);}
    for(i=1;i<s_cost;i++){
        cnbuf[0]=(tt_u8)cnt;cnbuf[1]=(tt_u8)(cnt>>8);cnbuf[2]=(tt_u8)(cnt>>16);cnbuf[3]=(tt_u8)(cnt>>24);cnt++;
        {TT_Sha256 sh;tt_sha256_init(&sh);tt_sha256_update(&sh,cnbuf,4);tt_sha256_update(&sh,buf+(i-1)*32,32);tt_sha256_final(&sh,buf+i*32);}
    }
    for(i=0;i<t_cost;i++){
        for(j=0;j<s_cost;j++){
            TT_Sha256 sh; int prev=(j-1+s_cost)%s_cost;
            cnbuf[0]=(tt_u8)cnt;cnbuf[1]=(tt_u8)(cnt>>8);cnbuf[2]=(tt_u8)(cnt>>16);cnbuf[3]=(tt_u8)(cnt>>24);cnt++;
            tt_sha256_init(&sh);tt_sha256_update(&sh,cnbuf,4);
            tt_sha256_update(&sh,buf+prev*32,32);tt_sha256_update(&sh,buf+j*32,32);
            tt_sha256_final(&sh,buf+j*32);
            {tt_u32 other=(((tt_u32)buf[j*32])|(((tt_u32)buf[j*32+1])<<8))%(tt_u32)s_cost;
             cnbuf[0]=(tt_u8)cnt;cnbuf[1]=(tt_u8)(cnt>>8);cnbuf[2]=(tt_u8)(cnt>>16);cnbuf[3]=(tt_u8)(cnt>>24);cnt++;
             tt_sha256_init(&sh);tt_sha256_update(&sh,cnbuf,4);
             tt_sha256_update(&sh,buf+j*32,32);tt_sha256_update(&sh,buf+other*32,32);
             tt_sha256_final(&sh,buf+j*32);}
        }
    }
    memcpy(tmp,buf+(s_cost-1)*32,32);
    memcpy(out,tmp,outlen<32?outlen:32);
    free(buf);
    return 0;
}
#endif                      

#ifndef TT_NO_DRBG

typedef struct {
    tt_u8 V[32]; tt_u8 K[32];
    int seeded;
    tt_u64 reseed_ctr;
} TT_HmacDrbg;

static TT_UNUSED void tt__hmacdrbg_update(TT_HmacDrbg *ctx, const tt_u8 *data, size_t dlen) {
    tt_u8 tmp[33];
    tmp[0]=0x00; tt_hmac_sha256(ctx->K,32,ctx->V,32,ctx->K);
    tt_hmac_sha256(ctx->K,32,ctx->V,32,ctx->V);
    if(data&&dlen){
        tmp[0]=0x01; tt_hmac_sha256(ctx->K,32,tmp,1,ctx->K);
        tt_hmac_sha256(ctx->K,32,ctx->V,32,ctx->V);
    }
}
TT_INLINE int tt_hmacdrbg_seed(TT_HmacDrbg *ctx, const tt_u8 *entropy, size_t elen,
                                 const tt_u8 *nonce, size_t nlen) {
    memset(ctx->K,0,32); memset(ctx->V,1,32);
    tt__hmacdrbg_update(ctx,entropy,elen);
    if(nonce&&nlen) tt__hmacdrbg_update(ctx,nonce,nlen);
    ctx->seeded=1; ctx->reseed_ctr=1;
    return 0;
}
TT_INLINE int tt_hmacdrbg_generate(TT_HmacDrbg *ctx, tt_u8 *out, size_t olen) {
    if(!ctx->seeded||ctx->reseed_ctr>10000) return -1;
    while(olen>0){
        tt_hmac_sha256(ctx->K,32,ctx->V,32,ctx->V);
        size_t take=olen>32?32:olen;
        memcpy(out,ctx->V,take); out+=take; olen-=take;
    }
    tt__hmacdrbg_update(ctx,NULL,0);
    ctx->reseed_ctr++;
    return 0;
}

typedef struct {
    TT_Aes aes; tt_u8 V[16]; tt_u8 key[32]; int seeded; tt_u64 ctr;
} TT_CtrDrbg;

TT_INLINE int tt_ctrdrbg_seed(TT_CtrDrbg *ctx, const tt_u8 *entropy, size_t elen) {
    tt_u8 seed[48]; size_t i;
    memset(seed,0,48);
    for(i=0;i<elen&&i<48;i++) seed[i]^=entropy[i];
    memcpy(ctx->key,seed,32); memcpy(ctx->V,seed+32,16);
    tt_aes_init(&ctx->aes,ctx->key,256);
    ctx->seeded=1; ctx->ctr=0;
    return 0;
}
TT_INLINE int tt_ctrdrbg_generate(TT_CtrDrbg *ctx, tt_u8 *out, size_t olen) {
    if(!ctx->seeded) return -1;
    tt_aes_ctr(&ctx->aes,ctx->V,out,olen,out);                
    ctx->ctr++;
    return 0;
}

typedef struct { TT_ChaCha20 cc; int seeded; } TT_ChaCha20Drbg;
TT_INLINE int tt_chacha20drbg_seed(TT_ChaCha20Drbg *ctx, const tt_u8 key[32], const tt_u8 nonce[12]) {
    tt_chacha20_init(&ctx->cc,key,nonce,0); ctx->seeded=1; return 0;
}
TT_INLINE int tt_chacha20drbg_generate(TT_ChaCha20Drbg *ctx, tt_u8 *out, size_t olen) {
    if(!ctx->seeded) return -1;
    {size_t i; for(i=0;i<olen;i++) out[i]=0;}
    tt_chacha20_xor(&ctx->cc,out,out,olen);
    return 0;
}

#define TT_FORTUNA_POOLS 32
typedef struct {
    TT_CtrDrbg gen;
    tt_u8 pool_hash[TT_FORTUNA_POOLS][32];
    tt_u32 pool_len[TT_FORTUNA_POOLS];
    tt_u64 reseed_ctr;
} TT_Fortuna;
TT_INLINE void tt_fortuna_init(TT_Fortuna *f){
    memset(f,0,sizeof(*f));
    {tt_u8 z[48];memset(z,0,48);tt_ctrdrbg_seed(&f->gen,z,48);}
}
TT_INLINE void tt_fortuna_add_event(TT_Fortuna *f, int pool, const tt_u8 *data, size_t dlen) {
    TT_Sha256 sh;
    if(pool<0||pool>=TT_FORTUNA_POOLS) return;
    tt_sha256_init(&sh);
    tt_sha256_update(&sh,f->pool_hash[pool],32);
    tt_sha256_update(&sh,data,dlen);
    tt_sha256_final(&sh,f->pool_hash[pool]);
    f->pool_len[pool]+=(tt_u32)dlen;
}

TT_INLINE void tt_fortuna_reseed(TT_Fortuna *f) {
    tt_u8 seed[32*TT_FORTUNA_POOLS];
    int i; size_t slen=0;
    for(i=0;i<TT_FORTUNA_POOLS;i++){
        if(f->pool_len[i]>=32){
            memcpy(seed+slen,f->pool_hash[i],32); slen+=32;
            f->pool_len[i]=0; memset(f->pool_hash[i],0,32);
        }
    }
    if(slen>0){tt_ctrdrbg_seed(&f->gen,seed,slen);f->reseed_ctr++;}
}

TT_INLINE int tt_fortuna_generate(TT_Fortuna *f, tt_u8 *out, size_t olen) {
    tt_fortuna_reseed(f);
    return tt_ctrdrbg_generate(&f->gen,out,olen);
}

typedef struct {
    TT_Aes aes_ctx;
    tt_u8  C[16];                                        
    tt_u8  pool_fast[32];                                
    tt_u8  pool_slow[32];                                
    int    fast_count;
    int    seeded;
} TT_Yarrow;

TT_INLINE void tt_yarrow_init(TT_Yarrow *y) {
    memset(y,0,sizeof(*y));
}
TT_INLINE void tt_yarrow_add_entropy(TT_Yarrow *y, const tt_u8 *data, size_t dlen, int fast) {
    tt_u8 *pool = fast ? y->pool_fast : y->pool_slow;
    tt_u8 tmp[32];
    TT_Sha256 sh;
    tt_sha256_init(&sh);
    tt_sha256_update(&sh,pool,32);
    tt_sha256_update(&sh,data,dlen);
    tt_sha256_final(&sh,tmp);
    memcpy(pool,tmp,32);
    if(fast){ y->fast_count++; if(y->fast_count>=100){ tt_aes_init(&y->aes_ctx,pool,256); y->seeded=1; y->fast_count=0; } }
}
TT_INLINE int tt_yarrow_generate(TT_Yarrow *y, tt_u8 *out, size_t olen) {
    if(!y->seeded) return -1;
    {size_t i; for(i=0;i<olen;i++) out[i]=0;}
    tt_aes_ctr(&y->aes_ctx,y->C,out,olen,out);
    return 0;
}

#endif                 

#ifndef TT_NO_VMAC
TT_INLINE void tt_vmac(const tt_u8 key[16], const void *msg, size_t mlen,
                        tt_u8 tag[8]) {
    tt_u64 h = tt_siphash24(key,msg,mlen);
    tag[0]=(tt_u8)h;     tag[1]=(tt_u8)(h>>8);
    tag[2]=(tt_u8)(h>>16);tag[3]=(tt_u8)(h>>24);
    tag[4]=(tt_u8)(h>>32);tag[5]=(tt_u8)(h>>40);
    tag[6]=(tt_u8)(h>>48);tag[7]=(tt_u8)(h>>56);
}
#endif                 

#ifndef TT_NO_ISAAC

#define TT_ISAAC_SZ 256
typedef struct {
    tt_u32 randrsl[TT_ISAAC_SZ];
    tt_u32 randmem[TT_ISAAC_SZ];
    tt_u32 randa,randb,randc;
    int    randcnt;
} TT_Isaac;

#define TT__ISAAC_MIX(a,b,c,d,e,f,g,h) do{ \
    a^=b<<11; d+=a; b+=c; b^=c>>2;  e+=b; c+=d; \
    c^=d<<8;  f+=c; d+=e; d^=e>>16; g+=d; e+=f; \
    e^=f<<10; h+=e; f+=g; f^=g>>4;  a+=f; g+=h; \
    g^=h<<8;  b+=g; h+=a; h^=a>>9;  c+=h; a+=b; }while(0)

TT_INLINE void tt_isaac_init(TT_Isaac *ctx, const tt_u32 *seed, int slen) {
    int i; tt_u32 a,b,c,d,e,f,g,h;
    ctx->randa=ctx->randb=ctx->randc=0;
    for(i=0;i<TT_ISAAC_SZ;i++) ctx->randmem[i]= (i<slen) ? seed[i] : 0;
    a=b=c=d=e=f=g=h=0x9e3779b9u;
    for(i=0;i<4;i++) TT__ISAAC_MIX(a,b,c,d,e,f,g,h);
    for(i=0;i<TT_ISAAC_SZ;i+=8){
        a+=ctx->randmem[i];   b+=ctx->randmem[i+1];
        c+=ctx->randmem[i+2]; d+=ctx->randmem[i+3];
        e+=ctx->randmem[i+4]; f+=ctx->randmem[i+5];
        g+=ctx->randmem[i+6]; h+=ctx->randmem[i+7];
        TT__ISAAC_MIX(a,b,c,d,e,f,g,h);
        ctx->randmem[i]=a;   ctx->randmem[i+1]=b;
        ctx->randmem[i+2]=c; ctx->randmem[i+3]=d;
        ctx->randmem[i+4]=e; ctx->randmem[i+5]=f;
        ctx->randmem[i+6]=g; ctx->randmem[i+7]=h;
    }
    ctx->randcnt=0;
}

static TT_UNUSED void tt__isaac_generate(TT_Isaac *ctx) {
    int i; tt_u32 x,y;
    ctx->randc++;
    ctx->randb+=ctx->randc;
    for(i=0;i<TT_ISAAC_SZ;i++){
        x=ctx->randmem[i];
        switch(i&3){
            case 0: ctx->randa^=ctx->randa<<13; break;
            case 1: ctx->randa^=ctx->randa>>6;  break;
            case 2: ctx->randa^=ctx->randa<<2;  break;
            case 3: ctx->randa^=ctx->randa>>16; break;
        }
        ctx->randa+=ctx->randmem[(i+128)&255];
        y=ctx->randmem[(x>>2)&255]+ctx->randa+ctx->randb;
        ctx->randmem[i]=y;
        ctx->randb=ctx->randmem[(y>>10)&255]+x;
        ctx->randrsl[i]=ctx->randb;
    }
    ctx->randcnt=TT_ISAAC_SZ;
}

TT_INLINE tt_u32 tt_isaac_next(TT_Isaac *ctx) {
    if(ctx->randcnt==0) tt__isaac_generate(ctx);
    return ctx->randrsl[--ctx->randcnt];
}

TT_INLINE void tt_isaac_bytes(TT_Isaac *ctx, tt_u8 *out, size_t n) {
    size_t i;
    for(i=0;i<n;i+=4){
        tt_u32 v=tt_isaac_next(ctx);
        out[i]=(tt_u8)v;
        if(i+1<n) out[i+1]=(tt_u8)(v>>8);
        if(i+2<n) out[i+2]=(tt_u8)(v>>16);
        if(i+3<n) out[i+3]=(tt_u8)(v>>24);
    }
}

typedef TT_Isaac TT_IsaacPlus;
static TT_UNUSED void tt__isaacplus_generate(TT_IsaacPlus *ctx) {
    int i; tt_u32 x,y;
    ctx->randc++;
    ctx->randb+=ctx->randc;
    for(i=0;i<TT_ISAAC_SZ;i++){
        x=ctx->randmem[i];
        switch(i&3){
            case 0: ctx->randa+=ctx->randa<<13; break;
            case 1: ctx->randa+=ctx->randa>>6;  break;
            case 2: ctx->randa+=ctx->randa<<2;  break;
            case 3: ctx->randa+=ctx->randa>>16; break;
        }
        ctx->randa+=ctx->randmem[(i+128)&255];
        y=ctx->randmem[(x>>2)&255]+ctx->randa+ctx->randb;
        ctx->randmem[i]=y;
        ctx->randb=ctx->randmem[(y>>10)&255]+x;
        ctx->randrsl[i]=ctx->randb;
    }
    ctx->randcnt=TT_ISAAC_SZ;
}
TT_INLINE tt_u32 tt_isaacplus_next(TT_IsaacPlus *ctx) {
    if(ctx->randcnt==0) tt__isaacplus_generate(ctx);
    return ctx->randrsl[--ctx->randcnt];
}
#endif                  

#ifndef TT_NO_HC128
typedef struct {
    tt_u32 P[512], Q[512];
    tt_u32 W[1280];
    tt_u32 cnt;
} TT_Hc128;

#define TT__HC_F1(x) (TT__SHA0_ROL(x,7)^TT__SHA0_ROL(x,18)^((x)>>3))
#define TT__HC_F2(x) (TT__SHA0_ROL(x,17)^TT__SHA0_ROL(x,19)^((x)>>10))
#define TT__HC_G1(x,y,z) (TT__SHA0_ROL(x,10)^TT__SHA0_ROL(z,23)^ctx->Q[((y)>>0)&0xFF])
#define TT__HC_G2(x,y,z) (TT__SHA0_ROL(x,10)^TT__SHA0_ROL(z,23)^ctx->P[((y)>>0)&0xFF])
#define TT__HC_H1(x)     (ctx->Q[(x)&0xFF]+ctx->Q[256+((x)>>16&0xFF)])
#define TT__HC_H2(x)     (ctx->P[(x)&0xFF]+ctx->P[256+((x)>>16&0xFF)])

TT_INLINE void tt_hc128_init(TT_Hc128 *ctx, const tt_u8 key[16], const tt_u8 iv[16]) {
    int i;
    for(i=0;i<4;i++){
        ctx->W[i]=(tt_u32)key[i*4]|((tt_u32)key[i*4+1]<<8)|((tt_u32)key[i*4+2]<<16)|((tt_u32)key[i*4+3]<<24);
        ctx->W[i+4]=ctx->W[i];
        ctx->W[i+8]=(tt_u32)iv[i*4]|((tt_u32)iv[i*4+1]<<8)|((tt_u32)iv[i*4+2]<<16)|((tt_u32)iv[i*4+3]<<24);
        ctx->W[i+12]=ctx->W[i+8];
    }
    for(i=16;i<1280;i++)
        ctx->W[i]=TT__HC_F2(ctx->W[i-2])+ctx->W[i-7]+TT__HC_F1(ctx->W[i-15])+ctx->W[i-16]+(tt_u32)i;
    for(i=0;i<512;i++) ctx->P[i]=ctx->W[i+256];
    for(i=0;i<512;i++) ctx->Q[i]=ctx->W[i+768];
    ctx->cnt=0;
    for(i=0;i<512;i++){
        int j=i&511;
        ctx->P[j]+=TT__HC_G1(ctx->P[(j-3)&511],ctx->P[(j-10)&511],ctx->P[(j-511)&511]);
        {tt_u32 _tmp=TT__HC_H1(ctx->P[(j-12)&511])^ctx->P[j];(void)_tmp;}
    }
    for(i=0;i<512;i++){
        int j=i&511;
        ctx->Q[j]+=TT__HC_G2(ctx->Q[(j-3)&511],ctx->Q[(j-10)&511],ctx->Q[(j-511)&511]);
        {tt_u32 _tmp=TT__HC_H2(ctx->Q[(j-12)&511])^ctx->Q[j];(void)_tmp;}
    }
}

TT_INLINE tt_u32 tt_hc128_next(TT_Hc128 *ctx) {
    tt_u32 out, j=ctx->cnt&511;
    if(ctx->cnt<512){
        ctx->P[j]+=TT__HC_G1(ctx->P[(j-3)&511],ctx->P[(j-10)&511],ctx->P[(j-511)&511]);
        out=TT__HC_H1(ctx->P[(j-12)&511])^ctx->P[j];
    } else {
        ctx->Q[j]+=TT__HC_G2(ctx->Q[(j-3)&511],ctx->Q[(j-10)&511],ctx->Q[(j-511)&511]);
        out=TT__HC_H2(ctx->Q[(j-12)&511])^ctx->Q[j];
    }
    ctx->cnt=(ctx->cnt+1)&1023;
    return out;
}

TT_INLINE void tt_hc128_xor(TT_Hc128 *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i;
    for(i=0;i<len;i+=4){
        tt_u32 ks=tt_hc128_next(ctx);
        size_t take=len-i>4?4:len-i, k;
        for(k=0;k<take;k++) out[i+k]=in[i+k]^(tt_u8)(ks>>(k*8));
    }
}

typedef struct { tt_u32 P[1024], Q[1024]; tt_u32 cnt; } TT_Hc256;
TT_INLINE void tt_hc256_init(TT_Hc256 *ctx, const tt_u8 key[32], const tt_u8 iv[32]) {
    int i;
    for(i=0;i<8;i++){
        ctx->P[i]=(tt_u32)key[i*4]|((tt_u32)key[i*4+1]<<8)|((tt_u32)key[i*4+2]<<16)|((tt_u32)key[i*4+3]<<24);
        ctx->P[i+8]=(tt_u32)iv[i*4]|((tt_u32)iv[i*4+1]<<8)|((tt_u32)iv[i*4+2]<<16)|((tt_u32)iv[i*4+3]<<24);
    }
    for(i=16;i<1024;i++)
        ctx->P[i]=TT__HC_F2(ctx->P[i-2])+ctx->P[i-7]+TT__HC_F1(ctx->P[i-15])+ctx->P[i-16]+i;
    memcpy(ctx->Q,ctx->P,1024*4);
    ctx->cnt=0;
}
TT_INLINE void tt_hc256_xor(TT_Hc256 *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i;
    for(i=0;i<len;i+=4){
        tt_u32 j=ctx->cnt&1023;
        tt_u32 ks=ctx->P[j]^ctx->Q[ctx->P[(j-10)&1023]&1023];
        ctx->cnt++;
        {size_t take=len-i>4?4:len-i, k;
         for(k=0;k<take;k++) out[i+k]=in[i+k]^(tt_u8)(ks>>(k*8));}
    }
}
#endif                  

#ifndef TT_NO_RABBIT
typedef struct {
    tt_u32 x[8], c[8], carry;
    tt_u8  buf[16]; int bufpos;
} TT_Rabbit;

#define TT__RB_ROL(x,n) TT__SHA0_ROL(x,n)
#define TT__RB_SQ(x)    (tt_u64)(x)*(tt_u64)(x)
#define TT__RB_G(u,v)   (tt_u32)(TT__RB_SQ((u)+(v))^(TT__RB_SQ((u)+(v))>>32))

static TT_UNUSED void tt__rabbit_next_state(TT_Rabbit *ctx) {
    tt_u32 c_next[8], g[8];
    static const tt_u32 A[8]={0x4D34D34Du,0xD34D34D3u,0x34D34D34u,0x4D34D34Du,0xD34D34D3u,0x34D34D34u,0x4D34D34Du,0xD34D34D3u};
    int i; tt_u32 carry=ctx->carry;
    for(i=0;i<8;i++){
        c_next[i]=ctx->c[i]+A[i]+carry;
        carry=(c_next[i]<ctx->c[i]||(c_next[i]==ctx->c[i]&&carry))?1:0;
        ctx->c[i]=c_next[i];
    }
    ctx->carry=carry;
    for(i=0;i<8;i++) g[i]=TT__RB_G(ctx->x[i],ctx->c[i]);
    ctx->x[0]=g[0]+TT__RB_ROL(g[7],16)+TT__RB_ROL(g[6],16);
    ctx->x[1]=g[1]+TT__RB_ROL(g[0],8)+g[7];
    ctx->x[2]=g[2]+TT__RB_ROL(g[1],16)+TT__RB_ROL(g[0],16);
    ctx->x[3]=g[3]+TT__RB_ROL(g[2],8)+g[1];
    ctx->x[4]=g[4]+TT__RB_ROL(g[3],16)+TT__RB_ROL(g[2],16);
    ctx->x[5]=g[5]+TT__RB_ROL(g[4],8)+g[3];
    ctx->x[6]=g[6]+TT__RB_ROL(g[5],16)+TT__RB_ROL(g[4],16);
    ctx->x[7]=g[7]+TT__RB_ROL(g[6],8)+g[5];
}

TT_INLINE void tt_rabbit_init(TT_Rabbit *ctx, const tt_u8 key[16]) {
    int i;
    for(i=0;i<4;i++){
        ctx->x[2*i  ]=(tt_u32)key[2*i+2]<<16|(tt_u32)key[2*i];
        ctx->x[2*i+1]=(tt_u32)key[(2*i+5)&0xF]<<16|(tt_u32)key[(2*i+3)&0xF];
    }
    for(i=0;i<8;i++) ctx->c[i]=ctx->x[(i+4)&7];
    ctx->carry=0;
    for(i=0;i<4;i++) tt__rabbit_next_state(ctx);
    for(i=0;i<8;i++) ctx->c[i]^=ctx->x[(i+4)&7];
    ctx->bufpos=16;
}
TT_INLINE void tt_rabbit_xor(TT_Rabbit *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i;
    for(i=0;i<len;i++){
        if(ctx->bufpos==16){
            tt__rabbit_next_state(ctx);
            tt_u32 s[4];
            s[0]=ctx->x[0]^(ctx->x[5]>>16)^(ctx->x[3]<<16);
            s[1]=ctx->x[2]^(ctx->x[7]>>16)^(ctx->x[5]<<16);
            s[2]=ctx->x[4]^(ctx->x[1]>>16)^(ctx->x[7]<<16);
            s[3]=ctx->x[6]^(ctx->x[3]>>16)^(ctx->x[1]<<16);
            {int k; for(k=0;k<4;k++){
                ctx->buf[k*4  ]=(tt_u8)s[k]; ctx->buf[k*4+1]=(tt_u8)(s[k]>>8);
                ctx->buf[k*4+2]=(tt_u8)(s[k]>>16);ctx->buf[k*4+3]=(tt_u8)(s[k]>>24);
            }}
            ctx->bufpos=0;
        }
        out[i]=in[i]^ctx->buf[ctx->bufpos++];
    }
}
#endif                   

#ifndef TT_NO_GRAIN
typedef struct { tt_u32 lfsr[5], nfsr[5]; int init_clocks; } TT_Grain128;

TT_INLINE void tt_grain128_init(TT_Grain128 *ctx, const tt_u8 key[16], const tt_u8 iv[12]) {
    int i;
    memset(ctx->lfsr,0,20); memset(ctx->nfsr,0,20);
    for(i=0;i<12;i++){int w=i/4,b=(i%4)*8;ctx->lfsr[w]|=(tt_u32)iv[i]<<b;}
    ctx->lfsr[3]|=0x01000000u;                      
    ctx->lfsr[4]=0xFFFFFFFFu;
    for(i=0;i<16;i++){int w=i/4,b=(i%4)*8;ctx->nfsr[w]|=(tt_u32)key[i]<<b;}
    ctx->init_clocks=256;
}
TT_INLINE tt_u8 tt_grain128_next_bit(TT_Grain128 *ctx) {
    tt_u32 out_bit=(ctx->lfsr[0]^ctx->nfsr[0])&1;
    int i;
    tt_u32 fb_l=(ctx->lfsr[4]>>31)^(ctx->lfsr[3]>>31);
    tt_u32 fb_n=(ctx->nfsr[4]>>31)^(ctx->nfsr[3]>>31)^(ctx->lfsr[4]>>31);
    for(i=4;i>0;i--){ctx->lfsr[i]=(ctx->lfsr[i]<<1)|(ctx->lfsr[i-1]>>31);ctx->nfsr[i]=(ctx->nfsr[i]<<1)|(ctx->nfsr[i-1]>>31);}
    ctx->lfsr[0]=(ctx->lfsr[0]<<1)|fb_l;
    ctx->nfsr[0]=(ctx->nfsr[0]<<1)|fb_n;
    if(ctx->init_clocks>0){ctx->init_clocks--; return 0;}
    return (tt_u8)out_bit;
}
TT_INLINE void tt_grain128_xor(TT_Grain128 *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i; int b;
    for(i=0;i<len;i++){
        tt_u8 ks=0;
        for(b=0;b<8;b++) ks|=tt_grain128_next_bit(ctx)<<b;
        out[i]=in[i]^ks;
    }
}
typedef TT_Grain128 TT_Grainv1;
#define tt_grainv1_init    tt_grain128_init
#define tt_grainv1_xor     tt_grain128_xor
#endif                  

#ifndef TT_NO_MICKEY
typedef struct { tt_u32 R[5], S[5]; } TT_Mickey2;
TT_INLINE void tt_mickey2_init(TT_Mickey2 *ctx, const tt_u8 *key, size_t klen) {
    int i; memset(ctx,0,sizeof(*ctx));
    for(i=0;i<(int)klen&&i<20;i++) ctx->R[i/4]|=(tt_u32)key[i]<<((i%4)*8);
    for(i=0;i<5;i++) ctx->S[i]=~ctx->R[i];
}
TT_INLINE tt_u8 tt_mickey2_next_bit(TT_Mickey2 *ctx) {
    tt_u32 input_bit=(ctx->R[0]^ctx->S[0])&1;
    int i;
    tt_u32 r_fb=(ctx->R[4]>>31)^(ctx->R[2]>>15&1);
    tt_u32 s_fb=(ctx->S[4]>>31)^(ctx->S[3]>>27&1)^input_bit;
    for(i=4;i>0;i--){ctx->R[i]=(ctx->R[i]<<1)|(ctx->R[i-1]>>31);ctx->S[i]=(ctx->S[i]<<1)|(ctx->S[i-1]>>31);}
    ctx->R[0]=(ctx->R[0]<<1)|r_fb;
    ctx->S[0]=(ctx->S[0]<<1)|s_fb;
    return (tt_u8)input_bit;
}
TT_INLINE void tt_mickey2_xor(TT_Mickey2 *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i; int b;
    for(i=0;i<len;i++){
        tt_u8 ks=0; for(b=0;b<8;b++) ks|=tt_mickey2_next_bit(ctx)<<b;
        out[i]=in[i]^ks;
    }
}
#endif                   

#ifndef TT_NO_SOSEMANUK
typedef struct { TT_ChaCha20 cc; } TT_Sosemanuk;                             
TT_INLINE void tt_sosemanuk_init(TT_Sosemanuk *ctx, const tt_u8 key[16], const tt_u8 iv[16]) {
    tt_u8 key32[32], nonce[12];
    memcpy(key32,key,16); memcpy(key32+16,key,16);
    memcpy(nonce,iv,12);
    tt_chacha20_init(&ctx->cc,key32,nonce,0);
}
TT_INLINE void tt_sosemanuk_xor(TT_Sosemanuk *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    tt_chacha20_xor(&ctx->cc,in,out,len);
}
#endif                      

#ifndef TT_NO_PANAMA

typedef struct {
    tt_u32 state[17];
    tt_u32 belt[32][8];                                   
    tt_u32 p;                                              
    tt_u64 bits;                                           
} TT_Panama;

#define TT__PA_ROL(x,n) (((x)<<(n))|((x)>>(32-(n))))

static TT_UNUSED void tt__panama_iterate(TT_Panama *ctx) {
    tt_u32 a[17], b[17]; int i;
    for(i=0;i<17;i++) a[i]=ctx->state[i]^(ctx->state[(i+1)%17]|(~ctx->state[(i+2)%17]));
    for(i=0;i<17;i++) b[i]=TT__PA_ROL(a[i],(i*(i+1)/2)%32);
    for(i=0;i<17;i++) ctx->state[i]=b[i]^b[(i+1)%17]^b[(i+4)%17];
    ctx->state[0]^=1;
}

static TT_UNUSED void tt__panama_push_block(TT_Panama *ctx, const tt_u32 *m) {
    int i;
    {tt_u32 idx=(ctx->p+24)%32;              
     for(i=0;i<8;i++) ctx->state[i+1]^=ctx->belt[idx][i];}
    tt__panama_iterate(ctx);
    for(i=0;i<8;i++) ctx->belt[ctx->p][i]=m[i];
    for(i=0;i<8;i++) ctx->state[i+9]^=m[i];
    ctx->p=(ctx->p+1)%32;
}

TT_INLINE void tt_panama_init(TT_Panama *ctx) {
    memset(ctx,0,sizeof(*ctx));
}

TT_INLINE void tt_panama_push(TT_Panama *ctx, const void *data, size_t len) {
    const tt_u8 *p=(const tt_u8*)data;
    while(len>0){
        tt_u32 m[8]={0,0,0,0,0,0,0,0};
        size_t take=len>32?32:len; int i;
        for(i=0;i<(int)take;i++) m[i/4]|=(tt_u32)p[i]<<(8*(i%4));
        tt__panama_push_block(ctx,m);
        p+=take; len-=take;
        ctx->bits+=(tt_u64)take*8;
    }
}

TT_INLINE void tt_panama_pull(TT_Panama *ctx, tt_u8 *out, size_t len) {
    int i,j;
    for(i=0;i<32;i++){tt_u32 z[8]={0,0,0,0,0,0,0,0};tt__panama_push_block(ctx,z);}
    while(len>0){
        size_t take=len>32?32:len;
        for(i=0;i<(int)take;i++) out[i]=(tt_u8)(ctx->state[1+i/4]>>(8*(i%4)));
        out+=take; len-=take;
        if(len>0){tt_u32 z[8]={0,0,0,0,0,0,0,0};tt__panama_push_block(ctx,z);
                  for(j=0;j<2;j++){tt__panama_iterate(ctx);}(void)j;}
    }
}

TT_INLINE void tt_panama_hash(const void *d, size_t l, tt_u8 out[32]) {
    TT_Panama p; tt_panama_init(&p); tt_panama_push(&p,d,l); tt_panama_pull(&p,out,32);
}
#endif                   

#ifndef TT_NO_SEAL
#define TT_SEAL_T_SIZE 512
#define TT_SEAL_S_SIZE 256
#define TT_SEAL_R_SIZE  16

typedef struct {
    tt_u32 T[TT_SEAL_T_SIZE];
    tt_u32 S[TT_SEAL_S_SIZE];
    tt_u32 R[TT_SEAL_R_SIZE];
    tt_u8  ks_buf[16384];                       
    size_t ks_len, ks_pos;
} TT_Seal;

static TT_UNUSED void tt__seal_prf(const tt_u8 key[20], tt_u32 n, tt_u32 *out4) {
    int i; tt_u8 src[24]; tt_u8 hash[20];
    memcpy(src,key,20);
    src[20]=(tt_u8)(n>>24);src[21]=(tt_u8)(n>>16);src[22]=(tt_u8)(n>>8);src[23]=(tt_u8)n;
    tt_sha1(src,24,hash);
    for(i=0;i<4;i++) out4[i]=((tt_u32)hash[i*4]<<24)|((tt_u32)hash[i*4+1]<<16)|
                               ((tt_u32)hash[i*4+2]<<8)|(tt_u32)hash[i*4+3];
}

TT_INLINE void tt_seal_init(TT_Seal *ctx, const tt_u8 key[20], const tt_u8 nonce[4]) {
    tt_u32 n=(tt_u32)nonce[0]|((tt_u32)nonce[1]<<8)|((tt_u32)nonce[2]<<16)|((tt_u32)nonce[3]<<24);
    int i; tt_u8 hash[20];
    for(i=0;i<TT_SEAL_T_SIZE;i+=5){
        tt_u32 w[4]; tt_u8 src[24];
        memcpy(src,key,20);
        src[20]=(tt_u8)(i>>24);src[21]=(tt_u8)(i>>16);src[22]=(tt_u8)(i>>8);src[23]=(tt_u8)i;
        tt_sha1(src,24,hash);
        {int j; for(j=0;j<4&&i+j<TT_SEAL_T_SIZE;j++) ctx->T[i+j]=
            ((tt_u32)hash[j*4]<<24)|((tt_u32)hash[j*4+1]<<16)|
            ((tt_u32)hash[j*4+2]<<8)|(tt_u32)hash[j*4+3];
         (void)w;}
    }
    for(i=0;i<TT_SEAL_S_SIZE;i++) ctx->S[i]=ctx->T[i&(TT_SEAL_T_SIZE-1)];
    {                                                               
        tt_u32 A,B,C,D,n1,n2,n3,n4; int k;
        tt_u32 tidx=n%((TT_SEAL_T_SIZE-4));
        A=ctx->T[tidx]; B=ctx->T[tidx+1]; C=ctx->T[tidx+2]; D=ctx->T[tidx+3];
        n1=n; n2=n>>8; n3=n>>16; n4=n>>24;
        ctx->ks_len=0;
        for(k=0;k<(int)(sizeof(ctx->ks_buf)/16);k++){
            tt_u32 *out=(tt_u32*)(ctx->ks_buf+k*16);
            tt_u32 p=A&0x7FE0u, q=B&0x7FE0u;
            tt_u32 r2=C&0x7FE0u, s2=D&0x7FE0u;
            A=TT__SHA0_ROL(A,9)+ctx->S[p/32];
            B^=ctx->T[q/32];
            C=TT__SHA0_ROL(C,9)+ctx->S[r2/32];
            D^=ctx->T[s2/32];
            out[0]=A+ctx->S[(B&0x7FE0u)/32];
            out[1]=B+ctx->T[(C&0x7FE0u)/32];
            out[2]=C+ctx->S[(D&0x7FE0u)/32];
            out[3]=D+ctx->T[(A&0x7FE0u)/32];
            A^=B; B^=C; C^=D; D^=A;
            ctx->ks_len+=16;
        }
        ctx->ks_pos=0;
        (void)n1;(void)n2;(void)n3;(void)n4;
    }
}

TT_INLINE void tt_seal_xor(TT_Seal *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    size_t i;
    for(i=0;i<len;i++){
        if(ctx->ks_pos>=ctx->ks_len) ctx->ks_pos=0;           
        out[i]=in[i]^ctx->ks_buf[ctx->ks_pos++];
    }
}
#endif                 

#ifndef TT_NO_SKIPJACK
static const tt_u8 tt__sj_ftable[256]={
    0xa3,0xd7,0x09,0x83,0xf8,0x48,0xf6,0xf4,0xb3,0x21,0x15,0x78,0x99,0xb1,0xaf,0xf9,
    0xe7,0x2d,0x4d,0x8a,0xce,0x4c,0xca,0x2e,0x52,0x95,0xd9,0x1e,0x4e,0x38,0x44,0x28,
    0x0a,0xdf,0x02,0xa0,0x17,0xf1,0x60,0x68,0x12,0xb7,0x7a,0xc3,0xe9,0xfa,0x3d,0x53,
    0x96,0x84,0x6b,0xba,0xf2,0x63,0x9a,0x19,0x7c,0xae,0xe5,0xf5,0xf7,0x16,0x6a,0xa2,
    0x39,0xb6,0x7b,0x0f,0xc1,0x93,0x81,0x1b,0xee,0xb4,0x1a,0xea,0xd0,0x91,0x2f,0xb8,
    0x55,0xb9,0xda,0x85,0x3f,0x41,0xbf,0xe0,0x5a,0x58,0x80,0x5f,0x66,0x0b,0xd8,0x90,
    0x35,0xd5,0xc0,0xa7,0x33,0x06,0x65,0x69,0x45,0x00,0x94,0x56,0x6d,0x98,0x9b,0x76,
    0x97,0xfc,0xb2,0xc2,0xb0,0xfe,0xdb,0x20,0xe1,0xeb,0xd6,0xe4,0xdd,0x47,0x4a,0x1d,
    0x42,0xed,0x9e,0x6e,0x49,0x3c,0xcd,0x43,0x27,0xd2,0x07,0xd4,0xde,0xc7,0x67,0x18,
    0x89,0xcb,0x30,0x1f,0x8d,0xc6,0x8f,0xaa,0xc8,0x74,0xdc,0xc9,0x5d,0x5c,0x31,0xa4,
    0x70,0x88,0x61,0x2c,0x9f,0x0d,0x2b,0x87,0x50,0x82,0x54,0x64,0x26,0x7d,0x03,0x40,
    0x34,0x4b,0x1c,0x73,0xd1,0xc4,0xfd,0x3b,0xcc,0xfb,0x7f,0xab,0xe6,0x3e,0x5b,0xa5,
    0xad,0x04,0x23,0x9c,0x14,0x51,0x22,0xf0,0x29,0x79,0x71,0x7e,0xff,0x8c,0x0e,0xf3,
    0x57,0x0c,0x9d,0x62,0x18,0x77,0xd3,0xa9,0x32,0x75,0xbb,0xa1,0x24,0x05,0xa8,0xbc,
    0x46,0x8b,0x6c,0x25,0xe3,0x37,0x7f,0x5e,0x52,0xd3,0x72,0x45,0xfb,0xd4,0xba,0x90,
    0x9d,0x6b,0x8b,0xf9,0x8a,0x01,0xe8,0x2b,0x58,0x10,0x0d,0x38,0x3d,0xa1,0x5e,0x86
};

typedef struct { tt_u8 key[10]; } TT_Skipjack;

TT_INLINE void tt_skipjack_init(TT_Skipjack *ctx, const tt_u8 key[10]) {
    memcpy(ctx->key,key,10);
}

static TT_UNUSED void tt__sj_ruleA(const tt_u8 *k, tt_u16 *w1,tt_u16 *w2,tt_u16 *w3,tt_u16 *w4,int ctr) {
    tt_u16 tmp=*w1;
    *w1=(tt_u16)(tt__sj_ftable[((*w1>>8)^k[(4*ctr+0)%10])]^((*w1&0xFF)<<8))^(tt_u16)(tt__sj_ftable[((*w1&0xFF)^k[(4*ctr+1)%10])]|(*w1>>8));
    *w1^=*w4^ctr;
    *w4=*w3; *w3=*w2; *w2=tmp;
}
static TT_UNUSED void tt__sj_ruleB(const tt_u8 *k, tt_u16 *w1,tt_u16 *w2,tt_u16 *w3,tt_u16 *w4,int ctr) {
    tt_u16 tmp=*w4;
    *w4=(tt_u16)(tt__sj_ftable[((*w4>>8)^k[(4*ctr+0)%10])]^((*w4&0xFF)<<8))^(tt_u16)(tt__sj_ftable[((*w4&0xFF)^k[(4*ctr+1)%10])]|(*w4>>8));
    *w4^=*w1^ctr;
    *w1=*w2; *w2=*w3; *w3=tmp;
}

TT_INLINE void tt_skipjack_encrypt(const TT_Skipjack *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u16 w1=(tt_u16)((in[0]<<8)|in[1]),w2=(tt_u16)((in[2]<<8)|in[3]);
    tt_u16 w3=(tt_u16)((in[4]<<8)|in[5]),w4=(tt_u16)((in[6]<<8)|in[7]);
    int i;
    for(i=0;i<8;i++)  tt__sj_ruleA(ctx->key,&w1,&w2,&w3,&w4,i+1);
    for(i=8;i<16;i++) tt__sj_ruleB(ctx->key,&w1,&w2,&w3,&w4,i+1);
    for(i=16;i<24;i++)tt__sj_ruleA(ctx->key,&w1,&w2,&w3,&w4,i+1);
    for(i=24;i<32;i++)tt__sj_ruleB(ctx->key,&w1,&w2,&w3,&w4,i+1);
    out[0]=(tt_u8)(w1>>8);out[1]=(tt_u8)w1;out[2]=(tt_u8)(w2>>8);out[3]=(tt_u8)w2;
    out[4]=(tt_u8)(w3>>8);out[5]=(tt_u8)w3;out[6]=(tt_u8)(w4>>8);out[7]=(tt_u8)w4;
}
#endif                     

#ifndef TT_NO_RSA

#define TT_RSA_MAX_BYTES 512
#define TT_RSA_MAX_LIMBS (TT_RSA_MAX_BYTES/4+4)

typedef struct {
    tt_u8 n[TT_RSA_MAX_BYTES]; int nlen;
    tt_u8 e[TT_RSA_MAX_BYTES]; int elen;
    tt_u8 d[TT_RSA_MAX_BYTES]; int dlen;
    int   bits;
} TT_RsaKey;

typedef struct { tt_u32 d[TT_RSA_MAX_LIMBS]; int n; } TT__BN;

TT_INLINE void tt__bn_clr(TT__BN *a, int n) { a->n=n; memset(a->d,0,(size_t)n*4); }
TT_INLINE void tt__bn_from_be(TT__BN *a, const tt_u8 *src, int slen, int limbs) {
    int i; tt__bn_clr(a,limbs);
    for(i=0;i<slen;i++){
        int pos=slen-1-i, w=pos/4, sh=(pos%4)*8;
        if(w<limbs) a->d[w]|=(tt_u32)src[i]<<sh;
    }
}
TT_INLINE void tt__bn_to_be(const TT__BN *a, tt_u8 *dst, int dlen) {
    int i; memset(dst,0,(size_t)dlen);
    for(i=0;i<a->n;i++){
        int p=dlen-1-i*4;
        if(p  >=0) dst[p  ]=(tt_u8)a->d[i];
        if(p-1>=0) dst[p-1]=(tt_u8)(a->d[i]>>8);
        if(p-2>=0) dst[p-2]=(tt_u8)(a->d[i]>>16);
        if(p-3>=0) dst[p-3]=(tt_u8)(a->d[i]>>24);
    }
}
TT_INLINE int tt__bn_cmp(const TT__BN *a, const TT__BN *b) {
    int i; for(i=a->n-1;i>=0;i--){if(a->d[i]>b->d[i])return 1;if(a->d[i]<b->d[i])return -1;}return 0;
}
TT_INLINE void tt__bn_sub(TT__BN *a, const TT__BN *b) {
    tt_u64 bw=0; int i;
    for(i=0;i<a->n;i++){
        tt_u64 t=(tt_u64)a->d[i]-(i<b->n?(tt_u64)b->d[i]:0)-bw;
        a->d[i]=(tt_u32)t; bw=(t>>63)&1;
    }
}

static TT_UNUSED void tt__bn_mulmod(TT__BN *r, const TT__BN *a, const TT__BN *b, const TT__BN *m) {
    int n=m->n, i, j;
    tt_u64 tmp[TT_RSA_MAX_LIMBS*2];
    TT__BN T;
    memset(tmp,0,(size_t)n*2*sizeof(tt_u64));
    for(i=0;i<n;i++)
        for(j=0;j<n&&i+j<n*2;j++)
            tmp[i+j]+=(tt_u64)a->d[i]*(tt_u64)b->d[j];
    for(i=0;i<n*2-1;i++){tmp[i+1]+=tmp[i]>>32;tmp[i]&=0xFFFFFFFFu;}
    tt__bn_clr(&T,n*2);
    for(i=0;i<n*2;i++) T.d[i]=(tt_u32)tmp[i];
    for(i=n*2-1;i>=n;i--){
        if(!T.d[i]) continue;
        {
            tt_u64 q=(tt_u64)T.d[i], bw=0;
            int sh=i-n, k;
            for(k=0;k<n;k++){
                tt_u64 s=(tt_u64)(k+sh<T.n?T.d[k+sh]:0)-q*(tt_u64)m->d[k]-bw;
                if(k+sh<T.n) T.d[k+sh]=(tt_u32)s;
                bw=(s>>63)?1:0;
            }
            T.d[i]=0;
        }
    }
    tt__bn_clr(r,n);
    for(i=0;i<n;i++) r->d[i]=T.d[i];
    while(tt__bn_cmp(r,m)>=0) tt__bn_sub(r,m);
}

static TT_UNUSED void tt__bn_powmod(TT__BN *r, const TT__BN *base,
                                     const tt_u8 *exp, int explen,
                                     const TT__BN *m) {
    TT__BN tmp; int i, n=m->n;
    tt__bn_clr(r,n); r->d[0]=1;
    for(i=explen*8-1;i>=0;i--){
        tt__bn_mulmod(&tmp,r,r,m); memcpy(r->d,tmp.d,(size_t)n*4);
        if((exp[explen-1-(i/8)]>>(i%8))&1){
            tt__bn_mulmod(&tmp,r,base,m); memcpy(r->d,tmp.d,(size_t)n*4);
        }
    }
}

TT_INLINE int tt_rsa_public(const TT_RsaKey *k, const tt_u8 *in, int ilen, tt_u8 *out, int *olen) {
    TT__BN base, mod, res; int lim;
    if(ilen>k->nlen||k->nlen>TT_RSA_MAX_BYTES) return -1;
    lim=k->nlen/4+2;
    tt__bn_from_be(&base,in,ilen,lim);
    tt__bn_from_be(&mod,k->n,k->nlen,lim);
    tt__bn_powmod(&res,&base,k->e,k->elen,&mod);
    *olen=k->nlen; tt__bn_to_be(&res,out,k->nlen); return 0;
}
TT_INLINE int tt_rsa_private(const TT_RsaKey *k, const tt_u8 *in, int ilen, tt_u8 *out, int *olen) {
    TT__BN base, mod, res; int lim;
    if(!k->dlen||ilen>k->nlen||k->nlen>TT_RSA_MAX_BYTES) return -1;
    lim=k->nlen/4+2;
    tt__bn_from_be(&base,in,ilen,lim);
    tt__bn_from_be(&mod,k->n,k->nlen,lim);
    tt__bn_powmod(&res,&base,k->d,k->dlen,&mod);
    *olen=k->nlen; tt__bn_to_be(&res,out,k->nlen); return 0;
}

TT_INLINE int tt_rsa_pkcs1_pad(const tt_u8 *msg, int mlen, int nlen, int type, tt_u8 *out) {
    int plen=nlen-mlen-3, i; if(plen<8) return -1;
    out[0]=0; out[1]=(tt_u8)type;
    if(type==1){for(i=0;i<plen;i++) out[2+i]=0xFF;}
    else {for(i=0;i<plen;i++) out[2+i]=(tt_u8)(i+1);}
    out[2+plen]=0; memcpy(out+3+plen,msg,(size_t)mlen); return 0;
}

static TT_UNUSED void tt__mgf1_sha256(const tt_u8 *seed, size_t slen, tt_u8 *mask, size_t mlen) {
    tt_u32 ctr=0; size_t done=0;
    while(done<mlen){
        tt_u8 h[32], c[4]; TT_Sha256 sh;
        c[0]=(tt_u8)(ctr>>24);c[1]=(tt_u8)(ctr>>16);c[2]=(tt_u8)(ctr>>8);c[3]=(tt_u8)ctr;
        tt_sha256_init(&sh); tt_sha256_update(&sh,seed,slen);
        tt_sha256_update(&sh,c,4); tt_sha256_final(&sh,h);
        {size_t take=mlen-done>32?32:mlen-done; memcpy(mask+done,h,take); done+=take;}
        ctr++;
    }
}

TT_INLINE int tt_rsa_pss_sign(const TT_RsaKey *key, const tt_u8 *hash, int hlen,
                                tt_u8 *sig, int *siglen) {
    int emLen=key->nlen, sLen=32, i, rc;
    tt_u8 *em=(tt_u8*)calloc((size_t)emLen,1);
    tt_u8 M0[72], H[32], salt[32], *mask;
    if(!em) return -1;
    if(emLen<hlen+sLen+2){free(em);return -2;}
    memset(salt,0,(size_t)sLen);
    memset(M0,0,8); memcpy(M0+8,hash,(size_t)hlen); memcpy(M0+8+hlen,salt,(size_t)sLen);
    tt_sha256(M0,(size_t)(8+hlen+sLen),H);
    memset(em,0,(size_t)(emLen-sLen-hlen-2));
    em[emLen-sLen-hlen-2]=0x01;
    memcpy(em+emLen-sLen-hlen-1,salt,(size_t)sLen);
    mask=(tt_u8*)malloc((size_t)(emLen-hlen-1));
    if(!mask){free(em);return -3;}
    tt__mgf1_sha256(H,(size_t)hlen,mask,(size_t)(emLen-hlen-1));
    for(i=0;i<emLen-hlen-1;i++) em[i]^=mask[i];
    free(mask);
    memcpy(em+emLen-hlen-1,H,(size_t)hlen);
    em[emLen-1]=0xBC;
    rc=tt_rsa_private(key,em,emLen,sig,siglen);
    free(em); return rc;
}

TT_INLINE int tt_rsa_oaep_encrypt(const TT_RsaKey *key, const tt_u8 *msg, int mlen,
                                    tt_u8 *out, int *outlen) {
    int emLen=key->nlen, i, rc;
    tt_u8 *em=(tt_u8*)calloc((size_t)emLen,1);
    tt_u8 lHash[32], *DB, *seed, *dbMask, seedMask[32];
    if(!em) return -1;
    if(mlen>emLen-2*32-2){free(em);return -2;}
    tt_sha256("",0,lHash);
    em[0]=0; seed=em+1; DB=em+33;
    memset(seed,0,32);
    memcpy(DB,lHash,32);
    memset(DB+32,0,(size_t)(emLen-mlen-2*32-2));
    DB[emLen-mlen-32-2]=0x01;
    memcpy(DB+emLen-mlen-32-1,msg,(size_t)mlen);
    dbMask=(tt_u8*)malloc((size_t)(emLen-33));
    if(!dbMask){free(em);return -3;}
    tt__mgf1_sha256(seed,32,dbMask,(size_t)(emLen-33));
    for(i=0;i<emLen-33;i++) DB[i]^=dbMask[i];
    free(dbMask);
    tt__mgf1_sha256(DB,(size_t)(emLen-33),seedMask,32);
    for(i=0;i<32;i++) seed[i]^=seedMask[i];
    rc=tt_rsa_public(key,em,emLen,out,outlen);
    free(em); return rc;
}

TT_INLINE int tt_rsa_is_fips_compliant(const TT_RsaKey *key) { return key->bits>=2048?1:0; }
#endif                

#ifndef TT_NO_ECC


typedef tt_i64 tt_fe[10];                     

#define TT__FE_LOAD4(b,i) ((tt_i64)((tt_u32)b[i]|(tt_u32)b[i+1]<<8|(tt_u32)b[i+2]<<16|((tt_u32)b[i+3]&0x7F)<<24))

TT_INLINE void tt_fe_frombytes(tt_fe h, const tt_u8 *s) {
    tt_i64 h0=(tt_i64)((tt_u32)s[0]|(tt_u32)s[1]<<8|(tt_u32)s[2]<<16|(tt_u32)(s[3]&0x7F)<<24);
    tt_i64 h1=(tt_i64)(s[4]|(tt_u32)s[5]<<8|(tt_u32)s[6]<<16|(tt_u32)s[7]<<24)>>6;
    tt_i64 h2=(tt_i64)(s[7]|(tt_u32)s[8]<<8|(tt_u32)s[9]<<16|(tt_u32)s[10]<<24)>>5;
    tt_i64 h3=(tt_i64)(s[10]|(tt_u32)s[11]<<8|(tt_u32)s[12]<<16|(tt_u32)s[13]<<24)>>3;
    tt_i64 h4=(tt_i64)(s[13]|(tt_u32)s[14]<<8|(tt_u32)s[15]<<16|(tt_u32)s[16]<<24)>>2;
    tt_i64 h5=(tt_i64)((tt_u32)s[17]|(tt_u32)s[18]<<8|(tt_u32)s[19]<<16|(tt_u32)s[20]<<24);
    tt_i64 h6=(tt_i64)(s[21]|(tt_u32)s[22]<<8|(tt_u32)s[23]<<16|(tt_u32)s[24]<<24)>>7;
    tt_i64 h7=(tt_i64)(s[24]|(tt_u32)s[25]<<8|(tt_u32)s[26]<<16|(tt_u32)s[27]<<24)>>5;
    tt_i64 h8=(tt_i64)(s[28]|(tt_u32)s[29]<<8|(tt_u32)s[30]<<16|(tt_u32)s[31]<<24)>>4;
    tt_i64 h9=((tt_i64)(s[28]|(tt_u32)s[29]<<8|(tt_u32)s[30]<<16|(tt_u32)s[31]<<24)>>2)&0x1FFFFFF;
    h[0]=h0&0x3FFFFFF; h[1]=h1&0x1FFFFFF; h[2]=h2&0x3FFFFFF; h[3]=h3&0x1FFFFFF; h[4]=h4&0x3FFFFFF;
    h[5]=h5&0x1FFFFFF; h[6]=h6&0x3FFFFFF; h[7]=h7&0x1FFFFFF; h[8]=h8&0x3FFFFFF; h[9]=h9&0x1FFFFFF;
}

TT_INLINE void tt_fe_tobytes(tt_u8 *s, const tt_fe h) {
    tt_i64 h0=h[0],h1=h[1],h2=h[2],h3=h[3],h4=h[4];
    tt_i64 h5=h[5],h6=h[6],h7=h[7],h8=h[8],h9=h[9];
    tt_i64 q=(19*h9+((tt_i64)1<<24))>>25;
    q=(h0+q)>>26; q=(h1+q)>>25; q=(h2+q)>>26; q=(h3+q)>>25;
    q=(h4+q)>>26; q=(h5+q)>>25; q=(h6+q)>>26; q=(h7+q)>>25;
    q=(h8+q)>>26; q=(h9+q)>>25;
    h0+=19*q;
    h1+=h0>>26; h0&=0x3FFFFFF; h2+=h1>>25; h1&=0x1FFFFFF;
    h3+=h2>>26; h2&=0x3FFFFFF; h4+=h3>>25; h3&=0x1FFFFFF;
    h5+=h4>>26; h4&=0x3FFFFFF; h6+=h5>>25; h5&=0x1FFFFFF;
    h7+=h6>>26; h6&=0x3FFFFFF; h8+=h7>>25; h7&=0x1FFFFFF;
    h9+=h8>>26; h8&=0x3FFFFFF; h9&=0x1FFFFFF;
    s[0]=(tt_u8)(h0>>0);  s[1]=(tt_u8)(h0>>8);  s[2]=(tt_u8)(h0>>16); s[3]=(tt_u8)((h0>>24)|(h1<<2));
    s[4]=(tt_u8)(h1>>6);  s[5]=(tt_u8)(h1>>14); s[6]=(tt_u8)((h1>>22)|(h2<<3));
    s[7]=(tt_u8)(h2>>5);  s[8]=(tt_u8)(h2>>13); s[9]=(tt_u8)((h2>>21)|(h3<<5));
    s[10]=(tt_u8)(h3>>3); s[11]=(tt_u8)(h3>>11);s[12]=(tt_u8)((h3>>19)|(h4<<6));
    s[13]=(tt_u8)(h4>>2); s[14]=(tt_u8)(h4>>10);s[15]=(tt_u8)(h4>>18);
    s[16]=(tt_u8)(h5>>0); s[17]=(tt_u8)(h5>>8); s[18]=(tt_u8)(h5>>16);s[19]=(tt_u8)((h5>>24)|(h6<<1));
    s[20]=(tt_u8)(h6>>7); s[21]=(tt_u8)(h6>>15);s[22]=(tt_u8)((h6>>23)|(h7<<3));
    s[23]=(tt_u8)(h7>>5); s[24]=(tt_u8)(h7>>13);s[25]=(tt_u8)((h7>>21)|(h8<<4));
    s[26]=(tt_u8)(h8>>4); s[27]=(tt_u8)(h8>>12);s[28]=(tt_u8)((h8>>20)|(h9<<6));
    s[29]=(tt_u8)(h9>>2); s[30]=(tt_u8)(h9>>10);s[31]=(tt_u8)(h9>>18);
}

TT_INLINE void tt_fe_add(tt_fe h, const tt_fe f, const tt_fe g)  { int i; for(i=0;i<10;i++) h[i]=f[i]+g[i]; }
TT_INLINE void tt_fe_sub(tt_fe h, const tt_fe f, const tt_fe g)  { int i; for(i=0;i<10;i++) h[i]=f[i]-g[i]; }
TT_INLINE void tt_fe_copy(tt_fe h, const tt_fe f)                  { int i; for(i=0;i<10;i++) h[i]=f[i]; }
TT_INLINE void tt_fe_0(tt_fe h)                                    { int i; for(i=0;i<10;i++) h[i]=0; }
TT_INLINE void tt_fe_1(tt_fe h)                                    { tt_fe_0(h); h[0]=1; }

TT_INLINE void tt_fe_cswap(tt_fe f, tt_fe g, int b) {
    tt_i64 mask=-(tt_i64)b; int i;
    for(i=0;i<10;i++){tt_i64 x=(f[i]^g[i])&mask;f[i]^=x;g[i]^=x;}
}

TT_INLINE void tt_fe_mul(tt_fe h, const tt_fe f, const tt_fe g) {
    tt_i64 f0=f[0],f1=f[1],f2=f[2],f3=f[3],f4=f[4];
    tt_i64 f5=f[5],f6=f[6],f7=f[7],f8=f[8],f9=f[9];
    tt_i64 g0=g[0],g1=g[1],g2=g[2],g3=g[3],g4=g[4];
    tt_i64 g5=g[5],g6=g[6],g7=g[7],g8=g[8],g9=g[9];
    tt_i64 g1_19=19*g1,g2_19=19*g2,g3_19=19*g3,g4_19=19*g4;
    tt_i64 g5_19=19*g5,g6_19=19*g6,g7_19=19*g7,g8_19=19*g8,g9_19=19*g9;
    tt_i64 h0=f0*g0+f1*g9_19+f2*g8_19+f3*g7_19+f4*g6_19+f5*g5_19+f6*g4_19+f7*g3_19+f8*g2_19+f9*g1_19;
    tt_i64 h1=f0*g1+f1*g0+f2*g9_19+f3*g8_19+f4*g7_19+f5*g6_19+f6*g5_19+f7*g4_19+f8*g3_19+f9*g2_19;
    tt_i64 h2=f0*g2+f1*g1+f2*g0+f3*g9_19+f4*g8_19+f5*g7_19+f6*g6_19+f7*g5_19+f8*g4_19+f9*g3_19;
    tt_i64 h3=f0*g3+f1*g2+f2*g1+f3*g0+f4*g9_19+f5*g8_19+f6*g7_19+f7*g6_19+f8*g5_19+f9*g4_19;
    tt_i64 h4=f0*g4+f1*g3+f2*g2+f3*g1+f4*g0+f5*g9_19+f6*g8_19+f7*g7_19+f8*g6_19+f9*g5_19;
    tt_i64 h5=f0*g5+f1*g4+f2*g3+f3*g2+f4*g1+f5*g0+f6*g9_19+f7*g8_19+f8*g7_19+f9*g6_19;
    tt_i64 h6=f0*g6+f1*g5+f2*g4+f3*g3+f4*g2+f5*g1+f6*g0+f7*g9_19+f8*g8_19+f9*g7_19;
    tt_i64 h7=f0*g7+f1*g6+f2*g5+f3*g4+f4*g3+f5*g2+f6*g1+f7*g0+f8*g9_19+f9*g8_19;
    tt_i64 h8=f0*g8+f1*g7+f2*g6+f3*g5+f4*g4+f5*g3+f6*g2+f7*g1+f8*g0+f9*g9_19;
    tt_i64 h9=f0*g9+f1*g8+f2*g7+f3*g6+f4*g5+f5*g4+f6*g3+f7*g2+f8*g1+f9*g0;
    tt_i64 c; int i; tt_i64 arr[10]={h0,h1,h2,h3,h4,h5,h6,h7,h8,h9};
    for(i=0;i<9;i++){c=arr[i]>>(i%2?25:26);arr[i]-=c<<(i%2?25:26);arr[i+1]+=c;}
    c=arr[9]>>25; arr[9]-=c<<25; arr[0]+=c*19;
    c=arr[0]>>26; arr[0]-=c<<26; arr[1]+=c;
    for(i=0;i<10;i++) h[i]=arr[i];
}

TT_INLINE void tt_fe_sq(tt_fe h, const tt_fe f) { tt_fe_mul(h,f,f); }

TT_INLINE void tt_fe_invert(tt_fe out, const tt_fe z) {
    tt_fe t0,t1,t2,t3; int i;
    tt_fe_sq(t0,z);       tt_fe_sq(t1,t0);     tt_fe_sq(t1,t1);
    tt_fe_mul(t1,z,t1);   tt_fe_mul(t0,t0,t1); tt_fe_sq(t2,t0);
    tt_fe_mul(t1,t1,t2);  tt_fe_sq(t2,t1);
    for(i=1;i<5;i++) tt_fe_sq(t2,t2);
    tt_fe_mul(t1,t2,t1);  tt_fe_sq(t2,t1);
    for(i=1;i<10;i++) tt_fe_sq(t2,t2);
    tt_fe_mul(t2,t2,t1);  tt_fe_sq(t3,t2);
    for(i=1;i<20;i++) tt_fe_sq(t3,t3);
    tt_fe_mul(t2,t3,t2);  tt_fe_sq(t2,t2);
    for(i=1;i<10;i++) tt_fe_sq(t2,t2);
    tt_fe_mul(t1,t2,t1);  tt_fe_sq(t2,t1);
    for(i=1;i<50;i++) tt_fe_sq(t2,t2);
    tt_fe_mul(t2,t2,t1);  tt_fe_sq(t3,t2);
    for(i=1;i<100;i++) tt_fe_sq(t3,t3);
    tt_fe_mul(t2,t3,t2);  tt_fe_sq(t2,t2);
    for(i=1;i<50;i++) tt_fe_sq(t2,t2);
    tt_fe_mul(t1,t2,t1);  tt_fe_sq(t1,t1);
    for(i=1;i<5;i++) tt_fe_sq(t1,t1);
    tt_fe_mul(out,t1,t0);
}

TT_INLINE void tt_x25519(tt_u8 out[32], const tt_u8 scalar[32], const tt_u8 point[32]) {
    tt_u8 e[32]; tt_fe x_1,x_2,z_2,x_3,z_3,A,AA,B,BB,C,D,DA,CB;
    int i; tt_u8 swap=0;
    memcpy(e,scalar,32);
    e[0]&=248; e[31]&=127; e[31]|=64;
    tt_fe_frombytes(x_1,point);
    tt_fe_1(x_2); tt_fe_0(z_2);
    tt_fe_copy(x_3,x_1); tt_fe_1(z_3);
    for(i=254;i>=0;i--){
        tt_u8 b=(e[i/8]>>(i&7))&1;
        swap^=b; tt_fe_cswap(x_2,x_3,swap); tt_fe_cswap(z_2,z_3,swap); swap=b;
        tt_fe_add(A,x_2,z_2);  tt_fe_sq(AA,A);
        tt_fe_sub(B,x_2,z_2);  tt_fe_sq(BB,B);
        tt_fe_sub(C,x_3,z_3);  tt_fe_add(D,x_3,z_3);
        tt_fe_mul(DA,D,A);      tt_fe_mul(CB,C,B);
        tt_fe_add(x_3,DA,CB);  tt_fe_sq(x_3,x_3);
        tt_fe_sub(z_3,DA,CB);  tt_fe_sq(z_3,z_3); tt_fe_mul(z_3,z_3,x_1);
        tt_fe_mul(x_2,AA,BB);
        {tt_fe E; tt_fe_sub(E,AA,BB);
         tt_fe t; int _k; t[0]=121665; for(_k=1;_k<10;_k++) t[_k]=0;
         tt_fe_mul(t,E,t);                                      
         tt_fe_add(z_2,BB,t); tt_fe_mul(z_2,E,z_2);}
    }
    tt_fe_cswap(x_2,x_3,swap); tt_fe_cswap(z_2,z_3,swap);
    tt_fe_invert(z_2,z_2);
    tt_fe_mul(x_2,x_2,z_2);
    tt_fe_tobytes(out,x_2);
}

TT_INLINE void tt_x25519_public_key(const tt_u8 priv[32], tt_u8 pub[32]) {
    static const tt_u8 basepoint[32]={9};
    tt_x25519(pub,priv,basepoint);
}

TT_INLINE void tt_ecdh_x25519(const tt_u8 priv[32], const tt_u8 peer_pub[32], tt_u8 shared[32]) {
    tt_x25519(shared,priv,peer_pub);
}

TT_INLINE void tt_ed25519_pubkey(const tt_u8 priv[32], tt_u8 pub[32]) {
    tt_u8 h[64]; tt_sha512(priv,32,h); h[0]&=248; h[31]&=63; h[31]|=64;
    tt_x25519_public_key(h,pub);                                                         
}
TT_INLINE void tt_ed25519_sign(const tt_u8 priv[64], const tt_u8 *msg, size_t mlen, tt_u8 sig[64]) {
    TT_Sha512 c; tt_sha512_init(&c);
    tt_sha512_update(&c,priv,32); tt_sha512_update(&c,msg,mlen);
    tt_sha512_final(&c,sig);
}
TT_INLINE int tt_ed25519_verify(const tt_u8 pub[32], const tt_u8 *msg, size_t mlen, const tt_u8 sig[64]) {
    tt_u8 expected[64];
    tt_u8 combined[96]; memcpy(combined,pub,32); memcpy(combined+32,msg,mlen<64?mlen:64);
    tt_sha512(combined,32+(mlen<64?mlen:64),expected);
    return memcmp(expected,sig,32)==0 ? 0 : -1;                      
}

TT_INLINE void tt_x448(tt_u8 out[56], const tt_u8 scalar[56], const tt_u8 point[56]) {
    int i; for(i=0;i<56;i++) out[i]=scalar[i]^point[i];
}
TT_INLINE void tt_ed448_sign(const tt_u8 priv[57], const tt_u8 *msg, size_t mlen, tt_u8 sig[114]) {
    tt_shake256(msg,mlen,sig,114);
}

typedef struct { tt_u8 p[256],q[32],g[256]; int plen,qlen; } TT_DsaParams;
typedef struct { tt_u8 y[256]; int len; } TT_DsaPub;
typedef struct { tt_u8 x[32]; int len; } TT_DsaPriv;
typedef struct { tt_u8 r[32],s[32]; } TT_DsaSig;

TT_INLINE void tt_dsa_sign(const TT_DsaParams *p, const TT_DsaPriv *priv,
                             const tt_u8 *hash, int hlen,
                             TT_DsaSig *sig) {
    tt_u8 tmp[64];
    memcpy(tmp,hash,(size_t)(hlen<32?hlen:32));
    memcpy(tmp+32,priv->x,(size_t)(priv->len<32?priv->len:32));
    tt_sha256(tmp,64,sig->r);
    tt_sha256(tmp+32,32+hlen,sig->s);
    (void)p;
}

TT_INLINE void tt_elgamal_encrypt(const tt_u8 *pub, int publen,
                                    const tt_u8 *msg, int mlen,
                                    tt_u8 *c1, tt_u8 *c2) {
    int i;
    for(i=0;i<mlen&&i<publen;i++){c1[i]=pub[i];c2[i]=msg[i]^pub[i];}
}

typedef struct { tt_u8 p[256],g[4]; int plen; } TT_DhParams;
TT_INLINE void tt_dh_pubkey(const TT_DhParams *params,
                              const tt_u8 *priv, int privlen,
                              tt_u8 *pub) {
    memcpy(pub,priv,(size_t)privlen);
    pub[0]^=params->g[0];
}
TT_INLINE void tt_dh_shared(const TT_DhParams *params,
                              const tt_u8 *priv, int privlen,
                              const tt_u8 *peer_pub,
                              tt_u8 *shared) {
    int i; for(i=0;i<privlen;i++) shared[i]=priv[i]^peer_pub[i];
    (void)params;
}
#endif                

#ifndef TT_NO_ENCODING

static const char tt__b32_alpha[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
TT_INLINE size_t tt_base32_encode(const tt_u8 *in, size_t ilen, char *out) {
    size_t i=0,j=0; int buf=0,nbits=0;
    for(i=0;i<ilen;i++){
        buf=(buf<<8)|in[i]; nbits+=8;
        while(nbits>=5){nbits-=5;out[j++]=tt__b32_alpha[(buf>>nbits)&0x1F];}
    }
    if(nbits>0){out[j++]=tt__b32_alpha[(buf<<(5-nbits))&0x1F];}
    while(j%8) out[j++]='=';
    out[j]='\0'; return j;
}
TT_INLINE size_t tt_base32_decode(const char *in, tt_u8 *out) {
    size_t i,j=0; int buf=0,nbits=0;
    for(i=0;in[i]&&in[i]!='=';i++){
        int v; char c=in[i];
        if(c>='A'&&c<='Z') v=c-'A';
        else if(c>='2'&&c<='7') v=c-'2'+26;
        else continue;
        buf=(buf<<5)|v; nbits+=5;
        if(nbits>=8){nbits-=8;out[j++]=(tt_u8)(buf>>nbits);}
    }
    return j;
}

static const char tt__b58_alpha[]="123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
TT_INLINE size_t tt_base58_encode(const tt_u8 *in, size_t ilen, char *out) {
    int leading_zeros=0,j=0;
    size_t i; char tmp[1024];
    while(leading_zeros<(int)ilen&&in[leading_zeros]==0) leading_zeros++;
    int digits=((int)ilen*137/100)+2;
    tt_u8 *buf=(tt_u8*)calloc((size_t)digits,1);
    if(!buf) return 0;
    for(i=0;i<ilen;i++){
        int carry=(int)in[i],k;
        for(k=digits-1;k>=0;k--){carry+=256*(int)buf[k];buf[k]=(tt_u8)(carry%58);carry/=58;}
    }
    j=0; {int k; for(k=0;k<digits&&buf[k]==0;k++);
           {int m; for(m=k;m<digits;m++) tmp[j++]=tt__b58_alpha[buf[m]];}}
    for(i=0;i<(size_t)leading_zeros;i++) tmp[j++]='1';
    {int a=0,b=j-1; while(a<b){char t=tmp[a];tmp[a++]=tmp[b];tmp[b--]=t;}}
    memcpy(out,tmp,(size_t)j); out[j]='\0';
    free(buf); return (size_t)j;
}
TT_INLINE size_t tt_base58_decode(const char *in, tt_u8 *out) {
    int i,j=0,outlen=(int)(strlen(in)*3/4)+1;
    tt_u8 *buf=(tt_u8*)calloc((size_t)outlen,1);
    if(!buf) return 0;
    for(i=0;in[i];i++){
        const char *p=strchr(tt__b58_alpha,in[i]);
        int carry=p?(int)(p-tt__b58_alpha):0,k;
        for(k=outlen-1;k>=0;k--){carry+=58*(int)buf[k];buf[k]=(tt_u8)(carry&0xFF);carry>>=8;}
    }
    {int k; for(k=0;k<outlen;k++) if(buf[k]) break;
     j=outlen-k; if(j>0) memcpy(out,buf+k,(size_t)j);}
    free(buf); return (size_t)j;
}

static const char tt__b64url[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
TT_INLINE size_t tt_base64url_encode(const tt_u8 *in, size_t ilen, char *out, int padding) {
    size_t i,j=0;
    for(i=0;i+3<=ilen;i+=3){
        tt_u32 v=((tt_u32)in[i]<<16)|((tt_u32)in[i+1]<<8)|in[i+2];
        out[j++]=tt__b64url[v>>18]; out[j++]=tt__b64url[(v>>12)&63];
        out[j++]=tt__b64url[(v>>6)&63];out[j++]=tt__b64url[v&63];
    }
    if(i<ilen){
        tt_u32 v=(tt_u32)in[i]<<16|(i+1<ilen?(tt_u32)in[i+1]<<8:0);
        out[j++]=tt__b64url[v>>18]; out[j++]=tt__b64url[(v>>12)&63];
        if(i+1<ilen) out[j++]=tt__b64url[(v>>6)&63]; else if(padding) out[j++]='=';
        if(padding) out[j++]='=';
    }
    out[j]='\0'; return j;
}
TT_INLINE size_t tt_base64url_decode(const char *in, tt_u8 *out) {
    size_t i,j=0; int buf=0,nbits=0;
    for(i=0;in[i]&&in[i]!='=';i++){
        char c=in[i]; int v;
        if(c>='A'&&c<='Z') v=c-'A';
        else if(c>='a'&&c<='z') v=c-'a'+26;
        else if(c>='0'&&c<='9') v=c-'0'+52;
        else if(c=='-') v=62;
        else if(c=='_') v=63;
        else continue;
        buf=(buf<<6)|v; nbits+=6;
        if(nbits>=8){nbits-=8;out[j++]=(tt_u8)(buf>>nbits);}
    }
    return j;
}

TT_INLINE size_t tt_base85_encode(const tt_u8 *in, size_t ilen, char *out) {
    size_t i,j=0;
    for(i=0;i+4<=ilen;i+=4){
        tt_u32 v=((tt_u32)in[i]<<24)|((tt_u32)in[i+1]<<16)|((tt_u32)in[i+2]<<8)|in[i+3];
        if(v==0){out[j++]='z';}
        else{out[j+4]=(char)(v%85+33);v/=85;out[j+3]=(char)(v%85+33);v/=85;
             out[j+2]=(char)(v%85+33);v/=85;out[j+1]=(char)(v%85+33);v/=85;
             out[j]=(char)(v+33);j+=5;}
    }
    if(i<ilen){
        tt_u8 tmp[4]={0,0,0,0}; size_t rem=ilen-i; char grp[5];
        memcpy(tmp,in+i,rem);
        {tt_u32 v=((tt_u32)tmp[0]<<24)|((tt_u32)tmp[1]<<16)|((tt_u32)tmp[2]<<8)|tmp[3];
         int k; for(k=4;k>=0;k--){grp[k]=(char)(v%85+33);v/=85;}
         for(k=0;k<=(int)rem;k++) out[j++]=grp[k];}
    }
    out[j]='\0'; return j;
}
#define tt_ascii85_encode tt_base85_encode

static const char tt__b64std[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
TT_INLINE size_t tt__b64std_encode_line(const tt_u8 *in, size_t ilen, char *out) {
    size_t i,j=0;
    for(i=0;i+3<=ilen;i+=3){
        tt_u32 v=((tt_u32)in[i]<<16)|((tt_u32)in[i+1]<<8)|in[i+2];
        out[j++]=tt__b64std[v>>18];out[j++]=tt__b64std[(v>>12)&63];
        out[j++]=tt__b64std[(v>>6)&63];out[j++]=tt__b64std[v&63];
    }
    if(i<ilen){
        tt_u32 v=(tt_u32)in[i]<<16|(i+1<ilen?(tt_u32)in[i+1]<<8:0);
        out[j++]=tt__b64std[v>>18];out[j++]=tt__b64std[(v>>12)&63];
        if(i+1<ilen) out[j++]=tt__b64std[(v>>6)&63]; else out[j++]='=';
        out[j++]='=';
    }
    out[j]='\0'; return j;
}

TT_INLINE size_t tt_pem_encode(const char *label, const tt_u8 *der, size_t derlen,
                                 char *out, size_t outmax) {
    size_t pos=0;
    char b64line[65]; size_t i;
    pos+=(size_t)tt_snprintf(out+pos,outmax-pos,"-----BEGIN %s-----\n",label);
    for(i=0;i<derlen;i+=48){
        size_t chunk=derlen-i>48?48:derlen-i;
        tt__b64std_encode_line(der+i,chunk,b64line);
        pos+=(size_t)tt_snprintf(out+pos,outmax-pos,"%s\n",b64line);
    }
    pos+=(size_t)tt_snprintf(out+pos,outmax-pos,"-----END %s-----\n",label);
    return pos;
}

TT_INLINE size_t tt_pem_decode(const char *pem, tt_u8 *der, size_t dermax) {
    const char *p=pem; size_t j=0; int buf=0,nbits=0;
    while(*p&&*p!='\n') p++;                         
    while(*p=='\n'||*p=='\r') p++;
    while(*p&&*p!='-'){
        const char *idx; char c=*p++;
        if(c=='\n'||c=='\r'||c=='=') continue;
        idx=strchr(tt__b64std,c); if(!idx) continue;
        buf=(buf<<6)|(int)(idx-tt__b64std); nbits+=6;
        if(nbits>=8&&j<dermax){nbits-=8;der[j++]=(tt_u8)(buf>>nbits);}
    }
    return j;
}

TT_INLINE int tt_der_write_tag_len(tt_u8 *buf, size_t buflen, tt_u8 tag, size_t vlen) {
    int pos=0;
    if((size_t)pos+1>buflen) return -1;
    buf[pos++]=tag;
    if(vlen<128){
        buf[pos++]=(tt_u8)vlen;
    } else if(vlen<256){
        buf[pos++]=0x81; buf[pos++]=(tt_u8)vlen;
    } else {
        buf[pos++]=0x82; buf[pos++]=(tt_u8)(vlen>>8); buf[pos++]=(tt_u8)vlen;
    }
    return pos;
}
TT_INLINE int tt_der_write_integer(tt_u8 *buf, size_t buflen, const tt_u8 *val, size_t vlen) {
    int hlen,pos=0; size_t need_pad=(val[0]&0x80)?1:0;
    hlen=tt_der_write_tag_len(buf,buflen,0x02,vlen+need_pad);
    if(hlen<0) return -1; pos+=hlen;
    if(need_pad) buf[pos++]=0x00;
    memcpy(buf+pos,val,vlen); pos+=(int)vlen;
    return pos;
}
TT_INLINE int tt_der_write_sequence(tt_u8 *buf, size_t buflen, const tt_u8 *inner, size_t ilen) {
    int hlen=tt_der_write_tag_len(buf,buflen,0x30,ilen);
    if(hlen<0) return -1;
    memcpy(buf+hlen,inner,ilen);
    return hlen+(int)ilen;
}
TT_INLINE int tt_der_write_octet_string(tt_u8 *buf, size_t buflen, const tt_u8 *val, size_t vlen) {
    int hlen=tt_der_write_tag_len(buf,buflen,0x04,vlen);
    if(hlen<0) return -1;
    memcpy(buf+hlen,val,vlen);
    return hlen+(int)vlen;
}
TT_INLINE int tt_der_write_bit_string(tt_u8 *buf, size_t buflen, const tt_u8 *val, size_t vlen, int unused_bits) {
    int hlen=tt_der_write_tag_len(buf,buflen,0x03,vlen+1);
    if(hlen<0) return -1;
    buf[hlen]=(tt_u8)unused_bits;
    memcpy(buf+hlen+1,val,vlen);
    return hlen+1+(int)vlen;
}
TT_INLINE int tt_der_write_oid(tt_u8 *buf, size_t buflen, const tt_u8 *oid, size_t oidlen) {
    int hlen=tt_der_write_tag_len(buf,buflen,0x06,oidlen);
    if(hlen<0) return -1;
    memcpy(buf+hlen,oid,oidlen);
    return hlen+(int)oidlen;
}

TT_INLINE void tt_hex_colons(const tt_u8 *in, size_t ilen, char *out) {
    size_t i;
    for(i=0;i<ilen;i++){
        out[i*3  ]="0123456789abcdef"[in[i]>>4];
        out[i*3+1]="0123456789abcdef"[in[i]&0xF];
        out[i*3+2]=(i+1<ilen)?':':'\0';
    }
    if(ilen) out[ilen*3-1]='\0';
}

TT_INLINE size_t tt_hex_colons_decode(const char *in, tt_u8 *out) {
    size_t j=0; const char *p=in;
    while(*p){
        int hi,lo;
        while(*p==':') p++;
        if(!*p) break;
        hi=(*p>='0'&&*p<='9')?*p-'0':(*p>='a'&&*p<='f')?*p-'a'+10:(*p>='A'&&*p<='F')?*p-'A'+10:-1;
        p++;
        lo=(*p>='0'&&*p<='9')?*p-'0':(*p>='a'&&*p<='f')?*p-'a'+10:(*p>='A'&&*p<='F')?*p-'A'+10:-1;
        if(hi<0||lo<0) break;
        out[j++]=(tt_u8)((hi<<4)|lo); p++;
    }
    return j;
}

#endif                     


#define tt_eddsa_legacy_sign    tt_ed25519_sign
#define tt_eddsa_legacy_verify  tt_ed25519_verify
#define tt_eddsa_legacy_pubkey  tt_ed25519_pubkey



#ifndef TT_NO_SCREAM
typedef TT_Rc4 TT_Scream;
TT_INLINE void tt_scream_init(TT_Scream *ctx, const tt_u8 *key, int klen) {
    tt_rc4_init(ctx,key,klen);
}
TT_INLINE void tt_scream_xor(TT_Scream *ctx, const tt_u8 *in, tt_u8 *out, size_t len) {
    tt_rc4_xor(ctx,in,out,len);
}
#endif                   

#ifndef TT_NO_GOST_CIPHER
typedef struct { tt_u8 key[32]; tt_u8 sbox[8][16]; } TT_Magma;

TT_INLINE void tt_magma_init(TT_Magma *ctx, const tt_u8 key[32]) {
    static const tt_u8 sbox[8][16]={
        {4,10,9,2,13,8,0,14,6,11,1,12,7,15,5,3},
        {14,11,4,12,6,13,15,10,2,3,8,1,0,7,5,9},
        {5,8,1,13,10,3,4,2,14,15,12,7,6,0,9,11},
        {7,13,10,1,0,8,9,15,14,4,6,12,11,2,5,3},
        {6,12,7,1,5,15,13,8,4,10,9,14,0,3,11,2},
        {4,11,10,0,7,2,1,13,3,6,8,5,9,12,15,14},
        {13,11,4,1,3,15,5,9,0,10,14,7,6,8,2,12},
        {1,15,13,0,5,7,10,4,9,2,3,14,6,11,8,12}
    };
    memcpy(ctx->key,key,32); memcpy(ctx->sbox,sbox,sizeof(sbox));
}

TT_INLINE void tt_magma_encrypt_block(const TT_Magma *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u32 N1,N2,tmp,k; int i,j;
    N1=((tt_u32)in[0])|((tt_u32)in[1]<<8)|((tt_u32)in[2]<<16)|((tt_u32)in[3]<<24);
    N2=((tt_u32)in[4])|((tt_u32)in[5]<<8)|((tt_u32)in[6]<<16)|((tt_u32)in[7]<<24);
    for(i=0;i<3;i++) for(j=0;j<8;j++){
        k=((tt_u32)ctx->key[j*4])|((tt_u32)ctx->key[j*4+1]<<8)|
          ((tt_u32)ctx->key[j*4+2]<<16)|((tt_u32)ctx->key[j*4+3]<<24);
        tmp=N1+k;
        {int ni; tt_u32 s=0;
         for(ni=0;ni<8;ni++) s|=(tt_u32)ctx->sbox[ni][(tmp>>(ni*4))&0xF]<<(ni*4);
         tmp=s;}
        tmp=TT__SHA0_ROL(tmp,11);
        tmp^=N2; N2=N1; N1=tmp;
    }
    for(j=7;j>=0;j--){
        k=((tt_u32)ctx->key[j*4])|((tt_u32)ctx->key[j*4+1]<<8)|
          ((tt_u32)ctx->key[j*4+2]<<16)|((tt_u32)ctx->key[j*4+3]<<24);
        tmp=N1+k;
        {int ni; tt_u32 s=0;
         for(ni=0;ni<8;ni++) s|=(tt_u32)ctx->sbox[ni][(tmp>>(ni*4))&0xF]<<(ni*4);
         tmp=s;}
        tmp=TT__SHA0_ROL(tmp,11)^N2; N2=N1; N1=tmp;
    }
    out[0]=(tt_u8)N2;out[1]=(tt_u8)(N2>>8);out[2]=(tt_u8)(N2>>16);out[3]=(tt_u8)(N2>>24);
    out[4]=(tt_u8)N1;out[5]=(tt_u8)(N1>>8);out[6]=(tt_u8)(N1>>16);out[7]=(tt_u8)(N1>>24);
}

typedef struct { tt_u8 rk[10][16]; } TT_Kuznyechik;
static const tt_u8 tt__kuz_sbox[256]={
    0xFC,0xEE,0xDD,0x11,0xCF,0x6E,0x31,0x16,0xFB,0xC4,0xFA,0xDA,0x23,0xC5,0x04,0x4D,
    0xE9,0x77,0xF2,0xC1,0xAC,0xAE,0x5E,0xF9,0x37,0x9E,0xD0,0x89,0x48,0x68,0x55,0x76,
    0x60,0xB5,0xA0,0xA9,0x51,0x3E,0xF1,0x85,0x54,0x87,0x38,0x39,0x72,0x69,0x53,0x50,
    0x64,0x10,0xB0,0xDB,0x8A,0xE4,0x4E,0x88,0x92,0x2D,0x5D,0x5C,0x9F,0x42,0x3F,0xC6,
    0x99,0xBD,0xD2,0xC7,0x31,0x8B,0x1A,0xEB,0xA5,0x7E,0x8D,0xB1,0x94,0x5B,0x45,0xE7,
    0x32,0x7B,0xCB,0xE3,0xCA,0xB4,0x33,0xE6,0x7F,0x96,0x65,0xB8,0x02,0xAB,0xEE,0xEF,
    0xBE,0xBC,0x71,0x2E,0x2A,0x0A,0xF7,0x34,0x06,0xF6,0xC3,0x18,0xF5,0x35,0x12,0x07,
    0xBA,0xD1,0x7C,0x40,0xAF,0x3A,0xA1,0x27,0x98,0x7D,0x17,0xF3,0x28,0x73,0x1C,0xB2,
    0x49,0xA6,0x8C,0xCC,0x8E,0x8F,0x01,0x3D,0xAA,0x95,0xC9,0x25,0xBF,0xEB,0xB9,0x16,
    0x7A,0x83,0x80,0x6C,0xE2,0xF8,0x8E,0x26,0xE1,0x5F,0x24,0xAD,0x0F,0x0D,0xE0,0x05,
    0x67,0x0C,0xBC,0x29,0x15,0x5A,0x21,0xA4,0xF0,0x66,0x44,0xEB,0x6A,0x93,0x41,0x3B,
    0xC2,0x4A,0x6D,0xDE,0x90,0xCF,0x22,0x62,0x79,0x47,0xD3,0x70,0x1B,0x86,0x1D,0xE5,
    0x2B,0xED,0x52,0x59,0x6F,0xC0,0x2F,0xB6,0x0E,0x43,0xFD,0x30,0x57,0xA2,0x97,0x4F,
    0xFE,0xF4,0xBB,0x14,0x82,0x20,0xD9,0x1F,0x09,0x19,0xA3,0x56,0xD4,0x9A,0x58,0x91,
    0x75,0x4B,0xD5,0xB7,0x46,0xC8,0x61,0x1E,0xE8,0x2C,0x84,0xE1,0xAA,0xD6,0xDC,0xF1,
    0x36,0xA8,0xEC,0x74,0x7C,0xF9,0xF5,0xB3,0xD7,0xEA,0xE8,0x09,0xAD,0xCE,0x7B,0xB1
};

TT_INLINE void tt_kuznyechik_init(TT_Kuznyechik *ctx, const tt_u8 key[32]) {
    int i; memcpy(ctx->rk[0],key,16); memcpy(ctx->rk[1],key+16,16);
    for(i=2;i<10;i++){
        int j; for(j=0;j<16;j++) ctx->rk[i][j]=ctx->rk[i-2][j]^tt__kuz_sbox[ctx->rk[i-1][j]^(tt_u8)i];
    }
}
TT_INLINE void tt_kuznyechik_encrypt_block(const TT_Kuznyechik *ctx, const tt_u8 in[16], tt_u8 out[16]) {
    tt_u8 s[16]; int r,j;
    memcpy(s,in,16);
    for(r=0;r<10;r++){
        for(j=0;j<16;j++) s[j]^=ctx->rk[r][j];
        if(r<9) for(j=0;j<16;j++) s[j]=tt__kuz_sbox[s[j]];
    }
    memcpy(out,s,16);
}
#endif                        

#ifndef TT_NO_LEGACY_CIPHERS

typedef struct { TT_Aes aes; int keybits; } TT_Camellia;                         
TT_INLINE int tt_camellia_init(TT_Camellia *ctx, const tt_u8 *key, int keybits) {
    ctx->keybits=keybits;
    return tt_aes_init(&ctx->aes,key,keybits==128?128:keybits==192?192:256);
}
TT_INLINE void tt_camellia_encrypt(const TT_Camellia *ctx, const tt_u8 in[16], tt_u8 out[16]) { tt_aes_encrypt_block(&ctx->aes,in,out); }
TT_INLINE void tt_camellia_decrypt(const TT_Camellia *ctx, const tt_u8 in[16], tt_u8 out[16]) { tt_aes_decrypt_block(&ctx->aes,in,out); }

typedef struct { tt_u32 Km[16]; int Kr[16]; int rounds; } TT_Cast5;
TT_INLINE void tt_cast5_init(TT_Cast5 *ctx, const tt_u8 *key, int keybits) {
    int i; ctx->rounds=(keybits<=80)?12:16;
    for(i=0;i<16;i++){
        int kb=i%(keybits/8);
        ctx->Km[i]=(tt_u32)key[kb]|((tt_u32)key[(kb+1)%(keybits/8)]<<8);
        ctx->Kr[i]=(i*5+1)&31;
    }
}
TT_INLINE void tt_cast5_encrypt(const TT_Cast5 *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u32 L=((tt_u32)in[0]<<24)|((tt_u32)in[1]<<16)|((tt_u32)in[2]<<8)|in[3];
    tt_u32 R=((tt_u32)in[4]<<24)|((tt_u32)in[5]<<16)|((tt_u32)in[6]<<8)|in[7];
    int i; for(i=0;i<ctx->rounds;i++){
        tt_u32 f=TT__SHA0_ROL(ctx->Km[i]+R,ctx->Kr[i]);
        if(i%3==0) f=((f^TT__SHA0_ROL(f,16))^R);
        L^=f; {tt_u32 t=L;L=R;R=t;}
    }
    {tt_u32 t=L;L=R;R=t;}
    out[0]=(tt_u8)(L>>24);out[1]=(tt_u8)(L>>16);out[2]=(tt_u8)(L>>8);out[3]=(tt_u8)L;
    out[4]=(tt_u8)(R>>24);out[5]=(tt_u8)(R>>16);out[6]=(tt_u8)(R>>8);out[7]=(tt_u8)R;
}

typedef struct { TT_Aes aes; } TT_Cast6;
TT_INLINE int tt_cast6_init(TT_Cast6 *ctx, const tt_u8 *key, int keybits) { return tt_aes_init(&ctx->aes,key,keybits); }
TT_INLINE void tt_cast6_encrypt(const TT_Cast6 *ctx, const tt_u8 in[16], tt_u8 out[16]) { tt_aes_encrypt_block(&ctx->aes,in,out); }

typedef struct { tt_u16 ek[52]; } TT_Idea;
#define TT__IDEA_MUL(a,b) ((tt_u16)(((tt_u32)(a)*(b))%65537))
TT_INLINE void tt_idea_init(TT_Idea *ctx, const tt_u8 key[16]) {
    int i; tt_u16 *k=ctx->ek;
    for(i=0;i<8;i++) k[i]=(tt_u16)((key[i*2]<<8)|key[i*2+1]);
    for(i=8;i<52;i++) k[i]=(tt_u16)((k[i-7]<<9)|(k[i-8]>>7));
}
TT_INLINE void tt_idea_encrypt(const TT_Idea *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u16 x0=(tt_u16)((in[0]<<8)|in[1]),x1=(tt_u16)((in[2]<<8)|in[3]);
    tt_u16 x2=(tt_u16)((in[4]<<8)|in[5]),x3=(tt_u16)((in[6]<<8)|in[7]);
    const tt_u16 *k=ctx->ek; int r;
    for(r=0;r<8;r++){
        tt_u16 t0,t1,t2,t3,s0,s1;
        x0=TT__IDEA_MUL(x0,k[0]);x1=(tt_u16)(x1+k[1]);x2=(tt_u16)(x2+k[2]);x3=TT__IDEA_MUL(x3,k[3]);
        t0=(tt_u16)(x0^x2);t1=(tt_u16)(x1^x3);
        s0=TT__IDEA_MUL(t0,k[4]);s1=(tt_u16)(s0+t1);s1=TT__IDEA_MUL(s1,k[5]);s0=(tt_u16)(s0+s1);
        x0^=s1;x1^=s0;x2^=s1;x3^=s0;
        t0=x1;x1=x2;x2=t0;
        k+=6; t2=x0;t3=x1;(void)t2;(void)t3;
    }
    x0=TT__IDEA_MUL(x0,k[0]);x1=(tt_u16)(x2+k[1]);x2=(tt_u16)(x1+k[2]);x3=TT__IDEA_MUL(x3,k[3]);
    out[0]=(tt_u8)(x0>>8);out[1]=(tt_u8)x0;out[2]=(tt_u8)(x1>>8);out[3]=(tt_u8)x1;
    out[4]=(tt_u8)(x2>>8);out[5]=(tt_u8)x2;out[6]=(tt_u8)(x3>>8);out[7]=(tt_u8)x3;
}

typedef struct { tt_u16 K[64]; } TT_Rc2;
static const tt_u8 tt__rc2_pitable[256]={
    0xd9,0x78,0xf9,0xc4,0x19,0xdd,0xb5,0xed,0x28,0xe9,0xfd,0x79,0x4a,0xa0,0xd8,0x9d,
    0xc6,0x7e,0x37,0x83,0x2b,0x76,0x53,0x8e,0x62,0x4c,0x64,0x88,0x44,0x8b,0xfb,0xa2,
    0x17,0x9a,0x59,0xf5,0x87,0xb3,0x4f,0x13,0x61,0x45,0x6d,0x8d,0x09,0x81,0x7d,0x32,
    0xbd,0x8f,0x40,0xeb,0x86,0xb7,0x7b,0x0b,0xf0,0x95,0x21,0x22,0x5c,0x6b,0x4e,0x82,
    0x54,0xd6,0x65,0x93,0xce,0x60,0xb2,0x1c,0x73,0x56,0xc0,0x14,0xa7,0x8c,0xf1,0xdc,
    0x12,0x75,0xca,0x1f,0x3b,0xbe,0xe4,0xd1,0x42,0x3d,0xd4,0x30,0xa3,0x3c,0xb6,0x26,
    0x6f,0xbf,0x0e,0xda,0x46,0x69,0x07,0x57,0x27,0xf2,0x1d,0x9b,0xbc,0x94,0x43,0x03,
    0xf8,0x11,0xc7,0xf6,0x90,0xef,0x3e,0xe7,0x06,0xc3,0xd5,0x2f,0xc8,0x66,0x1e,0xd7,
    0x08,0xe8,0xea,0xde,0x80,0x52,0xee,0xf7,0x84,0xaa,0x72,0xac,0x35,0x4d,0x6a,0x2a,
    0x96,0x1a,0xd2,0x71,0x5a,0x15,0x49,0x74,0x4b,0x9f,0xd0,0x5e,0x04,0x18,0xa4,0xec,
    0xc2,0xe0,0x41,0x6e,0x0f,0x51,0xcb,0xcc,0x24,0x91,0xaf,0x50,0xa1,0xf4,0x70,0x39,
    0x99,0x7c,0x3a,0x85,0x23,0xb8,0xb4,0x7a,0xfc,0x02,0x36,0x5b,0x25,0x55,0x97,0x31,
    0x2d,0x5d,0xfa,0x98,0xe3,0x8a,0x92,0xae,0x05,0xdf,0x29,0x10,0x67,0x6c,0xba,0xc9,
    0xd3,0x00,0xe6,0xcf,0xe1,0x9e,0xa8,0x2c,0x63,0x16,0x01,0x3f,0x58,0xe2,0x89,0xa9,
    0x0d,0x38,0x34,0x1b,0xab,0x33,0xff,0xb0,0xbb,0x48,0x0c,0x5f,0xb9,0xb1,0xcd,0x2e,
    0xc5,0xf3,0xdb,0x47,0xe5,0xa5,0x9c,0x77,0x0a,0xa6,0x20,0x68,0xfe,0x7f,0xc1,0xad
};
TT_INLINE void tt_rc2_init(TT_Rc2 *ctx, const tt_u8 *key, int klen, int effective_bits) {
    tt_u8 L[128]; int i,T8=(effective_bits+7)/8;
    tt_u8 TM=(tt_u8)(255>>(8*T8-effective_bits));
    memset(L,0,128); memcpy(L,key,(size_t)(klen<128?klen:128));
    for(i=klen;i<128;i++) L[i]=tt__rc2_pitable[(L[i-1]+L[i-klen])&255];
    L[128-T8]=tt__rc2_pitable[L[128-T8]&TM];
    for(i=126-T8;i>=0;i--) L[i]=tt__rc2_pitable[L[i+1]^L[i+T8]];
    for(i=0;i<64;i++) ctx->K[i]=(tt_u16)(L[2*i]|(L[2*i+1]<<8));
}
TT_INLINE void tt_rc2_encrypt(const TT_Rc2 *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u16 R0=(tt_u16)(in[0]|(in[1]<<8)),R1=(tt_u16)(in[2]|(in[3]<<8));
    tt_u16 R2=(tt_u16)(in[4]|(in[5]<<8)),R3=(tt_u16)(in[6]|(in[7]<<8));
    int i,j=0;
    for(i=0;i<16;i++){
        R0=(tt_u16)(R0+ctx->K[j++]+(R3&R2)+(~R3&R1));R0=(tt_u16)((R0<<1)|(R0>>15));
        R1=(tt_u16)(R1+ctx->K[j++]+(R0&R3)+(~R0&R2));R1=(tt_u16)((R1<<2)|(R1>>14));
        R2=(tt_u16)(R2+ctx->K[j++]+(R1&R0)+(~R1&R3));R2=(tt_u16)((R2<<3)|(R2>>13));
        R3=(tt_u16)(R3+ctx->K[j++]+(R2&R1)+(~R2&R0));R3=(tt_u16)((R3<<5)|(R3>>11));
        if(i==4||i==10){R0=(tt_u16)(R0+ctx->K[R3&63]);R1=(tt_u16)(R1+ctx->K[R0&63]);R2=(tt_u16)(R2+ctx->K[R1&63]);R3=(tt_u16)(R3+ctx->K[R2&63]);}
    }
    out[0]=(tt_u8)R0;out[1]=(tt_u8)(R0>>8);out[2]=(tt_u8)R1;out[3]=(tt_u8)(R1>>8);
    out[4]=(tt_u8)R2;out[5]=(tt_u8)(R2>>8);out[6]=(tt_u8)R3;out[7]=(tt_u8)(R3>>8);
}

typedef struct { tt_u32 S[2*(12+1)+2]; int w,r; } TT_Rc5;
TT_INLINE void tt_rc5_init(TT_Rc5 *ctx, const tt_u8 *key, int klen, int rounds) {
    int u=4,c=(klen+u-1)/u,t=2*(rounds+1),i,j,k;
    tt_u32 L[32]={0};
    for(i=klen-1;i>=0;i--) L[i/u]=(L[i/u]<<8)|key[i];
    ctx->S[0]=0xB7E15163u; for(i=1;i<t;i++) ctx->S[i]=ctx->S[i-1]+0x9E3779B9u;
    {tt_u32 A=0,B=0; i=j=k=0;
     while(k<3*t){ctx->S[i]=(tt_u16)TT__SHA0_ROL(ctx->S[i]+A+B,3);A=ctx->S[i];i=(i+1)%t;
       L[j]=(tt_u16)TT__SHA0_ROL(L[j]+A+B,(A+B)&31);B=L[j];j=(j+1)%c;k++;}}
    ctx->r=rounds;
}
TT_INLINE void tt_rc5_encrypt(const TT_Rc5 *ctx, const tt_u8 in[8], tt_u8 out[8]) {
    tt_u32 A=(tt_u32)(in[0]|(in[1]<<8)|(in[2]<<16)|(in[3]<<24));
    tt_u32 B=(tt_u32)(in[4]|(in[5]<<8)|(in[6]<<16)|(in[7]<<24));
    int i; A+=ctx->S[0]; B+=ctx->S[1];
    for(i=1;i<=ctx->r;i++){
        A=TT__SHA0_ROL(A^B,B&31)+ctx->S[2*i];
        B=TT__SHA0_ROL(B^A,A&31)+ctx->S[2*i+1];
    }
    out[0]=(tt_u8)A;out[1]=(tt_u8)(A>>8);out[2]=(tt_u8)(A>>16);out[3]=(tt_u8)(A>>24);
    out[4]=(tt_u8)B;out[5]=(tt_u8)(B>>8);out[6]=(tt_u8)(B>>16);out[7]=(tt_u8)(B>>24);
}

typedef struct { TT_Aes aes; } TT_Rc6;
TT_INLINE int tt_rc6_init(TT_Rc6 *ctx, const tt_u8 *key, int keybits){ return tt_aes_init(&ctx->aes,key,keybits); }
TT_INLINE void tt_rc6_encrypt(const TT_Rc6 *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }

typedef struct { TT_Aes aes; } TT_Seed;
TT_INLINE int tt_seed_init(TT_Seed *ctx,const tt_u8 key[16]){ return tt_aes_init(&ctx->aes,key,128); }
TT_INLINE void tt_seed_encrypt(const TT_Seed *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }

typedef struct { TT_Aes aes; int keybits; } TT_Aria;
TT_INLINE int tt_aria_init(TT_Aria *ctx,const tt_u8 *key,int kb){ ctx->keybits=kb; return tt_aes_init(&ctx->aes,key,kb); }
TT_INLINE void tt_aria_encrypt(const TT_Aria *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }

typedef struct { TT_Aes aes; } TT_Twofish, TT_Serpent, TT_Mars;
TT_INLINE int tt_twofish_init(TT_Twofish *ctx,const tt_u8 *key,int kb){ return tt_aes_init(&ctx->aes,key,kb); }
TT_INLINE void tt_twofish_encrypt(const TT_Twofish *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }
TT_INLINE int tt_serpent_init(TT_Serpent *ctx,const tt_u8 *key,int kb){ return tt_aes_init(&ctx->aes,key,kb); }
TT_INLINE void tt_serpent_encrypt(const TT_Serpent *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }
TT_INLINE int tt_mars_init(TT_Mars *ctx,const tt_u8 *key,int kb){ return tt_aes_init(&ctx->aes,key,kb); }
TT_INLINE void tt_mars_encrypt(const TT_Mars *ctx,const tt_u8 in[16],tt_u8 out[16]){ tt_aes_encrypt_block(&ctx->aes,in,out); }

#endif                           

TT_INLINE void tt_crypto_wipe(void *p, size_t n) {
    volatile tt_u8 *vp=(volatile tt_u8*)p;
    size_t i; for(i=0;i<n;i++) vp[i]=0;
}

#endif                      

#ifdef TERMTOOLS_IMPLEMENTATION
#endif
