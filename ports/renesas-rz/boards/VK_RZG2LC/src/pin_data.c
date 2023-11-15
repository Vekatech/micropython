#include "bsp_api.h"
#include "r_ioport_api.h"

const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
{
    // {
    //  .pin = BSP_IO_PORT_05_PIN_01,
    //  .pin_cfg = ((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW)
    // },
    // {
    //  .pin = BSP_IO_PORT_00_PIN_00,
    //  .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_MODE5)
    // },
    // {
    //  .pin = BSP_IO_PORT_00_PIN_01,
    //  .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN | (uint32_t) IOPORT_PERIPHERAL_MODE5)
    // },

    {
        .pin = BSP_IO_PORT_40_PIN_00,
        .pin_cfg = ((uint32_t)IOPORT_CFG_PERIPHERAL_PIN | (uint32_t)IOPORT_PERIPHERAL_MODE1)
    },
    {
        .pin = BSP_IO_PORT_40_PIN_01,
        .pin_cfg = ((uint32_t)IOPORT_CFG_PERIPHERAL_PIN | (uint32_t)IOPORT_PERIPHERAL_MODE1)
    }
};

const ioport_cfg_t g_bsp_pin_cfg =
{
    .number_of_pins = sizeof(g_bsp_pin_cfg_data) / sizeof(ioport_pin_cfg_t),
    .p_pin_cfg_data = &g_bsp_pin_cfg_data[0],
};
