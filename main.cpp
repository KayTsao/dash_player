#include <QCoreApplication>
#include "MPD/src/MPD.h"
#include <string>

using namespace dash;
using namespace std;

int main(int argc, char *argv[])
{

    char* MPD_URL = "http://10.3.57.202:8000/dash/rabbit/dash_tiled.mpd";

    MPD newMPD;
    newMPD.mpd_get_base_url(MPD_URL);
// parseData(File* mpd_file, MPD newMPD);
    newMPD.xmlns = "urn:mpeg:dash:schema:mpd:2011";
    newMPD.media_presentation_duration = 3000;
    newMPD.max_segment_duration = 1000;
    newMPD.type = MPD_TYPE_STATIC;
    int PeriodCount = 1;
    int AdaptationCount = 3;
    int RepresentationCount= 3;
    int i,j,k;
    for (i = 0; i < PeriodCount ; i++)
    {
        Period period;
        period.duration_in_ms = 3000;
        for(j = 0; j <AdaptationCount ; j++){
            AdaptationSet AdaSet;
            if(j == 0)
            {
                AdaSet.max_width = 2688;
                AdaSet.max_height= 1792;
                AdaSet.max_framerate = 30;
                AdaSet.par = "3:2";
                AdaSet.EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014";
                AdaSet.EssentialProperty.value = "1,0,0,0,0";

//                segTemp->initialization = "dash_tiled_set1_init.mp4";
                AdaSet.segment_template = new SegmentTemplate();
                AdaSet.segment_template->initialization="dash_tiled_set1_init.mp4";
                /*
                AdaSet.segment_template = new SegmentTemplate();
                AdaSet.segment_template->initialization = "dash_tiled_set1_init.mp4";
                */
                Representation rep;
                rep.id = "1";
                rep.mimeType = "video/mp4";
                rep.width = 2688; rep.height = 1792; rep.frame_rate = 30; rep.sar="1:1"; rep.bandwidth=7648;
                SegmentTemplate seg_temp_rep;
                seg_temp_rep.media = "1_tiled_dash_track1_$Number$.m4s";
                seg_temp_rep.timescale = 30000;
                seg_temp_rep.start_number = 1;
                seg_temp_rep.duration = 30000;
                rep.segment_template = &seg_temp_rep;
                AdaSet.representations.push_back(rep);
            }/*
            else
            {
                AdaSet.id = i;
                AdaSet.max_width = AdaSet.max_height = 448;
                AdaSet.max_framerate = 30;
                AdaSet.par = "1:1";
                AdaSet.EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014";
                AdaSet.EssentialProperty.value = "1,0,0,0,0";
                for(k = 0; k<RepresentationCount; k++)
                {
                    if(k == 0)
                    {

                    }
                    Representation rep;
                    rep.id = "1"; rep.width = rep.height = 448;
                    rep.segment_template.media = "1_tiled_dash_track2_$Number$.m4s";
                    AdaSet.representations.push_back(rep);
                }
            }*/
            period.adaptationSets.push_back(AdaSet);
        }
        newMPD.periods.push_back(period);
     }

//resolve and download init part
    int err;
    int AdaSetID = 0, repID=0, download_seg_index=1;
    MPD_URLResolveType resolve_type = MPD_RESOLVE_URL_INIT;
    uint64_t out_segment_duration_in_ms; char* out_url;
    err = newMPD.mpd_resolve_url(AdaSetID, repID, download_seg_index, resolve_type, &out_segment_duration_in_ms, &out_url);
    printf("out_url:%s\n", out_url);
    // call download with out_url


    bool go_on = true;
    while(go_on){

        for(int i = 0; i <1; i++){//nb_seg_in_rep; i++){
            int AdaSetID = 1, repID=0, download_seg_index= i ;
            MPD_URLResolveType resolve_type = MPD_RESOLVE_URL_MEDIA;
            uint64_t out_segment_duration_in_ms; char* out_url;
            err = newMPD.mpd_resolve_url(AdaSetID, repID, download_seg_index, resolve_type, &out_segment_duration_in_ms, &out_url);
            printf("out_url:%s\n", out_url);
            // call download with out_url
        }
    }


/*
    const char *url_to_solve, *media_template;
    std::string str("1_tiled_dash_track2_$Number$.m4s");
    url_to_solve = NULL;
    media_template = NULL;
    media_template = str.c_str() ;
    url_to_solve  = media_template;

    char solved_template[128];
    char* first_sep;

    strcpy(solved_template, url_to_solve);
printf("%s\n", solved_template);
    first_sep = strchr(solved_template, '$');
    if(first_sep) first_sep[0] = 0;
printf("%s\n", solved_template);

    first_sep = strchr(url_to_solve, '$');
    char*second_sep = strchr(first_sep+1, '$');
    printf("%s\n", first_sep);  //"$Number$.m4s"
    while(first_sep){
        char szPrintFormat[50];
        char szFormat[100];
        char *format_tag;
        char*second_sep = strchr(first_sep+1, '$');
        printf("%s\n", second_sep);// "$.m4s "

        if(!second_sep){
            return -1;
        }
        second_sep[0] = 0;
        printf("%s  %s\n", first_sep, second_sep);// "$Number"
        strcpy(szPrintFormat, "%d");
        if (!strcmp(first_sep+1, "Number")) {
            int start_number = 0;
            int item_index = 5;
            sprintf(szFormat, szPrintFormat, start_number + item_index);
            strcat(solved_template, szFormat);
            //check start time is in period
            {
                // do nothing here;
            }
        }
        second_sep[0] = '$';
        printf("%s  %s\n", first_sep, second_sep);// "$Number$.m4s  $.m4s"

        first_sep = strchr(second_sep+1, '$');
        if(first_sep) first_sep[0] = 0;
        if(strlen(second_sep+1))
            strcat(solved_template, second_sep+1);
        if(first_sep) first_sep[0] = '$';
    }
    printf("%s\n", solved_template);  //"1_tiled_dash_track2_X.m4s"
*/

    QCoreApplication a(argc, argv);
    return a.exec();
}
