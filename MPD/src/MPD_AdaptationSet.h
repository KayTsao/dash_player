#ifndef ADAPTATIONSET_H
#define ADAPTATIONSET_H
#include "MPD_Segment.h"
#include "MPD_Representation.h"
#include <cstring>
using namespace std;
namespace mpd
{

struct Property{
    string scheme_Id_Uri;
    string value;
    string *id;
};

class AdaptationSet
{
    //friend class Parser
public:
    AdaptationSet():
        max_width(0),
        max_height(0),
        max_framerate(0),
        par(""),
        lang(""),
        segment_template(NULL){}

    ~AdaptationSet(){
        int i, RepCount;
        RepCount = representations.size();
        for(i =0; i < RepCount; i++)
        {
            delete(representations[i]);
        }
        representations.clear();
        if(segment_template) delete(segment_template);
    }

    uint32_t id;

    bool segment_alignment;
    bool bitstream_switching;
    uint32_t max_width;
    uint32_t max_height;
    uint32_t max_framerate;
    string par;
    string lang;
    Property SupplementalProperty;
    Property EssentialProperty;
    SegmentTemplate *segment_template;
    vector<Representation*>   representations;

/*
    string content_type;
    uint32_t min_bandwidth;
    uint32_t max_bandwidth;
    uint32_t min_width;
    uint32_t min_height;
    uint32_t min_framerate;
    bool subsegment_alignment;
    bool subsegment_starts_with_sap;
    vector<BaseUrl>          base_URLs;
    SegmentBase *segment_base;
    SegmentList *segment_list;
    string xlink_href;
    string xlink_actuate_on_load;

    vector<Descriptor *> accessibility;
    vector<Descriptor *> role;
    vector<Descriptor *> rating;
    vector<Descriptor *> viewpoint;
    vector<ContentComponent *> content_component;
*/
    // info out of MPD
    uint32_t active_rep_index;
    uint32_t prev_active_rep_index;
    uint32_t download_start_time;
    uint32_t srd_x, srd_y, srd_w, srd_h;

    void solve_property(){
        if(!EssentialProperty.scheme_Id_Uri.empty()){
            if(EssentialProperty.scheme_Id_Uri.compare("urn:mpeg:dash:srd:2014") == 0){
                char * val = new char [EssentialProperty.value.length()+1];
                strcpy (val, EssentialProperty.value.c_str());
                sscanf(val, "%d,%d,%d,%d,%d", &id, &srd_x, &srd_y, &srd_w, &srd_h);
                delete[] val;
            }
        }
        if(!SupplementalProperty.scheme_Id_Uri.empty()){
            if(SupplementalProperty.scheme_Id_Uri.compare("urn:mpeg:dash:srd:2014") == 0){
                char * val = new char [SupplementalProperty.value.length()+1];
                strcpy (val, SupplementalProperty.value.c_str());
                sscanf(val, "%d,%d,%d,%d,%d", &id, &srd_x, &srd_y, &srd_w, &srd_h);
                delete[] val;
            }
        }

    }

private:
};
}
#endif // ADAPTATIONSET_H
