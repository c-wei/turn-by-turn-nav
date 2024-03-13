#include "tour_generator.h"

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) {
    gdb = &geodb;
    m_router = &router;
}
TourGenerator::~TourGenerator() {}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops &stops) const{
    std::vector<TourCommand> instructions;
    std::vector<GeoPoint> path;
    int i = 0;
    std::string poi1, poi2, talking_points1, talking_points2;
    GeoPoint geoPt1, geoPt2;
    
    while(stops.get_poi_data(i, poi1, talking_points1)){
        TourCommand commentary = TourCommand();
        commentary.init_commentary(poi1, talking_points1);
        instructions.push_back(commentary);
        
        if(stops.get_poi_data(i + 1, poi2, talking_points2)){
            gdb->get_poi_location(poi1, geoPt1);
            gdb->get_poi_location(poi2, geoPt2);
            path = m_router->route(geoPt1, geoPt2);
            
            for(std::vector<GeoPoint>::const_iterator it = path.begin(); it != path.end()-1; it++){
                TourCommand proceed = TourCommand();
                proceed.init_proceed(getLineAngle(*it, *(it+1)), gdb->get_street_name(*it, *(it+1)), distance_earth_miles(*it, *(it+1)), *it, *(it+1));
                instructions.push_back(proceed);
                if(it+2 != path.end()){
                    if(gdb->get_street_name(*it, *(it+1)) != gdb->get_street_name(*(it+1), *(it+2))){
                        double turnAngle = angle_of_turn(*it, *(it+1), *(it+2));
                        if(turnAngle >= 1 && turnAngle <= 359){
                            TourCommand turn = TourCommand();
                            turn.init_turn(getTurnValue(turnAngle), gdb->get_street_name(*(it+1), *(it+2)));
                            instructions.push_back(turn);
                        }
                    }
                }
            }
        }
        i++;
    }
    
    
    return instructions;
}
