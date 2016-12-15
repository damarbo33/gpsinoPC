#include "mercatorprojection.h"

static const double MERCATOR_RANGE_X = 256.0;
static const double MERCATOR_RANGE_Y = 256.0;

/**http://stackoverflow.com/questions/12688092/google-maps-static-api-get-sw-and-ne-by-center-coordinate*/

MercatorProjection::MercatorProjection(){
    pixelOrigin_ = new Point( MERCATOR_RANGE_X / 2.0, MERCATOR_RANGE_Y / 2.0);
    pixelsPerLonDegree_ = MERCATOR_RANGE_Y / 360.0;
    pixelsPerLonRadian_ = MERCATOR_RANGE_Y / (2.0 * PI);
}

/**
*
*/
MercatorProjection::~MercatorProjection(){
	delete pixelOrigin_;
}

/**
*
*/
double MercatorProjection::bound(double value, double opt_min, double opt_max) {
  value = min(value, opt_max);
  value = max(value, opt_min);
  return value;
}

/**
*
*/
void MercatorProjection::fromLatLngToPoint(VELatLong *latLng, Point *point){
    double siny = 0;

    if (point != NULL){
        siny = bound(sin(GeoStructs::toRadians(latLng->getLatitude())), -0.9999, 0.9999);
        //cout << pixelOrigin_->x << ";" << latLng->getLongitude() << ";" << pixelsPerLonDegree_ << endl;
        point->x = pixelOrigin_->x + latLng->getLongitude() * pixelsPerLonDegree_;
        // NOTE(appleton): Truncating to 0.9999 effectively limits latitude to
        // 89.189.  This is about a third of a tile past the edge of the world tile.
        point->y = pixelOrigin_->y + 0.5 * log((1.0 + siny) / (1.0 - siny)) * -pixelsPerLonRadian_;
//        cout << "point->x " << point->x << endl ;
//        cout << "point->y " << point->y <<endl ;
    }
}

/**
*
*/
void MercatorProjection::fromPointToLatLng(Point *point, VELatLong *latLng){
    double latRadians = 0;
    if (latLng != NULL){
        latLng->setLongitude((point->x - pixelOrigin_->x) / pixelsPerLonDegree_);
        latRadians = (point->y - pixelOrigin_->y) / -pixelsPerLonRadian_;
        //latLng->setLatitude(GeoDrawer::toDegrees(2.0 * atan(exp(latRadians)) - PI / 2.0));
        latLng->setLatitude(GeoStructs::toDegrees(atan(sinh(latRadians))));
    }
}

/**
* Dada la esquina superior izquierda de un tile de un mapa, permite obtener
* cual es la posicion x,y en el mismo mapa de otra lat,lon especificada
*/
void MercatorProjection::getPosPixelTile(VELatLong *currentLatLon, int zoom, Point *MapXY, Point *numTile){
    double scale = pow(2, zoom);
    Point NWPoint;
    Point currentPoint;

    GeoStructs geoStruct;
    numTile->x = geoStruct.long2tilex(currentLatLon->getLongitude(), zoom);
    numTile->y = geoStruct.lat2tiley(currentLatLon->getLatitude(), zoom);

    //el xtile es la posicion del latitud y longitud segun la esquina del mapa
    double longitud = geoStruct.tilex2long(numTile->x, zoom);
    double latitud = geoStruct.tiley2lat(numTile->y, zoom);

    //Indicamos la esquina del mapa que debe coincidir con la obtenida por la posicion del tile
    VELatLong *NWLatLon = new VELatLong(latitud, longitud);


    fromLatLngToPoint(NWLatLon, &NWPoint);
    fromLatLngToPoint(currentLatLon, &currentPoint);

    NWPoint.x = NWPoint.x * scale;
    NWPoint.y = NWPoint.y * scale;
    currentPoint.x = currentPoint.x * scale;
    currentPoint.y = currentPoint.y * scale;

    MapXY->x = floor(fabs(fabs(NWPoint.x) - fabs(currentPoint.x)));
    MapXY->y = floor(fabs(fabs(NWPoint.y) - fabs(currentPoint.y)));
}

/**
*
*/
void MercatorProjection::GetBounds(VELatLong center, int zoom, int mapWidth, int mapHeight,
                                   VELatLong * NELatLon, VELatLong *SWLatLon){
    double scale = pow(2, zoom);
    Point centerPixel;
    fromLatLngToPoint(&center, &centerPixel);

    centerPixel.x = centerPixel.x * scale;
    centerPixel.y = centerPixel.y * scale;
//    cout << "centerPixel.x " << centerPixel.x << endl ;
//    cout << "centerPixel.y " << centerPixel.y <<endl ;


    Point NEPixel;
    NEPixel.x = centerPixel.x + mapWidth / 2.0;
    NEPixel.y = centerPixel.y - mapHeight / 2.0;

    Point SWPixel;
    SWPixel.x = centerPixel.x - mapWidth / 2.0;
    SWPixel.y = centerPixel.y + mapHeight / 2.0;

    Point NEWorld;
    NEWorld.x = NEPixel.x / scale;
    NEWorld.y = NEPixel.y / scale;

    Point SWWorld;
    SWWorld.x = SWPixel.x/ scale;
    SWWorld.y = SWPixel.y / scale;

    fromPointToLatLng(&NEWorld, NELatLon);
    fromPointToLatLng(&SWWorld, SWLatLon);
}



/* NO USAR ESTE!!!!!!!!!!!!
void MercatorProjection::getCorners(VELatLong *center, int zoom, int mapWidth, int mapHeight,
                     VELatLong * NELatLon, VELatLong *SWLatLon){
    double scale;
    Point centerPx, *SWPoint, *NEPoint;

    scale = pow(2, zoom);
    centerPx.x=0;
    centerPx.y=0;
    fromLatLngToPoint(center, &centerPx);

    SWPoint = new Point(floor(centerPx.x-(mapWidth/2.0)/scale), floor(centerPx.y+(mapHeight/2.0)/scale));
    NEPoint = new Point(floor(centerPx.x+(mapWidth/2.0)/scale), floor(centerPx.y-(mapHeight/2.0)/scale));
    fromPointToLatLng(SWPoint, SWLatLon);
    fromPointToLatLng(NEPoint, NELatLon);
    delete SWPoint;
    delete NEPoint;
}
*/
