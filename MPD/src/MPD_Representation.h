#ifndef REPRESENTATION_H
#define REPRESENTATION_H
#include "MPD_Segment.h"
namespace mpd
{
class Representation
{
public:
    Representation():
        id(""),
        bandwidth       (0),
        segment_template (NULL){}

    ~Representation(){
        if(segment_template) delete(segment_template);
   }

    std::string id; /*MANDATORY*/
    std::string mimeType;
    std::string codecs;
    uint32_t width;
    uint32_t height;
    uint32_t frame_rate;
    std::string sar;
    int startWithSAP;
    uint32_t bandwidth; /*MANDATORY*/
    std::string dependency_id;
    SegmentTemplate *segment_template;

    uint32_t nb_seg_in_rep;
    /*
    uint32_t quality_ranking;
    std::string media_stream_structure_id;
    SegmentBase *segment_base;
    SegmentList *segment_list;
    std::vector<BaseUrl> base_URLs;
    sub_representations;
    */
};
}
#endif // REPRESENTATION_H
