/*
    CIRCLE 2
    Written by Amarillion (amarillion@yahoo.com)

    This example is almost the same as CIRCLE 1, only this works with
    fixed-point mathematics instead of floating-point.
*/

#include <allegro5/allegro.h>
#include <stdio.h>

#define SCREEN_W al_get_display_width(al_get_current_display())
#define SCREEN_H al_get_display_height(al_get_current_display())

void draw_circle_fixed ()
{
    al_fixed x, y;
    int length = 100;
    al_fixed angle = 0;
    al_fixed angle_stepsize = al_itofix (5);

    // go through all angles from 0 to 255
    while (al_fixtoi (angle) < 256)
    {
        // calculate x, y from a vector with known length and angle
        x = length * al_fixcos (angle);
        y = length * al_fixsin (angle);

        al_put_pixel (
            al_fixtoi(x) + SCREEN_W / 2, al_fixtoi(y) + SCREEN_H / 2,
            al_map_rgb (255, 255, 255));
        angle += angle_stepsize;
    }
}


int main(int argc, char **argv)
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_BITMAP *cursor;
    ALLEGRO_MOUSE_STATE msestate;
    ALLEGRO_KEYBOARD_STATE kbdstate;
    int i;

    (void)argc;
    (void)argv;

    // initialize Allegro
    if (!al_init())
    {
        printf("Could not init Allegro.\n");
        return -1;
    }

    // initialize mouse and keyboard
    al_install_mouse();
    al_install_keyboard();

    // initialize gfx mode
    display = al_create_display(640, 480);
    if (!display)
    {
        printf("Error creating display\n");
        return -1;
    }

    al_hide_mouse_cursor(display);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // call the example function
    draw_circle_fixed();

    // display the drawing on the screen
    al_flip_display();

    // wait for user input
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source()); 
    al_register_event_source(queue, al_get_display_event_source(display));
    bool quit = false;
    while (!quit) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event); // Wait for and get an event.

        switch (event.type) {
            // if user presses window close button
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            // if user presses any key
            case ALLEGRO_EVENT_KEY_CHAR:
                quit = true;
                break;
        }
    }

    return 0;
}
