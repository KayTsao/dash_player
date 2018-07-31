#ifndef MPD_H
#define MPD_H
#include "MPD_Period.h"
#include "MPD_Segment.h"

namespace mpd
{
typedef enum {
    MPD_TYPE_STATIC,
    MPD_TYPE_DYNAMIC,
} MPD_Type;

typedef enum {
    MEDIA_PRESENTATION_DURATION,
    MAX_SEGMENT_DURATION,
} MPD_Duration_Type;

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
    string scheme_id_uri; /*MANDATORY*/
    string value;
    string id;

} ;
struct MPD_ProgramInfo
{
    string lang;
    string title;
    string source;
    string copyright;
    string more_info_url;
} ;

class MPD
{
public:
    MPD();
    ~MPD(){
        int i, periodCount;
        periodCount = periods.size();
        for(i =0; i < periodCount; i++)
        {
            delete(periods[i]);
        }
        periods.clear();
    }


    std::string xmlns;
    uint64_t min_buffer_time; /* expressed in milliseconds */	/*MANDATORY*/
    MPD_Type type;
    uint64_t media_presentation_duration; /* expressed in milliseconds */	/*MANDATORY if type=static*/
    uint64_t max_segment_duration; /* expressed in milliseconds */
    std::string profiles;	/*MANDATORY*/
    MPD_ProgramInfo program_infos; /*list of GF_MPD_ProgramInfo */

    std::vector<Period *> periods;
    std::string MPD_URL;
    std::string basic_URL;
    int active_period_index;

//    std::string ID;
//    uint64_t availabilityStartTime; /* expressed in milliseconds */	/*MANDATORY if type=dynamic*/
//    uint64_t availabilityEndTime;/* expressed in milliseconds */
//    uint64_t publishTime;/* expressed in milliseconds */
//    uint64_t minimum_update_period; /* expressed in milliseconds */
//    uint64_t time_shift_buffer_depth; /* expressed in milliseconds */
//    uint64_t suggested_presentation_delay; /* expressed in milliseconds */
//    uint64_t max_subsegment_duration; /* expressed in milliseconds */
    /*list of GF_MPD_BaseURL */
    //std::vector<BaseUrl> base_URLs;
        /*list of strings */
    //std::vector<std::string> locations;
        /*list of GF_MPD_Period */
    //std::vector<AdaptationSet*> AdaptationSets;
        /*set during parsing*/
    //const string xml_namespace; /*won't be freed by GPAC*/

    void set_mpd_url(string input_url);
    void set_duration(string input_str, MPD_Duration_Type duration_type);
    void set_minBufferTime(string input_str);

    int get_resolved_url(int SetID, int repID, int download_seg_index, MPD_URLResolveType resolve_type,uint64_t *out_segment_duration_in_ms, string *out_url);
    void get_segment_duration(Representation *rep, uint64_t period_duration_ms,uint32_t* out_nb_segments, double* out_max_seg_duration);
};

}
#endif // MPD_H
