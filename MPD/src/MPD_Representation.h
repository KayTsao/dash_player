#ifndef REPRESENTATION_H
#define REPRESENTATION_H
#include "MPD_Segment.h"
namespace dash
{
class Representation
{
public:
    Representation():
        segment_base     (NULL),
        segment_list     (NULL),
        segment_template (NULL),
        id(""),
        bandwidth       (0),
        quality_ranking  (0){}

    ~Representation(){
        delete(this->segment_template);
        delete(this->segment_base);
        delete(this->segment_list);}

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

    uint32_t quality_ranking;
    std::string media_stream_structure_id;
    SegmentTemplate *segment_template;
    SegmentBase *segment_base;
    SegmentList *segment_list;
    std::vector<BaseUrl> base_URLs;

    uint32_t nb_seg_in_rep;
    // GF_List *sub_representations;
};
}
#endif // REPRESENTATION_H
