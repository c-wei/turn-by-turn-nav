#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "geopoint.h"
//correct?
#include "hashmap.h"
#include <string>
#include <vector>

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file) = 0;
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const = 0;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const = 0;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const = 0;
private:
    struct graphNode{
        std::string type, name;
        std::vector<graphNode*> connectedPts;
        GeoPoint val;
    };

    HashMap<std::string> pointsOfInterest;
    HashMap<std::string> pointToStreet;
    HashMap<graphNode> pointToNode;
    
    /*
    struct poi{
        std::string poiName;
        std::string streetName;
        std::string start, mid, end, actual;
    };
    
    struct poiPoint{
        double lat, lon;
    };
     */
};

#endif
