#ifndef router_h
#define router_h

#include "geodb.h"
#include "geotools.h"

#include <vector>

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const = 0;
    
private:
    
    struct AStarNode{
        GeoPoint geoPt;
        double aStarValue;
        double lengthToNode;
    };
    
    const GeoDatabaseBase *m_gdb;
    //HashMap<GeoPoint> visitedPoints;

    //std::vector<AStarNode> possPaths;

    bool comp(AStarNode &pt1, AStarNode &pt2) { return pt1.lengthToNode + pt1.aStarValue < pt2.lengthToNode + pt2.aStarValue; }   //return true if pt1 < pt2
    
    int findLeastIndex(const AStarNode &n, const std::vector<AStarNode> &vec) const{
        int min = 0;
        int max = vec.size() - 1;
        while(min < max){
            int mid = min + (max - min) / 2;

            if(vec[mid].aStarValue + vec[mid].lengthToNode == n.aStarValue + n.lengthToNode)
                return mid;
            
            if(n.aStarValue + n.lengthToNode > vec[mid].aStarValue + vec[mid].lengthToNode){
                max = mid - 1;
            }
            else
                min = mid + 1;
        }
        return min;
    }
};

#endif
