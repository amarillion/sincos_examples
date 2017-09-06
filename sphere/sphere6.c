/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program shows a day and night map of the earth.
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
    lit_color is a colordepth independant function to 
    adjust the lighting of a certain pixel
    
    color = color to adjust, in the same format as the screen
        (8, 15, 16, 24 or 32 bit)
    light = light factor from 0 to 255
    returns the adjusted color in 8, 15, 15, 24 or 32 bit format
*/
int lit_color (int color, int light)
{
    switch (bitmap_color_depth (screen))
    {
        case 8: return makecol8 (
            (getr8 (color) * light) >> 8,
            (getg8 (color) * light) >> 8,
            (getb8 (color) * light) >> 8);
        case 15: return makecol15 (
            (getr15 (color) * light) >> 8,
            (getg15 (color) * light) >> 8,
            (getb15 (color) * light) >> 8);
        case 16: return makecol16 (
            (getr16 (color) * light) >> 8,
            (getg16 (color) * light) >> 8,
            (getb16 (color) * light) >> 8);
        case 24: return makecol24 (
            (getr24 (color) * light) >> 8,
            (getg24 (color) * light) >> 8,
            (getb24 (color) * light) >> 8);
        case 32: return makecol32 (
            (getr32 (color) * light) >> 8,
            (getg32 (color) * light) >> 8,
            (getb32 (color) * light) >> 8);
    }
}

/* 
     lit_projection () draws a bitmap onto another bitmap, but lights each 
     individual pixel as if it was on a sphere with a light shining on it from
     one direction.
     
     BITMAP *target -> the bitmap to draw onto
     BITMAP *map -> the bitmap to draw
     fixed longitude, latitude -> the position of the lightsource. 
*/     
void lit_projection (BITMAP *target, BITMAP *map,
     fixed longitude, fixed latitude)
{
    int x, y; // coordinates on target bitmap
    int p, q; // coordinates on source bitmap
    fixed p_angle, q_angle; // the same, but scaled to a 256 degree circle
    // the range of p is 0..bmp->w, but the range of p_angle is always 0..255
    
    // calculate the light vector
    fixed lightx, lighty, lightz;    
    lightx = fixmul (fixsin (longitude), fixcos(latitude));
    lighty = fixsin (latitude);
    lightz = fixmul (fixcos (longitude), fixcos(latitude));
    
    for (y = 0; y < target->h; y++)
    {
        int q = y * map->h / target->h;
        p_angle = ((itofix(y) / target->h) * 128) - itofix (64);
        for (x = 0; x < target->w; x++)
        {
            int p = x * map->w / target->w;
            fixed light;
            int color, lighti;
            q_angle = (itofix (x) / target->w * 256);

            light = dot_product (
                 // normal of sphere surface
                 fixmul (fixsin (p_angle), fixcos (p_angle)),
                 fixsin (p_angle),
                 fixmul (fixcos (q_angle), fixcos (p_angle)),
                 // light source vector
                 lightx, lighty, lightz
            );
            if (light < 0) light = 0;
            lighti = fixtoi (light * 255);

            color = getpixel (map, p, q);
            putpixel (target, x, y, lit_color (color, lighti));
        }
    }
}


int main(void)
{
    if (init() == 0)
    {
        PALETTE pal;
        BITMAP *earthmap = load_bitmap ("earth.bmp", pal);
        BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);
        clear_bitmap (buffer);                
        lit_projection (buffer, earthmap, itofix (128), itofix (-20));
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        while (!key[KEY_ESC]) {}
        destroy_bitmap (earthmap);
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
