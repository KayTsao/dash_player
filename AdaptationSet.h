#ifndef ADAPTATIONSET_H
#define ADAPTATIONSET_H

class AdaptationSet
{
    //friend class Parser
public:
    AdaptationSet();
    ~AdaptationSet();

    uint32_t id;
    uint32_t group;
    std::string lang;
    std::string content_type;
    std::string par;
    uint32_t min_bandwidth;
    uint32_t max_bandwidth;
    uint32_t min_width;
    uint32_t max_width;
    uint32_t min_height;
    uint32_t max_height;
    uint32_t min_framerate;
    uint32_t max_framerate;


    bool segment_alignment;
    bool bitstream_switching;
    bool subsegment_alignment;
    bool subsegment_starts_with_sap;


    std::vector<Descriptor *> accessibility;
    std::vector<Descriptor *> role;
    std::vector<Descriptor *> rating;
    std::vector<Descriptor *> viewpoint;
    std::vector<ContentComponent *> content_component;

    std::vector<BaseUrl *>          base_URLs;
    SegmentBase                     *segment_base;
    SegmentList                     *segment_list;
    SegmentTemplate                 *segment_template;

    std::vector<Representation *>   representation;
    std::string                     xlink_href;
    std::string                     xlink_actuate_on_load;


private:

};

#endif // ADAPTATIONSET_H
