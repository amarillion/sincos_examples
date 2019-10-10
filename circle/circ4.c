/*
    CIRCLE 4
    Written by Amarillion (amarillion@yahoo.com)

    This program shows how you can go about making a top-down racing car
    game with sin and cos. Move the racing car with up, down, left and right.
    Press Esc to quit.
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <stdio.h>

#define SCREEN_W al_get_display_width(al_get_current_display())
#define SCREEN_H al_get_display_height(al_get_current_display())

void racing_car ()
{
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_start_timer(timer);
    al_register_event_source(queue, al_get_timer_event_source(timer)); 
    al_register_event_source(queue, al_get_display_event_source(al_get_current_display()));

    // length and angle of the racing car's velocity vector
    al_fixed angle = al_itofix (0);
    al_fixed length = al_itofix (0);
    // x- and y-coordinates of the velocity vector
    al_fixed vel_x, vel_y;

    // x- and y-position of the racing car
    al_fixed x = al_itofix (SCREEN_W / 2);
    al_fixed y = al_itofix (SCREEN_H / 2);

    bool quit = false;
    while (!quit) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event); // Wait for and get an event.
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                break;
                // if user presses window close button
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                quit = true;
                break;

        }

        ALLEGRO_KEYBOARD_STATE kbdstate;
        al_get_keyboard_state(&kbdstate);
                
        if(al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE)) {
            quit = true;
        }
        if(al_key_down(&kbdstate, ALLEGRO_KEY_UP)) {
            if (length < al_itofix (4)) length += al_ftofix (0.01);
        }
        if(al_key_down(&kbdstate, ALLEGRO_KEY_DOWN)) {
            if (length > al_itofix (0)) length -= al_ftofix (0.01);
        }
        if(al_key_down(&kbdstate, ALLEGRO_KEY_LEFT)) {
            angle = (angle - al_itofix (1)) & 0xFFFFFF;
        }
        if(al_key_down(&kbdstate, ALLEGRO_KEY_RIGHT)) {
            angle = (angle + al_itofix (1)) & 0xFFFFFF;
        }

        // erase the old image
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // calculate the x- and y-coordinates of the velocity vector
        vel_x = al_fixmul (length, al_fixcos (angle));
        vel_y = al_fixmul (length, al_fixsin (angle));

        // move the car, and make sure it stays within the screen
        x += vel_x;
        if (x >= al_itofix (SCREEN_W)) x -= al_itofix(SCREEN_W);
        if (x < al_itofix (0)) x += al_itofix(SCREEN_W);
        y += vel_y;
        if (y >= al_itofix (SCREEN_H)) y -= al_itofix(SCREEN_H);
        if (y < al_itofix (0)) y += al_itofix(SCREEN_H);

        // draw the racing car
        al_draw_circle (al_fixtoi(x), al_fixtoi(y), 10, al_map_rgb (0, 0, 255), 1.0);
        al_draw_line (al_fixtoi(x), al_fixtoi(y),
            al_fixtoi (x + 9 * al_fixcos (angle)),
            al_fixtoi (y + 9 * al_fixsin (angle)),
            al_map_rgb (255, 0, 0), 1.0);

        // display the drawing on the screen
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

    // call the example function
    racing_car();

    return 0;
}
