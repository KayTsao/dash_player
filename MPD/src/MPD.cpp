#include "MPD.h"
using namespace dash;
using namespace std;
#include <cstring>
#include <iostream>
MPD::MPD()
{
    active_period_index = 0;
}

char* MPD::mpd_get_base_url(char* MPD_URL){
    //char* MPD_URL = "http://10.3.57.202:8000/dash/rabbit/dash_tiled.mpd";
    char solved_base[128];
    char *sep1, *sep2;
    strcpy(solved_base, MPD_URL); //printf("%s\n", solved_template);
    sep1 = strchr(solved_base, '/');
    sep2 = strchr(sep1+1, '/');
    while(sep2){
        sep1 = strchr(sep2+1, '/');
        sep2 = sep2 = strchr(sep1+1, '/');
    }
    sep1[1] = 0;
    //printf("%s\n",solved_base);
    string str(solved_base);
    basic_URL = str;
    //printf("%s\n", basic_URL);
    return solved_base;
}

// mpd_resolve_segment_duration(period, AdaSet, rep, &out_duration, &out_timescale, NULL, NULL);
int MPD::mpd_resolve_segment_duration(Period period, AdaptationSet AdaSet, Representation rep,
                                      uint64_t *out_duration, uint32_t *out_timescale)
{
  //  if(!period||!AdaSet||!rep) return -1;
//    uint64_t pts_offset = 0;
    if(rep.segment_template)
    {
        if(rep.segment_template->timescale)
            *out_timescale = rep.segment_template->timescale;
        if(rep.segment_template->duration)
            *out_duration = rep.segment_template->duration;
    }
    return 0;
}

int MPD::mpd_resolve_url(int AdaSetID, int repID, int download_seg_index, MPD_URLResolveType resolve_type,
                         uint64_t *out_segment_duration_in_ms, string *out_url){
    Period* period = periods.at(active_period_index);
    //if(!period) return -1;
    AdaptationSet* AdaSet = period->adaptationSets.at(AdaSetID);
    //if(!AdaSet) return -1;
    Representation* rep = AdaSet->representations.at(repID);
    //if(!rep ) return -1;    

    uint32_t timescale = 0;
    uint64_t duration = 0;
  //  char* url;
    char *first_sep;
    const char *url_to_solve, *init_template, *index_template, *media_url;
    char solved_template[128];

//    char* input_url = "http://127.0.0.1:8000/";
//    url = mpd_get_base_url(input_url);

    //segment_template
    media_url = init_template = index_template = NULL;
    if(AdaSet->segment_template){
        if(AdaSet->segment_template->initialization.size())
            init_template = AdaSet->segment_template->initialization.c_str();
        if(AdaSet->segment_template->index.size())
            index_template = AdaSet->segment_template->index.c_str();
        if(AdaSet->segment_template->media.size())
            media_url = AdaSet->segment_template->media.c_str();
        if(!timescale && AdaSet->segment_template->timescale)
            timescale = AdaSet->segment_template->timescale;
        if(!duration && AdaSet->segment_template->duration)
            duration = AdaSet->segment_template->duration;
    }
    if(rep->segment_template){
        if(rep->segment_template->initialization.size())
            init_template = rep->segment_template->initialization.c_str();
        if(rep->segment_template->index.size())
            index_template = rep->segment_template->index.c_str();
        if(rep->segment_template->media.size())
            media_url = rep->segment_template->media.c_str();
        if(!timescale && rep->segment_template->timescale)
            timescale = rep->segment_template->timescale;
        if(!duration && rep->segment_template->duration)
            duration = rep->segment_template->duration;
    }
    //return segment duration in all cases
    {
        uint64_t out_duration;
        uint32_t out_timescale;
        mpd_resolve_segment_duration(*period, *AdaSet, *rep, &out_duration, &out_timescale);//, NULL, NULL);
        *out_segment_duration_in_ms = (uint64_t)((out_duration * 1000.0) / out_timescale);
    }
    //SKIP  offset the start_number with the number of discarded segments
    url_to_solve = NULL;
    switch (resolve_type){
    case MPD_RESOLVE_URL_INIT:
        url_to_solve = init_template;
        break;
    case MPD_RESOLVE_URL_MEDIA:
        url_to_solve = media_url;
        break;
    default:
        printf("Resolve Type Not Supported");
        return -1;
    }
    if(!url_to_solve)
        return 0;
    //solve the template
    strcpy(solved_template, url_to_solve);
//    printf("%s\n", solved_template);  // "1_tiled_dash_track2_$Number$.m4s"
    first_sep = strchr(solved_template, '$');
    if(first_sep) first_sep[0] = 0;
//    printf("%s\n", solved_template);// "1_tiled_dash_track2_"

    first_sep = strchr(url_to_solve, '$');
//    printf("%s\n", first_sep);// "$Number$.m4s"
    while(first_sep){
        char szPrintFormat[10];
        char szFormat[100];
        char*second_sep = strchr(first_sep+1, '$'); //second_sep == "$.m4s "
        if(!second_sep){
            return -1;
        }
        second_sep[0] = 0; //first_sep == "$Number"
        strcpy(szPrintFormat, "%d");
        if (!strcmp(first_sep+1, "Number")) {
//            int start_number = 0; int item_index = 5;
            sprintf(szFormat, szPrintFormat, download_seg_index);//start_number + item_index);
            strcat(solved_template, szFormat);
            //check start time is in period
            if (period->duration.size())
            {
                int request_in_ms = download_seg_index * (int)(*out_segment_duration_in_ms);
                if(request_in_ms > (int)period->duration_in_ms)
                {
                    printf("Exceed_period_duration!\n");
                    return -1;//EOS
                }
            }
        }
        second_sep[0] = '$';
        //printf("%s  %s\n", first_sep, second_sep);// "$Number$.m4s  $.m4s"
        first_sep = strchr(second_sep+1, '$');
        if(first_sep) first_sep[0] = 0;
        if(strlen(second_sep+1))
            strcat(solved_template, second_sep+1);
        if(first_sep) first_sep[0] = '$';
    }
    string solved_str(solved_template);
    string result_url = this->basic_URL;
    result_url.append(solved_str);
    *out_url = result_url; //    std::cout <<*out_url<< '\n';
    return 0;
}


// initial
/*void MPD::mpd_setup_period(){
    //Period
}
*/
void MPD::gf_dash_get_segment_duration(Representation rep, AdaptationSet AdaSet, Period period,
                               uint32_t* out_nb_segments, double* out_max_seg_duration){
    double mediaDuration;
    uint32_t timescale;
    uint64_t duration;
    *out_nb_segments = timescale = 0;
    duration = 0;
    bool single_segment = true;
    if(rep.segment_template){
        single_segment = false;
        if(rep.segment_template->duration) duration = rep.segment_template->duration;
        if(rep.segment_template->timescale) timescale = rep.segment_template->timescale;
    }
    if(!timescale) timescale = 1;
    if(out_max_seg_duration){
        *out_max_seg_duration = (double) duration;
        *out_max_seg_duration /= timescale;
    }
    mediaDuration = (double)period.duration_in_ms;
    if(!mediaDuration){
        mediaDuration = media_presentation_duration;
    }
    if(mediaDuration && duration){
        double nb_seg = (double)mediaDuration;
        /*duration is given in ms*/
        nb_seg /= 1000;
        nb_seg *= timescale;
        nb_seg /= duration;
        *out_nb_segments = (uint32_t)nb_seg;
        if(*out_nb_segments < nb_seg)
            (*out_nb_segments)++;
    }
    rep.nb_seg_in_rep = *out_nb_segments;
}
