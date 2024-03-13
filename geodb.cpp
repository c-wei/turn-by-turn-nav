#include "geodb.h"
#include "geotools.h"
#include <iostream>
#include <fstream>
using namespace std;
GeoDatabase::GeoDatabase(){}


GeoDatabase::~GeoDatabase() {}


bool GeoDatabase::load(const std::string& map_data_file){
    std::ifstream infile(map_data_file);
    if(!infile){ return false; }  //error loading file
    
    std::string streetName;
    
    while(getline(infile, streetName)){
        string geoPtSLat, geoPtSLon, geoPtELat, geoPtELon;
        infile >> geoPtSLat;
        infile >> geoPtSLon;
        infile >> geoPtELat;
        infile >> geoPtELon;
        infile.ignore(10000, '\n');
        
        GeoPoint start = GeoPoint(geoPtSLat, geoPtSLon);
        GeoPoint end = GeoPoint(geoPtELat, geoPtELon);
        
        //add to pointToStreet
        pointToStreet.insert(start.to_string() + end.to_string(), streetName);
        
        //check if the street's in the map & add it in if its not
        if(pointToNode.find(start.to_string()) == nullptr){
            pointToNode.insert(start.to_string(), graphNode{"street", streetName, vector<graphNode*>(), start});
        }
        if(pointToNode.find(end.to_string()) == nullptr){
            pointToNode.insert(end.to_string(), graphNode{"street", streetName, vector<graphNode*>(), end});
        }
        
        graphNode* startPt = pointToNode.find(start.to_string());
        graphNode* endPt = pointToNode.find(end.to_string());
        
        int poiCt;
        infile >> poiCt;
        infile.ignore(10000, '\n');
        
        if(poiCt == 0){
            endPt->connectedPts.push_back(startPt);
            startPt->connectedPts.push_back(endPt);
            continue;
        }
        
        else{
            //create midpoint
            GeoPoint midpt = midpoint(start, end);
            
            //add midpt to pointToStreet
            pointToStreet.insert(midpt.to_string(), streetName);
            
            //add midpt to hashmap
            pointToNode.insert(midpt.to_string(), graphNode{"midpoint", "a path", vector<graphNode*>(), midpt});
            graphNode* midptPt = pointToNode.find(midpt.to_string());
            //connect start/mid/end
            startPt->connectedPts.push_back(midptPt);
            endPt->connectedPts.push_back(midptPt);
            midptPt->connectedPts.push_back(startPt);
            midptPt->connectedPts.push_back(endPt);
            
            for(; poiCt > 0; poiCt--){
                //read in name, lat, long
                string poiName;
                string poiLat, poiLon;
                
                std::getline(infile, poiName, '|');
                
                infile >> poiLat >> poiLon;
                infile.ignore(10000, '\n');
                
                //add POI and connect POI to midpoint & vice versa
                GeoPoint poi = GeoPoint(poiLat, poiLon);
                pointToNode.insert(poi.to_string(), graphNode{"POI", poiName, vector<graphNode*>(), poi});
                graphNode* poiPt = pointToNode.find(poi.to_string());
                poiPt->connectedPts.push_back(midptPt);
                midptPt->connectedPts.push_back(poiPt);
                
                //add POI to poiToLoc
                poiToLoc.insert(poiName, poi);
                pointToStreet.insert(poi.to_string(), "a path");
            }
        }
        
        
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const{
    if(poiToLoc.find(poi) == nullptr){
        return false;
    }
    
    point = *poiToLoc.find(poi);
    return true;
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    if(pointToStreet.find(pt1.to_string() + pt2.to_string()) != nullptr)
        return *pointToStreet.find(pt1.to_string() + pt2.to_string());
    else if(pointToStreet.find(pt2.to_string() + pt1.to_string()) != nullptr)
        return *pointToStreet.find(pt2.to_string() + pt1.to_string());
    else if(pointToStreet.find(pt1.to_string()) != nullptr || pointToStreet.find(pt2.to_string()) != nullptr){
        if((pointToStreet.find(pt1.to_string()) != nullptr && *pointToStreet.find(pt1.to_string()) == "a path") || (pointToStreet.find(pt2.to_string()) != nullptr && *pointToStreet.find(pt2.to_string()) == "a path"))
            return "a path";
        else if(pointToStreet.find(pt1.to_string()) != nullptr)
                return *pointToStreet.find(pt1.to_string());
        else
            return *pointToStreet.find(pt2.to_string());
    }
    else return "";
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    if(pointToNode.find(pt.to_string()) == nullptr) return std::vector<GeoPoint>();
    
    const graphNode *ptVal = pointToNode.find(pt.to_string());
    const std::vector<graphNode*> connectedPoints = ptVal->connectedPts;
    std::vector<GeoPoint> geoPts;
    
    for(vector<graphNode*>::const_iterator it = connectedPoints.begin(); it != connectedPoints.end(); it++){
        geoPts.push_back((*it)->val);
    }
    return geoPts;
}


