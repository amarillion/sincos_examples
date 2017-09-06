/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program shows how to map a bitmap onto a sphere.
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
    mapped_sphere() maps a bitmap onto a sphere and displays it.
    
    BITMAP *target = bitmap to display onto
    int cx, cy = center of the sphere
    int r = radius of the sphere
    BITMAP *map = bitmap to map onto the sphere
*/
void mapped_sphere (BITMAP *target, int cx, int cy, int r, BITMAP *map)
{
    int x, y; // coordinates on the target bitmap 
    int p, q; // coordinates on the source bitmap
    
    // for each line
    for (y = -r; y < r; y++)
    {        
        fixed temp_p, temp_q;
        fixed q_cos;
        
        // y ranges from -r to r now. 
        // for asin() we need a number between -1 and 1.
        // therefor we need to divide by r
        temp_q = itofix (y) / r;
        // now we can apply the formula
        temp_q = fixasin (temp_q);        
        // we store the cosine of temp_q, because we will need it a couple of times
        // later on
        q_cos = fixcos (temp_q) * r;
        
        //we can calculate q already
        //temp_q is an angle ranging from -64 to 64
        //q must range from 0 to map->h
        //so we use the following expression:
        q = fixtoi (temp_q + itofix (64)) * (map->h-1) / 128;
        
        // instead of making x go from -r to +r, 
        // we skip the parts in the corners that lay outside the sphere
        for (x = - fixtoi (q_cos) + 1; x < fixtoi(q_cos) - 1; x++)
        {
             // at the very top of the sphere, q_cos can reach 0
             // this can create a "division by zero" error
             // so we need to check that first
             if (q_cos != 0)
                 // if not zero, we can go ahead and apply the formula
                 // to calculate p
                 temp_p = fixasin (fixdiv (itofix (x), q_cos));
             else
                 temp_p = 0;
                 
             //temp_p ranges from -64 to 64 now
             //again, we change that so we only have positive numbers
             temp_p &= 0xFFFFFF;

             // now we can finally calculate p
             // we change a range from 0..64 - 192..255 to
             // a range from 0..map->w
             p = fixtoi (temp_p) * (map->w-1) / 256;
             
             // we know p and q. copy a pixel.
             putpixel (target, x + cx, y + cy,
                 getpixel (map, p, q));
        }
    }
}

int main(void)
{
    if (init() == 0)
    {
        PALETTE pal;
        BITMAP *map = load_bitmap ("earth.bmp", pal);
        BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);
        clear_bitmap (buffer);
                
        // draw a mapped sphere on the center of the screen
        mapped_sphere (buffer,
            SCREEN_W / 2, SCREEN_H / 2, 110 * SCREEN_H / 240, map);
        
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        
        // wait until we press ESC
        while (!key[KEY_ESC]) {}
        
        destroy_bitmap (map);
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
