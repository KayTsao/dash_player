#ifndef MPD_H
#define MPD_H
#include "MPD_Period.h"
#include "MPD_Segment.h"
#include <string>

namespace dash
{
typedef enum {
    MPD_TYPE_STATIC,
    MPD_TYPE_DYNAMIC,
} MPD_Type;

typedef enum {
    MPD_RESOLVE_URL_MEDIA,
    MPD_RESOLVE_URL_INIT,
    MPD_RESOLVE_URL_INDEX,
    //same as GF_MPD_RESOLVE_URL_MEDIA but does not replace $Time$ and $Number$
    MPD_RESOLVE_URL_MEDIA_TEMPLATE,
    //same as GF_MPD_RESOLVE_URL_MEDIA but does not use startNumber
    MPD_RESOLVE_URL_MEDIA_NOSTART,
}MPD_URLResolveType;

struct MPD_Descriptor
{
    //MPD_EXTENSIBLE
    char *scheme_id_uri; /*MANDATORY*/
    char *value;
    char *id;

} ;
struct MPD_ProgramInfo
{
    char *lang;
    char *title;
    char *source;
    char *copyright;
    char *more_info_url;
} ;

class MPD
{
public:
    MPD();
    std::string basic_URL;

    std::string ID;
    int active_period_index;
    uint64_t min_buffer_time; /* expressed in milliseconds */	/*MANDATORY*/
    MPD_Type type;
    uint64_t media_presentation_duration; /* expressed in milliseconds */	/*MANDATORY if type=static*/
    uint64_t max_segment_duration; /* expressed in milliseconds */
    std::string profiles;	/*MANDATORY*/
    MPD_ProgramInfo *program_infos; /*list of GF_MPD_ProgramInfo */
    std::string xmlns;

    uint64_t availabilityStartTime; /* expressed in milliseconds */	/*MANDATORY if type=dynamic*/
    uint64_t availabilityEndTime;/* expressed in milliseconds */
    uint64_t publishTime;/* expressed in milliseconds */
    uint64_t minimum_update_period; /* expressed in milliseconds */
    uint64_t time_shift_buffer_depth; /* expressed in milliseconds */
    uint64_t suggested_presentation_delay; /* expressed in milliseconds */
    uint64_t max_subsegment_duration; /* expressed in milliseconds */

    std::vector<Period *> periods;

    /*list of GF_MPD_BaseURL */
    //std::vector<BaseUrl> base_URLs;
        /*list of strings */
    //std::vector<std::string> locations;
        /*list of GF_MPD_Period */
    //std::vector<AdaptationSet*> AdaptationSets;
        /*set during parsing*/
    //const char *xml_namespace; /*won't be freed by GPAC*/

    char* mpd_get_base_url(char* input_url);
    int mpd_resolve_segment_duration(Period period, AdaptationSet AdaSet, Representation rep,
                                          uint64_t *out_duration, uint32_t *out_timescale);

    int mpd_resolve_url(int AdaSetID, int repID, int download_seg_index, MPD_URLResolveType resolve_type,
                        uint64_t *out_segment_duration_in_ms, string *out_url);

    // initial
    //void get_segment_duration(Representation rep);
    void gf_dash_get_segment_duration(Representation rep, AdaptationSet AdaSet, Period period,
                                   uint32_t* out_nb_segments, double* out_max_seg_duration);
    /*
    bool mpd_parse_bool(const char* const AttributeValue);
    std::string mpd_parse_string(const char* const AttributeValue);
    uint32_t mpd_parse_int(const char* const AttributeValue);
    uint64_t mpd_parse_long_int(const char* const AttributeValue);
    double mpd_parse_double(const char* const AttributeValue);

    uint64_t mpd_parse_date(const char* const AttributeValue);
    uint64_t mpd_parse_duration(const char* const duration);
    uint32_t mpd_parse_duration_u32(const char* const duration);
    ByteRange mpd_parse_byte_range(const char * const AttributeValue);

    int mpd_parse_base_url(BaseUrl baseUrls);//, XMLNode* Node);
    int mpd_parse_program_info(MPD *mpd);//, GF_XMLNode *root)
    URL  mpd_parse_url( ); //, GF_XMLNode *root)

    void mpd_parse_segment_base_generic(MPD *mpd, SegmentBase *seg);//, GF_XMLNode *root)
    SegmentTimeline *mpd_parse_segment_timeline(MPD *mpd);//, GF_XMLNode *root)
    SegmentBase* mpd_parse_segment_base(MPD *mpd);//, GF_XMLNode *root)
    void mpd_parse_multiple_segment_base(MPD *mpd, MultipleSegmentBase *seg);//, GF_XMLNode *root)
    void mpd_parse_segment_url(SegmentURL *SegmentURLs);//, GF_XMLNode *root)
    SegmentList * mpd_parse_segment_list(MPD *mpd);//, GF_XMLNode *root);
    SegmentTemplate *mpd_parse_segment_template(MPD *mpd);//, GF_XMLNode *root)

    //int mpd_parse_descriptor(GF_List *container, string s);//, GF_XMLNode *root)

    int mpd_parse_period();
    int mpd_parse_adaptation_set(MPD *mpd, AdaptationSet* adaptation_sets);//, GF_XMLNode *root)
    int mpd_parse_representation(MPD *mpd, Representation* representations);//, GF_XMLNode *root)
    int mpd_init_from_dom(MPD *mpd, const char *base_url);//GF_XMLNode *root,
    int mpd_complete_from_dom(MPD *mpd, const char *base_url);//GF_XMLNode *root,
*/
};

}
#endif // MPD_H
