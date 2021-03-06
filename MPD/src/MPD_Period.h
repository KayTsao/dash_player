#ifndef PERIOD_H
#define PERIOD_H
#include "MPD_AdaptationSet.h"
//#include <cstring>

using namespace std;
namespace mpd
{
class Period
{
public:
    Period():
        duration(""),
        duration_in_ms (0){}

    ~Period(){
        int i, SetCount;
        SetCount = adaptationSets.size();
        for(i = 0; i < SetCount; i++)
        {
            delete(adaptationSets[i]);
        }
        adaptationSets.clear();
    }

    string duration;
    uint64_t duration_in_ms;
    vector<AdaptationSet*> adaptationSets;

    /*
    string id;
    bool isBitstreamSwitching;
    string start;
    string url;
    string serviceLocation;
    string byteRange;

    SegmentBase *segment_base;
    SegmentList *segment_list;
    SegmentTemplate *segment_template;
    //vector<Subset> subsets;
    string xlinkHref;
    string xlinkActuate;*/

    void set_duration_in_ms(){//string input_str = "PT0H2M4.000S";
        int h, m;
        float s;
        char * cstr = new char [duration.length()+1];
        strcpy (cstr, duration.c_str());
        sscanf(cstr, "PT%dH%dM%fS", &h, &m, &s);
        duration_in_ms = (uint64_t)((h*3600+m*60+s)*(uint64_t)1000);
        delete[] cstr;
    }

};
}
#endif // PERIOD_H
