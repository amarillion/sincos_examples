/*
    CIRCLE 1
    Written by Amarillion (amarillion@yahoo.com)

    This program demonstrates the use of sin and cos with floats.
    All it does is draw a circle on the screen.
*/

#include <allegro5/allegro.h>
#include <math.h>
#include <stdio.h>

// Make sure PI is defined.
// MinGW has some problems with this.
#ifndef PI
#define PI 3.1415927
#endif

#define SCREEN_W al_get_display_width(al_get_current_display())
#define SCREEN_H al_get_display_height(al_get_current_display())

void draw_circle () {

    int x, y;
    int length = 100;
    float angle = 0.0;
    float angle_stepsize = 0.1;

    // go through all angles from 0 to 2 * PI radians
    while (angle < 2 * PI) {

        // calculate x, y from a vector with known length and angle
        x = length * cos (angle);
        y = length * sin (angle);

        al_put_pixel (
            x + SCREEN_W / 2, y + SCREEN_H / 2,
            al_map_rgb(255, 255, 255));
        
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
    draw_circle();

    // display the drawing on the screen
    al_flip_display();

    // wait for user input
    wait_for_input();

    return 0;
}
