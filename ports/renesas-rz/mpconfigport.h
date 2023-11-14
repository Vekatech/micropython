/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Vekatech Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Options to control how MicroPython is built for this port,
// overriding defaults in py/mpconfig.h.

// board specific definitions
#include "mpconfigboard.h"
#include "mpconfigboard_common.h"

#ifndef MICROPY_CONFIG_ROM_LEVEL
// Use the minimal starting configuration (disables all optional features).
#define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
// #define MICROPY_CONFIG_ROM_LEVEL (MICROPY_CONFIG_ROM_LEVEL_EXTRA_FEATURES)
#endif

#ifndef MICROPY_HW_ENABLE_USBDEV
#define MICROPY_HW_ENABLE_USBDEV    (0)
#endif
#ifndef MICROPY_HW_ENABLE_UART_REPL
#define MICROPY_HW_ENABLE_UART_REPL (1) // useful if there is no USB
#endif

/*
// Heap start / end definitions
#ifndef MICROPY_HEAP_START
#define MICROPY_HEAP_START &_heap_start
#endif
#ifndef MICROPY_HEAP_END
#define MICROPY_HEAP_END &_heap_end
#endif
*/

#if MICROPY_HW_ENABLE_USBDEV
// Enable USB-CDC serial port
#ifndef MICROPY_HW_USB_CDC
#define MICROPY_HW_USB_CDC      (1)
#endif
// Enable USB Mass Storage with FatFS filesystem.
#ifndef MICROPY_HW_USB_MSC
#define MICROPY_HW_USB_MSC    (0)
#endif
// RA unique ID is 16 bytes (hex string == 32)
#ifndef MICROPY_HW_USB_DESC_STR_MAX
#define MICROPY_HW_USB_DESC_STR_MAX (32)
#endif
#endif

// You can disable the built-in MicroPython compiler by setting the following
// config option to 0.  If you do this then you won't get a REPL prompt, but you
// will still be able to execute pre-compiled scripts, compiled with mpy-cross.
#define MICROPY_ENABLE_COMPILER     (1)

#define MICROPY_QSTR_EXTRA_POOL           mp_qstr_frozen_const_pool
#define MICROPY_ENABLE_GC                 (1)

// defined in mpconfig
#define MICROPY_HELPER_REPL               (1)
#define MICROPY_MODULE_FROZEN_MPY         (1)
#define MICROPY_ENABLE_EXTERNAL_IMPORT    (1)

#define MICROPY_ALLOC_PATH_MAX            (128)

// Use the minimum headroom in the chunk allocator for parse nodes.
#define MICROPY_ALLOC_PARSE_CHUNK_INIT    (16)

#define MICROPY_PY_BUILTINS_BYTEARRAY     (1)

// type definitions for the specific machine

// Assume that if we already defined the obj repr then we also defined these items
#ifndef MICROPY_OBJ_REPR
#define UINT_FMT "%u"
#define INT_FMT "%d"
typedef int mp_int_t; // must be pointer size
typedef unsigned int mp_uint_t; // must be pointer size
#endif
typedef long mp_off_t;

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

// #define MICROPY_HW_BOARD_NAME "minimal"
// #define MICROPY_HW_MCU_NAME "unknown-cpu"

// #ifdef __linux__
// #define MICROPY_MIN_USE_STDOUT (1)
// #define MICROPY_HEAP_SIZE      (25600) // heap size 25 kilobytes
// #endif

// #ifdef __thumb__
// #define MICROPY_MIN_USE_CORTEX_CPU (1)
// #define MICROPY_MIN_USE_STM32_MCU (1)
#define MICROPY_HEAP_SIZE      (2048) // heap size 2 kilobytes
// #endif

#define MP_STATE_PORT MP_STATE_VM
