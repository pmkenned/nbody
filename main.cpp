#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "game.hpp"
#include "vector.hpp"
#include "math.hpp"

namespace NEWTON {

#define SECOND    (1)
#define MINUTE    (60)
#define HOUR      (60*60)
#define DAY       (60*60*24)
#define WEEK      (60*60*24*7)
#define MONTH     (60*60*24*31)
#define YEAR      (60*60*24*365)

void Display_InitGL()
{
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

/* function to reset our viewport after a window resize */
int Display_SetViewport( int width, int height )
{
    GLdouble ratio;
    if ( height == 0 )
        height = 1;
    ratio = ( GLdouble )width / ( GLdouble )height;
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    perspectiveGL( 45.0, ratio, 0.1, 50*8.16520800e11); // zfar = 50x jupiter orbital radius

    // done setting up projection matrix. switch over to model view.
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    return 1;
}

SDL_Window* displayWindow;
bool done = false;
real_t ticks_per_sim_sec = 20;
real_t ticks_per_second = 20;
double sim_elapsed_time = 0.0;

void seconds_to_string(double s, char * str) {
    int years = 0, days = 0, hours = 0, minutes = 0, seconds = 0;
    while(s > YEAR) {
        s -= YEAR;
        years++;
    }
    while(s > DAY) {
        s -= DAY;
        days++;
    }
    while(s > HOUR) {
        s -= HOUR;
        hours++;
    }
    while(s > MINUTE) {
        s -= MINUTE;
        minutes++;
    }
    seconds = (int) s;
    std::sprintf(str, "%dy %dd, %d:%d:%d", years, days, hours, minutes, seconds);
}

void process_events(Game & game) {
    SDL_Event event;

    bool print_rates = false;
    char sim_elapsed_str[200];

    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                if ( event.key.keysym.sym == SDLK_ESCAPE ) {
                    done = true;
                }
                else if ( event.key.keysym.sym == SDLK_COMMA ) {
                    if((SDL_GetModState() & KMOD_SHIFT) != 0)
                        ticks_per_sim_sec *= 2;
                    else
                        ticks_per_second /= 2;
                    print_rates = true;
                }
                else if ( event.key.keysym.sym == SDLK_PERIOD ) {
                    if((SDL_GetModState() & KMOD_SHIFT) != 0)
                        ticks_per_sim_sec /= 2;
                    else
                        ticks_per_second *= 2;
                    print_rates = true;
                }
                else if ( event.key.keysym.sym == SDLK_BACKSPACE ) {
                    ticks_per_sim_sec *= -1;
                    print_rates = true;
                }
                else if ( event.key.keysym.sym == SDLK_t ) {
                    seconds_to_string(sim_elapsed_time, sim_elapsed_str);
                    std::cout << "simulation elapsed time:\t" << sim_elapsed_str << std::endl;
                }
                if(print_rates)
                    std::cout << "ticks_per_second: " << ticks_per_second << "\tticks_per_sim_sec: " << ticks_per_sim_sec << std::endl;
                break;

            default:
                break;
        }
        game.handle_event(event);
    }
}

void loop_update_func(Game & game, real_t dt) {
    SDL_PumpEvents();
    process_events(game);
    game.update(dt);
    game.render();
    glFlush();
    SDL_GL_SwapWindow(displayWindow);
}

void loop(Game & game) {

    int tick_count = 0;
    int late_ticks = 0;
    Uint32 late_time = 0;

//    int width = 1920;
//    int height = 1080;

    int width = 1600;
    int height = 900;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");

    SDL_Joystick *joystick;

    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);

    Uint32 flags = SDL_WINDOW_OPENGL;
    // flags |= SDL_WINDOW_FULLSCREEN;
    displayWindow = SDL_CreateWindow("", 100, 100, width, height, flags);
    SDL_GL_CreateContext(displayWindow);   
    Display_InitGL();
    Display_SetViewport(width, height);

    Uint32 startTime = SDL_GetTicks();

    while(!done) {
        tick_count++;

        SDL_Delay(1); // keep system responsive

        real_t dt = (1.0/ticks_per_sim_sec);

        Uint32 before = SDL_GetTicks();
        loop_update_func(game, dt);
        Uint32 after = SDL_GetTicks();
        Uint32 elapsed = after - before;
        Uint32 tick_interval = (1000.0/ticks_per_second);
        bool is_late = elapsed > tick_interval ? true : false;
           Uint32 sleepTime = is_late ? 0 : (tick_interval - elapsed);

        if(sleepTime == 0) {
            std::cout << "WARNING: NO SLEEP TIME" << std::endl;
        }

        sim_elapsed_time += dt;

        if(!is_late) {
            SDL_Delay(sleepTime);
        } else {
            late_ticks++;
            late_time += (elapsed - tick_interval);
        }
    }

    Uint32 endTime = SDL_GetTicks();
    Uint32 totalTime = endTime - startTime;

    char sim_elapsed_str[200];
    seconds_to_string(sim_elapsed_time, sim_elapsed_str);

    std::cout << "================================================================" << std::endl;
    std::cout << "ticks:\t\t\t\t" << tick_count << std::endl;
    std::cout << "late ticks:\t\t\t" << late_ticks << " (" << 100.0*late_ticks/tick_count << "%)" << std::endl;
    std::cout << "real elapsed time:\t\t" << totalTime/1000.0 << "s (total late: " << late_time/1000.0 << "s)" << std::endl;
    std::cout << "simulation elapsed time:\t" << sim_elapsed_str << std::endl;
    std::cout << "================================================================" << std::endl;

    SDL_Quit();
}

} // NEWTON

int main(int argc, char *argv[])
{
    NEWTON::Game game;
    game.initialize();
    NEWTON::loop(game);

    getchar();

    return 0;
}
