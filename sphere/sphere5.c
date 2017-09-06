/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program combines lighting and spherical mapping.
   It is a part of the pixelate article "More fun things to do with
   your pals sin & cos"
*/

#include <allegro.h>
#include <math.h>

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
get_planet_rotation_matrix() is a little helper function to calculate a 
rotation matrix for a planet. An ordinary rotation matrix doesn't work well
for planets.

MATRIX *m: pointer to the resulting MATRIX struct
fixed rotation: rotation of the planet around its axis of rotation
fixed axisx, axisz: rotation around the x and z axes.
*/
void get_planet_rotation_matrix (MATRIX *m, fixed rotation, fixed axisx, fixed axisz)
{
    MATRIX m1, m2;
    // apply rotation around y axis first
    get_y_rotate_matrix (&m1, rotation);
    // then apply rotation around x and z axes.
    get_rotation_matrix (&m2, axisx, 0, axisz);
    matrix_mul (&m2, &m1, m);
}

/*
mapped_lit_sphere() maps a bitmap onto a sphere and applies lighting at the same time
This is essentially mapped_sphere_ex() and lit_sphere() combined. Look at 
those functions for more comments.

BITMAP *target = the bitmap to draw onto
int cx, cy = center of the sphere
int r = radius of the sphere
BITMAP *map = bitmap to map onto the sphere
MATRIX *rotmat = rotation of the sphere
fixed longitude, latitude = position of the light source, as if it were right above
    that spot on the earth
*/
void mapped_lit_sphere (BITMAP *target, int cx, int cy, int r, BITMAP *map,
    MATRIX *rotmat, fixed longitude, fixed latitude)
{
    int x, y; // coordinates on target bitmap
    int p, q; // coordinates on source bitmap
    
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
             fixed light;
             int lighti, color;
             fixed temp_p, temp_q;
             fixed newx, newy, newz;
             fixed z = ftofix (sqrt((double)(r * r - x * x - y * y)));
             
             // rotate x, y and z             
             // put the result in newx, newy and newz
             apply_matrix (rotmat, itofix(x), itofix(y), z,
                  &newx, &newy, &newz);

             // see if we are near the poles
             temp_q = - fixasin (newy / r);             
             if (temp_q != 0)
             {
                // again, I chose to use fixatan2 instead of 
                // temp_p = fixasin (newx)
                // so we'll have less problems with rounding errors.
                temp_p = fixatan2 (newx, newz);                
             }
             else
                 temp_p = 0;
             temp_p &= 0xFFFFFF;

             
             q = fixtoi (-temp_q + itofix (64)) * (map->h-1) >> 7;
             
             temp_q = fixcos (temp_q) * r + itofix (1);
             
             p = fixtoi (temp_p) * (map->w-1) >> 8;

             light = dot_product (
                 // normal of sphere surface
                 itofix(x) / r, itofix(y) / r, z / r,
                 // light source vector
                 lightx, lighty, lightz
             );

             if (light < 0) light = 0;

             //lighti = fixtoi (light * 255);
             lighti = fixtoi ((light << 8) - light);
             color = getpixel (map, p, q);
             putpixel (target, x + cx, y + cy, lit_color (color, lighti));
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
        MATRIX m;
        int i, j;
        int xgrid = SCREEN_W / 4;
        int ygrid = SCREEN_H / 3;
        int radius = (xgrid > ygrid ? ygrid : xgrid) / 2 - 2;
        clear_bitmap (buffer);        
        for (i = 0; i < 4; i ++)
            for (j = 0; j < 3; j ++)
            {
                get_planet_rotation_matrix (&m, (j * 4 + i) * itofix (16), 0, 0);
                mapped_lit_sphere (buffer,
                    (2 * i + 1) * xgrid / 2,
                    (2 * j + 1) * ygrid / 2,
                    radius, map, &m, i * itofix (32), (j + 1) * itofix (16));
            }
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        while (!key[KEY_ESC]) {}
        destroy_bitmap (map);
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
