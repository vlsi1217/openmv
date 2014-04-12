#include <libmp.h>
#include "xalloc.h"
#include "imlib.h"
#include "mdefs.h"
#define R8(p) \
    (uint8_t)((p>>11) * 255/31)
#define G8(p) \
    (uint8_t)(((p>>5)&0x3F)* 255/63)
#define B8(p) \
    (uint8_t)((p&0x1F) * 255/31)
#define SWAP(x)\
   ({ uint16_t _x = (x); \
    (((_x & 0xff)<<8 |(_x & 0xff00) >> 8));})
static int isspace(int c)
{
    return (c=='\n'||c=='\r'||c=='\t'||c==' ');
}

static int f_getc(FIL *fp)
{
    int c;
    UINT bytes;
    FRESULT res = f_read(fp, &c, 1, &bytes);
    if (res != FR_OK || bytes != 1) {
        c = EOF;
    }
    return c;
}

static DISABLE_OPT int read_num(FIL *fp)
{
    int x=0, c=0;
    while (1) {
        c = f_getc(fp);
        if (c == '#') {
            /* skip comments */
            while ((c = f_getc(fp)) != EOF && c != '\n');
        } else if (c >= 48 && c <= 57) {
            x *= 10;
            x += (c-48);
        } else {
            break;
        }
    }
    return x;
}

int ppm_write(image_t *img, const char *path)
{
    FIL fp;
    UINT bytes;
    FRESULT res=FR_OK;

    res = f_open(&fp, path, FA_WRITE|FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        return res;
    }

    if (img->bpp == 1) {
        bytes = f_printf(&fp, "P5\n%d %d\n255\n", img->w, img->h);
    } else {
        bytes = f_printf(&fp, "P6\n%d %d\n255\n", img->w, img->h);
    }

    if (bytes == -1) {
        res = FR_DENIED;
        goto error;
    }

    if (img->bpp == 1) {
        res = f_write(&fp, img->data, img->w*img->h, &bytes);
    } else {
        uint8_t rgb[3];
        uint16_t *pixels = (uint16_t*)img->data;
        for (int j=0; j<img->h; j++) {
            for (int i=0; i<img->w; i++) {
                uint16_t c = SWAP(pixels[j*img->w+i]);
                rgb[0] = R8(c); rgb[1] = G8(c); rgb[2] = B8(c);
                res = f_write(&fp, rgb, 3, &bytes);
                if (res != FR_OK || bytes != 3) {
                    goto error;
                }
            }
        }

    }

error:
    f_close(&fp);
    return res;
}

int ppm_read(image_t *img, const char *path)
{
    FIL fp;
    UINT bytes;
    FRESULT res=FR_OK;

    res = f_open(&fp, path, FA_READ|FA_OPEN_EXISTING);
    if (res != FR_OK) {
        return res;
    }

    int type;
    /* read image header */
    if (f_getc(&fp) != 'P' ||
       ((type = f_getc(&fp)) != '5' && type != '6')
       || !isspace(f_getc(&fp))) {
        printf("ppm:image format not supported\n");
        res = -1;
        goto error;
    }

    if ((img->w = read_num(&fp)) == -1 || /* read image width */
        (img->h = read_num(&fp)) == -1 || /* read image height */
         read_num(&fp) != 255) {          /* read image max gray */
        printf("ppm:image format not supported\n");
        res = -1;
        goto error;
    }

    img->bpp = (type=='5')? 1:2;
    int size = img->w*img->h*img->bpp;

    /* alloc image */
    img->data = xalloc(size);
    if (img->data == NULL) {
        printf("ppm: out of memory\n");
        res = -1;
        goto error;
    }

    /* read image data */
    res = f_read(&fp, img->data, size, &bytes);
    if (res != FR_OK || bytes != size) {
        goto error;
    }

error:
    f_close(&fp);
    return res;
}
