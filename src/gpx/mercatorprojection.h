#ifndef MERCATORPROJECTION_H
#define MERCATORPROJECTION_H

using namespace std;
#include "GeoStructs.h"
#include <iostream>
 #include <stdlib.h>

class MercatorProjection
{
    public:
        /** Default constructor */
        MercatorProjection();
        /** Default destructor */
        virtual ~MercatorProjection();

        void fromLatLngToPoint(VELatLong *latLng, Point *point);
        void fromPointToLatLng(Point *point, VELatLong *latLng);
        void GetBounds(VELatLong center, int zoom, int mapWidth, int mapHeight,
                                     VELatLong * NELatLon, VELatLong *SWLatLon);

        void getPosPixelTile(VELatLong *NWLatLon, VELatLong *currentLatLon, int zoom, int mapWidth, int mapHeight,
                                     Point *MapXY);

    protected:

    private:
        double bound(double value, double opt_min, double opt_max);

        Point *pixelOrigin_;
        double pixelsPerLonDegree_;
        double pixelsPerLonRadian_;
};

#endif // MERCATORPROJECTION_H
