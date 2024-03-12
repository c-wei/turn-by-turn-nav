#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "geopoint.h"
#include "hashmap.h"
#include <string>
#include <vector>

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    struct graphNode{
        std::string type, name;
        std::vector<graphNode*> connectedPts;
        GeoPoint val;
    };

    HashMap<GeoPoint> poiToLoc;
    HashMap<std::string> pointToStreet;
    HashMap<graphNode> pointToNode;
    
};

#endif
