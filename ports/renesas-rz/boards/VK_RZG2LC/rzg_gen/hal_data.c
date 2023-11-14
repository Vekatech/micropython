/* generated HAL source file - do not edit */
#include "hal_data.h"
scif_uart_instance_ctrl_t g_uart_ctrl;

scif_baud_setting_t g_uart_baud_setting =
{
/* Baud rate calculated with 0.469% error. */ .abcs = 0,
    .bgdm = 1, .cks = 0, .brr = 53, .mddr = (uint8_t)256, .brme = false
};

/** UART extended configuration for UARTonSCIF HAL driver */
const scif_uart_extended_cfg_t g_uart_cfg_extend =
{ .bri_ipl = 14, .bri_irq = SCIF1_BRK_IRQn, .clock = SCIF_UART_CLOCK_INT, .noise_cancel =
      SCIF_UART_NOISE_CANCELLATION_DISABLE,
  .p_baud_setting = &g_uart_baud_setting, .rx_fifo_trigger = SCIF_UART_RECEIVE_TRIGGER_MAX, .rts_fifo_trigger =
      SCIF_UART_RTS_TRIGGER_14,
  .uart_mode = SCIF_UART_MODE_RS232, .flow_control = SCIF_UART_FLOW_CONTROL_NONE, .driver_enable_pin =
      BSP_IO_PORT_05_PIN_01, };

/** UART interface configuration */
const uart_cfg_t g_uart_cfg =
{ .channel = 1,
  .data_bits = UART_DATA_BITS_8,
  .parity = UART_PARITY_OFF,
  .stop_bits = UART_STOP_BITS_1,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_uart_cfg_extend,
  .p_transfer_tx = NULL,
  .p_transfer_rx = NULL,
  .rxi_ipl = 14,
  .txi_ipl = 14,
  .tei_ipl = 14,
  .eri_ipl = 14,
  .rxi_irq = SCIF1_RXI_IRQn,
  .txi_irq = SCIF1_TXI_IRQn,
  .tei_irq = SCIF1_TEI_DRI_IRQn,
  .eri_irq = SCIF1_RERR_IRQn, };

/* Instance structure to use this module. */
const uart_instance_t g_uart =
{ .p_ctrl = &g_uart_ctrl, .p_cfg = &g_uart_cfg, .p_api = &g_uart_on_scif };
void g_hal_init(void) {
    g_common_init();
}
