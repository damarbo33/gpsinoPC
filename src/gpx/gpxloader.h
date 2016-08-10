#ifndef GPXLOADER_H
#define GPXLOADER_H

#include "tinyxml/tinyxml.h"
#include <string>
#include <vector>
#include <iostream>
#include "geodrawer.h"
#include "Dirutil.h"

using namespace std;


class GpxInfo{
    public:
        GpxInfo(){lat=0.0;lon=0.0;ele=0.0;time="";name="";}
        void setLat(double lat) { this->lat = lat; }
        double getLat() { return this->lat; }
        void setLon(double lon) { this->lon = lon; }
        double getLon() { return this->lon; }
        void setEle(double ele) { this->ele = ele; }
        double getEle() { return this->ele; }
        void setTime(string time) { this->time = time; }
        string getTime() { return this->time; }
        void setName(string name) { this->name = name; }
        string getName() { return this->name; }
    private:
        double lat;     //Para la latitud
        double lon;     //Para la longitud
        double ele;     //Para la elevacion
        string time;    //Para el tiempo
        string name;    //Para el waypoint



};

class GpxLoader
{
    public:
        /** Default constructor */
        GpxLoader();
        /** Default destructor */
        virtual ~GpxLoader();

        bool loadGPX(string filename);
        string SaveToFile(string filename);
        std::vector<GpxInfo> gpxData;

    protected:
    private:

        std::vector<GpxInfo> gpxWaypoints;
        void loadGPX( TiXmlNode* pParent);
        void addGpxPoint(TiXmlElement* elem, std::vector<GpxInfo> *gpxVect);
        void findWaypoints();

};

#endif // GPXLOADER_H
