#ifndef MULTIPLESEGMENTBASE_H
#define MULTIPLESEGMENTBASE_H

struct URLType{
    std::string sourceURL;
    std::string range;//*byte_range;
    //GPAC internal - indicates the URL has already been solved
    bool is_resolved;
    // dash::metrics::HTTPTransactionType  type;
};

struct SegmentTimelineEntry{
    uint64_t start_time;
    uint32_t duration; /*MANDATORY*/
        //may be -1 (FIXME this needs further testing)
    uint32_t repeat_count;
};

class SegmentTimeline
{
    std::vector<SegmentTimelineEntry> entries;
};

class MultipleSegmentBase
{
public:
    MultipleSegmentBase();
    SegmentTimeline *segment_timeline;
    URLType *bitstream_switching_url;
    uint32_t duration;
    uint32_t start_number;
};

#endif // MULTIPLESEGMENTBASE_H
