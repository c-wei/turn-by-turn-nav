#include "router.h"
Router::Router(const GeoDatabaseBase& geo_db) { m_gdb = &geo_db; }
Router::~Router() { }

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    std::vector<AStarNode> possPaths;
    std::vector<GeoPoint> toVisit;
    HashMap<GeoPoint> visitedPoints;
    AStarNode slow = {GeoPoint(), 0, 0};
    
    AStarNode n1 = {pt1, distance_earth_miles(pt1, pt2), 0};
    possPaths.push_back(n1);
    
    while(!possPaths.empty()){
        AStarNode frontNode = possPaths[possPaths.size() - 1];
        possPaths.pop_back();
        
        //frontNode.node.get_connected_points() to look @ children
        std::vector<GeoPoint> toVisit = m_gdb->get_connected_points(frontNode.geoPt);
        for(std::vector<GeoPoint>::const_iterator it = toVisit.begin(); it != toVisit.end(); it++){
            if(it->to_string() == pt2.to_string()) { 
                // reached end
                possPaths.clear();
                break;
            }
            double aStarNum = distance_earth_miles(*it, pt2);
            double pathLength = distance_earth_miles(*it, frontNode.geoPt);
            AStarNode addMe = {*it, aStarNum, pathLength};
            std::vector<AStarNode>::const_iterator index = possPaths.begin() + findLeastIndex(addMe, possPaths);
            possPaths.insert(index, addMe);
        }
        
        //TODO: UPDATE THIS FOR WHEN THERE IS ANOTHER ONE THAT HAS SMALLER VALUE
        visitedPoints.insert(frontNode.geoPt.to_string(), frontNode.geoPt);
        
        //TODO: THIS IS WRONG
        slow = frontNode;
    }
    
    return std::vector<GeoPoint>();
}



