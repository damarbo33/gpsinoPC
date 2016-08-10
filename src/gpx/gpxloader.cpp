#include "gpxloader.h"

GpxLoader::GpxLoader()
{
    //ctor
}

GpxLoader::~GpxLoader()
{
    //dtor
}

/**
* Carga un fichero gpx en memoria y lo guarda procesado pero en latitud y longitud
*/
bool GpxLoader::loadGPX(string filename){

    TiXmlDocument doc(filename.c_str());
	bool loadOkay = doc.LoadFile();

	if (loadOkay){
		cout << filename << endl;
		//Cargamos las querys
		loadGPX(&doc);
        //anyadimos info de waypoints si existen
        findWaypoints();
        //Guardamos el fichero procesado
        return true;
	}
	else{
		Traza::print("Ha ocurrido un error al abrir el fichero " + filename + ". Posible error de formato",W_DEBUG);
		return false;
	}
}

/**
*
*/
void GpxLoader::loadGPX( TiXmlNode* pParent)
{
	if ( !pParent ) return;
    TiXmlNode* pChild;
    TiXmlText* pText;
    int t = 0;
    int pos = 0;

    TiXmlElement* root = pParent->FirstChildElement();
    if(root == NULL){
		cerr << "Failed to load file: No root element." << endl;
		return;
	}

    gpxData.clear();
    gpxWaypoints.clear();
    bool isTrk = false;
    bool isWpt = false;

    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		string elemName = elem->Value();
		Constant::lowerCase(&elemName);
        //Traza::print( "TINYXML_ELEMENT:" + elemName, W_DEBUG);
//        cout << "TINYXML_ELEMENT:" << elemName << endl;
        const char* attr;

        GpxInfo strCoord;
        isTrk = elemName.compare("trk") == 0;
        isWpt = elemName.compare("wpt") == 0;

		if(isWpt){
            addGpxPoint(elem, &gpxWaypoints);
		} else if (isTrk){
            for (TiXmlElement* e = elem->FirstChildElement("trkseg"); e != NULL ; e = e->NextSiblingElement()){
                if (e != NULL){
                    for(TiXmlElement* elemtrkpt = e->FirstChildElement(); elemtrkpt != NULL; elemtrkpt = elemtrkpt->NextSiblingElement()){
                        elemName = elemtrkpt->Value();
                        Constant::lowerCase(&elemName);
//                        cout << "elemName:" << elemName << endl;
                        addGpxPoint(elemtrkpt, &gpxData);
                    }
                }
            }
		}
	}
}

/**
*
*/
void GpxLoader::addGpxPoint(TiXmlElement* elem, std::vector<GpxInfo> *gpxVect){
    const char* attr;
    GpxInfo strCoord;

    //Anyadimos latitud
    attr = elem->Attribute("lat");
    if(attr != NULL){
        strCoord.setLat(GeoDrawer::todouble(attr));
    }
    //Anyadimos longitud
    attr = elem->Attribute("lon");
    if(attr != NULL){
        strCoord.setLon(GeoDrawer::todouble(attr));
    }

    //Obtenemos la etiqueta ele que no contiene atributos
    TiXmlElement* e = elem->FirstChildElement("ele");
    if (e != NULL && e->FirstChild() != NULL && e->FirstChild()->ToText() != NULL){
        //Obtenemos el contenido del elemento ele
        strCoord.setEle(GeoDrawer::todouble(e->FirstChild()->ToText()->Value()));
    }

    //Obtenemos la etiqueta time que no contiene atributos
    e = elem->FirstChildElement("time");
    if (e != NULL && e->FirstChild() != NULL && e->FirstChild()->ToText() != NULL){
        //Obtenemos el contenido del elemento ele
        strCoord.setTime(e->FirstChild()->ToText()->Value());
    }

    //Obtenemos la etiqueta name que no contiene atributos
    e = elem->FirstChildElement("name");
    if (e != NULL && e->FirstChild() != NULL && e->FirstChild()->ToText() != NULL){
        //Obtenemos el contenido del elemento ele
        strCoord.setName(e->FirstChild()->ToText()->Value());
    } else {
        //Obtenemos la etiqueta name que no contiene atributos
        e = elem->FirstChildElement("cmt");
        if (e != NULL && e->FirstChild() != NULL && e->FirstChild()->ToText() != NULL){
            //Obtenemos el contenido del elemento ele
            strCoord.setName(e->FirstChild()->ToText()->Value());
        }
    }


    gpxVect->push_back(strCoord);
}

/**
*
*/
void GpxLoader::findWaypoints(){
    double minDist = 0;
    double actualDist = 0;
    int posMinDist = -1;

    cout << "hay " << gpxWaypoints.size() << " waypoints y " << gpxData.size() << " datos de gps" << endl;

    for (int i=0; i < gpxWaypoints.size(); i++){
        //Reseteamos las distancias
        actualDist = 0;
        posMinDist = -1;

        //Buscamos cual es la posicion con menor distancia
        for (int j=0; j < gpxData.size(); j++){
            actualDist = GeoDrawer::getDistance(gpxData.at(j).getLat(), gpxData.at(j).getLon(),
                                                gpxWaypoints.at(i).getLat(), gpxWaypoints.at(i).getLon());
            if (j == 0){
                minDist = actualDist;
            } else {
                if (actualDist < minDist){
                    minDist = actualDist;
                    posMinDist = j;
                }
            }
        }
        //Cuando la encontremos, asignamos el texto
        if (posMinDist >= 0)
            gpxData.at(posMinDist).setName(gpxWaypoints.at(i).getName());
    }
}

/**
*
*/
string GpxLoader::SaveToFile(string filename){
    Dirutil dir;


    string newDir = dir.getFolder(filename) + FILE_SEPARATOR + dir.getFileNameNoExt(filename);
    string processedFile = newDir + FILE_SEPARATOR + dir.getFileNameNoExt(filename) + ".dat";

    dir.createDir(newDir.c_str());

    ofstream myfile (processedFile.c_str());
    time_t time1, time2;

    if (gpxData.size() > 0){
        Constant::setTime(gpxData.at(0).getTime(), &time1);
    }

    for (int i=0; i < gpxData.size(); i++){
        Constant::setTime(gpxData.at(i).getTime(), &time2);

        myfile << std::setprecision(8) << std::fixed << gpxData.at(i).getLat() << "," << gpxData.at(i).getLon()
        << "," << std::setprecision(0) << gpxData.at(i).getEle()
        << "," << (gpxData.at(i).getTime().empty() ? "" : Constant::TipoToStr(Constant::diferenciaTiempos(time1, time2)))
        << "," << (gpxData.at(i).getName().empty() ? "" : gpxData.at(i).getName())
        << VBCRLF;
    }
    myfile.close();
    return processedFile;
}
