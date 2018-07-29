#ifndef SEGMENT_H
#define SEGMENT_H
#include <stdint.h>
#include <string>
#include <vector>
namespace dash
{
struct ByteRange
{
    uint64_t start_range, end_range;
};

struct BaseUrl{
    std::string URL;
    std::string service_location;
    ByteRange *byte_range;
    std::string redirection;
};

struct URL{
    std::string sourceURL;
    ByteRange  range;
    bool is_resolved;
    //dash::metrics::HTTPTransactionType  type;
};

struct SegmentTimelineEntry{
    uint64_t start_time;
    uint32_t duration; /*MANDATORY*/
        //may be -1 (FIXME this needs further testing)
    uint32_t repeat_count;
};

struct SegmentTimeline
{
    std::vector<SegmentTimelineEntry> entries;
};

struct SegmentBase{

    URL *initialization_segment;
    URL *representation_index;
    uint32_t timescale;
    uint32_t presentation_time_offset;
    ByteRange index_range;
    bool index_range_exact;

    uint32_t time_shift_buffer_depth; /* expressed in milliseconds */
    double availability_time_offset;
};

struct MultipleSegmentBase{
    SegmentBase segment_base;
    uint32_t duration;
    uint32_t start_number;
    SegmentTimeline *segment_timeline;
    URL *bitstream_switching_url;
};

struct SegmentURL
{
    std::string media;
    ByteRange *media_range;
    std::string index;
    ByteRange *index_range;
    uint32_t duration;
    /*HLS only*/
    std::string key_url;
  //  bin128 key_iv;
  //  u64 hls_utc_start_time;
} ;


struct SegmentList
{
    MultipleSegmentBase multiple_segment_base;
    /*list of segments - can be NULL if no segment*/
    std::vector<SegmentURL> segment_URLs;
    std::string xlink_href;
    bool xlink_actuate_on_load;
    uint32_t consecutive_xlink_count;
} ;



struct SegmentTemplate{

    //MultipleSegmentBase multiple_segment_base;
    std::string media;
    uint32_t timescale;
    uint32_t start_number;
    uint32_t duration;

    std::string index;
    std::string initialization;
    std::string bitstream_switching;
};

}

#endif // SEGMENT_H
