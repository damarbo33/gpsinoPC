#ifndef SCRAPPER_H
#define SCRAPPER_H

#include "Menuobject.h"
#include "../gpx/gpxloader.h"
#include "../../../BmpRLE/image565.h"

struct t_downMapData{
    string fileGpx;
    string dirImgDown;
    string server;
    int nTiles;
    bool calcNTiles;
};

class Scrapper
{
    public:
        Scrapper();
        virtual ~Scrapper();
        void setMapData(t_downMapData var){in = var;}
        t_downMapData getDownMapData(){return in;}
        void setObjectsMenu(tmenu_gestor_objects *var){ObjectsMenu = var;}

        uint32_t getLyricsSong();
        uint32_t downImgRoute();

    protected:

    private:
        tmenu_gestor_objects *ObjectsMenu;
        t_downMapData in;
};

#endif // SCRAPPER_H
