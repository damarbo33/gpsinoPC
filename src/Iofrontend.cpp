#include "Iofrontend.h"
#include "uilistgroup.h"
#include "beans/listgroupcol.h"
#include "uilistcommon.h"


const int SURFACE_MODE = SDL_SWSURFACE;

static int limitW = 0;
static int limitH = 0;
static int limitX = 0;
static int limitY = 0;
//For opencyclemap
static t_color umbral = {195,182,174};
//For openstreetmap
//static t_color umbral = {217,219,207};


/**
* Constructor
* HEREDA DE Ioutil
*
*/
Iofrontend::Iofrontend(){
    Traza::print("Constructor de IoFrontend", W_INFO);
    imgGestor = new ImagenGestor();

    Traza::print("Creando objetos de cada menu", W_INFO);
    for (int i=0; i < MAXMENU; i++){
        ObjectsMenu[i] = new tmenu_gestor_objects(this->getWidth(), this->getHeight()); //Inicializo el puntero asignado a cada menu. En el constructor de esta clase,                                                    //se crean tantos objetos como se defina en la constante MAXOBJECTS
    }

    for (int i=0; i < MAXMENU*MAXOBJECTS; i++){
        pt2Func[i] = NULL;
    }
    propertiesPt2Func.size = 0;

    scrapper = new Scrapper();
    threadMaps = NULL;
    Constant::setExecMethod(launch_create_process);
    Traza::print("Asignando elementos y acciones", W_INFO);
    initUIObjs();
    Traza::print("Objetos iniciados", W_INFO);
    geoDrawer = new GeoDrawer(gpsinoW, gpsinoH);
    posiciones = NULL;
    listaPixels = NULL;
    myGPSSurface = NULL;
    fileGPX = "";
    setSelMenu(PANTALLAGPSINO);
    tEvento evento;
    drawMenu(evento);
    Traza::print("Fin Constructor de IoFrontend", W_INFO);
}

/**
* Destructor
*/
Iofrontend::~Iofrontend(){
    Traza::print("Destructor de IoFrontend", W_INFO);

//  A los objetos no los podemos eliminar porque no fueron creados dinamicamente
    Traza::print("Eliminando objetos de cada Menu", W_INFO);
    for (int i=0; i < MAXMENU; i++){
        Traza::print("Eliminando menu: ", i, W_PARANOIC);
        delete ObjectsMenu[i];
    }
    delete scrapper;
    Traza::print("Destructor de IoFrontend FIN", W_INFO);
}


/**
* Inicializa los objetos que se pintaran por pantalla para cada pantalla
* Debe llamarse a este metodo despues de haber inicializado SDL para que se puedan centrar los componentes correctamente
**/

void Iofrontend::initUIObjs(){
    ObjectsMenu[PANTALLAPREGUNTA]->add("valor", GUIINPUTWIDE, 0, -20 * zoomText, INPUTW, Constant::getINPUTH(), "Dato:", true);
    ObjectsMenu[PANTALLAPREGUNTA]->add("btnAceptarPregunta", GUIBUTTON, -(BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    ObjectsMenu[PANTALLAPREGUNTA]->add("btnCancelarPregunta", GUIBUTTON, (BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);
    ObjectsMenu[PANTALLAPREGUNTA]->add("borde", GUIPANELBORDER,0,0,0,0, "Introduzca el dato", false);
    ObjectsMenu[PANTALLAPREGUNTA]->getObjByName("valor")->setColor(cBlanco);

    ObjectsMenu[PANTALLACONFIRMAR]->add("borde", GUIPANELBORDER,0,0,0,0, Constant::toAnsiString("Seleccione una opción"), false);
    ObjectsMenu[PANTALLACONFIRMAR]->add("textosBox", GUITEXTELEMENTSAREA, 0, -50 * zoomText, getWidth()-50, 120, "", true)->setVerContenedor(false);
    ObjectsMenu[PANTALLACONFIRMAR]->add("btnSiConfirma", GUIBUTTON, -(BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    ObjectsMenu[PANTALLACONFIRMAR]->add("btnNoConfirma", GUIBUTTON, (BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);

    UITextElementsArea *infoTextRom = (UITextElementsArea *)ObjectsMenu[PANTALLACONFIRMAR]->getObjByName("textosBox");
    
    TextElement detalleElement;

    
    
    t_posicion pos(0,0,0,0);
    detalleElement.setName("labelDetalle");
    detalleElement.setUseMaxLabelMargin(true);
    detalleElement.setPos(pos);
    infoTextRom->addField(&detalleElement);
    infoTextRom->setTextColor(cBlanco);
    infoTextRom->setColor(cNegro);

    ObjectsMenu[PANTALLABROWSER2]->add("ImgFondo", GUIPICTURE, 0, Constant::getINPUTH(), 0, 0, "ImgFondo", true)->setEnabled(false);
    ObjectsMenu[PANTALLABROWSER2]->getObjByName("ImgFondo")->setAlpha(150);
    ObjectsMenu[PANTALLABROWSER2]->add(OBJLISTABROWSER2, GUILISTBOX, 0, 0, 0, 0, "LISTADODIR", false)->setVerContenedor(false)->setShadow(false);
    ObjectsMenu[PANTALLABROWSER2]->add(BTNACEPTARBROWSER, GUIBUTTON, -(BUTTONW/2 + 5), 0, BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    ObjectsMenu[PANTALLABROWSER2]->add(BTNCANCELARBROWSER, GUIBUTTON, (BUTTONW/2 + 5), 0, BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);
    ObjectsMenu[PANTALLABROWSER2]->add(ARTDIRBROWSER, GUIARTSURFACE, 0, 0, INPUTW, Constant::getINPUTH(), Constant::toAnsiString("Dirección Browser"), false)->setEnabled(false);
    ObjectsMenu[PANTALLABROWSER2]->add("comboBrowser", GUICOMBOBOX, 0, 0, 0, 0, "", false);


    int despY = 7;
    int despX = 50;
    string checkName = "";

    for (int i=0; i < sizeof(googleZoom)/sizeof(int); i++){
        checkName = "checkZoom" + Constant::TipoToStr(googleZoom[i]);

        ObjectsMenu[PANTALLAGPSINO]->add(checkName,GUICHECK, despX + i*40, despY, 40, 20
                        ,Constant::TipoToStr(googleZoom[i]), false)->setEnabled(true);

        ObjectsMenu[PANTALLAGPSINO]->getObjByName(checkName)->setTextColor(cBlanco);
        if (i== 0)
        ObjectsMenu[PANTALLAGPSINO]->getObjByName(checkName)->setChecked(true);
    }

    ObjectsMenu[PANTALLAGPSINO]->add("lblZoom", GUILABEL, 3, despY, 50, 20, "Zoom: ", false)->setTextColor(cBlanco);
    despY = 30;

    ObjectsMenu[PANTALLAGPSINO]->add("chDownAround",GUICHECK, 5, despY, 150, 20
                        ,"Imagenes alrededor", false)->setTextColor(cBlanco);

    ObjectsMenu[PANTALLAGPSINO]->add("chDownFull",GUICHECK, 5 + 155, despY, 150, 20
                        ,"Imagenes area", false)->setTextColor(cBlanco);

    ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownAround")->setChecked(true);
    ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownFull")->setChecked(false);

    despY += 25;

    ObjectsMenu[PANTALLAGPSINO]->add("lblFile", GUILABEL, 3, despY + 3, 50, 20, "Archivo: ", false)->setTextColor(cBlanco);
    ObjectsMenu[PANTALLAGPSINO]->add("inputFileGPX",   GUIINPUTWIDE, despX, despY, 400, 20, "", false)->setEnabled(false);
    ObjectsMenu[PANTALLAGPSINO]->add("btnOpenLocal",   GUIBUTTON, despX + 405, despY,FAMFAMICONW, FAMFAMICONH, "Abrir fichero local", false)->setIcon(folder)->setVerContenedor(false);
    ObjectsMenu[PANTALLAGPSINO]->add("btnDownloadMap", GUIBUTTON, despX + 405 + 5 + FAMFAMICONW ,despY,FAMFAMICONW, FAMFAMICONH, "Descargar mapas", false)->setIcon(map_go)->setVerContenedor(false);
    ObjectsMenu[PANTALLAGPSINO]->add("btnInstallMap", GUIBUTTON, despX + 405 + 2*( 5 + FAMFAMICONW) ,despY,FAMFAMICONW, FAMFAMICONH, "Instalar mapas", false)->setIcon(map_add)->setVerContenedor(false);
    ObjectsMenu[PANTALLAGPSINO]->add("progDownload", GUIPROGRESSBAR, 0, getHeight() - 20, getWidth(), 20, "Progreso de descarga", false)->setEnabled(true);
    ObjectsMenu[PANTALLAGPSINO]->add("mapBox", GUIPICTURE, 0,0, gpsinoW, gpsinoH, "mapBox", true)->setVisible(true);
    //ObjectsMenu[PANTALLAGPSINO]->add("mapBox", GUIPICTURE, 2, Constant::getINPUTH()*3 + 25 * 2, gpsinoW, gpsinoH, "mapBox", true)->setVisible(true);

    UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");
    limitH = objPict->getH();
    limitW = objPict->getW();
    limitX = objPict->getX();
    limitY = objPict->getY();

//    string ruta = "LogoGpsino.png";
//    if (objPict->getImgGestor()->getRuta().compare(ruta) != 0){
//        objPict->loadImgFromFile(ruta);
//        objPict->getImgGestor()->setBestfit(false);
//        objPict->setImgDrawed(false);
//    }

    //Establecemos los elementos que se redimensionan
    setDinamicSizeObjects();

    //Botones para la pantalla de los directorios
    addEvent(BTNACEPTARBROWSER, &Iofrontend::marcarBotonSeleccionado);
    addEvent(BTNCANCELARBROWSER, &Iofrontend::marcarBotonSeleccionado);
    addEvent(OBJLISTABROWSER2, &Iofrontend::accionesListaExplorador);
    addEvent("btnAceptarPregunta", &Iofrontend::simularIntro);
    addEvent("btnCancelarPregunta", &Iofrontend::simularEscape);
    //Botones para la pantalla de confirmacion
    addEvent("btnSiConfirma", &Iofrontend::marcarBotonSeleccionado);
    addEvent("btnNoConfirma", &Iofrontend::marcarBotonSeleccionado);

    addEvent("btnOpenLocal", &Iofrontend::openGpx);
    addEvent("btnDownloadMap", &Iofrontend::downloadMaps);
    addEvent("btnInstallMap", &Iofrontend::actionInstallMaps);

    addEvent("chDownAround", &Iofrontend::chActionAround);
    addEvent("chDownFull", &Iofrontend::chActionFull);

}

/**
* Con el menu pasado por parametro lo dibujamos entero
*/
bool Iofrontend::drawMenu(tEvento evento){
    Traza::print("Iofrontend::drawMenu Inicio", W_PARANOIC);
    bool salir = false;
    this->clearScr(cGrisOscuro);
    Traza::print("Iofrontend::clearScr Fin", W_PARANOIC);
    //Realiza las acciones de cada elemento de pantalla
    salir = casoDEFAULT(evento);
    //Muestra un mensaje durante un tiempo determinado
    showAutoMessage();
    //Muestra el contador de cuadros por pantalla
    if (SHOWFPS) fps();
    Traza::print("Iofrontend::drawMenu Fin", W_PARANOIC);

    this->flipScr();
    return salir;
}

/**
*
*/
int Iofrontend::casoDEFAULT(tEvento evento){
    tmenu_gestor_objects *objMenu = ObjectsMenu[this->getSelMenu()];
    bool salir = procesarControles(objMenu, &evento, NULL);

    if (getSelMenu() == PANTALLAGPSINO){
//        if (evento.isKey && (evento.key == SDLK_a || evento.key == SDLK_d || evento.key == SDLK_s) ) {
            drawMapArduino(&evento);
//        }
    }

    return salir;
}



/**
* Se encarga de procesar las introducciones de teclas o de joystick para cambiar el contenido
* de los botones, inputs, checks, ... y almacenar en ellos los datos correpondientes.
*/
bool Iofrontend::procesarControles(tmenu_gestor_objects *objMenu, tEvento *evento, tscreenobj *screenEvents){
    Traza::print("procesarControles: Inicio", W_PARANOIC);

    bool execFunc = true;
    bool drawComp = true;
    if (screenEvents != NULL){
        execFunc = screenEvents->execFunctions;
        drawComp = screenEvents->drawComponents;
    }

    //Se procesan los eventos de cada uno de los objetos de un menu
    Object *object;

    if (execFunc){
        //Se llama al action del objeto que esta seleccionado en este momento.
        //Cada objeto tiene el suyo propio o heredado de Object
        objMenu->procEvent(*evento);
        //objMenu->procAllEvent(*evento);
        //Mostramos popups si es necesario
        procesarPopups(objMenu, evento);
        //Hacemos llamada para hacer otros procesados. Por ahora solo en la pantalla de seleccion
        //de roms
        procesarMenuActual(objMenu, evento);
    }

    int posBoton = 0;
    bool salir = false;
    bool botonPulsado = false;
    int estado = 0;
    vector<Object *> objPostProcesado;
    int cursorPrincipal = -1;
    bool updateCursor = false;

    //Recorremos todos los objetos para dibujarlos por pantalla
    try{
         //Procesando el redimensionado de ventana
        if (evento->resize)
            resizeMenu();

        //PINTAMOS ANTES DE PROCESAR LAS ACCIONES. NO SE SI ESTO ES BUENA IDEA
        for (int i=0;i<objMenu->getSize();i++){
            object = objMenu->getObjByPos(i);
            //Finalmente dibujamos el objeto
            if (drawComp && object != NULL){
                if (object->getObjectType() == GUICOMBOBOX){
                    objPostProcesado.push_back(object);
                } else {
                    drawObject(object, evento);
                }
            }
        }
        //Para los objetos que son prioritarios de pintar, lo hacemos en ultimo lugar
        //para que se dibujen sobre el resto
        for(vector<Object *>::iterator it = objPostProcesado.begin(); it < objPostProcesado.end(); ++it){
            Object * obj = *it;
            drawObject(obj, evento);
        }
        objPostProcesado.clear();

        //Procesamos las acciones
        for (int i=0;i<objMenu->getSize();i++){
            object = objMenu->getObjByPos(i);

            if (execFunc && object != NULL){
                if (!object->isPopup()){
                    switch(object->getObjectType()){
                        case GUIBUTTON:
                            //En el caso de los botones tambien procesamos sus eventos
                            botonPulsado = procesarBoton(object, objMenu);
                            estado = evento->mouse_state;
                            if (botonPulsado && ( (evento->isMouse && estado == SDL_RELEASED) || evento->isKey || evento->isJoy)){ //Comprobamos si se ha pulsado el elemento
                                posBoton = findEventPos(object->getName());  //Buscamos la posicion del elemento en el array de punteros a funcion
                                if (posBoton >= 0){ //Si hemos encontrado una funcion
                                    if (this->pt2Func[posBoton] != NULL){
                                        //Forzamos a que se actualicen todos los elementos
                                        objMenu->resetElements();
                                        /**Los botones no pueden hacer que se salga de la aplicacion. Solo ejecutamos la funcion*/
                                        //salir = (*this.*pt2Func[posBoton])(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                        (*this.*pt2Func[posBoton])(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                        Traza::print("procesarControles: Evento lanzado para " + object->getName(), W_INFO);
                                    }
                                }
                            }
                            break;
                        //case GUICHECK:
                        case GUILISTBOX:
                        case GUIPROGRESSBAR:
                        case GUISLIDER:
                        case GUIPOPUPMENU:
                        case GUILISTGROUPBOX:
                        case GUICOMBOBOX:
                        case GUITEXTELEMENTSAREA:
                            if (procesarBoton(object, objMenu)){ //Comprobamos si se ha pulsado el elemento
                                posBoton = findEventPos(object->getName());  //Buscamos la posicion del elemento en el array de punteros a funcion
                                if (posBoton >= 0){ //Si hemos encontrado una funcion
                                    if (this->pt2Func[posBoton] != NULL){
                                        //Forzamos a que se actualicen todos los elementos
                                        objMenu->resetElements();
                                        salir = (*this.*pt2Func[posBoton])(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                        Traza::print("procesarControles: Evento lanzado para " + object->getName(), W_INFO);
                                    }
                                }
                            }
                            break;
                        default:
                            break;
                    } // FIN CASE
                }
            } // FIN IF
//            //Finalmente dibujamos el objeto
            /**Aqui iba el dibujado del objeto para aprovechar el bucle pero esto
            * daba problemas. Se corrige para que tenga su propio bucle al inicio
            * de las acciones
            */
            //Comprobamos si el objeto esta lanzando un evento para cambiar el mouse.
            //Solo aceptamos el primer objeto que pida cambiar el mouse
            if (object->getCursor() >= 0){
                updateCursor = true;
                if (cursorPrincipal < 0){
                    cursorPrincipal = object->getCursor();
                }
            }
        }
        if (!updateCursor){
            //Reseteamos el cursor al que hay por defecto
            cursorPrincipal = cursor_arrow;
        }
        //Dibujamos el cursor solo si procede
        this->pintarCursor(evento->mouse_x, evento->mouse_y, cursorPrincipal);

    } catch (Excepcion &e) {
         Traza::print("Excepcion procesarControles: " + string(e.getMessage()), W_ERROR);
    }
    return salir;
}



/**
*
*/
int Iofrontend::accionesMenu(tEvento *evento){
    bool salir = false;

    try{
        int menu = this->getSelMenu();
        tmenu_gestor_objects *objsMenu = ObjectsMenu[menu];
        Object *object = objsMenu->getObjByPos(objsMenu->getFocus());

        if (object != NULL){
            if (object->getObjectType() == GUILISTBOX ||
                object->getObjectType() == GUIPOPUPMENU ||
                object->getObjectType() == GUILISTGROUPBOX){

                UIListCommon *objList = (UIListCommon *)object;
                unsigned int pos = objList->getPosActualLista();
                string valorSelec = objList->getValue(pos);
                int destino = objList->getDestino(pos);
                objList->setImgDrawed(false);

                if (valorSelec.compare("salir") == 0){
                    salir = true;
                } else if (destino != -1){
                    this->cargaMenuFromLista(objList, evento);
                }
            }
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion accionesMenu" + string(e.getMessage()), W_ERROR);
    }
    return salir;
}

/**
*
*/
void Iofrontend::cargaMenuFromLista(UIListCommon *obj, tEvento *evento){

    if (obj->getPosActualLista() < 0){
        obj->setPosActualLista(0);
    } else {
        int menucarga = obj->getDestino(obj->getPosActualLista());
        string valorSelec = obj->getValue(obj->getPosActualLista());
        //Si hemos pulsado el boton de volver (que es el ultimo de la lista)
        //reiniciamos la posicion por si volvemos a entrar
        if (obj->getPosActualLista() >= obj->getSize() - 1){
            obj->setPosActualLista(0);
            obj->calcularScrPos();
        }
        //Ahora cargamos el siguiente menu
        cargaMenu(menucarga, valorSelec, evento);
    }
}

/**
*
*/
void Iofrontend::cargaMenu(int menucarga, string valorSelec, tEvento *evento){
    comprobarUnicode(menucarga);
    if (menucarga >= 0 && menucarga <= MAXMENU){
        this->setSelMenu(menucarga);
        //Damos el foco al primer elemento que haya en el menu
        this->ObjectsMenu[menucarga]->setFirstFocus();
    }

    tmenu_gestor_objects *objsMenu = ObjectsMenu[menucarga];
    UIListCommon *objTemp = NULL;

    switch (menucarga){
        default:
            break;
    }
}


/**
*
*/
int Iofrontend::accionesCargaPantalla(tEvento *evento){
    int posMenu = accionesGotoPantalla(evento);
    if (posMenu >= 0){
        cargaMenu(posMenu, "", evento);
        return true;
    }
    return false;
}



/**
* El campo checked del control se da valor desde la llamada al procEvent del respectivo menu
* En esta funcion se comprueba el valor checked para saber si se ha pulsado el elemento
*/
bool Iofrontend::procesarBoton(Object * obj, tmenu_gestor_objects *gestorMenu){
    try{
        //Traza::print("Checkeando name: " + string(name) + " valor: " + string(objMenu->getObjByName(name)->isChecked()?"S":"N"), W_ERROR);
        if (obj->getObjectType() == GUICOMBOBOX){
            if (((UIComboBox *)obj)->isValueChanged()){
                ((UIComboBox *)obj)->setValueChanged(false);
                ((UIComboBox *)obj)->setChecked(false);
                ((UIComboBox *)obj)->setFocus(false);
                gestorMenu->findNextFocus();
                return true;
            }
        } else if (obj->isChecked()){
            obj->setChecked(false);
            return true;
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion procesarBoton" + string(e.getMessage()), W_ERROR);
    }
    return false;
}

/**
* Se simula que se ha pulsado la tecla Intro o el boton aceptar del joystick
*/
int Iofrontend::simularEscape(tEvento *evento){
    evento->isKey = true;
    evento->key = SDLK_ESCAPE;
    evento->isJoy = true;
    evento->joy = JOY_BUTTON_START;
    return true;
}

/**
* Se simula que se ha pulsado la tecla Intro o el boton aceptar del joystick
*/
int Iofrontend::simularIntro(tEvento *evento){
    evento->isKey = true;
    evento->key = SDLK_RETURN;
    evento->isJoy = true;
    evento->joy = JOY_BUTTON_A;
    return true;
}

/**
*
*/
int Iofrontend::marcarBotonSeleccionado(tEvento *evento){

    int menu = getSelMenu();
    tmenu_gestor_objects *objMenu = ObjectsMenu[menu];
    int pos = objMenu->getFocus();

    Traza::print("marcarBotonSeleccionado: " + objMenu->getObjByPos(pos)->getName(), W_DEBUG);
    if (pos >= 0){
        objMenu->getObjByPos(pos)->setTag("selected");
        return true;
    }
    return false;
}

/**
* Se asigna la funcion especificada al boton con el nombre especificado.
* No deberian haber botones del mismo nombre
*/
void Iofrontend::addEvent(string nombre, typept2Func funcion){
    addEvent(nombre, funcion, -1);
}

/**
*
*/
void Iofrontend::addEvent(string nombre, typept2Func funcion, int parms){
    if (propertiesPt2Func.size < MAXMENU*MAXOBJECTS){
        propertiesPt2Func.name.push_back(nombre);
        propertiesPt2Func.parms.push_back(Constant::TipoToStr(parms));
        pt2Func[propertiesPt2Func.size] = funcion;
        propertiesPt2Func.size++;
    }
}

/**
* Se busca la posicion del puntero a funcion asociado con el nombre del boton
*/
int Iofrontend::findEventPos(string nombre){

    int i=0;

    while (i < propertiesPt2Func.size){
        if (nombre.compare(propertiesPt2Func.name[i]) == 0)
            return i;
        i++;
    }
    return -1;
}

/**
* Se busca la posicion del puntero a funcion asociado con el nombre del boton y se modifica
*/
void Iofrontend::setEvent(string nombre, typept2Func funcion){
    int pos = findEventPos(nombre);
    if (pos != -1){
        pt2Func[pos] = funcion;
    }
}

void Iofrontend::setEvent(string nombre, typept2Func funcion, int parms){
    int pos = findEventPos(nombre);
    if (pos != -1){
        pt2Func[pos] = funcion;
        propertiesPt2Func.parms[pos] = Constant::TipoToStr(parms);
    }
}

/**
*
*/
void Iofrontend::popUpMenu(tEvento evento){
}

/**
* Con el menu pasado por parametro lo dibujamos entero
*/
void Iofrontend::resizeMenu(){
    for (int i=0; i< MAXMENU; i++){
        ObjectsMenu[i]->setAreaObjMenu(getWidth(),getHeight());
    }
    setDinamicSizeObjects();
}

/**
*
*/
void Iofrontend::setDinamicSizeObjects(){
    try{
        //Calculamos el tamanyo del titulo de los elementos que lo tengan, y redimensionamos el elemento
        //lista que tenga ese menu con el total de la ventana que queda
//        for (int i=0; i<MAXMENU; i++){
//            try{
//                //ObjectsMenu[i]->getObjByName(TITLESCREEN)->setTam( 0, 0, this->getWidth(),Constant::getINPUTH());
//                int j = 0;
//                Object *posibleObj = NULL;
//
//                while (j < ObjectsMenu[i]->getSize()){
//                    posibleObj = ObjectsMenu[i]->getObjByPos(j);
//                    if(posibleObj != NULL){
//                        if (posibleObj->getObjectType() == GUILISTBOX || ObjectsMenu[i]->getObjByPos(j)->getObjectType() == GUIPICTURE
//                            || posibleObj->getObjectType() == GUILISTGROUPBOX){
//                            posibleObj->setTam(0,Constant::getINPUTH(), this->getWidth(), this->getHeight()-Constant::getINPUTH());
//                        }
//
//                        if (ObjectsMenu[i]->getObjByPos(j)->getObjectType() == GUILISTBOX){
//                            ((UIList *)posibleObj)->calcularScrPos();
//                        } else if (ObjectsMenu[i]->getObjByPos(j)->getObjectType() == GUILISTGROUPBOX){
//                            ((UIListGroup *)posibleObj)->calcularScrPos();
//                        }
//                    }
//                    j++;
//                }
//            } catch (Excepcion &e){}
//        }

        //Redimension para el browser de directorios2
        ObjectsMenu[PANTALLABROWSER2]->getObjByName(OBJLISTABROWSER2)->setTam(0, Constant::getINPUTH() + COMBOHEIGHT + 4,this->getWidth(), this->getHeight() - BUTTONH - Constant::getINPUTH() - COMBOHEIGHT - 10 - 4);
        ObjectsMenu[PANTALLABROWSER2]->getObjByName("comboBrowser")->setTam(1, Constant::getINPUTH() + 4, 160, 100);
        ObjectsMenu[PANTALLABROWSER2]->getObjByName(BTNACEPTARBROWSER)->setTam( (this->getWidth() / 2) -(BUTTONW + 5), this->getHeight() - BUTTONH - 5, BUTTONW,BUTTONH);
        ObjectsMenu[PANTALLABROWSER2]->getObjByName(BTNCANCELARBROWSER)->setTam( (this->getWidth() / 2) + 5, this->getHeight() - BUTTONH - 5, BUTTONW,BUTTONH);
        ObjectsMenu[PANTALLABROWSER2]->getObjByName(ARTDIRBROWSER)->setTam( 0, 0, this->getWidth(), Constant::getINPUTH());
        int mapPosY = Constant::getINPUTH()*3 + 25 * 2;

        ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox")->setTam(2, mapPosY, getWidth(), getHeight() - 20 - mapPosY - 5);
        ObjectsMenu[PANTALLAGPSINO]->getObjByName("progDownload")->setTam( 0, getHeight() - 20, getWidth(), 20);

    } catch (Excepcion &e){
        Traza::print("setDinamicSizeObjects: " + string(e.getMessage()), W_ERROR);
    }
}

/**
*
*/
bool Iofrontend::casoPANTALLACONFIRMAR(string titulo, string txtDetalle){
    ignoreButtonRepeats = true;
    Traza::print("casoPANTALLACONFIRMAR: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    bool salida = false;
    int menuInicial = getSelMenu();

    //Procesamos el menu antes de continuar para que obtengamos la captura
    //de pantalla que usaremos de fondo
    procesarControles(ObjectsMenu[menuInicial], &askEvento, NULL);
    SDL_Rect iconRectFondo = {0, 0, this->getWidth(), this->getHeight()};
    SDL_Surface *mySurface = NULL;
    drawRectAlpha(iconRectFondo.x, iconRectFondo.y, iconRectFondo.w, iconRectFondo.h , cNegro, 200);
    takeScreenShot(&mySurface, iconRectFondo);

    //Seguidamente cambiamos la pantalla a la de la confirmacion
    setSelMenu(PANTALLACONFIRMAR);
    tmenu_gestor_objects *objMenu = ObjectsMenu[PANTALLACONFIRMAR];
    objMenu->getObjByName("borde")->setLabel(titulo);

    UITextElementsArea *textElems = (UITextElementsArea *)objMenu->getObjByName("textosBox");
    textElems->setImgDrawed(false);
    textElems->setFieldText("labelDetalle", txtDetalle);

    int len = fontStrLen(txtDetalle);
    if (len < this->getWidth()){
        textElems->setW(len + 4 * INPUTBORDER + TEXLABELTEXTSPACE);
        textElems->setH(Constant::getMENUSPACE() + 2*INPUTBORDER);
        objMenu->centrarObjeto(textElems);
//        textElems->setX(this->getWidth() - len/2);
    } else {
        textElems->setTam(0, -50 * zoomText, getWidth()-50, 120);
        objMenu->centrarObjeto(textElems);
    }

    long delay = 0;
    unsigned long before = 0;
    objMenu->setFocus(0);

    do{
        before = SDL_GetTicks();
        askEvento = WaitForKey();
//        clearScr(cBgScreen);
        printScreenShot(&mySurface, iconRectFondo);

        procesarControles(objMenu, &askEvento, NULL);

        flipScr();
        salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
        (askEvento.isKey && askEvento.key == SDLK_ESCAPE);

        if (objMenu->getObjByName("btnSiConfirma")->getTag().compare("selected") == 0){
            salir = true;
            salida = true;
            objMenu->getObjByName("btnSiConfirma")->setTag("");
            Traza::print("Detectado SI pulsado", W_DEBUG);
        } else if (objMenu->getObjByName("btnNoConfirma")->getTag().compare("selected") == 0){
            salir = true;
            salida = false;
            objMenu->getObjByName("btnNoConfirma")->setTag("");
            Traza::print("Detectado NO pulsado", W_DEBUG);
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);

    setSelMenu(menuInicial);
    return salida;
}

/**
*
*/
string Iofrontend::casoPANTALLAPREGUNTA(string titulo, string label){
    ignoreButtonRepeats = true;
    Traza::print("casoPANTALLAPREGUNTA: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    string salida = "";
    int menuAnt = getSelMenu();
    setSelMenu(PANTALLAPREGUNTA);

    tmenu_gestor_objects *objMenu = ObjectsMenu[PANTALLAPREGUNTA];
    objMenu->getObjByName("valor")->setLabel(label);
    ((UIInput *)objMenu->getObjByName("valor"))->setText("");
    objMenu->getObjByName("borde")->setLabel(titulo);


    long delay = 0;
    unsigned long before = 0;

    do{
        before = SDL_GetTicks();
        askEvento = WaitForKey();
        clearScr(cBgScreen);
        procesarControles(objMenu, &askEvento, NULL);

        flipScr();
        salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
        (askEvento.isKey && askEvento.key == SDLK_ESCAPE);

        UIInput *input = (UIInput *)objMenu->getObjByName("valor");

        if (input->getSize() > 0 &&
            ( (askEvento.isKey && askEvento.key == SDLK_RETURN)
              || (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_A))) ){
            salida = input->getText();
            salir = true;
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);
    setSelMenu(menuAnt);

    return salida;
}


/**
*
*/
int Iofrontend::accionesGotoPantalla(tEvento *evento){
    int menu = this->getSelMenu();
    tmenu_gestor_objects *objsMenu = ObjectsMenu[menu];
    Object *object = objsMenu->getObjByPos(objsMenu->getFocus());

    int posBoton = findEventPos(object->getName());  //Buscamos la posicion del elemento en el array de punteros a funcion
    if (posBoton >= 0){ //Si hemos encontrado una funcion
        int posMenu = Constant::strToTipo<int>(propertiesPt2Func.parms[posBoton]);
        if (posMenu >= 0){
            this->setSelMenu(posMenu);
            this->ObjectsMenu[posMenu]->findNextFocus();
            comprobarUnicode(posMenu);
            return posMenu;
        }
    }
    return false;
}

/**
* En este metodo se comprueba si tenemos que activar unicode para que
* las teclas del teclado se traduzcan sin problemas de dependencia de layouts
*/
void Iofrontend::comprobarUnicode(int menu){

    tmenu_gestor_objects *objsMenu = ObjectsMenu[menu];
    int i=0;
    bool found = false;

    while (i < objsMenu->getSize() && !found){
        if (objsMenu->getObjByPos(i) != NULL)
            if (objsMenu->getObjByPos(i)->getObjectType() == GUIINPUTWIDE)
                found = true;
        i++;
    }

    Traza::print("comprobarUnicode: " + Constant::TipoToStr(menu) + ((found == true) ? " UNICODE=S":" UNICODE=N"), W_PARANOIC);
    SDL_EnableUNICODE(found);
}

/**
* Establece el nombre del fichero o directorio seleccionado por el explorador de
* ficheros, en el contenido un campo especificado por parámetro
*/
void Iofrontend::setTextFromExplorador(tEvento *evento, UIInput *objCampoEdit){
    try{
        Traza::print("Iofrontend::setTextFromExplorador", W_INFO);
        Dirutil dir;
        tmenu_gestor_objects *objMenu = ObjectsMenu[this->getSelMenu()];
        //Si el objeto ya tiene datos, comprobamos si existe el directorio que supuestamente contiene
        string uri = dir.getFolder(objCampoEdit->getText());
        if (dir.existe(uri)){
            //Si resulta que existe, hacemos un cambio de directorio para que se muestre
            //el contenido del directorio nada mas pulsar en el boton
            dir.changeDirAbsolute(uri.c_str());
        }
        //Abrimos el explorador de archivos y esperamos a que el usuario seleccione un fichero
        //o directorio
        string fichName = showExplorador(evento);
        //Si se ha seleccionado algo, establecemos el texto en el objeto que hemos recibido por parametro
        if (!fichName.empty()){
            objCampoEdit->setText(fichName);
            objMenu->setFocus(objCampoEdit->getName());
        }
    } catch (Excepcion &e){
        Traza::print("setTextFromExplorador: " + string(e.getMessage()), W_ERROR);
    }
}

/**
*
*/
string Iofrontend::showExplorador(tEvento *evento){
    Traza::print("showExplorador: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    int menuInicio = this->getSelMenu();
    this->setSelMenu(PANTALLABROWSER2);
    tmenu_gestor_objects *objMenu = ObjectsMenu[PANTALLABROWSER2];
    UIList *obj = NULL;
    ignoreButtonRepeats = true;
    string fileUri = "";
    string fileTempSelec = "";
    Dirutil dir;
    static string lastDirOpened;

    try{
        loadComboUnidades("comboBrowser", PANTALLABROWSER2, -1);
        obj = (UIList *)objMenu->getObjByName(OBJLISTABROWSER2);
        obj->setFocus(true);
        obj->setTag("");
        obj->setPosActualLista(-1);
        //Forzamos a que se actualicen todos los elementos
        objMenu->resetElements();
        //Seleccionamos a la lista que esta en primer lugar
        //objMenu->findNextFocus();
        ObjectsMenu[PANTALLABROWSER2]->setFocus(OBJLISTABROWSER2);

        if (!lastDirOpened.empty()){
            dir.changeDirAbsolute(dir.getFolder(lastDirOpened).c_str());
        }

        long delay = 0;
        unsigned long before = 0;
        this->accionesListaExplorador(evento);

        do{
            before = SDL_GetTicks();
            askEvento = WaitForKey();
            clearScr(cBlanco);
            int pos = obj->getPosActualLista();
            //Carga de imagenes de fondo en la pantalla del explorador de ficheros
            if (pos >= 0){
                fileTempSelec = obj->getListNames()->get(pos);
                string ruta = dir.getDirActual() +  Constant::getFileSep() + fileTempSelec;
                UIPicture *objPict = (UIPicture *)objMenu->getObjByName("ImgFondo");

                if (objPict->getImgGestor()->getRuta().compare(ruta) != 0){
                    if (dir.findIcon(fileTempSelec.c_str()) == page_white_picture){
                        objPict->loadImgFromFile(ruta);
                        objPict->getImgGestor()->setBestfit(false);
                    } else {
                        if (objPict->getImgGestor()->clearFile())
                            objPict->setImgDrawed(false);
                    }
                    obj->setImgDrawed(false);
                    objMenu->getObjByName(BTNACEPTARBROWSER)->setImgDrawed(false);
                    objMenu->getObjByName(BTNCANCELARBROWSER)->setImgDrawed(false);
                    objPict->getImgGestor()->setRuta(ruta);
                }
            }

            procesarControles(objMenu, &askEvento, NULL);
            if (objMenu->getFocus() < 0){
                objMenu->findNextFocus();
            }

            //fps();
            flipScr();
            salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
            ( ((askEvento.isKey && askEvento.key == SDLK_ESCAPE) || !obj->getTag().empty())
             || objMenu->getObjByName(BTNACEPTARBROWSER)->getTag().compare("selected") == 0
             || objMenu->getObjByName(BTNCANCELARBROWSER)->getTag().compare("selected") == 0);


            delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
            if(delay > 0) SDL_Delay(delay);
        } while (!salir);

    } catch (Excepcion &e){
        Traza::print("Error en showExplorador: " + string(e.getMessage()), W_ERROR);
    }

    this->setSelMenu(menuInicio);
    string fileSelec;
    string diractual = dir.getDirActual();

    if (obj != NULL){
        // En el caso de que se haya pulsado el boton aceptar, obtenemos el elemento seleccionado
        if (objMenu->getObjByName(BTNACEPTARBROWSER)->getTag().compare("selected") == 0){
            int pos = obj->getPosActualLista();
            if (pos >= 0){
                fileSelec = obj->getListNames()->get(pos);
                bool tieneFileSep = diractual.substr(diractual.length()-1).compare(Constant::getFileSep()) == 0;
                obj->setTag(diractual + (!tieneFileSep ? Constant::getFileSep() : "") + fileSelec);
            }
        }
        fileUri = obj->getTag();
    }

    try{
        objMenu->getObjByName(BTNCANCELARBROWSER)->setTag("");
        objMenu->getObjByName(BTNACEPTARBROWSER)->setTag("");
    } catch (Excepcion &e){
        Traza::print("showExplorador: reseteando botones: " + string(e.getMessage()), W_ERROR);
    }

    //No queremos que se seleccionen directorios incorrectos
    if (fileSelec.compare("..") == 0){
//        obj->setTag("");
//        showMessage("Directorio no valido.", 2000);
//        fileUri = showExplorador(evento);
        fileUri = fileUri.substr(0, fileUri.find_last_of(tempFileSep));
    }
    lastDirOpened = fileUri;
    return fileUri;
}

/**
*
*/
int Iofrontend::accionesListaExplorador(tEvento *evento){

    string fileSelec = "";

    try{
        Traza::print("Iofrontend::accionesListaExplorador", W_INFO);
        tmenu_gestor_objects *objMenu = ObjectsMenu[PANTALLABROWSER2];
        UIList * obj = (UIList *)objMenu->getObjByName(OBJLISTABROWSER2);
        Dirutil dir;
        bool dirChanged = false;
        string diractual;
        int pos = obj->getPosActualLista();

        if (evento == NULL){
            pos = 0;
            dirChanged = true;
        } else if (pos >= 0){
            string fileSelec = obj->getListNames()->get(pos);
            string valorSelec = obj->getListValues()->get(pos);
            Traza::print("cambiando al directorio: " + fileSelec, W_DEBUG);
            if (Constant::strToTipo<int>(valorSelec) == TIPODIRECTORIO){
                dirChanged = dir.changeDirRelative(fileSelec.c_str());
            } else {
                diractual = dir.getDirActual();
                obj->setTag(diractual + tempFileSep + fileSelec);
            }
        }

        //Reseteamos la posicion del explorador para el siguiente directorio
        obj->setPosActualLista(0);
        obj->calcularScrPos();
        //Actualizamos la barra principal con la ruta actual
        diractual = dir.getDirActual();

        ((UIArt *)objMenu->getObjByName(ARTDIRBROWSER))->setLabel(diractual);

        if (dirChanged || pos < 0){
            //Obtenemos el directorio actual y sus elementos
            listaSimple<FileProps> *filelist = new listaSimple<FileProps>();
            unsigned int numFiles = dir.listarDir(diractual.c_str(), filelist);
            Traza::print("Ficheros: " + Constant::TipoToStr(numFiles), W_DEBUG);

            if (filelist != NULL && numFiles > 0){
                //Hacemos espacio en la lista para que la asignacion sea rapida
                obj->resizeLista(numFiles);
                //Recorremos la lista de ficheros y lo incluimos en el objeto de lista para mostrar los datos
                for (unsigned int i = 0; i < numFiles; i++){
                    obj->addElemLista(filelist->get(i).filename , Constant::TipoToStr(filelist->get(i).filetype), filelist->get(i).ico );
                }
            } else {
               obj->resizeLista(1);
               obj->addElemLista(".." , Constant::TipoToStr(TIPODIRECTORIO), folder);
            }
            delete filelist;
        }


    }catch (Excepcion &e){
        Traza::print("accionesListaExplorador: " + string (e.getMessage()), W_ERROR);
    }

    ObjectsMenu[PANTALLABROWSER2]->setFocus(OBJLISTABROWSER2);
    return true;
}

/**
*
*/
void Iofrontend::loadComboUnidades(string objName, int pantalla, int types){
    Traza::print("Iofrontend::loadComboUnidades", W_INFO);
    UIList *combo = (UIList *)ObjectsMenu[pantalla]->getObjByName(objName);
    combo->clearLista();
    combo->setPosActualLista(0);
    vector<t_drive *> drives;

    Dirutil dir;
    dir.getDrives(&drives);
    int actualDrive = 0;
    string actualDir = dir.getDirActual();

    for (int i=0; i < drives.size(); i++){
        if (types == -1 || types == drives.at(i)->driveType){
            combo->addElemLista(drives.at(i)->drive.substr(0,2)
                                + " (" + drives.at(i)->driveTypeString + ") "
                                + drives.at(i)->label, drives.at(i)->drive, drives.at(i)->ico);
            if (actualDir.find(drives.at(i)->drive) != string::npos){
                actualDrive = i;
            }
        }
    }
    combo->setPosActualLista(actualDrive);
    combo->calcularScrPos();
}

/**
*
*/
int Iofrontend::accionCombo(tEvento *evento){
    Traza::print("Iofrontend::accionCombo", W_INFO);
    UIComboBox *combo = (UIComboBox *)ObjectsMenu[PANTALLABROWSER2]->getObjByName("comboBrowser");
    string unidad = combo->getValue(combo->getPosActualLista());
    Traza::print("Iofrontend::accionCombo. Drive: " + unidad, W_DEBUG);
    Dirutil dir;
    bool cambioDir = dir.changeDirAbsolute(unidad.c_str());

    if (cambioDir){
        Traza::print("Iofrontend::accionCombo. EXITO Drive: " + unidad, W_DEBUG);
    } else {
        Traza::print("Iofrontend::accionCombo. ERROR Drive: " + unidad, W_ERROR);
    }

    clearEvento(evento);
    this->accionesListaExplorador(NULL);
    ObjectsMenu[PANTALLABROWSER2]->setFocus(OBJLISTABROWSER2);
    return 0;
}

/**
* Carga el resultado de la seleccion del explorador de archivos en un campo de texto
* - El objeto boton que llama a este metodo debe tener el campo tag rellenado con el campo de destino
*   En el que se quiere que se guarde el resultado
*/
int Iofrontend::loadDirFromExplorer(tEvento *evento){

    try{
        Traza::print("Iofrontend::loadDirFromExplorer", W_INFO);
        //Obtenemos los objetos del menu actual
        tmenu_gestor_objects *objMenu = ObjectsMenu[this->getSelMenu()];
        //Obtenemos el objeto que ha sido seleccionado y que tiene el foco
        Object *obj = objMenu->getObjByPos(objMenu->getFocus());
        //Obtenemos el tag del elemento que indica en que campo deberemos dar valor a la seleccion
        //que hagamos del explorador de archivos
        string tag = obj->getTag();
        if (!tag.empty()){
            setTextFromExplorador(evento, (UIInput *)objMenu->getObjByName(tag));
        } else {
            Traza::print("loadDirFromExplorer. El objeto: " + obj->getLabel() + " no tiene tag asociado " +
                         "que indique donde cargar el texto del explorador", W_ERROR);
        }
    } catch (Excepcion &e){
        Traza::print("loadDirFromExplorer: " + string(e.getMessage()), W_ERROR);
    }
    return 0;
}

/**
*
*/
UIPopupMenu * Iofrontend::addPopup(int pantalla, string popupName, string callerName){
    UIPopupMenu * popup1 = NULL;

    try{
        tmenu_gestor_objects *objMenu = ObjectsMenu[pantalla];
        objMenu->getObjByName(callerName)->setPopupName(popupName);
        ObjectsMenu[pantalla]->add(popupName, GUIPOPUPMENU, 0, 0, 170, 100, popupName, false)->setVisible(false);
        popup1 = (UIPopupMenu *) objMenu->getObjByName(popupName);
        popup1->setFont(getFont());
        popup1->setAutosize(true);
    } catch (Excepcion &e){
        Traza::print("addPopup: " + string(e.getMessage()), W_ERROR);
    }

    return popup1;
}

/**
* Desde este menu podremos mostrar por pantalla la informacion de las roms de cada emulador
*/
bool Iofrontend::procesarMenuActual(tmenu_gestor_objects *objMenu, tEvento *evento){

    return true;
}

/**
* Muestra el menu pasado por parametro como si fuera emergente, dejando el menu anterior
* difuminado sobre el fondo
*/
void Iofrontend::showMenuEmergente(int menu, string objImagenFondo){
    try{
         Traza::print("Iofrontend::showMenuEmergente", W_INFO);
        //Procesamos el menu actual para que se vuelva a repintar
        procesarControles(ObjectsMenu[getSelMenu()], new tEvento(), NULL);
        //Seleccionamos el menu que queremos mostrar como si fuese emergente
        tmenu_gestor_objects *objMenu = ObjectsMenu[menu];
        //Realizamos una captura de pantalla en el objeto imagen de fondo del menu de destino
        UIPicture * fondoImg = (UIPicture *) objMenu->getObjByName(objImagenFondo);
        //Le damos un toque mas oscuro a la imagen
        fondoImg->setAlpha(150);
        //Obtenemos una captura de la pantalla entera
        takeScreenShot(&fondoImg->getImgGestor()->surface);
        //Seleccionamos el menu de destino y procesamos sus controles para que se pinten
        setSelMenu(menu);
        tEvento askEvento;
        clearEvento(&askEvento);
        procesarControles(objMenu, &askEvento, NULL);
    } catch (Excepcion &e) {
         Traza::print("Excepcion showMenuEmergente: " + string(e.getMessage()), W_ERROR);
    }

}

/**
*
*/
bool Iofrontend::procesarPopups(tmenu_gestor_objects *objMenu, tEvento *evento){
        Object *object = objMenu->getObjByPos(objMenu->getFocus());
        if (object != NULL){
            try{
                //Comprobamos si el elemento que estamos pintando deberia mostrar su menu de popup
                if (object->isPopup() && object->isFocus()){
                    Traza::print("procesarPopups", W_DEBUG);
                    //Obtenemos el objeto popup
                    UIPopupMenu *objPopup = (UIPopupMenu *)objMenu->getObjByName(object->getPopupName());
                    //Mostramos el popup
                    objPopup->setVisible(true);
                    //Seteamos la posicion del menu popup
                    if (evento->mouse_x > 0 && evento->mouse_y > 0){
                        //Si no tenemos espacio horizontal por la derecha, intentamos mostrar el popup por
                        //la izquierda del lugar clickado
                        if (evento->mouse_x + objPopup->getW() > this->getWidth()){
                            objPopup->setX(evento->mouse_x - objPopup->getW());
                        } else {
                            objPopup->setX(evento->mouse_x);
                        }
                        //Si no tenemos espacio vertical por debajo, intentamos mostrar el popup por
                        //encima del lugar clickado
                        if (evento->mouse_y + objPopup->getH() > this->getHeight()){
                            objPopup->setY(evento->mouse_y - objPopup->getH());
                        } else {
                            objPopup->setY(evento->mouse_y);
                        }
                    }
                    //Asignamos el elemento que ha llamado al popup
                    objPopup->setCallerPopup(objMenu->getObjByPos(objMenu->getFocus()));
                    //Damos el foco al popup
                    objMenu->setFocus(object->getPopupName());
                    //Evitamos que el elemento procese cualquier evento mientras aparezca el popup
                    object->setEnabled(false);
                    //Forzamos a que se refresque el elemento padre que lanzo el popup
                    object->setImgDrawed(false);
                }
            } catch (Excepcion &e) {
                Traza::print("Excepcion en popup: " + object->getPopupName() + ". " + string(e.getMessage()), W_ERROR);
            }
        }

    return true;
}



/**
* Copia el texto seleccionado desde un popup al elemento que lo llama. Por ahora solo lo hace
* en campos input.
*/
int Iofrontend::accionCopiarTextoPopup(tEvento *evento){
    Traza::print("accionCopiarTextoPopup", W_INFO);
    //Se obtiene el objeto menupopup que en principio esta seleccionado
    int menu = this->getSelMenu();
    tmenu_gestor_objects *objsMenu = ObjectsMenu[menu];
    Object *obj = objsMenu->getObjByPos(objsMenu->getFocus());
    //Comprobamos que efectivamente, el elemento es un popup
    if (obj->getObjectType() == GUIPOPUPMENU){
        UIPopupMenu *objPopup = (UIPopupMenu *)obj;
        //Obtenemos el valor del elemento seleccionado en el popup
        string selected = objPopup->getListValues()->get(objPopup->getPosActualLista());
        if (objPopup->getCallerPopup() != NULL){
            //Obtenemos el objeto llamador
            if (objPopup->getCallerPopup()->getObjectType() == GUIINPUTWIDE){
                UIInput *objInput = (UIInput *)objPopup->getCallerPopup();
                //Anyadimos el texto al input
                objInput->setText(objInput->getText() + selected);
                //Devolvemos el foco al elemento que llamo al popup
                objsMenu->setFocus(objPopup->getCallerPopup()->getName());
            }
        }
    }
    return 0;
}


/***************************************************************************************************************/
/**                                ACCIONES DE LOS CAMPOS DE LA APLICACION                                     */
/***************************************************************************************************************/

/**
*
*/
string Iofrontend::casoJOYBUTTONS(tEvento *evento){
    ignoreButtonRepeats = true;
    configButtonsJOY(evento);
    return "";
}

/**
*
*/
int Iofrontend::openGpx(tEvento *evento){
    Traza::print("Iofrontend::openGpx", W_INFO);
    long delay = 0;
    unsigned long before = 0;

    try{
        Dirutil dir;
        //Abrimos el explorador de archivos y esperamos a que el usuario seleccione un fichero
        //o directorio
        string fichName = showExplorador(evento);
        //Si se ha seleccionado algo, establecemos el texto en el objeto que hemos recibido por parametro

        Traza::print("Iofrontend::openGpx. Cargando GPX: " + fichName, W_DEBUG);
        if (!fichName.empty()){
            fileGPX = fichName;
            ((UIInput *) ObjectsMenu[PANTALLAGPSINO]->getObjByName("inputFileGPX"))->setText(fileGPX);
            this->analyzeGpx(fichName);
        }
    } catch (Excepcion &e){
        Traza::print("Iofrontend::openGpx: " + string(e.getMessage()), W_ERROR);
    }
    return 0;
}

int Iofrontend::chActionAround(tEvento *evento){
//    addEvent("chDownAround", &Iofrontend::chActionAround);
//    addEvent("chDownFull", &Iofrontend::chActionFull);
//    if (evento->isMouse && evento->mouse_state == SDL_RELEASED)
//        ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownFull")->setChecked(
//            ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownAround")->isChecked());

    return 0;
}
int Iofrontend::chActionFull(tEvento *evento){
    if (evento->isMouse && evento->mouse_state == SDL_RELEASED)
        ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownAround")->setChecked(
            ObjectsMenu[PANTALLAGPSINO]->getObjByName("chDownFull")->isChecked());
    return 0;
}


/**
*
*/
int Iofrontend::downloadMaps(tEvento *evento){
    Traza::print("Iofrontend::downloadMaps", W_INFO);
    long delay = 0;
    unsigned long before = 0;
    int totalTiles = 0;
    UICheck *tmpCheck;
    FILE *fp;
    Dirutil dir;

    try{
//        string processedFile = dir.getFolder(fileGPSData) + FILE_SEPARATOR
//                                + dir.getFileNameNoExt(fileGPSData) + "_" + Constant::TipoToStr(geoDrawer->getZoomMeters()) +  "_px.dat";
        Traza::print("Descargando mapas: " + fileGPSData, W_DEBUG);

        if (!dir.existe(fileGPX) || fileGPX.empty()){
            showMessage("Debe cargar un fichero gpx valido", 3000);
        } else {
            t_downMapData in;
            in.server = OPENCYCLEMAP;
            in.dirImgDown = dir.getFolder(fileGPSData);
            in.fileGpx = fileGPX;
            in.calcNTiles = true;
            //Comprobamos el numero de mapas a descargar
            scrapper->setObjectsMenu(ObjectsMenu[PANTALLAGPSINO]);
            scrapper->setMapData(in);
            scrapper->downImgRoute();
            totalTiles = scrapper->getDownMapData().nTiles;

            Traza::print("numero mapas: " + Constant::TipoToStr(totalTiles), W_DEBUG);
            string msg = "Se descargaran " + Constant::TipoToStr(totalTiles);
            msg.append(" imagenes. Deseas continuar?");
            bool continuar = casoPANTALLACONFIRMAR("Advertencia", msg);
            if (continuar){
                UIProgressBar *progressBar = NULL;
                progressBar = (UIProgressBar *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("progDownload");
                progressBar->setProgressMax(totalTiles);
                progressBar->setProgressPos(0);
                in.calcNTiles = false;
                scrapper->setMapData(in);
                threadMaps = new Thread<Scrapper>(scrapper, &Scrapper::downImgRoute);
                threadMaps->start();
            }
        }
    } catch (Excepcion &e){
        Traza::print("Iofrontend::downloadMaps: " + string(e.getMessage()), W_ERROR);
    }
    return 0;
}

int Iofrontend::actionInstallMaps(tEvento *evento){

    if (fileGPSData.empty()){
        showMessage("Es necesario cargar un track previamente", 2000);
    } else {
        string fichName = showExplorador(evento);
        Dirutil dir;
        const string dirGpsino = "GPSINO";
        const string dirRutas = "rutas";


        if (!fichName.empty()){
            string unidad = fichName.substr(0,fichName.find_first_of(Constant::getFileSep()));
            unidad += Constant::getFileSep();
            //showMessage(string("se selecciona ") +unidad, 2000);
            string pathRutas = unidad + dirGpsino;// + Constant::getFileSep() + dirRutas;
            dir.createDir(pathRutas);
            pathRutas += Constant::getFileSep() + dirRutas;
            dir.createDir(pathRutas);
            string code = casoPANTALLAPREGUNTA(Constant::toAnsiString("Renombrar el track"),
                                               Constant::toAnsiString("Con qué nombre deseas que se guarde el track?"));
            if (!code.empty()){
                pathRutas += Constant::getFileSep() + code;
                dir.createDir(pathRutas);
                string dirGpxData = dir.getFolder(fileGPSData);
                vector <FileProps> filelist;
                dir.listarDirRecursivo(dirGpxData, &filelist, ".dat .565");
                bool isDat, isPx;

                for (int i=0; i < filelist.size(); i++){
                    if (filelist.at(i).filetype == TIPOFICHERO){

                        //Contiene la ruta de destino donde se escribirá el fichero
                        string imgZoomDir = pathRutas +
                             filelist.at(i).dir.substr(dirGpxData.length());

    //                    Traza::print("imgZoomDir: " + imgZoomDir, W_DEBUG);
                        if (!dir.existe(imgZoomDir)){
                            dir.mkpath(imgZoomDir.c_str(), 0777);
                        }

                        string filenamedest = filelist.at(i).filename;
                        isDat = filenamedest.find(".dat") != string::npos;
                        isPx = filenamedest.find("_px.dat") != string::npos;

                        //Renombramos para que lo lea bien el arduino
                        if (isPx){
                            size_t posIni = filenamedest.find("_simple_") + string("_simple_").length();
                            size_t posFin = filenamedest.find_last_of("_");
                            if(posIni != string::npos && posFin != string::npos){
                                filenamedest = filenamedest.substr(posIni, posFin - posIni) + ".dat";
                            }
                        }
                        //Copiamos el fichero al destino
                        if (!isDat || isPx){
                            dir.copyFile(filelist.at(i).dir + Constant::getFileSep()
                                    + filelist.at(i).filename,
                                     imgZoomDir + Constant::getFileSep()
                                    + filenamedest);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int Iofrontend::installMaps(tEvento *evento){

    return 0;
}

/**
*
*/
void Iofrontend::analyzeGpx(string ruta){
    tmenu_gestor_objects *pantRepr = ObjectsMenu[PANTALLAGPSINO];
    Traza::print("Iofrontend::analyzeGpx. Analizando ruta: " + ruta, W_INFO);

    Dirutil dir;
    /**Para generar los ficheros procesados y simplificados*/
    string dataGPS = procesaGPX(ruta, 180);
    Traza::print("Iofrontend::analyzeGpx. Generando fichero", W_INFO);
    string fileNameSimple = dir.getFolder(dataGPS) + FILE_SEPARATOR + dir.getFileNameNoExt(dataGPS) + "_simple.dat";
    Traza::print("Iofrontend::analyzeGpx. Guardando fichero", W_INFO);
    //Para generar unos puntos de ejemplo a seguir pulsando la tecla d
    generarFicheroRuta(dataGPS, fileNameSimple, googleZoom[0], 0.0, false);

    Traza::print("Iofrontend::analyzeGpx. Cargando ruta", W_INFO);
    /**Para cargar la ruta*/
    cargarFicheroRuta(fileNameSimple, geoDrawer->getZoomMeters());
    Traza::print("Iofrontend::analyzeGpx. Cargando estadisticas", W_INFO);
    geoDrawer->logEstadisticasRuta();

    mapUtilPoint.posTempArray = 0;
    mapUtilPoint.latestCoord.reset();
    mapUtilPoint.actualCoord.reset();
    mapUtilPoint.latestPixelToDist = 0;
    mapUtilPoint.lastPointVisited = 0;
}

/**
*
*/
string Iofrontend::procesaGPX(string filename, int angulo){
    GpxLoader loader;
    Dirutil dir;
    string dataGPS;

    if (loader.loadGPX(filename)){
        dataGPS = loader.SaveToFile(filename);
        double maxAnguloRad = GeoStructs::toRadians(angulo);
        cout << "dataGPS: " << dataGPS << endl;
        string fileNameSimple = dir.getFolder(dataGPS) + FILE_SEPARATOR
                            + dir.getFileNameNoExt(dataGPS) + "_simple.dat";
        UICheck *tmpCheck;

        for (int i=0; i < sizeof(googleZoom)/sizeof(int); i++){
            tmpCheck = (UICheck *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("checkZoom" + Constant::TipoToStr(googleZoom[i]));
            if (tmpCheck->isChecked()){
                //Generamos los mismos datos que el fichero gpx original, pero procesado.
                //generarFicheroRuta(dataGPS, dataGPS, googleZoom[i], 0.0, true);
                //Generamos el fichero con datos simplificados
                generarFicheroRuta(dataGPS, fileNameSimple, googleZoom[i], maxAnguloRad, true);
            }
        }
    }
    return dataGPS;
}
/**
*
*/
int Iofrontend::leerCamposDat(FILE *fp, PosMapa *dataFromFile){
    const int maxLine = 50;
    static int n=0;
    char line[maxLine];
    int data;

    bool endOfLine = false;
    static int lineLen = 0;
    static int nLine = 0;
    static int nCampo = 0;
    static char tmpLine[maxLine];
    static int buffPos=0;
    bool endOfFile = false;

    if (ftell(fp) == 0){
        lineLen = 0;
        nLine = 0;
        nCampo = 0;
        buffPos = 0;
        memset (tmpLine,0,maxLine);
    }

    while (!endOfLine && !endOfFile){
//        cout << "buffPos: " << buffPos << " tmpLine: " << tmpLine << endl;
        if (buffPos == maxLine || buffPos == 0){
            n = fread(tmpLine, 1, maxLine, fp);
            endOfFile = (n < maxLine);
        }
//        cout << "leido tmpLine: " << tmpLine << endl;
        //Se leen los datos mediante un buffer para aumentar rendimiento
        //En lugar de leer byte a byte
        if (!endOfFile){
            buffPos = buffPos % maxLine;
            for (buffPos; buffPos < maxLine; buffPos++){
                data = tmpLine[buffPos];
                endOfLine = (data == '\n');
                if (data != ',' && !endOfLine && lineLen <= maxLine-1 && !(data == ';')){
                  line[lineLen] = data;
                  lineLen++ % maxLine;
                } else {
                    line[lineLen >= maxLine ? maxLine-1 : lineLen] = '\0';
                    lineLen = 0;
//                        cout << "Campo " << nCampo <<  ": " << line << endl;
                    //Leemos el campo obtenido y almacenamos los valores en un objeto
                    leerLineaFicheroRuta(line, nLine, &nCampo, dataFromFile);
                    nCampo++;
                }

                if (endOfLine){
                    if (nLine == LINETOPOINFO)
                        geoDrawer->setNumTopo((geoDrawer->getNumTopo() + 1) % 10);

                    nLine++;
                    nCampo = 0;
//                    cout << "newLine!! " << nLine << " n: " << n << " lineLen: " << lineLen  << endl;
                    /**
                      * Debemos abandonar el bucle para que se pueda procesar los datos de la linea
                      * correspondiente
                     */
                    buffPos = buffPos + 1;
                    return nLine-1;
                } else if (data == ';' && nLine == LINETOPOINFO){
                    nCampo = 0;
                    geoDrawer->setNumTopo((geoDrawer->getNumTopo() + 1) % 10);
                }
            }
        }
    }

    if (endOfFile)
        return EOF;
    else
        return nLine-1;

}

/**
* http://stackoverflow.com/questions/2103924/mercator-longitude-and-latitude-calculations-to-x-and-y-on-a-cropped-map-of-the
*/
void Iofrontend::drawMapArduino(tEvento *evento){

    const int maxLine = 50;
    const int maxDistPxWaypoint = 10;
    //100 bytes de limite
    char line[maxLine];
    FILE *fp;
    Dirutil dir;
    string processedFile = dir.getFolder(fileGPSData) + FILE_SEPARATOR
                            + dir.getFileNameNoExt(fileGPSData) + "_" + Constant::TipoToStr(geoDrawer->getZoomMeters()) +  "_px.dat";

    //cout << "procesando: " << processedFile << endl;

    fp = fopen(processedFile.c_str(), "r"); // error check this!
    if (fp != NULL){
        Point posXY;
        Point posXY2;
        double minDist = 0.0;
        double latestMinDist = 0.0;
        double tempDist = 0.0;
        double distFromStart = 0.0;
        double eleActual = 0.0;
        bool foundDist = false;
        int data;
        int puntos = 0;
        int lineLen = 0;


        // ** Cargamos de la lista la posicion gps actual y centramos la pantalla
        //  * Esto se tiene que sustituir por la posicion obtenida por el modulo gps para saber la pos actual
        if (mapUtilPoint.posTempArray == 0){
            //currentGPSPos.setLatitude(40.453494);
            //currentGPSPos.setLongitude(-1.291035);
            std::vector<std::string> strSplitted = Constant::split(posiciones->at(mapUtilPoint.posTempArray),",");
            mapUtilPoint.currentGPSPos.setLatitude(geoDrawer->todouble(strSplitted.at(0)));
            mapUtilPoint.currentGPSPos.setLongitude(geoDrawer->todouble(strSplitted.at(1)));
            mapUtilPoint.lastGPSPos.setLatitude(geoDrawer->todouble(strSplitted.at(0)));
            mapUtilPoint.lastGPSPos.setLongitude(geoDrawer->todouble(strSplitted.at(1)));
        }

        UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");
        //Acciones del teclado

        string umbralStr;

        if (evento != NULL){
            if (evento->isKey && evento->key == SDLK_q){
                umbral.r--;
            } else if (evento->isKey && evento->key == SDLK_w){
                umbral.r++;
            } else if (evento->isKey && evento->key == SDLK_e){
                umbral.g--;
            } else if (evento->isKey && evento->key == SDLK_r){
                umbral.g++;
            } else if (evento->isKey && evento->key == SDLK_t){
                umbral.b--;
            } else if (evento->isKey && evento->key == SDLK_y){
                umbral.b++;
            }

            if (evento->isKey && (evento->key == SDLK_q || evento->key == SDLK_w
                                  || evento->key == SDLK_e || evento->key == SDLK_r
                                  || evento->key == SDLK_t || evento->key == SDLK_y)){

                umbralStr = "umbral: r=" + Constant::TipoToStr(umbral.r) + ", g="
                            + Constant::TipoToStr(umbral.g) + ", b="
                            + Constant::TipoToStr(umbral.b);

                Traza::print(umbralStr.c_str(), W_DEBUG);
            }

            if (evento->isKey && evento->key == SDLK_a){
                
                int res = 0;
                //If we don't find the maps for the zoom, we try to find some other
                //zooms until we arrive finally to the same level of zoom or find 
                //some
                int iniZoom = geoDrawer->getZoomLevel();
                do{
                    geoDrawer->incZoomLevel();
                    //Cargamos la ruta de nuevo con la informacion de pixels precalculada para la ruta
                    cout << "Buscando con el zoom " <<  geoDrawer->getZoomMeters() << endl;
                    res = cargarFicheroRuta(fileGPSData, geoDrawer->getZoomMeters());
                    cout << ", resultado: " << res << endl;
                } while (res == 1 && iniZoom != geoDrawer->getZoomLevel());
                
                fclose(fp);
                processedFile = dir.getFolder(fileGPSData) + FILE_SEPARATOR
                                + dir.getFileNameNoExt(fileGPSData) + "_" + Constant::TipoToStr(geoDrawer->getZoomMeters()) +  "_px.dat";
                fp = fopen(processedFile.c_str(), "r");
                

            } else if (evento->isKey && (evento->key == SDLK_d || evento->key == SDLK_s)) {
                //Evitamos overflow si sobrepasamos el tamanyo del track
                mapUtilPoint.lastGPSPos = mapUtilPoint.currentGPSPos;
                mapUtilPoint.posTempArray = mapUtilPoint.posTempArray % (posiciones->size() - 1);
                //Para ir en sentido contrario

                if (evento->key == SDLK_s){
                    if (mapUtilPoint.posTempArray > 0){
                        mapUtilPoint.posTempArray--;
                    } else {
                        mapUtilPoint.posTempArray = posiciones->size() - 1;
                    }
                }

                if (mapUtilPoint.posTempArray == 0){
                    mapUtilPoint.latestPixelToDist = 0;
                }

                //** Cargamos de la lista la posicion gps actual y centramos la pantalla
                //* Esto se tiene que sustituir por la posicion obtenida por el modulo gps para saber la pos actual
                if (posiciones->size() > mapUtilPoint.posTempArray){
                    std::vector<std::string> strSplitted = Constant::split(posiciones->at(mapUtilPoint.posTempArray),",");
                    mapUtilPoint.currentGPSPos.setLatitude(geoDrawer->todouble(strSplitted.at(0)));
                    mapUtilPoint.currentGPSPos.setLongitude(geoDrawer->todouble(strSplitted.at(1)));
                }

                if (evento->key == SDLK_d){
                    mapUtilPoint.posTempArray++;
                }
            }
        }
    //    drawText(Constant::TipoToStr(posTempArray).c_str(), 20, 20, cNegro);
        calcularPixels(&mapUtilPoint.lastGPSPos, &mapUtilPoint.currentGPSPos, &mapUtilPoint.latestCoord, &mapUtilPoint.actualCoord);
        pintarCapaTerreno(&mapUtilPoint.currentGPSPos);

        int nCampos = 0;
        PosMapa dataFromFile;
        PosMapa lastDataFromFile;
        string waypointText;
        bool hasWaypoint = false;
        geoDrawer->setNumTopo(0);
        int pendiente = 0;
        bool foundPendiente = false;
        int n=0;
        char tmpLine[maxLine];

        while ((puntos = leerCamposDat(fp, &dataFromFile)) >= 0){
            //cout << "linea leida" << endl;
            if (puntos >= LINETRACKDATA){
                //Procesamos la linea y obtenemos los datos para pintar el track
                //si estamos en la linea de datos y es el final de la linea
                if (puntos > LINETRACKDATA){
                    //Hasta que no tenemos dos puntos no podemos pintar
                    posXY2.x =  dataFromFile.point.x + geoDrawer->getMapOffsetX();
                    posXY2.y =  dataFromFile.point.y + geoDrawer->getMapOffsetY();
                    //cout << "Linetrack "  << puntos << posXY2.x << "," << posXY2.y << endl;
                    /**Dibujamos la linea del track que une los puntos. No podemos limitar porque es posible que
                    * al simplificar puntos, estos queden fuera de la pantalla, pero su vector si que puede pasar
                    * por pantalla.
                    * TODO: Posibilidad de calcular si quedan fuera de pantalla*/
//                    pintarPointLinea(posXY.x, posXY.y, posXY2.x, posXY2.y, puntos < lastPointVisited ? cRojo : cNegro,
//                                     objPict->getImgGestor()->getSurface());
                    plotLineWidth(posXY.x, posXY.y, posXY2.x, posXY2.y, 0 ,puntos < mapUtilPoint.lastPointVisited ? cRojo : cNegro,
                                  objPict->getImgGestor()->getSurface());


//                    int dist = sqrt(pow(posXY.x - posXY2.x,2) + pow(posXY.y - posXY2.y,2));
//                    if (dist > 30){
//                        cout << "puntos: "<< puntos << " -> "
//                            << dataFromFile.point.x << ":" << dataFromFile.point.y << " .... "
//                            << posXY.x  << "," << posXY.y  << " - "
//                            << posXY2.x << "," << posXY2.y << endl;
//                    }

                    //Dibujamos el texto del waypoint. Tambien dibujamos siempre un punto indicando su localizacion
                    if (!lastDataFromFile.name.empty()){
                        if (drawWaypoints && posXY.x <= mapUtilPoint.actualCoord.x + maxDistPxWaypoint && posXY.x >= mapUtilPoint.actualCoord.x - maxDistPxWaypoint
                            && posXY.y <= mapUtilPoint.actualCoord.y + maxDistPxWaypoint && posXY.y >= mapUtilPoint.actualCoord.y - maxDistPxWaypoint){
                            hasWaypoint = true;
                            waypointText = lastDataFromFile.name;
                        }

                        if (drawWaypoints && posXY.x < this->getWidth() && posXY.y < this->getHeight()){
                            drawRect(posXY.x, posXY.y, 4, 4, cVerde, objPict->getImgGestor()->getSurface());
                        }
                    }

                    //drawText(Constant::TipoToStr(lastDataFromFile.angle).c_str(), posXY.x, posXY.y, cAzulOscuro);

                    //Calculamos el modulo del vector para saber cual es el que tiene menor distancia con la pos actual
                    //Esto se deberia calcular segun la lat y lon
                    tempDist = sqrt(pow(posXY.x - mapUtilPoint.actualCoord.x, 2) + pow(posXY.y - mapUtilPoint.actualCoord.y, 2));

                    //Workaround para mostrar bien los datos de distancia hasta cumbre
                    if (tempDist < latestMinDist && puntos < mapUtilPoint.latestPixelToDist && mapUtilPoint.latestPixelToDist > 0){
                        mapUtilPoint.latestPixelToDist = puntos;
                    }

                    //Comprobamos si este es el punto del mapa mas cercano al actual
                    if (tempDist < minDist && puntos >= mapUtilPoint.latestPixelToDist){
                        minDist = tempDist;
                        distFromStart = lastDataFromFile.distancia;
                        eleActual = lastDataFromFile.ele;
                        mapUtilPoint.latestPixelToDist = puntos;
                        foundDist = true;
                    }

                    //Se calcula la pendiente de los proximos 100 metros
                    //if (foundDist && !foundPendiente){
                    if (puntos >= mapUtilPoint.lastPointVisited ){
                        double difDist = lastDataFromFile.distancia - distFromStart;
                        int difAltura = lastDataFromFile.ele - eleActual;
                        if (difDist >= 100.0 && difDist > 1.0 && !foundPendiente){
                            //Calculamos la pendiente. Siempre tiene que ser sobre 100m
                            pendiente = difAltura / difDist * 100;
                            foundPendiente = true;
                        }
                    }
                }

                //Guardamos para saber el punto anterior
                posXY.x =  dataFromFile.point.x + geoDrawer->getMapOffsetX();
                posXY.y =  dataFromFile.point.y + geoDrawer->getMapOffsetY();
    //                cout << "Linetrack anterior"  << puntos << posXY.x << "," << posXY.y << endl;

                lastDataFromFile = dataFromFile;

            } else if (puntos == LINEGPXCONFIG){
    //                cout << "LINEGPXCONFIG" << endl;
                //Despues de que se haya leido la linea de inicializacion, calculamos datos de pixels
                calcularPixels(&mapUtilPoint.lastGPSPos, &mapUtilPoint.currentGPSPos,
                               &mapUtilPoint.latestCoord, &mapUtilPoint.actualCoord);

            } else if (puntos == LINEGPXSTATS){
    //                cout << "LINEGPXSTATS" << endl;
                minDist = geoDrawer->getDistancia();
                latestMinDist = minDist;
            }
        }
        fclose(fp);

        //Si por lo que sea no hemos encontrado el punto mas cercano, establecemos el punto actual
    //    if (foundDist == false){
    //        latestPixelToDist = posTempArray;
    //        distFromStart = listaPixels->at(latestPixelToDist).distancia;
    //        eleActual = listaPixels->at(latestPixelToDist).ele;
    //    }


        SDL_Rect dest = {objPict->getX(),objPict->getY(),objPict->getW(), objPict->getH()};
        SDL_Rect orig = {0,0,objPict->getW(), objPict->getH()};
        SDL_BlitSurface(objPict->getImgGestor()->getSurface(), &orig, screen, &dest);

        Point actualCoordRelativeToImg = mapUtilPoint.actualCoord;
        actualCoordRelativeToImg.x += objPict->getX();
        actualCoordRelativeToImg.y += objPict->getY();
        pintarFlechaGPS(&actualCoordRelativeToImg, geoDrawer->calculaAnguloDireccion(&mapUtilPoint.latestCoord, &mapUtilPoint.actualCoord));
        mostrarDatosRuta(pendiente, distFromStart, eleActual, waypointText.c_str());

        latestMinDist = minDist;
        mapUtilPoint.lastPointVisited = mapUtilPoint.latestPixelToDist;
    }
}

/**
*
*/
void Iofrontend::mostrarDatosRuta(int pendiente, double distFromStart, double eleActual, const char *waypointText){
    UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");

    //Calculamos la distancia visible por pantalla en km segun el zoom que
    //hagamos
//    double maxDistScreenX = geoDrawer->calculaEscala();
//    string escala = "";
//    if (maxDistScreenX > 1.0){
//        escala = Constant::TipoToStr((int)maxDistScreenX) + " km.";
//    } else {
//        escala = Constant::TipoToStr(((int)(maxDistScreenX * 1000.0))) + " m.";
//    }
//    escala = "Escala: " + escala;
//    drawText( escala.c_str() , 0, 0, cNegro);

    //string texto = "Alt: " + Constant::TipoToStr<double>(listaPixels->at(posActual).ele) + " m";
//    string texto = "Alt: " + Constant::TipoToStr<double>(eleActual) + " m";
//    drawText(texto.c_str(), 0, 14, cNegro);

    double distToCheckpoint = 0.0;
    double eleToCheckpoint = 0.0;
    int tipoTopologia = 0;
    /*
    bool found = false;
    int totalValles = 0;
    int totalCumbres = 0;
    int posValle = 0;
    int posCumbre = 0;
//    string strTipoTopologia = "";


    for (int i=0; i < geoDrawer->getNumTopo(); i++){
        CumbreValle topologia = geoDrawer->arrayCumbresYValles[i];
        if (topologia.dist > distFromStart && !found){
            distToCheckpoint = topologia.dist - distFromStart;
            eleToCheckpoint = topologia.ele - eleActual;
            tipoTopologia = topologia.tipo;
            if (tipoTopologia == CUMBRE){
                posCumbre++;
//                strTipoTopologia = Constant::TipoToStr(posCumbre) + "/";
            } else {
                posValle++;
//                strTipoTopologia = Constant::TipoToStr(posValle)+ "/";
            }
            found = true;
        } else if (topologia.dist < distFromStart && !found){
            if (topologia.tipo == CUMBRE){
                posCumbre++;
            } else {
                posValle++;
            }
        }

        if (topologia.tipo == CUMBRE){
            totalCumbres++;
        } else {
            totalValles++;
        }
    }
    */

//    if (tipoTopologia == CUMBRE){
//        strTipoTopologia = strTipoTopologia + Constant::TipoToStr(totalCumbres);
//    } else {
//        strTipoTopologia = strTipoTopologia + Constant::TipoToStr(totalValles);
//    }

    if (drawWaypoints && strcmp(waypointText, "") != 0){
        //pintarLinea(0, getHeight() - 16, getWidth(), getHeight() - 16, cNegro);
        drawRect(objPict->getX(), objPict->getY() + objPict->getH() - 15, objPict->getW(), 15, cNegro);
        drawText(waypointText, objPict->getX() + 1, objPict->getY() + objPict->getH() - 15, cBlanco);
    }

    for (int i=geoDrawer->getNumTopo(); i >= 0; i--){
        CumbreValle topologia = geoDrawer->arrayCumbresYValles[i];
        if (topologia.dist > distFromStart){
            distToCheckpoint = topologia.dist - distFromStart;
            eleToCheckpoint = topologia.ele - eleActual;
            tipoTopologia = topologia.tipo;
        }
    }


     drawRect(objPict->getX(), objPict->getY(), objPict->getW(), 14, cBlanco);
     pintarLinea(objPict->getX(), objPict->getY()  + 14, objPict->getX() + objPict->getW(), objPict->getY() + 14, cNegro);
//     drawText(strTipoTopologia.c_str(), 2, 0, cRojo);
     int x = objPict->getX() + 6, y = objPict->getY();
     if (tipoTopologia == CUMBRE){
        //fillTriangle(x,y,x-6,y+10,x+6,y+10, cNegro);
        pintarTriangulo(x,y,12,12,true, cNegro);
     } else {
        //fillTriangle(x,y+10,x-6,y,x+6,y, cNegro);
        pintarTriangulo(x,y+12,12,12,false, cNegro);
     }
//     loadFont(10);
//     int fontStyle = TTF_GetFontStyle(font);
//     TTF_SetFontStyle(font, fontStyle | TTF_STYLE_BOLD);
//     drawText(Constant::TipoToStr(tipoTopologia == CUMBRE ? posCumbre : posValle).c_str(), x - 3,y, cBlanco);
//     TTF_SetFontStyle(font, fontStyle);
//     loadFont(FONTSIZE);

     x+=8;
     drawText(Constant::TipoToStr(eleToCheckpoint).c_str(), x, y, cRojo);

     x+=30;
     drawText("a", x, y, cNegro);
     x+=10;
     drawText(Constant::TipoToStr(eleActual).c_str(), x, y, cRojo);

     x+=30;
     drawText("d", x, y, cNegro);
     x+=10;
     drawText(Constant::TipoToStr(distToCheckpoint).c_str(), x, y, cRojo);

     //Dibujando el porcentaje
     drawText(Constant::TipoToStr(pendiente).c_str(), x, y + 20, cRojo);
     drawText("%", x+19, y+20, cRojo);
}

/**
* Dibuja un triangulo con un vertice donde se indica la posicion x, y con su cuerpo girado
* tantos grados como indique el parametro rotacion
*/
void Iofrontend::pintarFlechaGPS(Point *punto, double rotacion){
    if (punto->x != 0 && punto->y != 0){
        int posFlechas[4]={0,0,0,0};
        geoDrawer->calculaVerticesFlecha(rotacion, posFlechas, anguloFlechas);
        /*pintarLinea(punto->x, punto->y, posFlechas[0] + punto->x, posFlechas[1] + punto->y, cAzulOscuro);
        pintarLinea(punto->x, punto->y, posFlechas[2] + punto->x, posFlechas[3] + punto->y, cAzulOscuro);
        pintarLinea(posFlechas[2] + punto->x, posFlechas[3] + punto->y, posFlechas[0] + punto->x, posFlechas[1] + punto->y, cAzulOscuro);
        */
        fillTriangle(punto->x, punto->y, posFlechas[0] + punto->x, posFlechas[1] + punto->y,
                     posFlechas[2] + punto->x, posFlechas[3] + punto->y,cVerdeOscuro);
        /*

        //Calculamos el punto medio de los dos vertices contrarios al central
        int xm = (posFlechas[0] + punto->x + posFlechas[2] + punto->x) / 2.0;
        int ym = (posFlechas[1] + punto->y + posFlechas[3] + punto->y) / 2.0;

        //Calculamos la mitad desde el punto medio anterior al central
        int xm2 = (xm + punto->x) / 2.0;
        int ym2 = (ym + punto->y) / 2.0;

        //Calculamos la mitad de la mitad
        xm2 = (xm2 + xm) / 2.0;
        ym2 = (ym2 + ym) / 2.0;

        fillTriangle(xm2, ym2, posFlechas[0] + punto->x, posFlechas[1] + punto->y,
                     posFlechas[2] + punto->x, posFlechas[3] + punto->y,cBlanco);
                     */

//        pintarLinea(posFlechas[0] + punto->x, posFlechas[1] + punto->y,
//                    xm2, ym2,
//                    cNegro);
//
//        pintarLinea(xm2, ym2,
//                    posFlechas[2] + punto->x, posFlechas[3] + punto->y,
//                    cNegro);
    }
}

/**
* Cualquier punto que supere el angulo, no sera tenido en cuenta
*/
void Iofrontend::generarFicheroRuta(string fileOri, string fileDest, int zoomMeters, double anguloLimite, bool generatePixels){
    if (posiciones != NULL) {
        delete posiciones;
        posiciones = NULL;
    }
    posiciones = new std::vector<std::string>();
    loadFromFileToVector(fileOri, posiciones);

    double latitud = 0.0;
    double longitud = 0.0;

    UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");
    GeoDrawer *geoDrawerAngulos = new GeoDrawer(objPict->getW(), objPict->getH());

    if (posiciones->size() > 0){
        Dirutil dir;
        //Creamos el fichero con todos los puntos
        string processedFile;
        if (generatePixels){
            processedFile = dir.getFolder(fileOri) + FILE_SEPARATOR
                        + dir.getFileNameNoExt(fileDest) + "_"
                        + Constant::TipoToStr(zoomMeters) +  "_px.dat";
        } else {
            processedFile = dir.getFolder(fileOri) + FILE_SEPARATOR
                        + dir.getFileNameNoExt(fileDest) + ".dat";
        }

        ofstream myfile( (generatePixels ? processedFile + "_tmp" : processedFile).c_str());
        //Se calculan los limites aproximados que forman el area de la ruta
        geoDrawer->calcLimites(posiciones);
        //La mayor precision se obtiene con el minimo zoom para calcular los angulos de las lineas
        //que unen cada punto
        geoDrawerAngulos->calcLimites(posiciones);
        //Se intenta hacer un zoom APROXIMADO en metros que abarque toda la pantalla
        geoDrawer->doGoogleZoom(zoomMeters);
        geoDrawerAngulos->doGoogleZoom(googleZoom[0]);
        //Obtenemos la primera posicion de la ruta y centramos en pantalla
        std::vector<std::string> strSplitted;
        strSplitted = Constant::split(posiciones->at(0),",");
        latitud = geoDrawer->todouble(strSplitted.at(GPXLAT));
        longitud = geoDrawer->todouble(strSplitted.at(GPXLON));

        geoDrawer->setPosicionCursor(latitud,longitud);
        geoDrawer->centerScreen();

        //Creamos un fichero en base al indicado con todo el procesamiento de los puntos
        //de latitud y longitud, pero convertidos a coordenadas cartesianas x,y para poder
        //dibujarlos en pantalla. (Convertimos a pixels)
        string linea = "";

        PosMapa xy;    //Posicion actual en longitud y latitud
        PosMapa xy_1;  //Posicion anterior (-1) en longitud y latitud
        PosMapa xy_2;  //Posicion anterior (-2) en longitud y latitud
        double angulo = 0.0;

        Point xy_t;    //Posicion actual del pixel de la longitud y latitud
        Point xy_1_t;  //Posicion anterior (-1) del pixel de la longitud y latitud
        Point xy_2_t;  //Posicion anterior (-2) del pixel de la longitud y latitud

        double distanciaPuntos = 0.0;
        unsigned long numPoints = 0;

        if (myfile.is_open()){
            for (int i = 0; i < posiciones->size(); i++){
                strSplitted = Constant::split(posiciones->at(i),",");
                if (strSplitted.size() >= 2){
                    //Almacenamos los datos actuales
                    xy.geopos.setLatitude(geoDrawer->todouble(strSplitted.at(GPXLAT)));
                    xy.geopos.setLongitude(geoDrawer->todouble(strSplitted.at(GPXLON)));
                    xy.name = strSplitted.size() > GPXNAME ? strSplitted.at(GPXNAME) : "";
                    xy.ele = strSplitted.size() > GPXALT ? geoDrawer->todouble(strSplitted.at(GPXALT)) : 0.0;
                    xy.time = strSplitted.size() > GPXTIME ? Constant::strToTipo<unsigned long>(strSplitted.at(GPXTIME)) : 0;

                    if (xy.geopos.getLatitude() != xy_1.geopos.getLatitude() && xy.geopos.getLongitude() != xy_1.geopos.getLongitude()){
                        //Convertimos la latitud y longitud
                        geoDrawer->convertGeoToPixel(xy.geopos.getLatitude(),xy.geopos.getLongitude(),&xy.point);
                        //Usamos el geoDrawerAngulos con la mayor precision para calcular los angulos
                        geoDrawerAngulos->convertGeoToPixel(xy.geopos.getLatitude(),xy.geopos.getLongitude(),&xy_t);

                        if (i > 0){
                            //Calculamos la distancia con el punto anterior
                            distanciaPuntos = fabs(GeoDrawer::getDistance(xy_1.geopos.getLatitude(), xy_1.geopos.getLongitude(),
                                                        xy.geopos.getLatitude(), xy.geopos.getLongitude())) * 1000; // en metros

                            //Para saber la distancia sin hacer calculos
                            xy.distancia = (xy_1.distancia + distanciaPuntos);
                        }

                        //Dados 3 puntos, calculamos el angulo que forman los vectores, cuyo vertice comun es xy_1
                        //Con este angulo podremos calcular si hay una desviacion en la ruta y simplificarla o utilizar
                        //este dato para avisar al usuario de que hay un cambio de direccion
                        if (i > 1){
                            angulo = calculaAnguloTrackpoint(&xy_2_t, &xy_1_t, &xy_t);
                            //El angulo se calcula con el vertice que forma xy_1
                            xy_1.angle = angulo;
                        }

                        //Tenemos varias opciones segun los parametros: Generar track con todos los puntos originales,
                        //generar track con los puntos simplificados con puntos (x, y) o (lat, lon)
                        if (i > 0 && anguloLimite == 0.0){
                            //Tenemos que esperarnos una iteracion para poder pintar el punto. Se hace de esta forma para
                            //poder tener el dato del angulo que forman 3 puntos consecutivos (2 vectores)
                            //Regeneramos el track con los puntos simplificados si es necesario
                            if (generatePixels){
                                anyadirPunto(&myfile, &xy_1);
                                numPoints++;
                            } else {
                                anyadirGeopos(&myfile, &xy_1);
                            }
                        } else if ( ( !(i > 2 && angulo == 0.0 && xy_2.angle == 0)
                                && (angulo < anguloLimite && i > 1) || i == 1)
                                && ((int)xy.point.x != (int)xy_1.point.x && (int)xy.point.y != (int)xy_1.point.y)
                                || !xy_1.name.empty()){
                            //Regeneramos el track con los puntos simplificados si es necesario
                            //cout << xy_t.x << "," << xy_1_t.x << "," << xy_t.y << "," << xy_1_t.y << endl;
                            if (generatePixels){
                                anyadirPunto(&myfile, &xy_1);
                                numPoints++;
                            } else {
                                anyadirGeopos(&myfile, &xy_1);
                            }
                        }
                        xy_2 = xy_1;
                        xy_1 = xy;
                        xy_2_t = xy_1_t;
                        xy_1_t = xy_t;
                    }
                }
            }

            //El ultimo punto no se pinta por lo que se ha anyadido para calcular el angulo de los dos vectores que forman
            //los 3 ultimos puntos dibujados. Por eso forzamos a pintar el ultimo punto
            if (generatePixels){
                anyadirPunto(&myfile, &xy_1);
                numPoints++;
            } else {
                anyadirGeopos(&myfile, &xy_1);
            }
        }
        myfile.close();

        //Los datos de resumen de la ruta deben estar al principio, por lo que tenemos que
        //regenerar el fichero con los datos de la cabecera en primer lugar y concatenando
        //los datos de posicionamiento
        if (generatePixels){
            Dirutil dir;
            dir.borrarArchivo(processedFile);
            geoDrawer->setMapNumPoints(numPoints);
            std::ifstream  src((processedFile + "_tmp").c_str());
            //Primero creamos el fichero con la cabecera
            std::ofstream  dst(processedFile.c_str(), std::ios::app);
            //Creamos la cabecera de 3 lineas con datos importantes que evitaran procesos en arduino
            crearCabecera(&dst);
            //Y anyadimos el contenido de los puntos creados en el temporal
            dst << src.rdbuf();
            dst.close();
            src.close();
            dir.borrarArchivo(processedFile + "_tmp");
        }
    }
    delete geoDrawerAngulos;
}

/**
* Dados 3 puntos, calculamos el angulo que forman los vectores, cuyo vertice comun es xy0
* Con este angulo podremos calcular si hay una desviacion en la ruta y simplificarla o utilizar
* este dato para avisar al usuario de que hay un cambio de direccion
*
* xy1      xy2
*  ·       ·
*   \     /
*    \   /
*     \ /
*      ·
*      x0
*/
double Iofrontend::calculaAnguloTrackpoint(Point *xy1, Point *xy0, Point *xy2){

    //V1(x0-x1,y0-y1)
    //V2(x0-x2,y0-y2)
    //Hacemos el producto escalar de los dos vectores
    //V1 V2= ( (x0-x1)(x0-x2)+(y0-y1)(y0-y2))
    double prodEscalar = (xy0->x - xy1->x) * (xy0->x - xy2->x) + (xy0->y - xy1->y) * (xy0->y - xy2->y);
    //El producto escalar de dos vectores es un escalar igual al mósulo del uno por el módulo del otro por el coseno del ángulo que forman.
    double modulos = sqrt(pow(xy0->x - xy1->x, 2) + pow(xy0->y - xy1->y, 2)) * sqrt(pow(xy0->x - xy2->x, 2) + pow(xy0->y - xy2->y, 2));
    if (modulos > 0.0){
        //alfa=arccos(( (x0-x1)(x0-x2)+(y0-y1)(y0-y2))/[V1][V2])
        return acos(prodEscalar / modulos);
    }else {
//        cout
//        << "xy0->x: " << xy0->x << ","
//        << "xy0->y: " << xy0->y << ","
//        << "xy1->x: " << xy1->x << ","
//        << "xy1->y: " << xy1->y << ","
//        << "xy2->x: " << xy2->x << ","
//        << "xy2->y: " << xy2->y << endl;
        return 0;
    }

}

/**
*
*/
void Iofrontend::anyadirPunto(ofstream *myfile, PosMapa *xy){
    *myfile << std::setprecision(0) << std::fixed << xy->point.x << "," << xy->point.y            /*Posicion xy en pixels del punto gps*/
    << "," << xy->ele   /*Altura*/
    << "," << xy->time /*tiempo en segundos*/
    << "," << Constant::TipoToStr(ceil(GeoStructs::toDegrees(xy->angle)))
    << "," << xy->distancia
    << "," << xy->name /*Nombre waypoint*/
    << VBCRLF;
}

/**
*
*/
void Iofrontend::anyadirGeopos(ofstream *myfile, PosMapa *posmapa){
    *myfile << std::setprecision(8) << std::fixed << posmapa->geopos.getLatitude() << "," << posmapa->geopos.getLongitude()
    << "," << std::setprecision(0) << posmapa->ele
    << "," << posmapa->time
    << "," << posmapa->name
    << VBCRLF;
}

void Iofrontend::crearCabecera(ofstream *myfile){

    //La primera fila incluimos datos importantes precalculados. SUPERIMPORTANTE que se almacene
        //con al menos una precision de 8 decimales. Sino falla el calculo del gps
        *myfile << std::setprecision(8) << std::fixed << geoDrawer->getMapLonLeft() << "," << geoDrawer->getMapLonRight()
        << "," << geoDrawer->getMapLonDelta() << "," << geoDrawer->getMapLatBottom() << "," << geoDrawer->getMapLatUp()
        << "," << geoDrawer->getMapLatBottomDegree() << "," << geoDrawer->getDesplazamiento()
        << "," << geoDrawer->getMapOffsetX() << "," << geoDrawer->getMapOffsetY() << "," << geoDrawer->getMapNumPoints()
        << VBCRLF;

        //La fila siguiente es para las estadisticas
        *myfile << std::setprecision(0) << std::fixed << geoDrawer->getAcumSubida() << "," << geoDrawer->getAcumBajada()
        << "," << geoDrawer->getMaxAltitud() << "," << geoDrawer->getMinAltitud() << "," << geoDrawer->getDistancia()
        << std::setprecision(2) << "," << geoDrawer->getPendienteMediaSubida() << "," << geoDrawer->getPendienteMediaBajada()
        << "," << geoDrawer->getDistLlano() << "," << geoDrawer->getDistSubida() << "," << geoDrawer->getDistBajada()
        << "," << geoDrawer->getVelocidadMinima() << "," << geoDrawer->getVelocidadMaxima()
        << "," << geoDrawer->getVelocidadMedia() << "," << geoDrawer->getTiempoTotal()
        << "," << geoDrawer->getTiempoMovimiento() << "," << geoDrawer->getTiempoParado()
        << VBCRLF;

        //Incluimos informacion de topologia para saber donde estan los puntos mas y menos elevados
        for (int i=0; i < geoDrawer->getListaCumbresYValles()->size(); i++){
            CumbreValle topologia = geoDrawer->getListaCumbresYValles()->at(i);
            *myfile << std::setprecision(0)  << std::fixed << topologia.tipo
            << "," << topologia.dist << "," << topologia.ele
            << "," << std::setprecision(8)  << topologia.geoPos.getLatitude() << "," << topologia.geoPos.getLongitude();
            if (i < geoDrawer->getListaCumbresYValles()->size() - 1){
                *myfile << ";";
            }
        }
        *myfile << VBCRLF;
}




/**
*
*/
int Iofrontend::cargarFicheroRuta(string file, int zoomMeters){
    try{
        if (listaPixels != NULL){
            delete listaPixels;
            listaPixels = NULL;
        }
            

        fileGPSData = file;
        Dirutil dir;
        string processedFile = dir.getFolder(file) + FILE_SEPARATOR
                            + dir.getFileNameNoExt(file) + "_" + Constant::TipoToStr(zoomMeters) +  "_px.dat";

        std::vector<std::string> ficheroProcesado;
        loadFromFileToVector(processedFile, &ficheroProcesado);
        cout << "Abriendo fichero con los puntos del mapa: " <<  processedFile;
        cout << " con un tamanyo de: " << ficheroProcesado.size() << " puntos" << endl;

        if (ficheroProcesado.size() == 0) return 1;

        /**Cargamos las posiciones solo para ejemplo de simulacion de puntos gps al pulsar la tecla d**/
        if (posiciones != NULL){
            delete posiciones;
            posiciones = NULL;
        }
            

        posiciones = new std::vector<std::string>();
        loadFromFileToVector(file, posiciones);
        cout << "Abriendo fichero con las posiciones de prueba: " <<  file;
        cout << " con un tamanyo de: " << posiciones->size() << " puntos" << endl;

        /**HARCODEOOOOOOOO**/
        //loadFromFileToVector("C:\\cabeco-dor-pr-cv-2 (1)_simple.dat", posiciones);
        /**HARCODEOOOOOOOO**/

        /**FIN Cargamos las posiciones solo para ejemplo**/

        if (ficheroProcesado.size() > 1){
            string linea = "";
            std::vector<std::string> strSplitted;
            listaPixels = new std::vector<PosMapa>();

            //Obtenemos los datos importantes del mapa que estan en la primera linea
            strSplitted = Constant::split(ficheroProcesado.at(LINEGPXCONFIG),",");
            geoDrawer->setMapLonLeft(geoDrawer->todouble(strSplitted.at(MAPLONLEFT)));
            geoDrawer->setMapLonRight(geoDrawer->todouble(strSplitted.at(MAPLONRIGHT)));
            geoDrawer->setMapLonDelta(geoDrawer->todouble(strSplitted.at(MAPLONDELTA)));
            geoDrawer->setMapLatBottom(geoDrawer->todouble(strSplitted.at(MAPLATBOTTOM)));
            geoDrawer->setMapLatUp(geoDrawer->todouble(strSplitted.at(MAPLATUP)));
            geoDrawer->setMapLatBottomDegree(geoDrawer->todouble(strSplitted.at(MAPLATBOTTOMDEGREE)));
            geoDrawer->setDesplazamiento(geoDrawer->todouble(strSplitted.at(DESPLAZAMIENTO)));
            geoDrawer->setMapOffsetX(Constant::strToTipo<int>(strSplitted.at(OFFSETX)));
            geoDrawer->setMapOffsetY(Constant::strToTipo<int>(strSplitted.at(OFFSETY)));
            geoDrawer->setMapNumPoints(Constant::strToTipo<int>(strSplitted.at(MAPNUMPOINTS)));

            //Obtenemos los datos con las estadisticas del track
            strSplitted = Constant::split(ficheroProcesado.at(LINEGPXSTATS),",");
            geoDrawer->setAcumSubida(geoDrawer->todouble(strSplitted.at(STATACUMSUBIDA)));
            geoDrawer->setAcumBajada(geoDrawer->todouble(strSplitted.at(STATACUMBAJADA)));
            geoDrawer->setMaxAltitud(geoDrawer->todouble(strSplitted.at(STATMAXALT)));
            geoDrawer->setMinAltitud(geoDrawer->todouble(strSplitted.at(STATMINALT)));
            geoDrawer->setDistancia(geoDrawer->todouble(strSplitted.at(STATDIST)));
            geoDrawer->setPendienteMediaSubida(geoDrawer->todouble(strSplitted.at(STATPENDIENTEMEDIASUBIDA)));
            geoDrawer->setPendienteMediaBajada(geoDrawer->todouble(strSplitted.at(STATPENDIENTEMEDIABAJADA)));
            geoDrawer->setDistLlano(geoDrawer->todouble(strSplitted.at(STATDISTLLANO)));
            geoDrawer->setDistSubida(geoDrawer->todouble(strSplitted.at(STATDISTSUBIDA)));
            geoDrawer->setDistBajada(geoDrawer->todouble(strSplitted.at(STATDISTBAJADA)));
            geoDrawer->setVelocidadMinima(geoDrawer->todouble(strSplitted.at(STATVELMIN)));
            geoDrawer->setVelocidadMaxima(geoDrawer->todouble(strSplitted.at(STATVELMAX)));
            geoDrawer->setVelocidadMedia(geoDrawer->todouble(strSplitted.at(STATVELMEDIA)));
            geoDrawer->setTiempoTotal(Constant::strToTipo<unsigned long long>(strSplitted.at(STATTOTALTIME)));
            geoDrawer->setTiempoMovimiento(Constant::strToTipo<unsigned long long>(strSplitted.at(STATMOVINGTIME)));
            geoDrawer->setTiempoParado(Constant::strToTipo<unsigned long long>(strSplitted.at(STATSTOPTIME)));

            /**Regeneramos la informacion de topologia*/
            std::vector<std::string> strSplitted2;
            strSplitted = Constant::split(ficheroProcesado.at(LINETOPOINFO),";");
            geoDrawer->getListaCumbresYValles()->clear();
            for (int i=0; i < strSplitted.size(); i++){
                strSplitted2 = Constant::split(strSplitted.at(i), ",");
                CumbreValle topografia;
                topografia.geoPos.setLatitude(geoDrawer->todouble(strSplitted2.at(TOPOLAT)));
                topografia.geoPos.setLongitude(geoDrawer->todouble(strSplitted2.at(TOPOLON)));
                topografia.ele = geoDrawer->todouble(strSplitted2.at(TOPOELE));
                topografia.tipo = Constant::strToTipo<int>(strSplitted2.at(TOPOTYPE));
                topografia.dist = geoDrawer->todouble(strSplitted2.at(TOPODIST));
                geoDrawer->getListaCumbresYValles()->push_back(topografia);
            }

            /**Cargamos las posiciones ya procesadas para la pantalla**/
            listaPixels->clear();
            for (int i = LINETRACKDATA; i < ficheroProcesado.size(); i++){
                strSplitted = Constant::split(ficheroProcesado.at(i),",");
                if (strSplitted.size() >= 2){
                    PosMapa data;
                    data.point.x = geoDrawer->todouble(strSplitted.at(PXLAT));
                    data.point.y = geoDrawer->todouble(strSplitted.at(PXLON));
                    data.ele = (strSplitted.size() > PXALT ? geoDrawer->todouble(strSplitted.at(PXALT)) : 0.0);
                    data.time = (strSplitted.size() > PXTIME ? Constant::strToTipo<unsigned long>(strSplitted.at(PXTIME)) : 0);
                    data.name = (strSplitted.size() > PXNAME ? strSplitted.at(PXNAME) : "");
                    data.angle = (strSplitted.size() > PXANGLE ? geoDrawer->todouble(strSplitted.at(PXANGLE)) : 0.0);
                    data.distancia = (strSplitted.size() > PXDISTANCIA ? geoDrawer->todouble(strSplitted.at(PXDISTANCIA)) : 0.0);
                    listaPixels->push_back(data);
                }
            }
        }
        cout << "listaPixels cargado con " << listaPixels->size() << " elementos" << endl;
        return 0;
    } catch (Excepcion &e){
        Traza::print("Error al cargar la configuracion", W_ERROR);
        cout << "ERROR ENCONTRADO" << e.getMessage() << endl;
        return 1;
    }
}

/**
*
*/
void Iofrontend::loadFromFileToVector(string file, std::vector<std::string> *myVector){
    std::string line;
    std::ifstream infile(file.c_str());
    while (std::getline(infile, line)){
        std::istringstream iss(line);
        myVector->push_back(line);
    }
}

/**
*
*/
void Iofrontend::pintarCapaTerreno(VELatLong *currentGPSPos){

    if (currentGPSPos != NULL){
        const int zoom = geoDrawer->getZoomMeters();

        /**Obtener el tile correspondiente a la latitud y longitud.*/
        Point pixelTile, numTile;
        MercatorProjection merca;
        //Obtenemos en pixelTile, el pixel exacto en el que se encuentra nuestra lat y lon
        //en el propio tile
        merca.getPosPixelTile(currentGPSPos, zoom, &pixelTile, &numTile);

//        Traza::print("xtile: " + Constant::TipoToStr(numTile.x) + ", " +
//                     "ytile: " + Constant::TipoToStr(numTile.y), W_DEBUG);

        /**Dibujar el tile centrado en la posicion central en la pantalla y los del alrededor*/
        UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");
        SDL_Surface *myDestSurface = objPict->getImgGestor()->getSurface();
        //Color blanco de fondo
        SDL_FillRect(myDestSurface, NULL, SDL_MapRGB(myDestSurface->format, cBlanco.r,cBlanco.g,cBlanco.b));
        objPict->setImgDrawed(false);
        Point newPos;
        newPos.x = objPict->getW() / 2 - pixelTile.x;
        newPos.y = objPict->getH() / 2 - pixelTile.y;

        int countTileX = 0, countTileY = 0;
        int posTile00Left, posTile00Right, posTile00Up, posTile00Down;
        bool leftLimit = false;
        bool rightLimit = false;
        bool upLimit = false;
        bool downLimit = false;
        int numTilesDrawed = 0;

        cout << "drawing screen" << endl;

        while (!upLimit || !downLimit){
            countTileX = 0;
            leftLimit = false;
            rightLimit = false;
            posTile00Up = newPos.y + mapHeight * (-1 * countTileY);
            posTile00Down = newPos.y + mapHeight * countTileY;
            upLimit = posTile00Up <= -1 * mapHeight;
            downLimit = posTile00Down > objPict->getH();

            while (!leftLimit || !rightLimit){
                posTile00Left  = newPos.x + mapWidth * (-1 * countTileX);
                posTile00Right = newPos.x + mapWidth * countTileX;
                leftLimit = posTile00Left <= -1 * mapWidth;
                rightLimit = posTile00Right > objPict->getW();

                if (countTileX == 0){
                    drawTile(currentGPSPos, zoom, 0, countTileY, numTile, pixelTile);
                    numTilesDrawed++;
                    if (countTileY > 0) {
                        drawTile(currentGPSPos, zoom, 0, -1 * countTileY, numTile, pixelTile);
                        numTilesDrawed++;
                    }
                }
                else {
                    //Pintando por arriba
                    if (!leftLimit && !upLimit){
                        drawTile(currentGPSPos, zoom, -1 * countTileX, -1 * countTileY, numTile, pixelTile);
                        numTilesDrawed++;
                    }

                    if (!rightLimit && !upLimit){
                        drawTile(currentGPSPos, zoom,      countTileX, -1 * countTileY, numTile, pixelTile);
                        numTilesDrawed++;
                    }

                    if (countTileY > 0) {
                        //Pintando por debajo
                        if (!leftLimit && !downLimit){
                            drawTile(currentGPSPos, zoom, -1 * countTileX, countTileY, numTile, pixelTile);
                            numTilesDrawed++;
                        }

                        if (!rightLimit  && !downLimit){
                            drawTile(currentGPSPos, zoom,      countTileX, countTileY, numTile, pixelTile);
                            numTilesDrawed++;
                        }
                    }

                }
                countTileX++;
            }
            countTileY++;
        }
        //cout << "numTilesDrawed: " << numTilesDrawed << endl;
    }
}


/**
* draw a tile relative to from a central point and
*/
void Iofrontend::drawTile(VELatLong *currentLatLon, int zoom, int sideTileX, int sideTileY, Point numTile, Point pixelTile){
    Dirutil dir;
    ImagenGestor imgGestor;
    UIPicture *objPict = (UIPicture *)ObjectsMenu[PANTALLAGPSINO]->getObjByName("mapBox");
    Point newPos;

    //Pixel tile contiene el punto (x,y) de la imagen que representa la
    //posicion actual del gps. Calculamos el desplazamiento que habria
    //que aplicar para llevar ese pixel de la imagen al centro de la pantalla.
    newPos.x = objPict->getW() / 2 - pixelTile.x;
    newPos.y = objPict->getH() / 2 - pixelTile.y;

    //Dependiendo del tile que pintemos en base alrededor del obtenido de
    //la coordenada del gps inicial, desplazamos el mapa
    int desplazaX = mapWidth * sideTileX;
    int desplazaY = mapHeight * sideTileY;

    string imgLocation = dir.getFolder(fileGPSData) //+ FILE_SEPARATOR + "Tiles"
                + FILE_SEPARATOR + Constant::TipoToStr(zoom)
                + FILE_SEPARATOR + Constant::TipoToStr((long)numTile.x + (long)sideTileX)
                + FILE_SEPARATOR + Constant::TipoToStr((long)numTile.y + (long)sideTileY)
                + string(".png");
                //+ string(".bmp")
                //;

    const int offsetX = newPos.x + desplazaX;
    const int offsetY = newPos.y + desplazaY;

    //Se calcula desde donde se debe pintar en la pantalla
    const int inicioX = (offsetX < 0) ? abs(offsetX) : 0;
    const int inicioY = (offsetY < 0) ? abs(offsetY) : 0;

    if (dir.existe(imgLocation)){
        SDL_Surface *optimizedImage;
        imgGestor.loadImgDisplay(imgLocation.c_str(), &optimizedImage);
        const int finx = (gpsinoW - offsetX > optimizedImage->w ) ? optimizedImage->w  : gpsinoW - offsetX;
        const int finy = (gpsinoH - offsetY > optimizedImage->h) ? optimizedImage->h : gpsinoH - offsetY;


        for (int i=inicioX; i < finx; i++){
            for (int j=inicioY; j < finy; j++){
                Uint8 r,g,b;

//                SDL_GetRGB(getpixel(optimizedImage, i, j), optimizedImage->format,&r,&g,&b);

//                putpixelSafe(objPict->getImgGestor()->getSurface(), i + newPos.x + desplazaX, j + newPos.y + desplazaY,
//                             SDL_MapRGB(optimizedImage->format,
//                                        r > umbral.r ? 0xFF : 0,
//                                        g > umbral.g ? 0xFF : 0,
//                                        b > umbral.b ? 0xFF : 0));

                putpixelSafe(objPict->getImgGestor()->getSurface(), i + newPos.x + desplazaX, j + newPos.y + desplazaY,
                             getpixel(optimizedImage, i, j));
            }
        }
        SDL_FreeSurface(optimizedImage);

//        cout << "tile: " << imgLocation << endl;
//        Image565 imagen;
//        t_mapSurface bmpFile;
//        imagen.screen = objPict->getImgGestor()->getSurface();
//        if (imagen.tileLoad(imgLocation , &bmpFile)){
//            imagen.tileDraw(&bmpFile, inicioX, inicioY, offsetX, offsetY);
//        }

    }
}

/**
*
*/
void Iofrontend::leerLineaFicheroRuta(char *text, int numLinea, int *numCampo, PosMapa *data){
    if (numLinea >= LINETRACKDATA){
        //Obtenemos los datos de la linea actual
        if (*numCampo == PXLAT) data->point.x = atof(text);
        if (*numCampo == PXLON) data->point.y = atof(text);
        if (*numCampo == PXALT) data->ele = atoi(text);
        if (*numCampo == PXTIME) data->time = atol(text);
        if (*numCampo == PXNAME) {Constant::utf8ascii(text); data->name = text;}
        if (*numCampo == PXANGLE) data->angle = atoi(text);
        if (*numCampo == PXDISTANCIA) data->distancia = atof(text);
    } else if (numLinea == LINEGPXCONFIG){
        //Obtenemos los datos importantes del mapa que estan en la primera linea
        if (*numCampo == MAPLONLEFT) geoDrawer->setMapLonLeft( atof(text));
        if (*numCampo == MAPLONRIGHT) geoDrawer->setMapLonRight( atof(text));
        if (*numCampo == MAPLONDELTA) geoDrawer->setMapLonDelta( atof(text));
        if (*numCampo == MAPLATBOTTOM) geoDrawer->setMapLatBottom( atof(text));
        if (*numCampo == MAPLATBOTTOMDEGREE) geoDrawer->setMapLatBottomDegree( atof(text));
        if (*numCampo == DESPLAZAMIENTO) geoDrawer->setDesplazamiento( atof(text));
        if (*numCampo == OFFSETX) geoDrawer->setMapOffsetX(atoi(text));
        if (*numCampo == OFFSETY) geoDrawer->setMapOffsetY(atoi(text));
        if (*numCampo == MAPLATUP) geoDrawer->setMapLatUp(atoi(text));
        if (*numCampo == MAPNUMPOINTS) geoDrawer->setMapNumPoints(atoi(text));
    } else if (numLinea == LINEGPXSTATS){
        //Obtenemos los datos con las estadisticas del track
        if (*numCampo == STATACUMSUBIDA) geoDrawer->setAcumSubida(atof(text));
        if (*numCampo == STATACUMBAJADA) geoDrawer->setAcumBajada(atof(text));
        if (*numCampo == STATMAXALT) geoDrawer->setMaxAltitud(atof(text));
        if (*numCampo == STATMINALT) geoDrawer->setMinAltitud(atof(text));
        if (*numCampo == STATDIST) geoDrawer->setDistancia(atof(text));
        if (*numCampo == STATPENDIENTEMEDIASUBIDA) geoDrawer->setPendienteMediaSubida(atof(text));
        if (*numCampo == STATPENDIENTEMEDIABAJADA) geoDrawer->setPendienteMediaBajada(atof(text));
        if (*numCampo == STATDISTLLANO) geoDrawer->setDistLlano(atof(text));
        if (*numCampo == STATDISTSUBIDA) geoDrawer->setDistSubida(atof(text));
        if (*numCampo == STATDISTBAJADA) geoDrawer->setDistBajada(atof(text));
        if (*numCampo == STATVELMIN) geoDrawer->setVelocidadMinima(atof(text));
        if (*numCampo == STATVELMAX) geoDrawer->setVelocidadMaxima(atof(text));
        if (*numCampo == STATVELMEDIA) geoDrawer->setVelocidadMedia(atof(text));
        if (*numCampo == STATTOTALTIME) geoDrawer->setTiempoTotal(atol(text));
        if (*numCampo == STATMOVINGTIME) geoDrawer->setTiempoMovimiento(atol(text));
        if (*numCampo == STATSTOPTIME) geoDrawer->setTiempoParado(atol(text));
    } else if (numLinea == LINETOPOINFO){
          if (*numCampo == TOPOTYPE) geoDrawer->arrayCumbresYValles[geoDrawer->getNumTopo()].tipo = atoi(text);
          if (*numCampo == TOPOELE) geoDrawer->arrayCumbresYValles[geoDrawer->getNumTopo()].ele = atoi(text);
          if (*numCampo == TOPODIST) geoDrawer->arrayCumbresYValles[geoDrawer->getNumTopo()].dist = atof(text);
    }
}

/**
*
*/
int Iofrontend::getPosY(int altura, int graphW, int graphH){
    double correccionY = geoDrawer->getMaxAltitud() - geoDrawer->getMinAltitud();
    double actual = altura - geoDrawer->getMinAltitud();
    return graphH - ceil(actual / correccionY * graphH);
}

/**
*
*/
void Iofrontend::drawGraphSlope(tEvento *evento, int graphX, int graphY, int graphW, int graphH){

    const int maxLine = 50;
    //100 bytes de limite
    FILE *fp;
	char line[maxLine];
	char tmpLine[maxLine];
	Point posXY;
    int data;
    int puntos = 0;
    int lineLen = 0;
    int nCampos = 0;
    PosMapa dataFromFile;
    bool endOfLine = false;
    int readBytes=0;
    Dirutil dir;
    static string processedFile = dir.getFolder(fileGPSData) + FILE_SEPARATOR
                            + dir.getFileNameNoExt(fileGPSData) + "_" + Constant::TipoToStr(geoDrawer->getZoomMeters()) +  "_px.dat";

    fp = fopen(processedFile.c_str(), "r"); // error check this!

    int nPoint = 0;
    int xPointDisplay = 0;
    int divPointsWidth = 0;
    int lastNPoint = -1;
    int tmpPoint = 0;

    clearScr(cFondoMapa);

    while ((readBytes = fread(tmpLine, 1, maxLine, fp)) > 0 ) {
        int i=0;
        while (i < readBytes){
            for (i; i < readBytes; i++){
                data = tmpLine[i];
                endOfLine = (data == '\n');

                if (data != ',' && !endOfLine && lineLen <= readBytes-1 && !(data == ';' && puntos == LINETOPOINFO)){
                  line[lineLen] = tmpLine[i];
                  lineLen++ % maxLine;
                } else {

                    //truncated =  lineLen > maxLine-1;
                    line[lineLen >= maxLine ? maxLine-1 : lineLen] = '\0';
                    //cout << line << endl;
                    lineLen = 0;
                    //Leemos la linea y almacenamos los valores en un objeto
                    leerLineaFicheroRuta(line, puntos, &nCampos, &dataFromFile);

                    if (endOfLine){
                        if (puntos >= LINETRACKDATA){
                            tmpPoint = floor((graphW - 1) / (double)geoDrawer->getMapNumPoints() * nPoint);
                            if (tmpPoint != lastNPoint){
                                pintarLinea(tmpPoint + graphX, graphH - 1 + graphY,
                                            tmpPoint + graphX, getPosY(dataFromFile.ele, graphW, graphH) + graphY,
                                            cColorElevacion);
                                lastNPoint = tmpPoint;
                            }
                            nPoint++;
                        }
//                        else if (puntos == LINEGPXCONFIG){
//                        }
                    } // if (endOfLine){

                    //Reseteamos variables
                    if (data == ','){
                        nCampos++;
                    } else if (endOfLine){
                        nCampos = 0;
                        puntos++;
                    } else if (data == ';' && puntos == LINETOPOINFO){
                        nCampos = 0;
                    }
                } // if (data != ','
            } // for (i; i < readBytes
        } // while (i < readBytes
    } // while ((readBytes = fread

    fclose(fp);

    drawText(Constant::TipoToStr(geoDrawer->getMaxAltitud()).c_str(), 0, 0, cBlanco);
    drawText(Constant::TipoToStr(geoDrawer->getMinAltitud()).c_str(), 0, graphH - 13, cBlanco);

    drawText("0", graphX, graphH + 5 , cBlanco);

    char format[11];
    sprintf(format,"%6.1f", geoDrawer->getDistancia() / 1000);
    strcat(format, " km");

    drawText(format, graphX + graphW - strlen(format) * 5.5, graphH + 5 , cBlanco);
}

/**
*
*/
void Iofrontend::plotLineWidth(int x0, int y0, int x1, int y1, int wd, t_color color, SDL_Surface *surface)
{
   int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
   int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1;
   int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);

    for (wd = (wd+1)/2; ; ) {  /* pixel loop */
//        putpixelSafe(surface,x0, y0, pixColor);
        gestorIconos->drawIcono(trackSeg, surface, x0, y0);

        e2 = err; x2 = x0;
        if (2*e2 >= -dx) {                                           /* x step */
             for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx){
//                        putpixelSafe(surface,x0, y2 += sy, pixColor );
                    gestorIconos->drawIcono(trackSeg, surface, x0, y2 += sy);
             }

            if (x0 == x1) break;
            e2 = err; err -= dy; x0 += sx;
        }

        if (2*e2 <= dy) {                                            /* y step */
             for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy){
//                        putpixelSafe(surface,x2 += sx, y0, pixColor );
                    gestorIconos->drawIcono(trackSeg, surface, x2 += sx, y0);
             }
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }


    }
}

/**
*
*/
void Iofrontend::calcularPixels(VELatLong *lastGPSPos, VELatLong *currentGPSPos,
                            Point *latestCoord, Point *actualCoord){
    geoDrawer->setPosicionCursor(currentGPSPos->getLatitude(), currentGPSPos->getLongitude());
    geoDrawer->centerScreen();
    //Recargamos la posicion actual y la anterior para que se oriente bien la flecha. En arduino no sabremos
    //la posicion anterior para ahorrar memoria, probamos con la posicion que se obtenga
    geoDrawer->convertGeoToPixel(currentGPSPos->getLatitude(), currentGPSPos->getLongitude(), actualCoord);
    geoDrawer->convertGeoToPixel(lastGPSPos->getLatitude(), lastGPSPos->getLongitude(), latestCoord);

    actualCoord->x += geoDrawer->getMapOffsetX();
    actualCoord->y += geoDrawer->getMapOffsetY();
    latestCoord->x += geoDrawer->getMapOffsetX();
    latestCoord->y += geoDrawer->getMapOffsetY();
}

/**
*
*/
//void Iofrontend::downImgRoute(t_downMapData *in){
//    GpxLoader loader;
//    Dirutil dir;
//    string dataGPS;
//    GeoStructs geoStruct;
//    int xtile = 0, ytile = 0;
//    Image565 imagen;
//    string tmpTile;
//    std::map<string, int> mTiles;
//
//    if (!in->calcNTiles){
//        Constant::setPROXYIP("10.129.8.100");
//        Constant::setPROXYPORT("8080");
//        Constant::setPROXYUSER("dmarcobo");
//        Constant::setPROXYPASS("eO2E0G3Q8p");
//    }
//
//    in->nTiles = 0;
//
//    if (loader.loadGPX(in->fileGpx)){
//        for (int i=0; i < loader.gpxData.size(); i++){
//            xtile = geoStruct.long2tilex(loader.gpxData.at(i).getLon(), in->zoom);
//            ytile = geoStruct.lat2tiley(loader.gpxData.at(i).getLat(), in->zoom);
//            tmpTile = Constant::TipoToStr(xtile) + "/" + Constant::TipoToStr(ytile);
//            if (mTiles.count(tmpTile) <= 0){
//                mTiles.insert(std::pair<string,int>(tmpTile, 1));
//
//                if (!in->calcNTiles){
//                    imagen.downloadMap(in->server + Constant::TipoToStr(in->zoom) + "/"
//                               + tmpTile + ".png", in->dirImgDown);
//                }
//                in->nTiles++;
//            }
//        }
//    }
//    mTiles.clear();
//}
//
