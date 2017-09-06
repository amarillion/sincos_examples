/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program shows how to apply lighting to a sphere.
   It is a part of the pixelate article "More fun things to do with
   your pals sin & cos"
*/

#include <allegro.h>

/*
   The function init() initializes allegro and the graphics mode.
   returns 0 on success.
*/
int init()
{
    // list of color depths we are going to try:
    int color_depths[] = {32, 24, 16, 15, 0};
    int i, bpp;

    allegro_init();
    i = 0;
    // try a couple of different color depths
    // keep on trying until bpp reaches 0
    while (bpp = color_depths[i])
    {
        set_color_depth (bpp);
        if (set_gfx_mode (GFX_AUTODETECT, 640, 480, 0, 0) == 0)
            break;
    }
    // if bpp reached 0, it means we failed finding a suitable color depth
    if (bpp == 0) return -1;
    if (install_keyboard() != 0) return -1;
    if (install_timer() != 0) return -1;
    return 0;
}

/*
lit_sphere() draws a plain, solid sphere with lighting.

BITMAP *target: bitmap to draw onto
int cx, cy: center of the sphere
int r: radius of the sphere
fixed longitude, latitude: direction of the lightsource
*/
void lit_sphere (BITMAP *target, int cx, int cy, int r, fixed longitude, fixed latitude)
{
    int x, y;
    // no p and q this time, because there is no source bitmap
      
    // calculate the light vector
    fixed lightx, lighty, lightz;
    lightx = fixmul (fixsin (longitude), fixcos(latitude));
    lighty = fixsin (latitude);
    lightz = fixmul (fixcos (longitude), fixcos(latitude));
    
    for (y = -r; y < r; y++)
    {
        fixed q_cos = fixcos (- fixasin (itofix (y) / r)) * r;
        for (x = - fixtoi (q_cos) + 1; x < fixtoi(q_cos) - 1; x++)
        {
             // -64 < angle_p < 64
             fixed light; // light factor
             int lighti; // idem, but int instead of fixed
             fixed xnormal = itofix (x);
             fixed ynormal = itofix (y);
             fixed znormal = fixsqrt (r * itofix (r) -
                 x * itofix (x) - y * itofix (y));
             light = dot_product (
                 // normal of sphere surface
                 xnormal / r, ynormal / r, znormal / r,
                 // light source vector
                 lightx, lighty, lightz
             );
             // if light is negative, we are in the shadow region
             // so it should be black
             if (light < 0) light = 0;             
             lighti = fixtoi (light * 255);
             putpixel (target, x + cx, y + cy,
                 makecol (lighti, lighti, lighti));
        }
    }
}

int main(void)
{
    if (init() == 0)
    {
        PALETTE pal;
        BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);
        int i, j;
        int xgrid = SCREEN_W / 4;
        int ygrid = SCREEN_H / 3;
        int radius = (xgrid > ygrid ? ygrid : xgrid) / 2 - 2;
        clear_bitmap (buffer);        
        for (i = 0; i < 4; i ++)
            for (j = 0; j < 3; j ++)
            {
                lit_sphere (buffer,
                    (2 * i + 1) * xgrid / 2,
                    (2 * j + 1) * ygrid / 2,
                    radius, i * itofix (32), (j + 1) * itofix (16));
            }
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        while (!key[KEY_ESC]) {}
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
