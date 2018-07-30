#ifndef PERIOD_H
#define PERIOD_H
#include "MPD_AdaptationSet.h"
#include "MPD_Segment.h"
namespace dash
{
class Period
{
public:
    Period():
        id(""),
        duration(""),
        segment_base     (NULL),
        segment_list     (NULL),
        segment_template (NULL){}
    ~Period(){;}

    std::string id;
    std::string duration;
    uint64_t duration_in_ms;
    std::vector<AdaptationSet> adaptationSets;




    bool isBitstreamSwitching;
    std::string start;
    std::string url;
    std::string serviceLocation;
    std::string byteRange;

    SegmentBase *segment_base;
    SegmentList *segment_list;
    SegmentTemplate *segment_template;

    //std::vector<Subset> subsets;
    std::string xlinkHref;
    std::string xlinkActuate;
};
}
#endif // PERIOD_H
