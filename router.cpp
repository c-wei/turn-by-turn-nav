#include "router.h"
Router::Router(const GeoDatabaseBase& geo_db) { m_gdb = &geo_db; }
Router::~Router() { }

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    std::priority_queue<AStarNode, std::vector<AStarNode>, compare> possPaths;    //open list
    std::vector<GeoPoint> toVisit;           //holds the children/connected points of a parent
    HashMap<AStarNode> visitedPoints;         //closed list
    std::vector<GeoPoint> shortestRoute;
    
    AStarNode n1 = {pt1, distance_earth_miles(pt1, pt2), 0};
    possPaths.push(n1);
    
    AStarNode frontNode;
    while(!possPaths.empty()){
        frontNode = possPaths.top();
        possPaths.pop();
        
        //check if current node is the goal point
        if(frontNode.geoPt.to_string() == pt2.to_string())
            break;
        
        AStarNode *dup = visitedPoints.find(frontNode.geoPt.to_string());
        if(dup != nullptr){
            if(dup -> lengthToNode < frontNode.lengthToNode ) continue;
        }
        //otherwise visit the node's connected points
        std::vector<GeoPoint> toVisit = m_gdb->get_connected_points(frontNode.geoPt);
        for(GeoPoint &g : toVisit){
        //for(std::vector<GeoPoint>::const_iterator it = toVisit.begin(); it != toVisit.end(); it++){
            double heuristicNum = distance_earth_miles(g, pt2);
            //double heuristicNum = distance_earth_miles(*it, pt2);
            double pathLength = frontNode.lengthToNode + distance_earth_miles(g, frontNode.geoPt);
            //double pathLength = frontNode.lengthToNode + distance_earth_miles(*it, frontNode.geoPt);
            AStarNode addMe = {g, heuristicNum, pathLength};
            //AStarNode addMe = {*it, heuristicNum, pathLength};
            
            possPaths.push(addMe);
            
            AStarNode *foundVal = visitedPoints.find(addMe.geoPt.to_string());
            if(foundVal != nullptr && foundVal->lengthToNode > addMe.lengthToNode){
                visitedPoints.insert(addMe.geoPt.to_string(), {frontNode.geoPt, addMe.heuristicVal, addMe.lengthToNode});
            }
            else if(foundVal == nullptr){
                visitedPoints.insert(addMe.geoPt.to_string(), {frontNode.geoPt, addMe.heuristicVal, addMe.lengthToNode});
            }
        }
    }
    
    if(frontNode.geoPt.to_string() != pt2.to_string()){
        return std::vector<GeoPoint>();
    }
    
    while(frontNode.geoPt.to_string() != pt1.to_string()){
        shortestRoute.insert(shortestRoute.begin(), frontNode.geoPt);
        AStarNode *parent = visitedPoints.find(frontNode.geoPt.to_string());
        frontNode = *parent;
    }
    
    shortestRoute.insert(shortestRoute.begin(), frontNode.geoPt);
    return shortestRoute;
    
}



