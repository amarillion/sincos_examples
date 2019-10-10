/*
    CIRCLE 3
    Written by Amarillion (amarillion@yahoo.com)

    This example shows a graphical plot of the sine function.
    Note that the plot will appear upside down. This is because
    the y-coordinates are upside down - that is, a higher
    y-coordinate represents a point lower down on the screen.
*/

#include <allegro5/allegro.h>
#include <stdio.h>

#define SCREEN_W al_get_display_width(al_get_current_display())
#define SCREEN_H al_get_display_height(al_get_current_display())

void draw_sine ()
{
    int length = 100;
    al_fixed x, y;
    al_fixed angle = 0;
    al_fixed angle_stepsize = al_itofix (5);

    while (al_fixtoi(angle) < 256)
    {
        // the angle is plotted along the x-axis
        x = angle;
        // the sine function is plotted along the y-axis
        y = length * al_fixsin (angle);

        al_put_pixel (
            al_fixtoi (x) * 2, al_fixtoi (y) + SCREEN_H / 2,
            al_map_rgb (255, 255, 255));

        angle += angle_stepsize;
    }
}


bool init() {
    // initialize Allegro
    if (!al_init()) {
        printf("Could not init Allegro.\n");
        return false;
    }

    // initialize keyboard
    al_install_keyboard();

    // initialize gfx mode
    ALLEGRO_DISPLAY *display = al_create_display(640, 480);
    if (!display) {
        printf("Error creating display\n");
        return false;
    }

    return true;
}

void wait_for_input() {

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source()); 
    al_register_event_source(queue, al_get_display_event_source(al_get_current_display()));
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
}

int main(int argc, char **argv) {
    int i;

    (void)argc;
    (void)argv;

    if (!init()) return -1;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // call the example function
    draw_sine();

    // display the drawing on the screen
    al_flip_display();

    // wait for user input
    wait_for_input();

    return 0;
}
