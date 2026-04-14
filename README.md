# termtools.h

A single-header portable terminal and tool toolkit for C.

It is a very small file that does a surprising amount of things. We tried to keep it simple, and compatible with almost everything, including some systems that are older than most of our developers.

## Overview

termtools.h is a C89/C90 compatible header-only library (with minor C99 fallbacks for standard integers) that provides utilities for:

- Terminal manipulation (cursor control, colors, clearing, resizing)
- Basic graphics and UI elements (boxes, progress bars, menus, consoles)
- Time and clock management
- Input handling (keyboard polling, raw mode)
- Simple game-dev helpers (physics bodies, collision, sprite drawing, animation)
- Data structures (dynamic arrays, hash tables, linked lists, ring buffers, arenas)
- File and path utilities
- String manipulation
- Math and interpolation helpers
- Audio (WAV generation/reading)
- Image handling (PNM reading/writing, ASCII conversion)
- Encoding (Base64, CRC32, Hex dump)
- And various other tiny tools like INI parsing, CSV parsing, and benchmarking.

It targets a wide range of platforms, including Windows (95 through 11), Linux, macOS, BSDs, DOS, Haiku, and generally any POSIX.1 system.

## Usage

This is a single-header library. To use it, you must define the implementation macro in exactly one source file before including the header.

In your implementation file (e.g., main.c):

```c
#define TERMTOOLS_IMPLEMENTATION
#include "termtools.h"
```

In all other files where you need the declarations:

```c
#include "termtools.h"
```

## Features

Here is a non-exhaustive list of the simple tools included:

### Terminal Control
- `tt_goto`, `tt_clear`, `tt_cls`: Cursor movement and screen clearing.
- `tt_hide_cursor`, `tt_show_cursor`: Cursor visibility.
- `tt_term_size`: Get terminal dimensions.
- `tt_ansi_supported`: Check for ANSI support.
- Color macros and functions (`TT_BOLD`, `tt_fg`, `tt_rgb_fg`, etc.).

### UI Components
- `tt_box`: Draw boxes with various styles (single, double, unicode, round).
- `tt_progress`: Simple progress bar.
- `tt_menu`: Navigable text menu.
- `tt_console`: Scrollable log/console window.
- `tt_spinner`: Loading spinners.
- `tt_table`: Basic text table rendering.
- `tt_modal`: Simple modal dialog box.

### Input
- `tt_kbhit`, `tt_getch`, `tt_waitkey`: Keyboard input.
- `tt_raw_mode_enter`, `tt_raw_mode_leave`: Raw terminal mode (POSIX).

### Time
- `tt_time`: High-resolution timer.
- `tt_sleep_ms`, `tt_sleep_sec`: Sleep functions.
- `TT_Clock`: Frame-rate limiting and delta time calculation.

### Graphics & Game Helpers
- `tt_draw`, `tt_drawf`: Text drawing at coordinates.
- `tt_sprite`, `tt_anim_step`: Sprite and animation handling.
- `TT_Body`: Simple physics body integration.
- `tt_rect_collide`: AABB collision detection.
- `tt_ray_cast`, `tt_fov`, `tt_astar`: Raycasting, field of view, and pathfinding.
- `tt_mandelbrot`: Because we could.

### Data Structures
- `TT_DArr`: Dynamic array.
- `TT_HTable`: Hash table.
- `TT_ListNode`: Linked list.
- `TT_RingBuf`: Ring buffer.
- `TT_Arena`: Linear memory arena allocator.

### Utilities
- String helpers: `tt_strdup`, `tt_strtrim`, `tt_strreplace`, `tt_strsplit`.
- File I/O: `tt_file_read_all`, `tt_file_write_all`, `tt_list_dir`.
- Math: `tt_lerp`, `tt_ease_in_out`, `tt_dist2d`.
- Encoding: `tt_base64_encode`, `tt_crc32`, `tt_hexdump`.
- Parsing: `tt_ini_parse`, `tt_csv_parse`.
- Audio: `tt_wav_write`, `tt_wav_read`, `tt_wav_generate_sine`.
- Image: `tt_image_read_pnm`, `tt_image_to_ascii`.

## Example

```c
#define TERMTOOLS_IMPLEMENTATION
#include "termtools.h"
#include <stdio.h>

int main(void) {
    TT_Size sz = tt_term_size();
    
    tt_clear();
    tt_draw(1, 1, "Hello from termtools.h");
    tt_drawf(1, 2, "Terminal size: %dx%d", sz.w, sz.h);
    
    tt_progress(1, 4, 40, 75.0, 100.0, '#', '.');
    
    tt_draw(1, 6, "Press any key to exit...");
    tt_waitkey();
    
    return 0;
}
```

## Compatibility

We have tested or targeted:
- Windows 95/98/ME/NT/2000/XP/Vista/7/8/10/11
- Linux 2.0+
- macOS 10.0+
- BSD variants
- Solaris, IRIX, AIX
- DOS (DJGPP)
- Haiku
- Plan 9 (limited)
- Any POSIX.1 system

It aims for C89/C90 compatibility. If your compiler is truly ancient, it falls back to manual typedefs for integers.

## License

MIT License

Copyright (c) 2026 TERMTOOLS Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See the LICENSE file in the root directory for details.

## Notes

This is just a simple collection of tools we found useful. It is not perfect, but it works for us. If it works for you too, that is great. If not, well, it is just one header file.

Thanks for using our software.

- TERMTOOLS Contributors
