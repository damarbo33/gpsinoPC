#ifndef UTMUTILS_H
#define UTMUTILS_H

#include <iostream>
#include <math.h>
using namespace std;

class Utmutils
{
    public:
        Utmutils();
        virtual ~Utmutils();
        void initDatum(int projection);
        void getFastUTM(double theLatitude, double theLongitude);
        void getUTM(double theLatitude, double theLongitude);
        double getX(){return x;}
        double getY(){return y;}
        double getZone(){return utmz;}

    protected:
        void GeogToUTM(double theLatitude, double theLongitude);
    private:
        //Symbols as used in USGS PP 1395: Map Projections - A Working Manual
        double DatumEqRad[14];
        double DatumFlat[14];
        int Item;//Default
        double k0;//scale on central meridian
        double a;//equatorial radius, meters.
        double f;//polar flattening.
        double b;//polar axis.
        double e;//eccentricity
        double esq;
        double e0sq;
        const static double drad = M_PI/180;//Convert degrees to radians)
        double latd;//latitude in degrees
        double phi;//latitude (north +, south -), but uses phi in reference
        double e0;//e prime in reference
        double N;
        double T;
        double C;
        double lng;//Longitude (e = +, w = -) - can't use long - reserved word
        double lng0;//longitude of central meridian
        double lngd;//longitude in degrees
        double M;//M requires calculation
        double x;//x coordinate
        double y;//y coordinate
        int k;//local scale
        int utmz;//utm zone
        int zcm;//zone central meridian
        string DigraphLetrsE;
        string DigraphLetrsN;
        bool OOZok;
};

#endif // UTMUTILS_H
