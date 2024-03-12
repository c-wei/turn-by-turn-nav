#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"

#include "router.h"
#include "stops.h"
#include "tour_generator.h"

#include "hashmap.h"
#include <cassert>

using namespace std;

int main(){
    GeoDatabase gdb = GeoDatabase();
    gdb.load("/Users/carolinewei/Downloads/mapdata.txt");
    /*
    GeoPoint p1("34.0601422", "-118.4468929");
        GeoPoint p2("34.0600768", "-118.4467216");
        assert(gdb.get_street_name(p1, p2)=="a path");
        assert(gdb.get_street_name(p2, p1)=="a path");
        
        GeoPoint geopoints[4] = {GeoPoint("34.0602175","-118.4464952"), GeoPoint("34.0599361","-118.4469479"), GeoPoint("34.0601422","-118.4468929"), GeoPoint("34.0591552","-118.4463759")};
        GeoPoint p = GeoPoint("34.0600768", "-118.4467216");
        std::vector<GeoPoint> pts = gdb.get_connected_points(p);
    */
    GeoPoint poiPoint;
    gdb.get_poi_location("Ackerman Union", poiPoint);
    assert(poiPoint.to_string() == "34.0705851,-118.4439429");
    
    GeoPoint poiLoc;
    gdb.get_poi_location("Diddy Riese", poiLoc);
    cout << "POI Loc: "<< poiLoc.to_string()<<endl;
    
    GeoPoint p1("34.0732851", "-118.4931016");
    GeoPoint p2("34.0736122", "-118.4927669");
    cout<<gdb.get_street_name(p1, p2)<<endl;
    //assert( gdb.get_street_name(p1, p2) == "Glenmere Way"); // writes "Glenmere Way"
    assert(gdb.get_street_name(p2, p1) == "Glenmere Way"); // writes "Glenmere Way"
    
    GeoPoint p3("34.0601422", "-118.4468929");
    GeoPoint p4("34.0600768", "-118.4467216");
    assert(gdb.get_street_name(p3, p4) == "a path"); // writes "a path"
    assert(gdb.get_street_name(p4, p3) == "a path"); // writes "a path"

    GeoPoint p5("34.0602175", "-118.4464952");
    GeoPoint p6("34.0600768", "-118.4467216");
    assert(gdb.get_street_name(p5, p6) == "Kinross Avenue"); // writes "Kinross Avenue"
    assert(gdb.get_street_name(p6, p5) == "Kinross Avenue"); // writes "Kinross Avenue
    
    //cout<<"check connected points:"<<endl;
    
    std::vector<GeoPoint> pts = gdb.get_connected_points(GeoPoint("34.0736122", "-118.4927669"));
    /*
    if (pts.empty())
      cout << "There are no points connected to your specified point\n";
  else {
  for (const auto p: pts)
        cout << p.sLatitude << ", "  << p.sLongitude << endl;
    }
     */
    
    cout << pts.size()<<endl;
      if (pts.empty())
        cout << "There are no points connected to your specified point\n";
    else {
    for (const auto p: pts)
          cout << p.sLatitude << ", "  << p.sLongitude << endl;
      }
    
    cout<<"finished"<<endl;

}
    /*
    HashMap<int> hashmap(0.5);
    std::string keys[10] = {"ahello", "bhello", "chello", "dhello", "ehello", "fhello","ghello", "hhello", "ihello", "jhello"};
    int values[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(int i = 0; i<10;i++){
        hashmap.insert(keys[i], values[i]);
    }
    assert(*hashmap.find("ahello")==1);
    assert(hashmap["dhello"] == 4);
    hashmap["dhello"] = 12;
    assert(hashmap["dhello"] == 12);
    hashmap["khello"] = 11;
    assert(hashmap["khello"] == 11);
    hashmap["xhello"] = 13;
    assert(hashmap["xhello"] == 13);
    assert(hashmap.size() == 12); //12 elements in the hashmap
    std::cout<<"All tests passed!"<<std::endl;

// Define a hashmap that maps strings to doubles and has a maximum // load factor of 0.3. It will initially have 10 buckets when empty.
    HashMap<double> nameToGPA(0.3);
  // Add new items to the hashmap. Inserting the third item will cause
  // the hashmap to increase the number of buckets (since the maximum
  // load factor is 0.3), forcing a rehash of all items.
  nameToGPA.insert("Carey", 3.5);  // Carey has a 3.5 GPA
  nameToGPA.insert("David", 2.99); // David needs to up his game
    nameToGPA.insert("John", 2.99); // David needs to up his game
    nameToGPA.insert("jason", 2.99); // David needs to up his game

  // you can also use brackets like C++'s unordered_map!
    nameToGPA["Annie"] = 3.85; // Adds Annie, who has the highest GPA of all
double* davidsGPA = nameToGPA.find("David"); 
    if (davidsGPA != nullptr)
    *davidsGPA = 3.1; // after a re-grade of David's exam, update 2.99 -> 3.1
 // nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
  // sees if linda is in the map; if not, creates a new entry for linda in map
  cout << nameToGPA["David"];
     
}
 */
/*
void print_tour(vector<TourCommand> &tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        // cout << tcs[i] << endl;
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i < tcs.size() - 1 && tcs[i + 1].get_command_type() == TourCommand::proceed
                && tcs[i + 1].get_street() == tcs[i].get_street() && tcs[i].get_street() != GeoDatabase::kPathString)
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "usage: BruinNav mapdata.txt stops.txt\n";
        return -1;
    }

    GeoDatabase geodb;
    if (!geodb.load(argv[1]))
    {
        cout << "Unable to load map data: " << argv[1] << endl;
        return -1;
    }

    Router router(geodb);
    TourGenerator tg(geodb, router);

    Stops stops;
    if (!stops.load(argv[2]))
    {
        cout << "Unable to load tour data: " << argv[2] << endl;
        return -1;
    }

    std::cout << "\nRouting...\n\n";

    string error;
    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
}
*/
