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
        duration(""){}
    ~Period(){;}

    std::string id;
    std::string duration;
    std::vector<AdaptationSet> adaptationSets;




    bool isBitstreamSwitching;
    std::string start;
    std::string url;
    std::string serviceLocation;
    std::string byteRange;

    SegmentBase *segmentBase;
    SegmentList *segmentList;
    SegmentTemplate *segmentTemplate;

    //std::vector<Subset> subsets;
    std::string xlinkHref;
    std::string xlinkActuate;
};
}
#endif // PERIOD_H
