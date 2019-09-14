/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <stdio.h>
#include <SDL2/SDL.h>
#include "Rasterizer.hpp"

enum Program {
    LazyFoo,
    JoshBeam,
    JoshBeamSdlTexture,
    TestRender,
    TestTexture,
};

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL init fail. %s\n", SDL_GetError());
        return false;
    }
    
    gWindow = SDL_CreateWindow("SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (gWindow == NULL)
    {
        printf("Window could not be created. %s\n", SDL_GetError());
        return false;
    }
    
    // end error checking
    
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}

bool loadMedia()
{
    gHelloWorld = SDL_LoadBMP("hello_world.bmp");
    
    if (gHelloWorld == NULL)
    {
        printf("Unable to load image hello_world.bmp. %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

void close()
{
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}

int lazyfoo()
{
    if (!init())
    {
        printf("failed to init\n");
        return 0;
    }
    
    if (!loadMedia())
    {
        printf("failed load media\n");
        return 0;
    }
    
    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(gWindow);
    
    SDL_Delay(5000);
    
    close();
    return 0;
}

// begin joshbeam
const int WIN_WIDTH = 320;
const int WIN_HEIGHT = 240;

static bool running = true;

static void HandleKeyEvent(const SDL_Event &event)
{
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            running = false;
            break;
        default:
            break;
    }
}

static void HandleEvent(const SDL_Event &event)
{
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            HandleKeyEvent(event);
            break;
        default:
            break;
    }
}

int testrenderer()
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    /* Initialize SDL. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;
    
    /* Create the window where we will draw. */
    window = SDL_CreateWindow("SDL_RenderClear",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              512, 512,
                              0);
    
    /* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    /* Select the color for drawing. It is set to red here. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    /* Clear the entire screen to our selected color. */
    SDL_RenderClear(renderer);
    
    /* Up until now everything was drawn behind the scenes.
     This will show the new, red contents of the window. */
    SDL_RenderPresent(renderer);
    
    /* Give us time to see the window. */
    SDL_Delay(5000);
    
    /* Always be sure to clean up */
    SDL_Quit();
    
    return 0;
}

void printarray(uint32_t *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%i\n", array[i]);
    }
}

int textureexample()
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    SDL_Rect r;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    
    window = SDL_CreateWindow("SDL_CreateTexture",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1024, 768,
                              SDL_WINDOW_RESIZABLE);
    
    r.w = 100;
    r.h = 50;
    r.x = 10;
    r.y = 10;
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET, 1024, 768);
    
    while (1) {
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;
//        r.x=rand()%500;
//        r.y=rand()%500;
        
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer,&r);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

void fillrect(uint32_t *px, int buffer_w, SDL_Rect rect, uint32_t color)
{
//    for (int i = 0; i < rect.w * rect.h; i++)
//    {
//        px[i] = color;
//    }
    
    // for each y pos
    //   for each x pos
    //     y * width + x
    
    // remember the px is the entire pixel buffer and rect is just the fill region
    for (int y = rect.y; y < rect.h; y++)
    {
        for (int x = rect.x; x < rect.w; x++)
        {
            px[y * buffer_w + x] = color;
        }
    }
}

// filltri
// use drawline-esque method to get boundaries
// draw lines between boundaries

int testsdltexture()
{
//    uint32_t numbers[] = {0xf,0x10000,3,4};
//    printarray(numbers, 4);
//    return 0;
    
    const int width = 640, height = 480;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { return 1; }
//    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window, &renderer)) { return 2; }
    
    window = SDL_CreateWindow("popper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    
    if (texture == NULL) { return 3; }
    
    uint32_t px[width * height]; // abgr
    
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 100;
    rect.h = 100;
    
//    for (int i = 0; i < width * height; i++) {
//            px[i] = 0xff0000ff;
////            printf("%i ", i*j);
//    }
    
    fillrect(px, width, {0,0,width, height}, 0);
    fillrect(px, width, rect, 0x000000ff);
    printf("filling rects 0x000000ff\n");
    
    SDL_UpdateTexture(texture, NULL, px, width * sizeof(px[0]));
    
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            HandleEvent(event);
        }
    }
    
//    SDL_Delay(60 * 60 * 1000);
    SDL_Quit();
    return 0;
}

void drawline(uint32_t *pixel_buffer, SDL_Point start, SDL_Point end)
{
    SDL_Point vector = {end.x - start.x, end.y - start.y};
    float slope = vector.y / vector.x;
    
    // joshbeam method
    // draw pixels from 0 to x. y pixel derived from y:x ratio. just cast the float value.
    
    for (int i = 0; i < abs(vector.x); i++) {
    }
}

int joshbeamline()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "sdlinit failed\n");
        return 1;
    }
    
    SDL_Window *win = SDL_CreateWindow("joshbeam",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (win == NULL)
    {
        printf("win not created %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Surface *surface = SDL_GetWindowSurface(win);
    printf("surfformat %u\n", surface->format->format);
    
    Rasterizer rast;
    rast.setFrameBuffer((uint32_t *)surface->pixels, WIN_WIDTH, WIN_HEIGHT);
    
    float r = 0.0f;
    unsigned int lastTicks = SDL_GetTicks();
    
    while (running)
    {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            HandleEvent(event);
        }
        
        SDL_LockSurface(surface);
        rast.clear();
        
        // calculate coordinates for triangle
        const float size = 110.0f;
        float x1 = (WIN_WIDTH / 2) + cosf(r - M_PI / 6.0) * size;
        float y1 = (WIN_HEIGHT / 2) + sinf(r - M_PI / 6.0) * size;
        float x2 = (WIN_WIDTH / 2) + cosf(r + M_PI / 2.0) * size;
        float y2 = (WIN_HEIGHT / 2) + sinf(r + M_PI / 2.0) * size;
        float x3 = (WIN_WIDTH / 2) + cosf(r + M_PI + M_PI / 6.0) * size;
        float y3 = (WIN_HEIGHT / 2) + sinf(r + M_PI + M_PI / 6.0) * size;
        
        Color col1(1.0f, 0.0f, 0.0f);
        Color col2(0.0f, 1.0f, 0.0f);
        Color col3(0.0f, 0.0f, 1.0f);

        // render
        rast.drawLine(col1, x1, y1, col2, x2, y2);
        rast.drawLine(col2, x2, y2, col3, x3, y3);
        rast.drawLine(col3, x3, y3, col1, x1, y1);
        
        SDL_UnlockSurface(surface);
        SDL_UpdateWindowSurface(win);
        
        unsigned int ticks = SDL_GetTicks();
        unsigned int ticksDiff = ticks - lastTicks;
        
        if (ticksDiff == 0) { continue; }
        
        float time = ticksDiff / 1000.0f;
        lastTicks = ticks;
        
        // rotate
        r += M_PI / 2.0f * time;

//        printf("%u\t\r", 1000.0 / ticksDiff); // print fps
    }
    
    return 0;
}

int joshbeamsdltexture()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "sdlinit failed\n");
        return 1;
    }
    
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Window *win;
//    SDL_Window *win = SDL_CreateWindow("joshbeam",
//                                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//                                       WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &win, &renderer);
    
    if (win == NULL)
    {
        printf("win not created %s\n", SDL_GetError());
        return 1;
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                                WIN_WIDTH, WIN_HEIGHT);
    uint32_t px[WIN_WIDTH * WIN_HEIGHT];
//    SDL_Surface *surface = SDL_GetWindowSurface(win);
//    
//    printf("surfformat %u\n", surface->format->format);
    
    Rasterizer rast;
    rast.setFrameBuffer(px, WIN_WIDTH, WIN_HEIGHT);
    
    float r = 0.0f;
    unsigned int lastTicks = SDL_GetTicks();
    
    while (running)
    {
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            HandleEvent(event);
        }
        
//        SDL_LockSurface(surface);
        rast.clear();
        
        // calculate coordinates for triangle
        const float size = 110.0f;
        float x1 = (WIN_WIDTH / 2) + cosf(r - M_PI / 6.0) * size;
        float y1 = (WIN_HEIGHT / 2) + sinf(r - M_PI / 6.0) * size;
        float x2 = (WIN_WIDTH / 2) + cosf(r + M_PI / 2.0) * size;
        float y2 = (WIN_HEIGHT / 2) + sinf(r + M_PI / 2.0) * size;
        float x3 = (WIN_WIDTH / 2) + cosf(r + M_PI + M_PI / 6.0) * size;
        float y3 = (WIN_HEIGHT / 2) + sinf(r + M_PI + M_PI / 6.0) * size;
        
        Color col1(1.0f, 0.0f, 0.0f);
        Color col2(0.0f, 1.0f, 0.0f);
        Color col3(0.0f, 0.0f, 1.0f);
        
        // render
        rast.drawLine(col1, x1, y1, col2, x2, y2);
        rast.drawLine(col2, x2, y2, col3, x3, y3);
        rast.drawLine(col3, x3, y3, col1, x1, y1);
        
//        SDL_UnlockSurface(surface);
//        SDL_UpdateWindowSurface(win);
        
        SDL_UpdateTexture(texture, NULL, px, WIN_WIDTH * sizeof(px[0]));
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        unsigned int ticks = SDL_GetTicks();
        unsigned int ticksDiff = ticks - lastTicks;
        
        if (ticksDiff == 0) { continue; }
        
        float time = ticksDiff / 1000.0f;
        lastTicks = ticks;
        
        // rotate
        r += M_PI / 2.0f * time;
        
        //        printf("%u\t\r", 1000.0 / ticksDiff); // print fps
    }
    
    return 0;
}

// end joshbeam
int main( int argc, char* args[] )
{
    switch (TestTexture) {
        case LazyFoo:
            return lazyfoo();
            break;
        case JoshBeam:
            return joshbeamline();
            break;
        case JoshBeamSdlTexture:
            return joshbeamsdltexture();
            break;
        case TestRender:
            return testrenderer();
            break;
        case TestTexture:
            return testsdltexture();
            break;
        default:
            break;
    }
    
    return 0;
}
