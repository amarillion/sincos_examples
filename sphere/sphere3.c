/*
   SPHERE1.C
   written by Martijn van Iersel (Amarillion)
   copyright 2003
   e-mail: amarillion@yahoo.com
   
   This program shows how to make a mapped sphere rotated at different angles.
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
    get_mapped_sphere_ex() is similar to get_mapped_sphere(), but also takes
    a rotation matrix as argument to draw a rotated sphere.

    BITMAP *target = bitmap to display onto
    int cx, cy = center of the sphere
    int r = radius of the sphere
    BITMAP *map = bitmap to map onto the sphere
    MATRIX *rotmat = rotation matrix
*/
void mapped_sphere_ex (BITMAP *target, int cx, int cy, int r, BITMAP *map,
    MATRIX *rotmat)
{
    int x, y; // coordinates on the target bitmap
    int p, q; // coordinates on the source bitmap    
    for (y = -r; y < r; y++)
    {
        fixed q_cos = fixcos (- fixasin (itofix (y) / r)) * r;
        for (x = - fixtoi (q_cos) + 1; x < fixtoi(q_cos) - 1; x++)
        {
             fixed newq_cos, temp_p, temp_q; // some temporary variables
             fixed newx, newy, newz; // x, y and z after rotation             
             fixed z; // z before rotation. we don't have to calculate x and y
             
             // calculate z
             // using pythagoras and the formula for a sphere:
             // x^2 + y^2 + z^2 = r^2
             // we know x, y and r
             z = fixsqrt (r * itofix (r) -
                 x * itofix (x) - y * itofix (y));
                 
             // apply the rotation matrix to x, y, z. 
             // put the result in newx, newy, newz
             apply_matrix (rotmat, itofix(x), itofix(y), z,
                 &newx, &newy, &newz);

             //just as in sphere2.c, we need to check if q_cos is 0
             //however, q_cos depends on y, and we just calculated a new y
             //thus we have to calculate q_cos again.
             newq_cos = fixcos (temp_q) * r;
             if (newq_cos != 0)
             {
                 // it is possible to use temp_p 
                 // temp_p = fixasin (fixdiv (itofix (x), q_cos));
                 // however, I found I get less rounding errors if I use
                 // fixatan2 instead. The principle remains the same.
                 temp_p = fixatan2 (fixdiv (newx, newq_cos), fixdiv (newz, newq_cos));                 
             }
             else
                 temp_p = 0;
             temp_p &= 0xFFFFFF;

             p = fixtoi (temp_p) * (map->w-1) / 256;

             // calculate q
             temp_q = fixasin (newy / r);             
             q = fixtoi (temp_q + itofix (64)) * (map->h-1) / 128;
             
             putpixel (target, x + cx, y + cy,
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
        BITMAP *map = load_bitmap ("earth.bmp", pal);
        BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);        
        MATRIX m;
        int i;
        int xgrid = SCREEN_W / 8;
        int ygrid = SCREEN_H / 6;
        int radius = (xgrid > ygrid ? ygrid : xgrid) / 2 - 2;
        clear_bitmap (buffer);        
        for (i = 0; i < 8; i ++)
        {
            //draw rotated spheres
            
            // the first two rows are rotated around the earth's rotation axis
            get_planet_rotation_matrix (&m, i * itofix (16), 0, 0);
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, ygrid / 2, radius, map, &m);
            get_planet_rotation_matrix (&m, (8 + i) * itofix (16), 0, 0);
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, 3 * ygrid / 2, radius, map, &m);
                
            // the third and fourth rows are rotated around the x axis
            get_planet_rotation_matrix (&m, 0, i * itofix (16), 0);
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, 5 * ygrid / 2, radius, map, &m);
            get_planet_rotation_matrix (&m, 0, (8 + i) * itofix (16), 0);
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, 7 * ygrid / 2, radius, map, &m);
                
            // the 5th and 6th rows are rotated around the z axis
            get_planet_rotation_matrix (&m, 0, 0, i * itofix (16));
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, 9 * ygrid / 2, radius, map, &m);
            get_planet_rotation_matrix (&m, 0, 0, (8 + i) * itofix (16));
            mapped_sphere_ex (buffer,
                (1 + 2 * i) * xgrid / 2, 11 * ygrid / 2, radius, map, &m);
        }
        blit (buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        while (!key[KEY_ESC]) {}
        destroy_bitmap (map);
        destroy_bitmap (buffer);
    }
    return 0;

} END_OF_MAIN();
