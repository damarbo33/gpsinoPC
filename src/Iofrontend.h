/**
* Clase que sirve para dibujar los menus o cualquier pantalla
* de la aplicacion
*/

#ifndef Iofrontend_H
#define Iofrontend_H

#include <string>
#include <vector>
#include "Ioutil.h"
#include "Colorutil.h"
#include "Menuobject.h"
#include "ImagenGestor.h"
#include "Icogestor.h"
#include "Launcher.h"
#include "thread.h"

#include "uipicture.h"
#include "uiinput.h"
#include "uipicture.h"
#include "uiart.h"
#include "uibutton.h"
#include "uiprogressbar.h"
#include "uipopupmenu.h"
#include "servers/dropbox.h"
#include "AudioPlayer.h"
#include "scrapper/scrapper.h"
#include "gpx/GeoStructs.h"
#include "gpx/geodrawer.h"
#include "gpx/gpxloader.h"
#include "image565.h"
#include <vector>

    struct t_mapSurface{
        int orientacion;
        SDL_Surface *tmpSurface;
    };




class Iofrontend : public Ioutil{

     private :
        Scrapper *scrapper;
        int selMenu;
        //Funciones para anyadir punteros a funciones
        typedef int (Iofrontend::*typept2Func)(tEvento *); //Se declara el puntero a funcion
        typept2Func pt2Func[MAXMENU*MAXOBJECTS]; //Se declara un array de punteros a funcion

        struct tprops{
            vector<string> name;
            vector<string> parms;
            int size;
        } propertiesPt2Func;

        struct tscreenobj{
            bool drawComponents;
            bool execFunctions;
        };

        GeoDrawer *geoDrawer;
        string fileGPSData;
        string fileGPX;
        std::vector<std::string> *posiciones;
        std::vector<PosMapa> *listaPixels;
        SDL_Surface *myGPSSurface;

        void setDinamicSizeObjects();
        void mensajeCompilando(tEvento, string, bool);
        bool procesarBoton(Object *, tmenu_gestor_objects *);
        string casoJOYBUTTONS(tEvento *evento);
        void setPanelMediaVisible(bool );
        bool bucleReproductor();
        void addEvent(string, typept2Func);
        void addEvent(string, typept2Func, int);
        int findEventPos(string);
        void setEvent(string, typept2Func);
        void setEvent(string nombre, typept2Func funcion, int parms);
        //Eventos asignados a los botones
        int simularEscape(tEvento *);
        int simularIntro(tEvento *);
        int marcarBotonSeleccionado(tEvento *);
        int casoDEFAULT(tEvento );
        int accionesMenu(tEvento *);
        int accionesGotoPantalla(tEvento *);
        int accionesCargaPantalla(tEvento *);
        int loadDirFromExplorer(tEvento *);
        int accionesListaExplorador(tEvento *evento);
        int accionCombo(tEvento *evento);
        string showExplorador(tEvento *);
        void cargaMenuFromLista(UIListCommon *, tEvento *);
        void cargaMenu(int, string, tEvento *);
        bool browser(int, string, int, string, string);
        void comprobarUnicode(int);
        void setTextFromExplorador(tEvento *, UIInput *);
        void clearEmuFields();
        UIPopupMenu * addPopup(int pantalla, string popupName, string callerName);
        bool procesarPopups(tmenu_gestor_objects *objMenu, tEvento *evento);
        bool procesarMenuActual(tmenu_gestor_objects *objMenu, tEvento *evento);
        //Muestra el menu especificado como si fuera emergente
        void showMenuEmergente(int menu, string objImagenFondo);
        void loadComboUnidades(string objName, int pantalla,  int types);





        void leerLineaFicheroRuta(char *text, int numLinea, int *numCampo, PosMapa *data);
        int getPosY(int altura, int graphW, int graphH);
        void drawGraphSlope(tEvento *evento, int graphX, int graphY, int graphW, int graphH);
        void plotLineWidth(int x0, int y0, int x1, int y1, int wd, t_color color,SDL_Surface *surface);
        void calcularPixels(VELatLong *lastGPSPos, VELatLong *currentGPSPos, Point *latestCoord, Point *actualCoord);
        int leerCamposDat(FILE *fp, PosMapa *dataFromFile);
        void drawMapArduino(tEvento *evento);
        void mostrarDatosRuta(int pendiente, double distFromStart, double eleActual, const char *waypointText);
        void pintarFlechaGPS(Point *punto, double rotacion);
        void generarFicheroRuta(string fileOri, string fileDest, int zoomMeters, double anguloLimite, bool generatePixels);
        double calculaAnguloTrackpoint(Point *xy1, Point *xy0, Point *xy2);
        void anyadirPunto(ofstream *myfile, PosMapa *xy);
        void anyadirGeopos(ofstream *myfile, PosMapa *posmapa);
        void crearCabecera(ofstream *myfile);
        void cargarFicheroRuta(string file, int zoomMeters);
        void loadFromFileToVector(string file, std::vector<std::string> *myVector);
        void pintarCapaTerreno(VELatLong *currentGPSPos);
        void getSurfaceMap(SDL_Surface **myDestSurface, Point *pixelTile, int xtile, int ytile, int zoom);
        Uint32 getPixelFromOrientarion(t_mapSurface *mapSurfaces, int orientacion, int virtualMapX, int virtualMapY, int mapWidth, int mapHeight);
        void getPosPixelFromOrient(int orientacion, int virtualMapX, int virtualMapY,
                                    int mapWidth, int mapHeight, t_iPos *pos);
        void getAroundTile(int xtile, int ytile, int orientacion, t_posicion *posTile);
        int getDrawableMapOrientation(int virtualMapX, int virtualMapY, int mapWidth, int mapHeight);
        Uint32 get_pixel32( SDL_Surface *surface, int x, int y);
        void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
        void showIcons();
        int calcNumProcessedPoints(string fileOri, int zoomMeters, double anguloLimite);


        int openGpx(tEvento *evento);
        int downloadMaps(tEvento *evento);

        string procesaGPX(string filename, int angulo);
        void analyzeGpx(string ruta);
        void downImgRoute(t_downMapData *in);
        Thread<Scrapper> *threadMaps;

    public :
        Iofrontend();
        ~Iofrontend();
        bool drawMenu(tEvento );
        void resizeMenu();
        void popUpMenu(tEvento);
        void initUIObjs();
        ImagenGestor *imgGestor ;
        bool isMenuLeftVisible();
        string casoPANTALLAPREGUNTA(string, string);
        bool casoPANTALLACONFIRMAR(string, string);
        //DEBERIAN SER PRIVATE!!!!!!!!!!!!!!!!!!!!!!!!!!!
        tmenu_gestor_objects *ObjectsMenu[MAXMENU];  //Creo tantos punteros a tmenu_gestor_objects como menus dispongo
                                                    //De esta forma podre asignar objetos a cada menu

        bool procesarControles(tmenu_gestor_objects *, tEvento *, tscreenobj *);
        void setSelMenu(int var){comprobarUnicode(var); selMenu = var;}
        int getSelMenu(){ return selMenu;}
        int accionCopiarTextoPopup(tEvento *evento);
        void autenticateAndRefresh();

};
#endif //Iofrontend_H
