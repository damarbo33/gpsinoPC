#include "geodrawer.h"

/**
*
*/
GeoDrawer::GeoDrawer(int w, int h){
    mapLonLeft = 0.0;
    mapLonRight = 0.0;
    mapLatBottom = 0.0;
    mapOffsetX = 0;
    mapOffsetY = 0;
    width = w;
    height = h;
    desplazamiento = 0.0;
    latitudCursor = 0.0;
    longitudCursor = 0.0;
    autocenter = true;
    zoomLevel = 0;
    detalleDouble = false;
    acumSubida = 0;
    acumBajada = 0;
    maxAltitud = 0;
    minAltitud = 0;
    correccionGPS = true;
    sumaPendienteSubida = 0;
    sumaPendienteBajada = 0;
    numPendienteSubida = 0;
    numPendienteBajada = 0;
    velocidadMedia = 0;
    numTopo = 0;
}

/**
*
*/
GeoDrawer::~GeoDrawer(){

}

/**
* Calcula el offset necesario para que el punto especificado por parametro
* quede justo en el centro de la pantalla
*/
void GeoDrawer::centerScreenToLocation(double latitud, double longitud){
    Point point;
    convertGeoToPixel(latitud, longitud, &point);
    mapOffsetX = getWidth() / 2 - point.x;
    mapOffsetY = getHeight() / 2 - point.y;
    //cout << "centrando offsetx: " << mapOffsetX << " y offsety: " << mapOffsetY << endl;
}

/**
* Calcula los extremos de las longitudes y la latitud inferior para tener
* una orientación del tamanyo final del track en pantalla
*/
void GeoDrawer::calcLimites(std::vector<std::string> * coordinates){
    int datoResp [2]= {0,0};
    double lat;
    double lon;

    strSplitted2 = Constant::split(coordinates->at(0), ",");
    mapLonLeft = todouble(strSplitted2.at(1));
    mapLonRight = mapLonLeft;
    mapLatBottom = todouble(strSplitted2.at(0));
    mapLatUp = mapLatBottom;

    StatsClass stats;
    stats.maxCoordRuta = coordinates->size();

    for (stats.numCoordRuta=1; stats.numCoordRuta < stats.maxCoordRuta; stats.numCoordRuta++){

        strSplitted2 = Constant::split(coordinates->at(stats.numCoordRuta),",");
        if (strSplitted2.size() > 0){
            lon = todouble(strSplitted2.at(GPXLON));
            if (lon < mapLonLeft){
                mapLonLeft = lon;
            } else if (lon > mapLonRight){
                mapLonRight = lon;
            }
            lat = todouble(strSplitted2.at(GPXLAT));

            if (lat > mapLatUp){
                mapLatUp = lat;
            }

            if (lat < mapLatBottom){
                mapLatBottom = lat;
            }
            calcularEstadisticasRuta(lat, lon,
                                     todouble(strSplitted2.at(GPXALT)),
                                     Constant::strToTipo<long>(strSplitted2.at(GPXTIME)),
                                     &stats);
        }
    }

    mapLonDelta = mapLonRight - mapLonLeft;
    mapLatBottomDegree = mapLatBottom * PI / 180.0;
    desplazamiento = mapLonDelta * 0.1;
    //Calculamos temas de medias
    pendienteMediaSubida = ceil(sumaPendienteSubida / (float)numPendienteSubida);
    pendienteMediaBajada = ceil(sumaPendienteBajada / (float)numPendienteBajada);
    velocidadMedia = sumaVelocidadMedia / (float) numSumaVelocidad;
}

/**
*
*/
void GeoDrawer::logEstadisticasRuta(){
    cout << "GeoDrawer::logEstadisticasRuta: ";
    cout << std::setprecision(0) << std::fixed << "acumSubida: " << acumSubida << " acumBajada: " << acumBajada
    << " maxAltitud: " << maxAltitud << " minAltitud: " << minAltitud << " distancia: " << distancia
    << " maxDesnivelPuntos: " << std::setprecision(2) << maxDesnivel2Puntos
    << " pendienteMediaSubida: " << pendienteMediaSubida << "% pendienteMediaBajada: " << pendienteMediaBajada << "%"
    << " distLlano: " << distLlano << " distSubida: " << distSubida << " distBajada: " << distBajada
    << " velocidad minima: " << (velocidadMinima / 1000.0 * 3600.0) << " velocidad maxima: " << (velocidadMaxima / 1000.0 * 3600.0)
    << " velocidad media: " << (velocidadMedia / 1000.0 * 3600.0) << " tiempo total: " << Constant::timeFormat(tiempoTotal)
    << " tiempoMovimiento: " << Constant::timeFormat(tiempoMovimiento) << " tiempoParado: " << Constant::timeFormat(tiempoParado)
    << endl;

    for (int i=0; i < listaCumbresYValles.size(); i++){
        CumbreValle topografia = listaCumbresYValles.at(i);
        cout << std::setprecision(1) << (topografia.tipo == CUMBRE ? "Cumbre" : "Valle") <<  " de altura " << topografia.ele << " en km "
             << std::setprecision(3) << topografia.dist / 1000.0
             << std::setprecision(8) << " lat: " << topografia.geoPos.getLatitude() << " lon: " << topografia.geoPos.getLongitude()
             << endl;
    }

}

/**
*
*/
void GeoDrawer::resetStats(double lat, double lon, double alt){
    this->acumSubida = 0.0;
    this->acumBajada = 0.0;
    this->maxAltitud = alt;
    this->minAltitud = alt;
    this->distancia = 0;
    this->maxDesnivel2Puntos = 0;
    this->sumaPendienteSubida = 0;
    this->sumaPendienteBajada = 0;
    this->numPendienteSubida = 0;
    this->numPendienteBajada = 0;
    this->distLlano = 0.0;
    this->distSubida = 0.0;
    this->distBajada = 0.0;
    this->velocidadMaxima = 0.0;
    this->velocidadMinima = 300000; //La velocidad de la luz 300.000 m/s. ! A ver quien lo supera
    this->sumaVelocidadMedia = 0;
    this->numSumaVelocidad = 0;
    this->velocidadMedia = 0;
    this->tiempoTotal = 0;
    this->tiempoParado = 0;
    this->tiempoMovimiento = 0;
    this->listaCumbresYValles.clear();
}

/**
*
*/
void GeoDrawer::calcularEstadisticasRuta(double lat, double lon, double alt, long time, StatsClass *stats){

    const int maxPendientePermitida = 100;  //En senderismo es posible pendientes del 100 o mayores. Pero establecemos este limite para evitar problemas con gps
    const int limitePendienteLlano = 3;     //Establecemos que una pendiente menor al 3% es terreno llano
    const float margenPendiente = 0.05;     //En porcentaje 0.05 = 5% para medir cambios de tendencias en ascension o descenso
    const float limiteVelocidadMin = 0.15;     //En porcentaje 0.9 = 90% para no contar velocidades demasiado pequeñas en relacion a la distancia recorrida
    const double minDiffAltToCumbres = 20.0;

    //Variables temporales
    double distanciaPuntos = 0.0;
    double diferenciaAlt = 0.0;
    double pendiente = 0.0;
    double velocidadActual = 0.0;
    double difTiempos = 0.0;

    distanciaPuntos = fabs(getDistance(stats->lastLat, stats->lastLon, lat, lon)) * 1000; // en metros
    diferenciaAlt = fabs(alt - stats->lastAlt);
    //Calculamos la pendiente en porcentaje. Si la distancia de puntos es 0, le ponemos un
    //desnivel del 100% (casi imposible normalmente)
    pendiente = distanciaPuntos != 0.0 ? diferenciaAlt * 100 / distanciaPuntos : 100;

    bool endRoutePoint = stats->numCoordRuta == stats->maxCoordRuta - 1;

    if (stats->numCoordRuta <= 1){
        stats->init(lat, lon, alt);
        this->resetStats(lat, lon, alt);
    } else if (alt != 0.0){
        //Si la altitud de la cumbre es exactamente 0.0 es muy probable que se haya EDITADO el track y
        //que la altura no sea correcta. Por lo tanto no lo procesamos

        //Nos fiamos de la coordenadas de latitud y longitud para calcular distancias
        distancia += distanciaPuntos;
        //No permitimos pendientes superiores a un porcentaje por posibles
        //desajustes en las mediciones que obtenemos en el gps. (Si somos Felix Baumgartner
        //lo deberiamos desactivar)
        if (pendiente < maxPendientePermitida || correccionGPS == false){
            //Calculamos las pendientes medias de subida y bajada
            if (pendiente > limitePendienteLlano && alt > stats->lastAlt){
                sumaPendienteSubida += pendiente;
                numPendienteSubida++;
                acumSubida += diferenciaAlt;
            } else if (pendiente > limitePendienteLlano && alt < stats->lastAlt){
                sumaPendienteBajada += pendiente;
                numPendienteBajada++;
                acumBajada += diferenciaAlt;
            }

            bool processDiff = fabs(alt - stats->alturaValle) > minDiffAltToCumbres
                            || fabs(alt - stats->alturaCumbre) > minDiffAltToCumbres;

            if (endRoutePoint && listaCumbresYValles.size() > 0){
                CumbreValle tmpTopo = listaCumbresYValles.at(listaCumbresYValles.size() - 1);
                CumbreValle topografia;
                topografia.geoPos.setLatitude(lat);
                topografia.geoPos.setLongitude(lon);
                topografia.ele =  alt > tmpTopo.ele ? stats->altitudCumbre : stats->altitudValle;
                topografia.tipo = alt > tmpTopo.ele ? CUMBRE : VALLE;
                topografia.dist = alt > tmpTopo.ele ? stats->distCumbre : stats->distValle;
                listaCumbresYValles.push_back(topografia);

            //Calculos de cumbres y valles de la ruta
            } else if ( alt > stats->alturaCumbre * (double)(1.0 + margenPendiente) && processDiff ){
                //Estamos subiendo
                stats->alturaCumbre = alt;
                stats->alturaValle = alt;

                if (stats->isSubiendo == false && stats->lastPosVallesCumbres > 0){
                    CumbreValle topografia;
                    topografia.geoPos.setLatitude(lat);
                    topografia.geoPos.setLongitude(lon);
                    topografia.ele = stats->altitudValle;
                    topografia.tipo = VALLE;
                    topografia.dist = stats->distValle;
                    listaCumbresYValles.push_back(topografia);

//                    cout << std::setprecision(1) << "Valle de altura " << altitudValle << " en km "
//                         << std::setprecision(3) << distValle / 1000.0
//                         << std::setprecision(8) << " lat: " << lat << " lon: " << lon
//                         << endl;
                }
                stats->lastPosVallesCumbres++;
                stats->altitudCumbre = 0.0;
                stats->altitudValle = alt;
                stats->isSubiendo = true;
            } else if (alt < stats->alturaValle * (double)(1.0 - margenPendiente) && processDiff){
                //Estamos bajando
                stats->alturaValle = alt;
                stats->alturaCumbre = alt;

                if (stats->isSubiendo == true && stats->lastPosVallesCumbres > 0){
                    CumbreValle topografia;
                    topografia.geoPos.setLatitude(lat);
                    topografia.geoPos.setLongitude(lon);
                    topografia.ele = stats->altitudCumbre;
                    topografia.tipo = CUMBRE;
                    topografia.dist = stats->distCumbre;
                    listaCumbresYValles.push_back(topografia);
//                    cout << std::setprecision(1) << "Cumbre de altura " << altitudCumbre << " en km "
//                         << std::setprecision(3) << distCumbre / 1000.0
//                         << std::setprecision(8) << " lat: " << lat << " lon: " << lon
//                         << endl;
                }
                stats->lastPosVallesCumbres++;
                stats->altitudCumbre = 0.0;
                stats->altitudValle = alt;
                stats->isSubiendo = false;
            } else if (alt != 0.0){
                //No se detecta variacion suficiente para decidir si la tendencia es de subida o bajada
                if (alt > stats->altitudCumbre){
                    stats->altitudCumbre = alt;
                    stats->distCumbre = distancia;
                }
                if (alt < stats->altitudValle){
                    stats->altitudValle = alt;
                    stats->distValle = distancia;
                }
            }
        } //Fin correccion del gps

        //Calculamos la distancia de subida, bajada o llano segun la pendiente
        if (pendiente > limitePendienteLlano && alt > stats->lastAlt){
            distSubida += distanciaPuntos;
        } else if (pendiente > limitePendienteLlano && alt < stats->lastAlt){
            distBajada += distanciaPuntos;
        } else {
            distLlano += distanciaPuntos;
        }

        //Maximo desnivel de dos puntos
        if (diferenciaAlt > maxDesnivel2Puntos){
                maxDesnivel2Puntos = diferenciaAlt;
        }

        //Se calcula el maximo y el minimo de altitud
        if (alt > maxAltitud){
            maxAltitud = alt;
        }
        if (alt < minAltitud){
            minAltitud = alt;
        }

        difTiempos = fabs(time - stats->lastTime);
        tiempoTotal += difTiempos;

        //Calculamos las velocidades
        if (difTiempos > 0.0 && distanciaPuntos > 0.0 ){
            if (distanciaPuntos / difTiempos > limiteVelocidadMin || correccionGPS == false){
                velocidadActual = distanciaPuntos / difTiempos;
//                cout << std::setprecision(1) << "velocidadActual: " << (velocidadActual * 3600.0 / 1000.0) << " (km/h)"
//                     << " distanciaPuntos: " << distanciaPuntos << " (m)"
//                     << " tiempo: " << difTiempos << " (s)" <<  endl;
                sumaVelocidadMedia += velocidadActual;
                numSumaVelocidad++;
                tiempoMovimiento += difTiempos;
            } else {
                tiempoParado += difTiempos;
            }
        } else {
            velocidadActual = 0.0;
            tiempoParado += difTiempos;
        }

        if (velocidadActual > velocidadMaxima){
            velocidadMaxima = velocidadActual;
        }

        //No aceptamos una velocidad de 0 km/h para poner en la minima
        if ((velocidadActual < velocidadMinima && velocidadActual > 0.0) || correccionGPS == false){
            velocidadMinima = velocidadActual;
        }

        //Actualizamos valores. ESTO SIEMPRE AL FINAL******
        stats->lastLat = lat;
        stats->lastLon = lon;
        stats->lastAlt = alt;
        stats->lastTime = time;
    }
}

/**
* http://stackoverflow.com/questions/2103924/mercator-longitude-and-latitude-calculations-to-x-and-y-on-a-cropped-map-of-the

var mapWidth = 1500;
var mapHeight = 1577;
var mapLonLeft = 9.8;
var mapLonRight = 10.2;
var mapLonDelta = mapLonRight - mapLonLeft;
var mapLatBottom = 53.45;
var mapLatBottomDegree = mapLatBottom * Math.PI / 180;
*/
void GeoDrawer::convertGeoToPixel(double latitude, double longitude, Point *posicion){
    posicion->x = (longitude - mapLonLeft) * getWidth() / mapLonDelta;
    latitude = GeoStructs::toRadians(latitude);
    posicion->y = getHeight() - ((((getWidth() / mapLonDelta) * 360) / (2 * PI) / 2 * log((1 + sin(latitude)) /
                  (1 - sin(latitude)))) - (((getWidth() / mapLonDelta) * 360) / (2 * PI) / 2 *
                  log((1 + sin(GeoStructs::toRadians(mapLatBottom))) / (1 - sin(GeoStructs::toRadians(mapLatBottom))))));
}

void GeoDrawer::convertPixelToGeo(Point *posicion, VELatLong *geoPos)
{
    double mapLatBottomRadian = GeoStructs::toRadians(mapLatBottom);
    /* called worldMapWidth in Raphael's Code, but I think that's the radius since it's the map width or circumference divided by 2*PI  */
    double worldMapRadius = getWidth() / mapLonDelta * 360/(2 * PI);
    double mapOffsetY = ( worldMapRadius / 2 * log( (1 + sin(mapLatBottomRadian) ) / (1 - sin(mapLatBottomRadian))  ));
    double equatorY = getHeight() + mapOffsetY;
    double a = (equatorY-posicion->y)/worldMapRadius;

    geoPos->setLatitude(180/PI * (2 * atan(exp(a)) - PI/2));
    geoPos->setLongitude(mapLonLeft+posicion->x/getWidth()*mapLonDelta);

}

/**
*
*/
double GeoDrawer::todouble(string dato){
  return atof(dato.c_str());
}

/**
* Devuelve la distancia entre dos puntos en km
* @param latlong1
* @param latlong2
* @return
*/
double GeoDrawer::getDistance(double latitude1, double longitude1, double latitude2, double longitude2) {
    if (latitude1 == latitude2 && longitude1 == longitude2) {
        return 0.0;
    } else {
        return GeoStructs::toDegrees(acos(sin(GeoStructs::toRadians(latitude1)) * sin(GeoStructs::toRadians(latitude2)) +
                              cos(GeoStructs::toRadians(latitude1)) * cos(GeoStructs::toRadians(latitude2)) * cos(GeoStructs::toRadians(longitude2 - longitude1)))) * 111.302;
    }
    // alternative formulae (haversine)
//    const double two=2.0;
//    double dist = two*asin(sqrt(pow(sin((latitude1-latitude2)/two), 2) +
//        cos(latitude1)*cos(latitude2)*pow(sin((longitude2-longitude1)/two), 2)));
//    return dist;
}

/**
 * Convierte las posiciones de esfericas a cartesianas
 *
 * @param latlong {@link VELatLong}
 * @return {@link Cartesian}
 */
Cartesian * GeoDrawer::convertSphericalToCartesian(VELatLong latlong) {
    return new Cartesian(EARTHRADIUS * cos(GeoStructs::toRadians(latlong.getLatitude())) * cos(GeoStructs::toRadians(latlong.getLongitude())),
                         EARTHRADIUS * cos(GeoStructs::toRadians(latlong.getLatitude())) * sin(GeoStructs::toRadians(latlong.getLongitude())),
                         EARTHRADIUS * sin(GeoStructs::toRadians(latlong.getLatitude())));
}

/**
 * Convierte las posiciones de cartesianas a esfericas
 *
 * @param cartesian {@link Cartesian}
 * @return {@link VELatLong}
 */
VELatLong * GeoDrawer::convertCartesianToSpherical(Cartesian cartesian) {
    return new VELatLong(GeoStructs::toDegrees(asin(cartesian.getZ() / sqrt(cartesian.getX() * cartesian.getX() + cartesian.getY() *
            cartesian.getY() + cartesian.getZ() * cartesian.getZ()))), GeoStructs::toDegrees(atan2(cartesian.getY(), cartesian.getX())));
}

/**
*Intenta hacer zoom para que quepa una distancia dada por pantalla
*/
//void GeoDrawer::zoomToMeters(double meters){
    /** Modelo antiguo
    VELatLong latlong;
    //Partimos por ejemplo de la esquina inferior del mapa
    latlong.setLatitude(mapLatBottom);
    latlong.setLongitude(mapLonLeft);
    //Convertimos esa latitud y longitud en metros
    Cartesian *cart = convertSphericalToCartesian(latlong);
    //sumamos la distancia especificada para tener otro punto del mapa
    //alejado los metros especificados por paramentro
    cart->setX(cart->getX() + meters / 1000.0);
    cart->setY(cart->getY() + meters / 1000.0);
    //Obtenemos cual es la longitud del punto que representa la longitud y latitud alejada
    VELatLong *latLong2 = convertCartesianToSpherical(*cart);
    //Calculamos cuales son las diferencias de latitud y longitud
    const double deltaLon = latLong2->getLongitude() - latlong.getLongitude();
    mapLonDelta  = deltaLon;
    //Centramos el mapa debido al desplazamiento
    mapLonLeft  += mapLonDelta / 2.0;
    mapLonRight -= mapLonDelta / 2.0;
    */

//}

void GeoDrawer::doGoogleZoom(int zoomLevel){
    MercatorProjection *projection= new MercatorProjection();
    VELatLong NELatLon;
    VELatLong SWLatLon;

    VELatLong *center = new VELatLong(mapLatBottom, mapLonLeft);
    projection->GetBounds(*center, zoomLevel, width, height, &NELatLon, &SWLatLon);
    mapLonDelta = fabs(fabs(NELatLon.getLongitude()) - fabs(SWLatLon.getLongitude()));
    cout << "googleZoom: " << zoomLevel << ", mapLonDelta: " << mapLonDelta << endl;
    mapLonLeft  += mapLonDelta / 2.0;
    mapLonRight -= mapLonDelta / 2.0;
    delete projection;
}

/**
*
*/
double GeoDrawer::calculaEscala(){
    return getDistance(mapLonLeft, mapLatBottom, mapLonLeft + mapLonDelta, mapLatBottom);
}

/**
*
*/
void GeoDrawer::zoomOut(){
    desplazamiento = mapLonDelta * 0.1;
    mapLonRight += desplazamiento;
    mapLonLeft -= desplazamiento;
    mapLonDelta = mapLonRight - mapLonLeft;
    if (autocenter) centerScreen(autocenter);
}

/**
*
*/
void GeoDrawer::zoomIn(){
    desplazamiento = mapLonDelta * 0.1;
    mapLonRight -= desplazamiento;
    mapLonLeft += desplazamiento;
    mapLonDelta = mapLonRight - mapLonLeft;
    if (autocenter) centerScreen();
}

/**
*
*/
void GeoDrawer::moveRight(){
    mapLonLeft += desplazamiento;
    mapLonRight += desplazamiento;
    longitudCursor += desplazamiento;
    autocenter = false;
}

/**
*
*/
void GeoDrawer::moveLeft(){
    mapLonLeft -= desplazamiento;
    mapLonRight -= desplazamiento;
    longitudCursor -= desplazamiento;
    autocenter = false;
}

/**
*
*/
void GeoDrawer::moveUp(){
    mapLatBottom += desplazamiento;
    latitudCursor += desplazamiento;
    autocenter = false;
}

/**
*
*/
void GeoDrawer::moveDown(){
    mapLatBottom -= desplazamiento;
    latitudCursor -= desplazamiento;
    autocenter = false;
}

/**
*
*/
void GeoDrawer::setPosicionCursor(double latitud, double longitud){
    latitudCursor = latitud;
    longitudCursor = longitud;
}

/**
*
*/
void GeoDrawer::centerScreen(bool centered){
    centerScreenToLocation(latitudCursor, longitudCursor);
    autocenter = centered;
}

/**
* Ejemplo para pintar varias flechas
*/
int GeoDrawer::calculaAnguloDireccion(Point *lastPixelMapPosT, Point *currentPixelMapPosT){

    int angulo = 0;
    int correccionCuadrante = 0;

    //Si estamos en el cuadrante 1 o 4 hay que sumar 180 grados al angulo calculado
    //El cuadrante se calcula con el centro de coordenadas de la posicion anterior
    if (currentPixelMapPosT->x > lastPixelMapPosT->x && currentPixelMapPosT->y > lastPixelMapPosT->y ||
        currentPixelMapPosT->x > lastPixelMapPosT->x && currentPixelMapPosT->y < lastPixelMapPosT->y){
        correccionCuadrante = 180;
    }

    if (currentPixelMapPosT->x != lastPixelMapPosT->x){
        angulo = -1* atan((currentPixelMapPosT->y - lastPixelMapPosT->y) /(double) (currentPixelMapPosT->x - lastPixelMapPosT->x)) * 180 / (double)PI;
        angulo += correccionCuadrante;
    } else {
        if (currentPixelMapPosT->y > lastPixelMapPosT->y ){
            angulo = 90;
        } else {
            angulo = 270;
        }
    }

    if (currentPixelMapPosT->y == lastPixelMapPosT->y){
        if (currentPixelMapPosT->x > lastPixelMapPosT->x ){
            angulo = 180;
        } else {
            angulo = 0;
        }
    }

    //Dibujamos la posicion centrada
    //pintarTriangulo(currentPixelMapPos->x,currentPixelMapPos->y, angulo);
    return angulo;
}


/**
* Calcula los dos vertices de un triangulo equilatero, dado uno de ellos que sera el que apunte al
* punto indicado
*/
void GeoDrawer::calculaVerticesFlecha( double angulo, int *datosFlecha, double anguloFlecha){
    anguloFlecha = anguloFlecha * PI / 180.0;        // En radianes. Indica lo abierta que esta la flecha
    double radAngulo = -(angulo + 180.0) * PI / 180.0;     // En radianes. Calcula el angulo indicado pero en radianes.
                                                      // Le restamos 180 para que cuadre con la bisectriz del triangulo
                                                      // cuando lo imaginamos sobre coordenadas cartesianas

    //Calculamos la posicion de un vertice
    datosFlecha[0] = -cos(anguloFlecha - radAngulo) * hipotenusa;
    datosFlecha[1] = sin(anguloFlecha - radAngulo) * hipotenusa;
    //Calculamos la posicion de otro vertice
    datosFlecha[2] = -cos(anguloFlecha + radAngulo) * hipotenusa;
    datosFlecha[3] = -sin(anguloFlecha + radAngulo) * hipotenusa;
}

void GeoDrawer::calcTilesPixels(){

    double difLon = fabs(fabs(mapLonLeft) - fabs(mapLonRight));
    double difLat = fabs(fabs(mapLatUp) - fabs(mapLatBottom));



}

