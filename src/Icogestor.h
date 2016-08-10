#ifndef ICOGESTOR_H
#define ICOGESTOR_H

#include "Dirutil.h"
#include "Fileio.h"
#include "Constant.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"


class IcoGestor{
    public :
        IcoGestor();
        ~IcoGestor();
        bool drawIcono(int, SDL_Surface *, int, int);
        bool drawIcono(int, SDL_Surface *, int, int, int, int, int);
        static float relacion(int, int, int , int );

    private:
        Fileio *fileio;
        void cargarIcono(string);
        void cargarIconos();
        string ruta;
        void setRuta(string var){ ruta = var; }
        string getRuta(){ return ruta; }


        struct my_struct_arrays {
          SDL_Surface *Icon[totalIcons];
          int numIcons;
        } arrayIcons;

        bool redimension(SDL_Surface *, int, int, int,  SDL_Surface **);


};
#endif
