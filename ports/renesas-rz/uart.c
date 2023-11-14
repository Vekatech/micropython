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

#include <unistd.h>
#include "py/mpconfig.h"
#include "py/mperrno.h"
#include "py/runtime.h"
#include "hal_data.h"
#include "uart.h"



#define TX_BUF_SIZE     BUF_SIZE
#define RX_BUF_SIZE     BUF_SIZE

/*
 * Core UART functions to implement for a port
 */

volatile uint8_t RXcnt, TXcnt = 0;
volatile uint8_t RXidx, TXidx = 0;
volatile uint8_t GETidx, PUTidx = 0;
volatile uint8_t OFlow = 0, TXloc = 0;


static volatile eUARTstate_t _uart_state = STATE_UART_IDLE;

static uint8_t xSerialPortInitialized = 0;

uint8_t RX[RX_BUF_SIZE];
// uint8_t TX[TX_BUF_SIZE];

static void uart_callback(uart_callback_args_t *const p_arg) {
    switch (p_arg->event)
    {
        case UART_EVENT_RX_CHAR:
            if (RXcnt < (RX_BUF_SIZE - 1)) {
                RX[RXidx++] = (uint8_t)p_arg->data;
                RXcnt++;
            } else {
                OFlow = (uint8_t)p_arg->data;
                OFlow = 1;
            }
            break;

        case UART_EVENT_TX_DATA_EMPTY:
            TXloc = 1;
            break;

        case UART_EVENT_TX_COMPLETE:
        case UART_EVENT_RX_COMPLETE:
        case UART_EVENT_ERR_PARITY:
        case UART_EVENT_ERR_FRAMING:
        case UART_EVENT_ERR_OVERFLOW:
        case UART_EVENT_BREAK_DETECT:
        default:
            __NOP();
            break;
    }
}

static void mp_hal_stdio_init(void) {
    if (!xSerialPortInitialized) {
        RXcnt = TXcnt = 0;
        RXidx = TXidx = 0;
        GETidx = PUTidx = 0;
        TXloc = 0;
        R_SCIF_UART_Open(&g_uart_ctrl, &g_uart_cfg);
        R_SCIF_UART_CallbackSet(&g_uart_ctrl, uart_callback, NULL, NULL);
        xSerialPortInitialized = 1;
    }
}

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    mp_hal_stdio_init();

    if (OFlow) {
        OFlow = 0;
        // mp_raise_OSError(MP_ENOBUFS);  //RX //mp_raise_ValueError("Value is too large");
        return 0;
    }

    if (RXcnt) {
        RXcnt--;
        return RX[GETidx++];
    } else {
        return 0;
    }
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    TXloc = 0;

    mp_hal_stdio_init();

    R_SCIF_UART_Write(&g_uart_ctrl, (uint8_t *)str, len);

    while (TXloc == 0) {
        __WFI();
    }
}
