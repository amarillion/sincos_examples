/*
    CIRCLE 5
    Written by Amarillion (amarillion@yahoo.com)

    This example shows how you can make an object (a point in this case)
    move around in a circle. You could use this to simulate the orbit
    of a planet for example.
*/

#include <allegro5/allegro.h>
#include <stdio.h>

#define SCREEN_W al_get_display_width(al_get_current_display())
#define SCREEN_H al_get_display_height(al_get_current_display())

void orbit ()
{
    int x = 0, y = 0;

    al_fixed angle = al_itofix (0);
    al_fixed angle_stepsize = al_itofix (1);

    // These determine the radius of the orbit.
    // See what happens if you change length_x to 200 :)
    int length_x = 100;
    int length_y = 100;

    // repeat this until a key is pressed
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_start_timer(timer);

    al_register_event_source(queue, al_get_timer_event_source(timer)); 
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
        // clear screen
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // calculate the new position
        x = length_x * al_fixcos (angle);
        y = length_y * al_fixsin (angle);

        // draw the point in the new position
        al_put_pixel (
            al_fixtoi(x) + SCREEN_W / 2, al_fixtoi(y) + SCREEN_H / 2,
            al_map_rgb (255, 255, 255));

        // increment the angle so that the point moves around in circles
        angle += angle_stepsize;

        // make sure angle is in range
        angle &= 0xFFFFFF;

        al_flip_display();
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

int main(int argc, char **argv) {
    int i;

    (void)argc;
    (void)argv;

    if (!init()) return -1;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // call the example function
    orbit();

    return 0;
}
