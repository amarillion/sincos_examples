/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program shows how to map a bitmap onto a cylinder.
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
   mapped_cylinder() maps a bitmap onto a cylinder.
   
   BITMAP *target = bitmap to display
   int cx = horizontal position of the center of the cylinder
   int top = position of the top of the cylinder
   int r = radius of the cylinder
   int h = height of the cylinder
   BITMAP *map = the thing that is going to be mapped onto the cylinder
*/
void mapped_cylinder (BITMAP *target, int cx, int top, int r, int h, BITMAP *map)
{    
    int x, y; // coordinates on the target bitmap 
    int p, q; // coordinates on the source bitmap
    
    // for each line:
    for (y = 0; y < h; y++)
    {
        //q is the same throughout this line. we can calculate it already
        //y has a range from 0..h, and we have to change that into a
        //range from 0..map->h
        //so we use this expression:
        q = y * map->h / h;
        
        // for each pixel in the line:
        for (x = - r; x < r; x++)
        {            
            fixed temp; // a temporary variable we use for calculating p
            
            // right now, x is between -r and r.
            // x must be between 1 and -1 before we can use asin
            // so therefor we divide by r
            temp = itofix(x) / r; 
            
            // now we can use the asin() function
            temp = fixasin (temp);
            // the result is an angle between -64 and 64.            
            
            // the next expression changes that to angle between 0..64 and 192..255,
            // which is equivalent, because 192 == -64,
            // but postive integers are a bit easier to deal with
            temp &= 0xFFFFFF;
            
            // the actual p has to cover the whole bitmap 
            // so we go from 0..256 to 0..bmp->w
            p = fixtoi (temp) * (map->w-1) / 256;
            
            // now we know p and q. copy a pixel.
            putpixel (target, x + cx, top + y,
                getpixel (map, p, q)
                );
        }
    }
}

int main(void)
{
    if (init() == 0)
    {
        PALETTE pal;
        
        //load a bitmap
        BITMAP *map = load_bitmap ("earth.bmp", pal);

        //create a buffer for double buffering
        //we use a buffer because it is a bit faster
        //to draw on an offscreen bitmap first
        BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);
        clear_bitmap (buffer);
        
        //draw the cylinder in the center of the screen 
        mapped_cylinder (buffer,
            SCREEN_W / 2, 10, 100 * SCREEN_W / 320, SCREEN_H - 20, map);

        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        
        // wait until we press ESC
        while (!key[KEY_ESC]) {}
        
        destroy_bitmap (map);        
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
