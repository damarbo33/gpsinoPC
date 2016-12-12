#include "Icogestor.h"

/**
* Constructor
*/
IcoGestor::IcoGestor(){
    ruta = "";
    arrayIcons.numIcons = 0;
    const char  tempFileSep[2] = {FILE_SEPARATOR,'\0'};
    string str = "imgs" + string(tempFileSep);
    Dirutil *dirio = new Dirutil();
    char rutaTemp[PATH_MAX];
    strcpy(rutaTemp,Constant::getAppDir().c_str());
    dirio->dircat(rutaTemp,str.c_str());
    delete dirio;
    setRuta(rutaTemp);
    cargarIconos();
}

/**
* Destructor
*/
IcoGestor::~IcoGestor(){
    for (int i=0;i<arrayIcons.numIcons;i++)
	{
        SDL_FreeSurface(arrayIcons.Icon[i]);
	}
}

/**
*   Lista de iconos. TIENEN que cargarse en el mismo orden
*   definido en la constante Constant::enumIco
*/
void IcoGestor::cargarIconos(){

    Traza::print("Cargando iconos de la ruta: " + string(getRuta()), W_DEBUG);

    cargarIcono("page_white_text.png");
    cargarIcono("folder.png");
    cargarIcono("page_white.png");
    cargarIcono("bullet_go.png");
    cargarIcono("door_out.png");
    cargarIcono("bullet_wrench.png");
    cargarIcono("clock.png");
    cargarIcono("page_find.png");
    cargarIcono("folder_explore.png");
    cargarIcono("magnifier.png");
    cargarIcono("chart_bar.png");
    cargarIcono("cross.png");
    cargarIcono("tick.png");
    cargarIcono("font.png");
    cargarIcono("font_add.png");
    cargarIcono("color_swatch.png");
    cargarIcono("text_align_justify.png");
    cargarIcono("layout_sidebar.png");
    cargarIcono("clock_delete.png");
    cargarIcono("book_go.png");
    cargarIcono("bullet_black.png");
    cargarIcono("page_white_gear.png");
    cargarIcono("page_white_compressed.png");
    cargarIcono("page_white_acrobat.png");
    cargarIcono("page_white_c.png");
    cargarIcono("page_white_h.png");
    cargarIcono("page_white_paint.png");
    cargarIcono("application_form_edit.png");
    cargarIcono("delete.png");
    cargarIcono("sound.png");
    cargarIcono("sound_mute.png");
    cargarIcono("emoticon_waii.png");
    cargarIcono("page_white_zip.png");
    cargarIcono("page_white_picture.png");
    cargarIcono("controller.png");
    cargarIcono("add.png");
    cargarIcono("boton.png");
    cargarIcono("boton_selected.png");
    cargarIcono("reloj_de_arena.png");
    cargarIcono("arrow_refresh.png");
    cargarIcono("application_xp_terminal.png");
    cargarIcono("film.png");
    cargarIcono("control_fastforward.png");
    cargarIcono("control_fastforward_blue.png");
    cargarIcono("control_pause.png");
    cargarIcono("control_pause_blue.png");
    cargarIcono("control_play.png");
    cargarIcono("control_play_blue.png");
    cargarIcono("control_rewind.png");
    cargarIcono("control_rewind_blue.png");
    cargarIcono("control_stop.png");
    cargarIcono("control_stop_blue.png");
    cargarIcono("cursor_h_resize.png");
    cargarIcono("database_add.png");
    cargarIcono("music.png");
    cargarIcono("btn_repeat.png");
    cargarIcono("btn_repeat_off.png");
    cargarIcono("btn_random.png");
    cargarIcono("btn_random_off.png");
    cargarIcono("FlechaEsquinaSupIzq.png");
    cargarIcono("control_equalizer.png");
    cargarIcono("control_equalizer_blue.png");
    cargarIcono("btnSliderEQ.png");
    cargarIcono("btn_off.png");
    cargarIcono("btn_on.png");
    cargarIcono("btn_reset_eq.png");
    cargarIcono("Dropbox.png");
    cargarIcono("Google-Drive-icon.png");
    cargarIcono("boton_disabled.png");
    cargarIcono("server_add.png");
    cargarIcono("cd_add.png");
    cargarIcono("cd.png");
    cargarIcono("drive.png");
    cargarIcono("drive_cd.png");
    cargarIcono("drive_disk.png");
    cargarIcono("drive_network.png");
    cargarIcono("map_go.png");
    cargarIcono("trackSeg.png");


}

/**
* Carga el icono del disco duro para el nombre de archivo
*/
void IcoGestor::cargarIcono(string filename){
    if (arrayIcons.numIcons < totalIcons){
        arrayIcons.Icon[arrayIcons.numIcons] = IMG_Load(string(getRuta() + filename).c_str());
        if (zoomText > 1){
            int w = arrayIcons.Icon[arrayIcons.numIcons]->w * zoomText;
            int h = arrayIcons.Icon[arrayIcons.numIcons]->h * zoomText;
            int bpp = arrayIcons.Icon[arrayIcons.numIcons]->format->BitsPerPixel;
            SDL_Surface *bitmap;
            redimension(arrayIcons.Icon[arrayIcons.numIcons], 0, w, h, &bitmap);
            SDL_SetAlpha(bitmap, 0, 0) ;
            SDL_FreeSurface(arrayIcons.Icon[arrayIcons.numIcons]);
            arrayIcons.Icon[arrayIcons.numIcons] = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,w,h,bpp,
                                                        bitmap->format->Rmask, bitmap->format->Gmask, bitmap->format->Bmask, bitmap->format->Amask);
            SDL_BlitSurface(bitmap, NULL, arrayIcons.Icon[arrayIcons.numIcons], NULL);
            SDL_FreeSurface(bitmap);
        }
        arrayIcons.numIcons++;
    }
}

/**
* Se pinta el icono en la superficie pasada por parametro en las coordenadas indicadas
*/
bool IcoGestor::drawIcono(int numIco, SDL_Surface * dstSurface, int x, int y){
    if (arrayIcons.Icon[numIco] != NULL && dstSurface != NULL && numIco < arrayIcons.numIcons && numIco >= 0) {
        SDL_Rect dstRect = { (short int)x, (short int)y, 0, 0 };
        if (zoomText != 1){
            drawIcono(numIco, dstSurface, 0, x, y, arrayIcons.Icon[numIco]->w * zoomText, arrayIcons.Icon[numIco]->h * zoomText);
        } else {
            SDL_BlitSurface(arrayIcons.Icon[numIco], NULL, dstSurface, &dstRect);
        }
        return true;
    } else {
        return false;
    }
}

/**
* Se pinta el icono en la superficie pasada por parametro en las coordenadas indicadas
*/
bool IcoGestor::drawIcono(int numIco, SDL_Surface * dstSurface, int angle, int x, int y, int destw, int desth){
    if (arrayIcons.Icon[numIco] != NULL && dstSurface != NULL && numIco < arrayIcons.numIcons && numIco >= 0) {
        SDL_Rect dstRect = { (short int)x, (short int)y, 0, 0 };
        SDL_Surface *bitmap = NULL;
        redimension(arrayIcons.Icon[numIco], angle, destw, desth, &bitmap);

        if (angle != 0){
            //offsetX = (newWidth - originalWidth) / 2
            int offsetX = (bitmap->w - destw) / 2;
            dstRect.x -= offsetX;

            int offsetY = (bitmap->h - desth) / 2;
            dstRect.y -= offsetY;
        }


        SDL_BlitSurface(bitmap, NULL, dstSurface, &dstRect);
        SDL_FreeSurface(bitmap);
        return true;
    } else {
        return false;
    }
}

/**
* Devuelve un nuevo surface en base a un contenedor
*
* Si redimensiona la imagen, la deja en el puntero destino y devuelve true.
* Si no la redimensiona, solo devuelve false y deja tal cual la variable srcSurface
*/
bool IcoGestor::redimension(SDL_Surface *srcSurface, int angle, int w_destino, int h_destino,  SDL_Surface **destino){
    bool smooth = true;
    float nPercentW = ((float)srcSurface->w / (float)w_destino);
    float nPercentH = ((float)srcSurface->h / (float)h_destino);
    //*destino = zoomSurface(srcSurface, 1/nPercentW, 1/nPercentH, smooth);
    *destino = rotozoomSurfaceXY(srcSurface, angle, 1/nPercentW, 1/nPercentH, smooth);
    return true;
}

/**
* Devuelve la relacion de una imagen para hacerla mas pequenya o mas grange
* Si isBestfit=true intenta aprovechar toda la pantalla disponible aunque la imagen sea
* mas pequenya
*/
float IcoGestor::relacion(int altoOrig, int anchoOrig, int altoDest, int anchoDest){
   int destWidth = anchoDest;
   int destHeight = altoDest;
   float nPercent = 0;
   float nPercentW = 0;
   float nPercentH = 0;

   nPercentW = ((float)anchoOrig / (float)destWidth);
   nPercentH = ((float)altoOrig / (float)destHeight);

   if (nPercentH > nPercentW)
      nPercent = nPercentH;
   else
      nPercent = nPercentW;

   return nPercent;
}

