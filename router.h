#ifndef router_h
#define router_h

#include "geodb.h"
#include "geotools.h"

#include <queue>
#include <vector>

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
    
private:
    
    struct AStarNode{
        GeoPoint geoPt;
        double heuristicVal;
        double lengthToNode;
    };
    
    struct compare
    {
        bool operator()(const AStarNode &pt1, AStarNode &pt2) const { return (pt1.heuristicVal + pt1.lengthToNode) > (pt2.heuristicVal + pt2.lengthToNode); }
    };
    
    const GeoDatabaseBase *m_gdb;
};

#endif
