#include <sdl/sdl.h>

#ifdef WIN32
#pragma comment(lib,"sdl.lib")
#pragma comment(lib,"sdlmain.lib")
#endif

void UpdateEvents(Sint32* mousex,Sint32* mousey,char boutons[8],char key[SDLK_LAST])
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            key[event.key.keysym.sym]=1;
            break;
        case SDL_KEYUP:
            key[event.key.keysym.sym]=0;
            break;
        case SDL_MOUSEMOTION:
            *mousex=event.motion.x;
            *mousey=event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            boutons[event.button.button]=1;
            break;
        case SDL_MOUSEBUTTONUP:
            boutons[event.button.button]=0;
            break;
        }
    }
}

void SDL_PutPixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32*)p = pixel;
}

Uint32 SDL_GetPixel32(SDL_Surface *surface, int x, int y)
{
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * 4;
    return *(Uint32*)p;
}

void Mandelbrot(SDL_Surface *surface,int X,int Y,double range[])
{
    int i,j;
    unsigned char k;
    double xmin = range[0];
    double ymin = range[1];
    double xstep = (range[2]-range[0])/X;
    double ystep = (range[3]-range[1])/Y;
    double x,y,z0x,z0y,temp;
    for(i=0;i<X;i++)
        for(j=0;j<Y;j++)
        {
            z0x = xmin + xstep*i;
            z0y = ymin + ystep*j;
            x = 0.0;
            y = 0.0;
            k = 0;
            while((x*x+y*y)<4.0 && k<255)
            {
                temp=x;
                x=x*x-y*y+z0x;
                y=2*temp*y+z0y;
                k++;
            }
            if (k>10 && k<250)
                while(0);
            SDL_PutPixel32(surface,i,j,k);
        }
}

int main(int argc,char** argv)
{
    Sint32 mousex = 0;
    Sint32 mousey = 0;
    char boutons[8] = {0};
    char key[SDLK_LAST] = {0};
    SDL_Surface* screen;
    double zoomfact;
    int X,Y;
    double range[4] = {-1.0,-1.0,1.0,1.0};
    X = Y = 256;
    SDL_Init(SDL_INIT_VIDEO);
    screen=SDL_SetVideoMode(X,Y,32,SDL_SWSURFACE|SDL_DOUBLEBUF);
    SDL_ShowCursor(1);
    Mandelbrot(screen,X,Y,range);
    zoomfact = 0.9;
    while(!key[SDLK_ESCAPE])
    {
        UpdateEvents(&mousex,&mousey,boutons,key);
        if (SDL_MUSTLOCK(screen))
            SDL_LockSurface(screen);
        if (boutons[SDL_BUTTON_LEFT])
        {
            double xdrange = range[2]-range[0];
            double ydrange = range[3]-range[1];
            double dx = range[0] + mousex*xdrange/X;
            double dy = range[1] + mousey*ydrange/Y;
            xdrange*=zoomfact;
            ydrange*=zoomfact;
            range[0] = dx - xdrange/2.0;
            range[1] = dy - ydrange/2.0;
            range[2] = dx + xdrange/2.0;
            range[3] = dy + ydrange/2.0;
            boutons[SDL_BUTTON_LEFT] = 0;
            Mandelbrot(screen,X,Y,range);
        }
        if (boutons[SDL_BUTTON_RIGHT])
        {
            double xdrange = range[2]-range[0];
            double ydrange = range[3]-range[1];
            double dx = range[0] + mousex*xdrange/X;
            double dy = range[1] + mousey*ydrange/Y;
            xdrange/=zoomfact;
            ydrange/=zoomfact;
            range[0] = dx - xdrange/2.0;
            range[1] = dy - ydrange/2.0;
            range[2] = dx + xdrange/2.0;
            range[3] = dy + ydrange/2.0;
            boutons[SDL_BUTTON_RIGHT] = 0;
            Mandelbrot(screen,X,Y,range);
        }
        if (SDL_MUSTLOCK(screen))
            SDL_UnlockSurface(screen);
        SDL_Flip(screen);
    }
    return 0;
}
