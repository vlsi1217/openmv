#include "mp.h"
#include <string.h>
#include "usbdbg.h"
#include "framebuffer.h"

#define USB_TX_BUF_SIZE (64)
static int xfer_bytes;
static int xfer_length;
static enum usbdbg_cmd cmd;

static vstr_t script;
static int script_ready=0;
mp_obj_t mp_const_ide_interrupt = MP_OBJ_NULL;

void usbdbg_init()
{
    vstr_init(&script, 64);
    mp_const_ide_interrupt = mp_obj_new_exception_msg(&mp_type_OSError, "IDEInterrupt");
}

int usbdbg_script_ready()
{
    return script_ready;
}

vstr_t *usbdbg_get_script()
{
    return &script;
}

void usbdbg_clr_script()
{
    script_ready =0;
    vstr_reset(&script);
}

void usbdbg_data_in(void *buffer, int length)
{
    switch (cmd) {
        case USBDBG_FB_SIZE:  /* dump framebuffer */
            memcpy(buffer, fb, length);
            cmd = USBDBG_NONE;
            break;
        case USBDBG_DUMP_FB:  /* dump framebuffer */
            if (xfer_bytes < xfer_length) {
                memcpy(buffer, fb->pixels+xfer_bytes, length);
                xfer_bytes += length;
            } else {
                cmd = USBDBG_NONE;
            }
            break;

        default: /* error */
            break;
    }
}

void usbdbg_data_out(void *buffer, int length)
{
    switch (cmd) {
        case USBDBG_EXEC_SCRIPT: /* execute script */
            vstr_add_strn(&script, buffer, length);
            xfer_bytes += length;
            if (xfer_bytes == xfer_length) {
                script_ready = 1;
                /* Interrupt REPL */
                mp_obj_exception_clear_traceback(mp_const_ide_interrupt);
                pendsv_nlr_jump(mp_const_ide_interrupt);
            }
            break;

        default: /* error */
            break;
    }
}

void usbdbg_control(uint8_t request, int length)
{
    cmd = (enum usbdbg_cmd) request;
    switch (cmd) {
        case USBDBG_FB_SIZE:     /* read framebuffer size */
            xfer_bytes = 0;
            xfer_length = length;
            break;

        case USBDBG_DUMP_FB:     /* dump framebuffer */
            /* reset bytes counter */
            xfer_bytes = 0;
            xfer_length = length;
            break;

        case USBDBG_EXEC_SCRIPT: /* execute script */
            /* reset bytes counter */
            xfer_bytes = 0;
            xfer_length =length;
            script_ready = 0;
            vstr_reset(&script);
            break;

        case USBDBG_STOP_SCRIPT:
            /* Stop any running code by raising an exception */
            mp_obj_exception_clear_traceback(mp_const_ide_interrupt);
            pendsv_nlr_jump(mp_const_ide_interrupt);
            cmd = USBDBG_NONE;
            break;

        default: /* error */
            cmd = USBDBG_NONE;
            break;
    }
}

