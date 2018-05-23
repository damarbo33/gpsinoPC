#include "scrapper.h"

/**
*
*/
Scrapper::Scrapper(){
    ObjectsMenu = NULL;
}

/**
*
*/
Scrapper::~Scrapper(){
}

/**
*
*/
uint32_t Scrapper::downImgRoute(){
    GpxLoader loader;
    Dirutil dir;
    string dataGPS;
    GeoStructs geoStruct;
    int xtile = 0, ytile = 0;
    Image565 imagen;
    string tmpTile;
    std::map<string, int> mTiles;
    UIProgressBar *progressBar = NULL;

    if (!loader.loadGPX(in.fileGpx))
        return 0;

    if (!in.calcNTiles){
        progressBar = (UIProgressBar *)ObjectsMenu->getObjByName("progDownload");
    }

    in.nTiles = 0;
    UICheck *tmpCheck;
    int zoom;

    for (int j=0; j < sizeof(googleZoom)/sizeof(int); j++){
        zoom = googleZoom[j];
        tmpCheck = (UICheck *)ObjectsMenu->getObjByName("checkZoom" + Constant::TipoToStr(zoom));
        if (tmpCheck->isChecked()){
            //Traza::print("Downloading with zoom", zoom, W_DEBUG);
            for (int i=0; i < loader.gpxData.size(); i++){
                xtile = geoStruct.long2tilex(loader.gpxData.at(i).getLon(), zoom);
                ytile = geoStruct.lat2tiley(loader.gpxData.at(i).getLat(), zoom);
                for (int xSide = -1; xSide < 2; xSide++){
                    for (int ySide = -1; ySide < 2; ySide++){
                        tmpTile = Constant::TipoToStr(xtile + xSide) + "/" + Constant::TipoToStr(ytile + ySide);
                        if (mTiles.count(tmpTile) <= 0){
                            mTiles.insert(std::pair<string,int>(tmpTile, 1));
                            in.nTiles++;
                            if (!in.calcNTiles){
                                imagen.downloadMap(in.server + Constant::TipoToStr(zoom) + "/"
                                           + tmpTile + ".png", in.dirImgDown);
                                progressBar->setProgressPos(in.nTiles);
                            }
                            //Traza::print("Descargando tile n", in.nTiles, W_DEBUG);
                        }
                    }
                }
            }
        }
    }
    mTiles.clear();
    return 0;
}
