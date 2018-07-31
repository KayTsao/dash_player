#ifndef ADAPTATIONSET_H
#define ADAPTATIONSET_H
#include "MPD_Segment.h"
#include "MPD_Representation.h"
namespace dash
{

struct Property{
    std::string scheme_Id_Uri;
    std::string value;
    std::string *id;
};

class AdaptationSet
{
    //friend class Parser
public:
    AdaptationSet():
        id(0),
        max_width(0),
        max_height(0),
        max_framerate(0),
        par(""),
        lang(""),
        segment_base     (NULL),
        segment_list     (NULL),
        segment_template (NULL){}

    //~AdaptationSet(){;}


    uint32_t id;

    bool segment_alignment;
    bool bitstream_switching;
    uint32_t max_width;
    uint32_t max_height;
    uint32_t max_framerate;
    std::string par;
    std::string lang;
    Property SupplementalProperty;
    Property EssentialProperty;
    std::vector<Representation*>   representations;



    uint32_t group;
    std::string content_type;
    uint32_t min_bandwidth;
    uint32_t max_bandwidth;
    uint32_t min_width;
    uint32_t min_height;
    uint32_t min_framerate;
    bool subsegment_alignment;
    bool subsegment_starts_with_sap;

    std::vector<BaseUrl>          base_URLs;
    SegmentBase *segment_base;
    SegmentList *segment_list;
    SegmentTemplate *segment_template;

    std::string                     xlink_href;
    std::string                     xlink_actuate_on_load;

    // info out of MPD
    uint32_t active_rep_index;
    uint32_t prev_active_rep_index;
    uint32_t download_start_time;

    /*
        std::vector<Descriptor *> accessibility;
        std::vector<Descriptor *> role;
        std::vector<Descriptor *> rating;
        std::vector<Descriptor *> viewpoint;
        std::vector<ContentComponent *> content_component;
    */
private:

};
}
#endif // ADAPTATIONSET_H
