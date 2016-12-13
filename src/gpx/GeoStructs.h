#ifndef GEOSTRUCTS_H_INCLUDED
#define GEOSTRUCTS_H_INCLUDED

using namespace std;
#include <string>
#include <Constant.h>
#include <math.h>

/**
 * Clase para almacenar coordenadas en formato cartesiano
 * @author everis
 *
 */

class Cartesian{
	public:

        /**
         * Constructor
         * @param x {@link double}
         * @param y {@link double}
         * @param z {@link double}
         */
        Cartesian(double x, double y, double z){
              this->x = x;
              this->y = y;
              this->z = z;
        }

        Cartesian(){
            this->x = 0.0;
            this->y = 0.0;
            this->z = 0.0;
        }

        /**
         * @return the x
         */
        double getX() {
            return x;
        }
        /**
         * @param x the x to set
         */
        void setX(double x) {
            this->x = x;
        }
        /**
         * @return the y
         */
        double getY() {
            return y;
        }
        /**
         * @param y the y to set
         */
        void setY(double y) {
            this->y = y;
        }
        /**
         * @return the z
         */
        double getZ() {
            return z;
        }
        /**
         * @param z the z to set
         */
        void setZ(double z) {
            this->z = z;
        }
    private:
        //Representa la coordenada x
        double x;
        //Representa la coordenada y
        double y;
        //Representa la coordenada z
        double z;
};

class VELatLong {
    public:
        /**
         * [CAM_CONSTRUCTOR]
         */
        VELatLong() {
            latitude = 0.0;
            longitude = 0.0;
        }

        /**
         * Constructor
         *
         * @param latitude {@link double}
         * @param longitude {@link double}
         */
        VELatLong(double latitude, double longitude) {
            this->latitude = latitude;
            this->longitude = longitude;
        }

        void reset(){
            latitude = 0.0;
            longitude = 0.0;
        }

        /**
         * @return the latitude
         */
        double getLatitude() {
            return latitude;
        }

        /**
         * @param latitude the latitude to set
         */
        void setLatitude(double latitude) {
            this->latitude = latitude;
        }

        /**
         * @return the longitude
         */
        double getLongitude() {
            return longitude;
        }

        /**
         * @param longitude the longitude to set
         */
        void setLongitude(double longitude) {
            this->longitude = longitude;
        }

    private:
        //Campo de latitud
        double latitude;
        //Campo de longitud
        double longitude;
};

class Point{
    public :
        double x, y;       //Coordenadas convertidas para poder pintarlas por pantalla
        Point(){x=0; y=0;};
        Point(double x, double y){
            this->x = x;
            this->y = y;
        }
        void reset(){
            x=0; y=0;
        }
};

class PosMapa{
    public :
        Point point;       //Coordenadas convertidas para poder pintarlas por pantalla
        int ele;     //Para la elevacion
        unsigned long time;    //Para el tiempo
        string name;    //Para el waypoint
        double angle;   //Angulo de la posicion
        VELatLong geopos; //localizacion en coordenadas gps
        double distancia; //Distancia recorrida

        PosMapa(){
            ele=0;
            time=0;
            name="";
            angle=0;
            geopos.setLatitude(0.0);
            geopos.setLongitude(0.0);
            distancia = 0.0;
        }

        PosMapa(int x1, int y1){
            PosMapa();
            point.x=x1;
            point.y=y1;
        }
};

class CumbreValle{
    public:
        VELatLong geoPos;   //Latitud y longitud
        double ele;         //Para la elevacion
        int tipo;
        double dist;
};

 class BoundingBox {
     public:
        double north;
        double south;
        double east;
        double west;
  };

class GeoStructs{
    public:
        /**
        *
        */
        static double toRadians(double degrees){
            return (degrees * PI) / 180.0 ;
        }

        /**
        *
        */
        static double toDegrees(double radians){
            return (radians * 180.0) / PI ;
        }


        /**
        * http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Zoom_levels
        */
        static int long2tilex(double lon, int z){
            return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
        }

        /**
        *
        */
        static int lat2tiley(double lat, int z){
            return (int)(floor( (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
        }

        /**
        *
        */
        static double tilex2long(int x, int z){
            return x / pow(2.0, z) * 360.0 - 180;
        }

        /**
        *
        */
        static double tiley2lat(int y, int z){
            double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
            return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
        }

        /**
        *
        */
        static BoundingBox tile2boundingBox(int x, int y, int zoom) {
            BoundingBox bb;
            //El noroeste es la esquina superior izquierda
            bb.north = tiley2lat(y, zoom);
            bb.west = tilex2long(x, zoom);
            //El sudoeste es la esquina inferior derecha
            bb.south = tiley2lat(y + 1, zoom);
            bb.east = tilex2long(x + 1, zoom);
            return bb;
        }
};



#endif // GEOSTRUCTS_H_INCLUDED
