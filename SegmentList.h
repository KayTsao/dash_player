#ifndef SEGMENTLIST_H
#define SEGMENTLIST_H

struct s_URL{
    std::string sourceURL;
    std::string  range;
    bool is_resolved;
    //dash::metrics::HTTPTransactionType  type;
};

struct SegmentBase{

    s_URL *initialization_segment;
    s_URL *representation_index;
    uint32_t timescale;
    uint32_t presentation_time_offset;
    std::string index_range;
    bool index_range_exact;
    //uint32_t time_shift_buffer_depth; /* expressed in milliseconds */
    //availability_time_offset;
};

struct SegmentTemplate{
    int num, den;
};




class SegmentList
{
public:
    SegmentList();
};

#endif // SEGMENTLIST_H
