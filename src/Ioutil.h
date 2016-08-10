#ifndef Ioutil_H
#define Ioutil_H

    #include <iostream>
    #include <map>
    #include <cmath>        // std::abs
    #include <sstream>
    #include "SDL/SDL.h"
    #include "SDL/SDL_ttf.h"
    #include "SDL/SDL_image.h"
    #include "SDL/SDL_rotozoom.h"
    #include <SDL/SDL_mixer.h>
    #include "Fileio.h"
    #include "Constant.h"
    #include "ImagenGestor.h"
    #include "Traza.h"
    #include "Menuobject.h"
    #include "Icogestor.h"
    #include "joymapper.h"
    #include "thread.h"
    #include "uiinput.h"
    #include "uiprogressbar.h"
    #include "uicombobox.h"
    #include "cursor.h"
    #include "listaIni.h"
    #include "Colorutil.h"

    struct tInput{
        int cursorX;
        int cursorY;
        int selCursorX1;
        int selCursorX2;
        string texto;
    };

    static const char WINDOW_TITLE[] = {"GPSINO"};

    //*********************************************************************************/
    //Cuidado con no sobrepasar el numero de elementos que define la constante MAXMENU
    //*********************************************************************************/
     typedef enum { PANTALLAGPSINO,     //0
            PANTALLACONFIRMAR,
            PANTALLABROWSER2,
            PANTALLAPREGUNTA,
            PANTALLAMAP,
            MAXMENU
    } enumMenus;



    static const t_color ColorScaleFFT [] =
    {
        cAzulOscuro, // dk blue
        cAzulTotal, // lt blue
        cVerde, // lt green
        cVerde,
        cVerde,
        cVerde,
        cVerde,
        cAmarillo,  // lt yellow
        cAmarillo,
        cAmarillo,
        cAmarillo,
        cAmarillo,
        cNaranja,   // orange
        cNaranja,
        cNaranja,
        cNaranja,
        cRojo // lt red
    };

    class Ioutil : public Fileio{
        public:
            Ioutil();
            virtual ~Ioutil();
            tEvento WaitForKey(); //Espera a que se pulse una tecla
            void initSDL(bool); //Inicia el subsistema de SDL
            void killSDL();
            void toggleFullScreen();
            void drawText(const char* , int , int , t_color ); //Escribe texto en la pantalla
            void drawTextInt(int , int , int ,  t_color ); // Escribe un entero en la pantalla
            void drawTextCent( const char* , int , int , bool , bool , t_color );
            void drawTextInArea( const char*, int, int, t_color, SDL_Rect *);
            void drawTextInsideArea( int posArrayTexto, int x, int y, Object *obj, SDL_Rect *textLocation);
            void drawRect(int, int, int, int, t_color); //Dibuja un rectangulo rellenado
            void drawRect(int x, int y, int w, int h, t_color color, SDL_Surface *destSurf);
            void drawRectAlpha(int, int, int, int, t_color, int); //Dibuja un rectangulo rellenado con valor alpha de transparencia
            void drawRectAlpha(int x, int y, int w, int h, t_color color, int colorAlpha, SDL_Surface *destSurf);
            void drawRectLine(int , int , int , int , int , t_color);//Dibuja un rectangulo sin relleno del ancho especificado
            bool drawImgObj(Object *);
            bool drawFondoImgSel(tEvento, int, int, int, t_region, t_color);
            void drawUITitleBorder(const char *);
            void drawUITitleBorder(const char *, int, t_color);
            void drawUIButton(Object *obj);
            void drawUIListBox(Object *obj);
            void drawUIPopupMenu(Object *obj);
            void drawUIPicture(Object *obj);
            void drawUIInputWide(Object *obj);
            void drawUIArt(Object *obj);
            void drawUILabel(Object *obj);
            void drawUIProgressBar(Object *obj, tEvento *evento);
            void drawUIPanel(Object *obj);
            void drawUILetraPopup(Object *obj);
            void drawUIPopupFondo(Object *obj, int);
            void drawUIComboBox(Object *obj);
            void drawListContent(Object *obj, int x, int y, int w, int h);
            void marcarPos(int x, int y);
            void drawUIListGroupBox(Object *obj);
            void drawListGroupContent(Object *obj, int x, int y, int w, int h);
            void drawScrollBar(Object *obj);
            void drawUITextElementsArea(Object *obj);
            void drawUISpectrum(Object *obj);
            t_color MapColor (int s);
            void drawUISpectrumFft(Object *obj);
            void drawUISlider(Object *obj, tEvento *evento);
            void drawIco(int , int , int , int , int );
            void drawIco(int numIco, int angle, int x, int y, int w, int h);
            void pintarHint(int x1, int y1, int w1, int h1, string text, t_color color);
            void pintarCirculo (int, int, int, t_color);
            void pintarSemiCirculo (int x, int y, int r, t_color color, int angle);
            void pintarFillCircle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel);
            void pintarLinea (int , int , int , int  , t_color );
            void pintarLineaV (int x1, int y1, int h, t_color color);
            void pintarLineaSpectrum (int x1, int y1, int h, int maxh, t_color color);
            void pintarTriangulo (int , int , int , int , bool , t_color);
            void fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, t_color color);
            void drawFastHLine(int x, int y, int w, t_color color);
            void pintarContenedor(int x1, int y1, int w1, int h1, bool selected, Object *obj, t_color color);
            void pintarDegradado(int x1, int y1, int x2, int y2, int lineas, int grayIni, int grayFin);
            void pintarCursor(int x, int y, int cursor);
            void printContImg(const char *);
            void printGrafica(double, int);
            void showCheck(Object *);
            void fps();
            void clearScr(); //Borra el contenido de la pantalla
            void clearScr(t_color);
            void clearScr(t_color, t_region);
            void flipScr(); //Actualiza la pantalla
            void updateScr(t_region *);
            bool drawImgMoved(ImagenGestor *);
            int fontStrLen(string); //Calcula el ancho de un string en pixels
            int fontStrLenCent(string); //Calcula el ancho de un string en pixels para poder centrarlo
            void getMousePos(int *, int *);
            unsigned int getPosThumb(int, int,  t_region);
            int getWidth(){return w;} //Devuelve el tamanyo de la imagen cargada en memoria
            int getHeight(){return h;}
            void takeScreenShot(SDL_Surface **);
            void takeScreenShot(SDL_Surface **, SDL_Rect);
            void printScreenShot(SDL_Surface **);
            void printScreenShot(SDL_Surface **, SDL_Rect);
            void showMessage(string, unsigned long);
            void drawIco(int numIco, int x, int y);
            void clearEvento(tEvento *);
            void showAutoMessage();
            void setAutoMessage(string);
            void makeMoveSurface(SDL_Surface *);
            void desactivarEventos();
            void activarEventos();
            void pintarIconoProcesando(bool refreshBackground);
            void msg_processing(unsigned int, unsigned int);
            bool isCanFlip(){return canFlip;}
            void setCanFlip(bool var){canFlip = var;}
            void clearLastEvento(){clearEvento(&lastEvento);}
            string configButtonsJOY(tEvento *evento);
            TTF_Font* getFont(){ return font; }
            inline Uint32 getpixel(SDL_Surface *, const int, const int);
            inline void putpixel(SDL_Surface *, const int, const int, const Uint32);
            inline void putpixelSafe(SDL_Surface *, const int , const int , const Uint32 );
            IcoGestor *gestorIconos;


        protected:
            bool ignoreButtonRepeats;
            SDL_Surface* screen; //Puntero para dibujar por pantalla
            void drawObject(Object *, tEvento *evento);
            CursorGestor *gestorCursor;
            void setCursor(int cursor);
            string dirInicial;
            void drawTextInsideAreaScroll( TextElement *elem, UITextElementsArea *objText, int maxPxLabel, t_color color, SDL_Rect *textLocation);

        private:
            std::map<int, int>* mPrevAxisValues; //Almacena los valores de los ejes de cada joystick
            std::map<int, int>* mPrevHatValues; //Almacena los valores de las crucetas de cada joystick
            SDL_Joystick** mJoysticks;
            int mNumJoysticks;
            void initJoyMapper();
            TTF_Font* font; //Fuente de textos
            SDL_RWops *RWFont;
            int fontAscent;
            int fontDescent;
            int fontHeight;
            int WINDOW_WIDTH_FULLSCREEN;
            int WINDOW_HEIGHT_FULLSCREEN;
            int SCREEN_BITS_FULLSCREEN;
            int w,h;
            Uint32 fullsflags;
            bool autosize;
            int frames;
            int time;
            long lastTime;
            int totalMuestreo;
            int mediaFps;
            Fileio *fontfile;
            SDL_Surface * moveSurface;
            string mensajeAuto;
            bool canFlip;
            tEvento evento;
            tEvento lastEvento;
            void cachearObjeto(Object *obj);
            void cachearPosicion(Object *obj, SDL_Rect *imgLocation);
            void ResizeSurface(float , SDL_Surface *, SDL_Surface **);
            void loadFont(unsigned char); //Carga la fuente de determinado tamanyo
            void loadFontFromFile(const char *);

            void Zoom(int );
            tInput calculaTextoInput(Object *);

            SDL_Surface *screenShotSurface;
            SDL_Surface *screenShotThumbSurface;
            //Cursores
            int actualCursor;
            Colorutil color1Spectrum;
            Colorutil color2Spectrum;
            Colorutil color3Spectrum;
            t_color degColorSpectrum;
    };
#endif //Ioutil_H
