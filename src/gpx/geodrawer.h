#ifndef GEODRAWER_H
#define GEODRAWER_H

//static const int zoomArray[]={3,10,50,150,300,1000,2000,5000,10000};
//static const int zoomArray[]={25,50,150,300,1000,2000,5000,10000,50000};
static const int googleZoom[]={16,15,14,13,12,11,10};
//static const int googleZoom[]={16};

#include <string>
#include <vector>
#include <Constant.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include "mercatorprojection.h"
#include "GeoStructs.h"

using namespace std;



//Indica el orden de las lineas generadas en el fichero procesado
    typedef enum{LINEGPXCONFIG,LINEGPXSTATS,LINETOPOINFO,LINETRACKDATA} GPXLines;

    //Datos del fichero obtenido despues de leer el gpx
    typedef enum{GPXLAT,GPXLON,GPXALT,GPXTIME,GPXNAME} GPXOrigPoint;

    //Datos de la primera cabecera del fichero con datos procesados
    typedef enum{MAPLONLEFT,MAPLONRIGHT,MAPLONDELTA,MAPLATBOTTOM,MAPLATUP,
                 MAPLATBOTTOMDEGREE,DESPLAZAMIENTO,OFFSETX,OFFSETY,MAPNUMPOINTS} initGPXData;

    //Datos de la segunda cabecera del fichero con datos procesados
    typedef enum{STATACUMSUBIDA,STATACUMBAJADA,STATMAXALT,
    STATMINALT,STATDIST,STATPENDIENTEMEDIASUBIDA,STATPENDIENTEMEDIABAJADA,
    STATDISTLLANO,STATDISTSUBIDA,STATDISTBAJADA,STATVELMIN,
    STATVELMAX,STATVELMEDIA,STATTOTALTIME,STATMOVINGTIME,STATSTOPTIME} GPXStats;

    //Datos de la tercera cabecera del fichero con datos procesados
    typedef enum{TOPOTYPE, TOPODIST, TOPOELE, TOPOLAT, TOPOLON} TopoData;

    //Datos procesados de cada latitud y longitud
    typedef enum{PXLAT,PXLON,PXALT,PXTIME,PXANGLE,PXDISTANCIA,PXNAME, ENDPXDATA} GPXConvertedPoint;

    //Especifica si el tipo es de un valle o de una cumbre
    typedef enum{CUMBRE,VALLE} GPXTipoCumbre;

    //Especifica las coordenadas que se deben dibujar
    typedef enum{GmCN,GmN,GmS,GmE,GmW,GmNE,GmNW,GmSE,GmSW} GeoMapOrientation;

    static const int anguloFlechas = 20;
    static const int hipotenusa = 20;  //Indica lo larga que es la flecha
    static const bool drawWaypoints = true;
    static const int maxPendientePermitida = 100;  //En senderismo es posible pendientes del 100 o mayores. Pero establecemos este limite para evitar problemas con gps
    static const int limitePendienteLlano = 3;     //Establecemos que una pendiente menor al 3% es terreno llano
    static const float margenPendiente = 0.05;     //En porcentaje 0.05 = 5% para medir cambios de tendencias en ascension o descenso
    static const float limiteVelocidadMin = 0.15;     //En porcentaje 0.9 = 90% para no contar velocidades demasiado peque�as en relacion a la distancia recorrida
    static const double minDiffAltToCumbres = 20.0;



class StatsClass{
    public :
        StatsClass(){
            reset();
        }

        ~StatsClass(){};

        void reset(){
            lastLat = 0.0;
            lastLon = 0.0;
            lastAlt = 0.0;
            lastTime = 0.0;
            alturaCumbre = 0.0;
            alturaValle = 0.0;
            lastPosVallesCumbres = 0;
            altitudCumbre = 0;
            altitudValle = 0;
            isSubiendo = false;
            distValle = 0.0;
            distCumbre = 0.0;
            tempDist = 0.0;
            numCoordRuta = 1;
            maxCoordRuta = 0;
        }

        void init(double lat, double lon, double alt){
            this->lastLat = lat;
            this->lastLon = lon;
            this->lastAlt = alt;
            this->altitudCumbre = alt;     //Altitud en la que se ha detectado una cumbre
            this->altitudValle = alt;      //Altitud en la que se ha detectado un valle
        }

        //Variables para calcular la pendiente entre dos valores sucesivos
        double lastLat;
        double lastLon;
        double lastAlt;
        double lastTime;

        //Para medir las tendencias
        double alturaCumbre;    //Altitud en la que se ha detectado una cumbre
        double alturaValle;     //Altitud en la que se ha detectado una cumbre
        int lastPosVallesCumbres;
        double altitudCumbre;
        double altitudValle;
        bool isSubiendo;        //Con esta variable detectamos si ha habido un cambio de tendencia de subida o bajada
        double distValle;       //Distancia en la que se ha detectado un valle
        double distCumbre;      //Distancia en la que se ha detectado una cumbre
        double tempDist;
        int numCoordRuta;
        int maxCoordRuta;
};

class GeoDrawer
{
    public:
        GeoDrawer(int w, int h);
        virtual ~GeoDrawer();

        bool detalleDouble;

        /*Metodos estaticos*/
        static double todouble(string dato);
        static double getDistance(double latitude1, double longitude1, double latitude2, double longitude2);

        /*metodos propios*/
        void centerScreenToLocation(double latitud, double longitud);
        void convertGeoToPixel(double latitude, double longitude, Point *posicion); // in Radians
        void convertPixelToGeo(Point *posicion, VELatLong *geoPos);

        void calcLimites(std::vector<std::string> * coordinates);
        int getWidth(){return width;}
        int getHeight(){return height;}
        void zoomIn();
        void zoomOut();
        void moveRight();
        void moveLeft();
        void moveUp();
        void moveDown();
        void setPosicionCursor(double latitud, double longitud);
        void centerScreen(bool centered = true);
        double calculaEscala();
        Cartesian * convertSphericalToCartesian(VELatLong latlong);
        VELatLong * convertCartesianToSpherical(Cartesian cartesian);
//        void zoomToMeters(double meters);
        void doGoogleZoom(int zoomLevel);
        void logEstadisticasRuta();

        /*Getters y setters*/
        void setMapLonLeft(double mapLonLeft) { this->mapLonLeft = mapLonLeft; }
        double getMapLonLeft() { return this->mapLonLeft; }
        void setMapLonRight(double mapLonRight) { this->mapLonRight = mapLonRight; }
        double getMapLonRight() { return this->mapLonRight; }

        void setMapLatBottom(double mapLatBottom) { this->mapLatBottom = mapLatBottom; }
        double getMapLatBottom() { return this->mapLatBottom; }

        void setMapLatUp(double mapLatUp) { this->mapLatUp = mapLatUp; }
        double getMapLatUp() { return this->mapLatUp; }

        void setDesplazamiento(double desplazamiento) { this->desplazamiento = desplazamiento; }
        double getDesplazamiento() { return this->desplazamiento; }
        void setLatitudCursor(double latitudCursor) { this->latitudCursor = latitudCursor; }
        double getLatitudCursor() { return this->latitudCursor; }
        void setLongitudCursor(double longitudCursor) { this->longitudCursor = longitudCursor; }
        double getLongitudCursor() { return this->longitudCursor; }
        void setMapLonDelta(double mapLonDelta) { this->mapLonDelta = mapLonDelta; }
        double getMapLonDelta() { return this->mapLonDelta; }
        void setMapLatBottomDegree(double mapLatBottomDegree) { this->mapLatBottomDegree = mapLatBottomDegree; }
        double getMapLatBottomDegree() { return this->mapLatBottomDegree; }
        void setAcumSubida(double acumSubida) { this->acumSubida = acumSubida; }
        double getAcumSubida() { return this->acumSubida; }
        void setAcumBajada(double acumBajada) { this->acumBajada = acumBajada; }
        double getAcumBajada() { return this->acumBajada; }
        void setMaxAltitud(double maxAltitud) { this->maxAltitud = maxAltitud; }
        double getMaxAltitud() { return this->maxAltitud; }
        void setMinAltitud(double minAltitud) { this->minAltitud = minAltitud; }
        double getMinAltitud() { return this->minAltitud; }
        void setDistancia(double distancia) { this->distancia = distancia; }
        double getDistancia() { return this->distancia; }
        void setPendienteMediaSubida(double pendienteMediaSubida) { this->pendienteMediaSubida = pendienteMediaSubida; }
        double getPendienteMediaSubida() { return this->pendienteMediaSubida; }
        void setPendienteMediaBajada(double pendienteMediaBajada) { this->pendienteMediaBajada = pendienteMediaBajada; }
        double getPendienteMediaBajada() { return this->pendienteMediaBajada; }
        void setMaxDesnivel2Puntos(double maxDesnivel2Puntos) { this->maxDesnivel2Puntos = maxDesnivel2Puntos; }
        double getMaxDesnivel2Puntos() { return this->maxDesnivel2Puntos; }
        void setSumaPendienteSubida(unsigned long long sumaPendienteSubida) { this->sumaPendienteSubida = sumaPendienteSubida; }
        unsigned long long getSumaPendienteSubida() { return this->sumaPendienteSubida; }
        void setSumaPendienteBajada(unsigned long long sumaPendienteBajada) { this->sumaPendienteBajada = sumaPendienteBajada; }
        unsigned long long getSumaPendienteBajada() { return this->sumaPendienteBajada; }
        void setNumPendienteSubida(long numPendienteSubida) { this->numPendienteSubida = numPendienteSubida; }
        long getNumPendienteSubida() { return this->numPendienteSubida; }
        void setNumPendienteBajada(long numPendienteBajada) { this->numPendienteBajada = numPendienteBajada; }
        long getNumPendienteBajada() { return this->numPendienteBajada; }
        void setDistLlano(double distLlano) { this->distLlano = distLlano; }
        double getDistLlano() { return this->distLlano; }
        void setDistSubida(double distSubida) { this->distSubida = distSubida; }
        double getDistSubida() { return this->distSubida; }
        void setDistBajada(double distBajada) { this->distBajada = distBajada; }
        double getDistBajada() { return this->distBajada; }
        void setVelocidadMaxima(double velocidadMaxima) { this->velocidadMaxima = velocidadMaxima; }
        double getVelocidadMaxima() { return this->velocidadMaxima; }
        void setVelocidadMinima(double velocidadMinima) { this->velocidadMinima = velocidadMinima; }
        double getVelocidadMinima() { return this->velocidadMinima; }
        void setSumaVelocidadMedia(double sumaVelocidadMedia) { this->sumaVelocidadMedia = sumaVelocidadMedia; }
        double getSumaVelocidadMedia() { return this->sumaVelocidadMedia; }
        void setVelocidadMedia(double velocidadMedia) { this->velocidadMedia = velocidadMedia; }
        double getVelocidadMedia() { return this->velocidadMedia; }
        void setNumSumaVelocidad(unsigned long numSumaVelocidad) { this->numSumaVelocidad = numSumaVelocidad; }
        unsigned long getNumSumaVelocidad() { return this->numSumaVelocidad; }
        void setTiempoTotal(unsigned long long tiempoTotal) { this->tiempoTotal = tiempoTotal; }
        unsigned long long getTiempoTotal() { return this->tiempoTotal; }
        void setTiempoParado(unsigned long long tiempoParado) { this->tiempoParado = tiempoParado; }
        unsigned long long getTiempoParado() { return this->tiempoParado; }
        void setTiempoMovimiento(unsigned long long tiempoMovimiento) { this->tiempoMovimiento = tiempoMovimiento; }
        unsigned long long getTiempoMovimiento() { return this->tiempoMovimiento; }
        int getMapOffsetX(){return mapOffsetX;}
        int getMapOffsetY(){return mapOffsetY;}
        void setMapOffsetX(int var){mapOffsetX = var;}
        void setMapOffsetY(int var){mapOffsetY = var;}
        void setZoomLevel(int var){zoomLevel = var;}
        int getZoomLevel(){return zoomLevel;}
        int getZoomMeters(){return googleZoom[zoomLevel];}
        void incZoomLevel(){ zoomLevel = (zoomLevel + 1) % (sizeof(googleZoom)/sizeof(int));}
        void decZoomLevel(){ if (zoomLevel > 0) zoomLevel = zoomLevel - 1; }
        std::vector<CumbreValle> *getListaCumbresYValles(){return &listaCumbresYValles;}
        void setNumTopo(int numTopo) { this->numTopo = numTopo; }
        int getNumTopo() { return this->numTopo; }


        int calculaAnguloDireccion(Point *lastPixelMapPosT, Point *currentPixelMapPosT);
        void calculaVerticesFlecha( double angulo, int *datosFlecha, double anguloFlecha);
        void resetStats(double lat, double lon, double alt);
        void procesaPicosTerreno(double lat, double lon, double alt, long time, StatsClass *stats, bool isCumbre);

        CumbreValle arrayCumbresYValles[10];
        void calcTilesPixels();

        void setMapNumPoints(int var){mapNumPoints = var;}
        int getMapNumPoints(){return mapNumPoints;}

    protected:
    private:


        void calcularEstadisticasRuta(double lat, double lon, double alt, long time, StatsClass *stats);
        double mapLonDelta;
        double mapLatBottomDegree;
        std::vector<std::string> strSplitted;
        std::vector<std::string> strSplitted2;
        std::vector<CumbreValle> listaCumbresYValles;
        int numTopo;
        int width;
        int height;
        double mapLonLeft;
        double mapLonRight;
        double mapLatBottom;
        double mapLatUp;
        double desplazamiento;
        double latitudCursor;
        double longitudCursor;
        bool autocenter;
        int mapOffsetX;
        int mapOffsetY;
        int mapNumPoints;
        int zoomLevel;
        //Calculo de estadisticas
        bool correccionGPS;
        double acumSubida;
        double acumBajada;
        double maxAltitud;
        double minAltitud;
        double distancia;
        double pendienteMediaSubida;
        double pendienteMediaBajada;
        double maxDesnivel2Puntos;
        unsigned long long sumaPendienteSubida;
        unsigned long long sumaPendienteBajada;
        unsigned long numPendienteSubida;
        unsigned long numPendienteBajada;
        double distLlano;
        double distSubida;
        double distBajada;
        double velocidadMaxima;
        double velocidadMinima;
        double sumaVelocidadMedia;
        unsigned long numSumaVelocidad;
        double velocidadMedia;
        unsigned long long tiempoTotal;
        unsigned long long tiempoParado;
        unsigned long long tiempoMovimiento;
};

#endif // GEODRAWER_H
