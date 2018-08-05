#include "MPD.h"

#include <cstring>
#include <iostream>

using namespace mpd;
using namespace std;

MPD::MPD()
{
    MPD_URL = "";
    basic_URL = "";
    active_period_index = 0;
}
MPD::MPD(string url)
{
    active_period_index = 0;
    set_mpd_url(url);
}

MPD::~MPD(){
    int i, periodCount;
    periodCount = periods.size();
    for(i =0; i < periodCount; i++)
    {
        delete(periods[i]);
    }
    periods.clear();
}

uint32_t MPD::setup_mpd(){
    uint32_t err = 0;
    if(MPD_URL.empty()||basic_URL.empty())
        err = 1;
    if(!err){
        int i, p_count;
        p_count = periods.size();
        for(i = 0; i < p_count; i++){
            Period* p = periods.at(i);
            err = p->setup_period(basic_URL);
            if(err)
                return err;
        }
    }
    return err;
}

void MPD::set_duration(string input_str, MPD_Duration_Type duration_type){
    int h, m;
    float s;
    char * cstr = new char [input_str.length()+1];
    strcpy (cstr, input_str.c_str());
    sscanf(cstr, "PT%dH%dM%fS", &h, &m, &s);
    uint64_t duration = (uint64_t)((h*3600+m*60+s)*(uint64_t)1000);
    switch (duration_type) {
    case MAX_SEGMENT_DURATION:
        max_segment_duration = duration;
        break;
    case MEDIA_PRESENTATION_DURATION:
        media_presentation_duration = duration;
        break;
    default:
        break;
    }
    delete[] cstr;
}

void MPD::set_minBufferTime(string input_str){
    //string input_str = "PT1.500S";
    float s;
    char * cstr = new char [input_str.length()+1];
    strcpy (cstr, input_str.c_str());
    sscanf(cstr, "PT%fS", &s);
    min_buffer_time = (uint64_t)(s*(uint64_t)1000);
    delete[] cstr;
}

int MPD::get_resolved_url(int SetID, int repID, int download_seg_index, MPD_URLResolveType resolve_type,
                         uint64_t *out_segment_duration_in_ms, string *out_url){
    int err=0;
    Period* period = periods.at(active_period_index);
    AdaptationSet* Set = period->adaptationSets.at(SetID);
    Representation* rep = Set->representations.at(repID);
    if(!period||!Set||!rep) return -1;

    uint32_t timescale = 0;
    uint64_t duration = 0;
    char *first_sep;
    char *url_to_solve;
    const char *init_template, *index_template, *media_url;
    char solved_template[128];

    //segment_template
    media_url = init_template = index_template = NULL;
    if(Set->segment_template){
        if(Set->segment_template->initialization.size())
            init_template = Set->segment_template->initialization.c_str();
//        if(Set->segment_template->index.size())
//            index_template = Set->segment_template->index.c_str();
        if(Set->segment_template->media.size())
            media_url = Set->segment_template->media.c_str();
        if(!timescale && Set->segment_template->timescale)
            timescale = Set->segment_template->timescale;
        if(!duration && Set->segment_template->duration)
            duration = Set->segment_template->duration;
    }
    if(rep->segment_template){
        if(rep->segment_template->initialization.size())
            init_template = rep->segment_template->initialization.c_str();
//        if(rep->segment_template->index.size())
//            index_template = rep->segment_template->index.c_str();
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
        //resolve_segment_duration(*period, *Set, *rep, &out_duration, &out_timescale);//, NULL, NULL);
        {
            if(rep->segment_template)
            {
                if(rep->segment_template->timescale)
                    out_timescale = rep->segment_template->timescale;
                if(rep->segment_template->duration)
                    out_duration = rep->segment_template->duration;
            }
        }
        *out_segment_duration_in_ms = (uint64_t)((out_duration * 1000.0) / out_timescale);
    }
    //SKIP  offset the start_number with the number of discarded segments
    url_to_solve = NULL;
    switch (resolve_type){
    case MPD_RESOLVE_URL_INIT:
        url_to_solve = const_cast< char * >(init_template);
        break;
    case MPD_RESOLVE_URL_MEDIA:
        url_to_solve = const_cast< char *>(media_url);
        break;
    default:
        printf("Resolve Type Not Supported");
        err = 1;
    }
    if(!url_to_solve){
        err = 1;
    }
    if(!err)
    {
        //solve the template
        strcpy(solved_template, url_to_solve);//    printf("%s\n", solved_template);  // "1_tiled_dash_track2_$Number$.m4s"
        first_sep = strchr(solved_template, '$');
        if(first_sep) first_sep[0] = 0;//    printf("%s\n", solved_template);// "1_tiled_dash_track2_"
        first_sep = strchr(url_to_solve, '$');//    printf("%s\n", first_sep);// "$Number$.m4s"
        while(first_sep){
            char szPrintFormat[10];
            char szFormat[100];
            char*second_sep = strchr(first_sep+1, '$'); //second_sep == "$.m4s "
            if(!second_sep){
                err = 1;
                break;
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
                        err = 2;//EOS
                        break;
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
    }
    if(!err){
        string solved_str(solved_template);
        string result_url = this->basic_URL;
        result_url.append(solved_str);
        *out_url = result_url; //    std::cout <<*out_url<< '\n';
    }
    return err;
}



//设置MPD文件url以及服务器url(basic_URL)
void MPD::set_mpd_url(string mpd_url){

    MPD_URL = mpd_url;
    char solved_base[128];
    char *sep1, *sep2;
    strcpy(solved_base, MPD_URL.c_str()); //printf("%s\n", solved_template);
    sep1 = strchr(solved_base, '/');
    sep2 = strchr(sep1+1, '/');
    while(sep2){
        sep1 = strchr(sep2+1, '/');
        sep2 = strchr(sep1+1, '/');
    }
    sep1[1] = 0;
    string str(solved_base);
    basic_URL = str;
}

