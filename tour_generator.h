#ifndef tour_generator_h
#define tour_generator_h
#include "base_classes.h"
#include "geotools.h"
#include "stops.h"
#include "tourcmd.h"

class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
 virtual ~TourGenerator();
 virtual std::vector<TourCommand> generate_tour(const Stops &stops) const;
private:
    const GeoDatabaseBase *gdb;
    const RouterBase *m_router;
    
    std::string getLineAngle(const GeoPoint &pt1, const GeoPoint &pt2) const{
        double angle = angle_of_line(pt1, pt2);
        if(0 <= angle < 22.5) return "east";
            else if(22.5 <= angle < 67.5) return "northeast";
            else if(67.5 <= angle < 112.5) return "north";
            else if(112.5 <= angle < 157.5) return "northwest";
            else if(157.5 <= angle < 202.5) return "west";
            else if(202.5 <= angle < 247.5) return "southwest";
            else if(247.5 <= angle < 292.5) return "south";
            else if(292.5 <= angle < 337.5) return "southeast";
            else return "east";
    }
};

#endif
