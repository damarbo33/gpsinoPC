#include "main.h"
#include "Dirutil.h"
#include <iostream>
#include <fstream>

bool procesarTeclado(tEvento *evento, Iofrontend *ioFront){
    bool salir = false;
    if (evento->key == SDLK_RETURN && evento->keyMod & KMOD_LALT){
        ioFront->toggleFullScreen();
        evento->resize = true;
    }
    return salir;
}

/**
*
*/
void Terminate(void)
{
#ifdef UNIX
//    chdir("/home/pi");
//    execl("/bin/sh", "/bin/sh", NULL);
#endif
}

/**
*
*/

int main(int argc, char *argv[]){
    #ifdef WIN
        string appDir = argv[0];
        int pos = appDir.rfind(Constant::getFileSep());
        if (pos == string::npos){
            FILE_SEPARATOR = FILE_SEPARATOR_UNIX;
            pos = appDir.rfind(FILE_SEPARATOR);
            tempFileSep[0] = FILE_SEPARATOR;
        }
        appDir = appDir.substr(0, pos);
        if (appDir[appDir.length()-1] == '.'){
            appDir.substr(0, appDir.rfind(Constant::getFileSep()));
        }
        Constant::setAppDir(appDir);
    #endif // WIN

    #ifdef UNIX
        Dirutil dir;
        Constant::setAppDir(dir.getDirActual());
    #endif // UNIX

    string rutaTraza = appDir + Constant::getFileSep() + "Traza.txt";

    Traza *traza = new Traza(rutaTraza.c_str());
    Iofrontend *ioFront = new Iofrontend();
    srand(time(NULL));

    try{

        bool salir = false;
        long delay = 0;
        unsigned long before = 0;
        ioFront->setCanFlip(true);
        atexit (Terminate);

        while (!salir){

            tEvento evento = ioFront->WaitForKey();
            procesarTeclado(&evento, ioFront);

            before = SDL_GetTicks();
            salir = evento.quit;

            if (!salir){
                salir = ioFront->drawMenu(evento);

                if (LIMITFPS){
                //Calculamos el tiempo que deberia pasar de frame a frame en un lapso de 1 seg (TIMETOLIMITFRAME) y le restamos
                //el tiempo que realmente ha pasado (before - SDL_GetTicks()), para esperar el tiempo que indique
                //esta diferencia. No es el mejor método pero es sencillo de implementar
                    delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
                    if(delay > 0) SDL_Delay(delay);
                }
            }
        }
    } catch(Excepcion &e) {
        Traza::print("Error capturado en hilo principal:",e.getCode(), W_FATAL);
        //exit(e.getCode());
    }

    Traza::print("Eliminando iofront", W_DEBUG);
    delete ioFront;
    Traza::print("Eliminando traza", W_DEBUG);
    delete traza;
    Traza::print("Saliendo de la aplicación", W_DEBUG);
    exit(0);
}
