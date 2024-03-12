#include "router.h"
Router::Router(const GeoDatabaseBase& geo_db) { }
Router::~Router() { }

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    std::vector<AStarNode> possPaths;
    HashMap<GeoPoint> visitedPoints;

    AStarNode p1 = {pt1, distance_earth_miles(pt1, pt2), 0};
    possPaths.push_back(p1);
}



