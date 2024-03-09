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
        double geoPtSLat, geoPtSLon, geoPtELat, geoPtELon;
        infile >> geoPtSLat;
        infile >> geoPtSLon;
        infile >> geoPtELat;
        infile >> geoPtELon;
        infile.ignore(10000, '\n');
        
        GeoPoint start = GeoPoint(std::to_string(geoPtSLat), std::to_string(geoPtSLon));
        GeoPoint end = GeoPoint(std::to_string(geoPtELat), std::to_string(geoPtELon));
        
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
            //add midpt to hashmap
            pointToNode.insert(midpt.to_string(), graphNode{"midpoint", "", vector<graphNode*>(), midpt});
            graphNode* midPt = pointToNode.find(midpt.to_string());
            //connect start/mid/end
            startPt->connectedPts.push_back(midPt);
            endPt->connectedPts.push_back(midPt);
            midPt->connectedPts.push_back(startPt);
            midPt->connectedPts.push_back(endPt);
            
            for(; poiCt > 0; poiCt--){
                //read in name, lat, long
                std::string name;
                double poiLat, poiLon;
                
                std::getline(infile, name, '|');
                
                infile >> poiLat >> poiLon;
                infile.ignore(10000, '\n');
                
                //add POI and connect POI to midpoint & vice versa
                GeoPoint poi = GeoPoint(std::to_string(poiLat), std::to_string(poiLon));
                pointToNode.insert(poi.to_string(), graphNode{"POI", name, vector<graphNode*>(), poi});
                graphNode* poiPt = pointToNode.find(poi.to_string());
                poiPt->connectedPts.push_back(midPt);
                midPt->connectedPts.push_back(poiPt);
            }
        }
        
        
    }
    return true;
}

/*
 bool GeoDatabase::load(const std::string& map_data_file){ }
 
 bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const{ }
 
 std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const { }
 
 std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const { }
 */
