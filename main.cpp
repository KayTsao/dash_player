#include <QCoreApplication>
#include "MPD/src/MPD.h"
#include <string>
#include <iostream>
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
    int AdaptationCount = 1;
   std::cout << newMPD.basic_URL << '\n';

    // int RepresentationCount= 3;mpd_get_base_url
    int i,j,k;
    for (i = 0; i < PeriodCount ; i++)
    {
        Period* period = new Period();
        period->duration_in_ms = 3000;
        for(j = 0; j <AdaptationCount ; j++){
            AdaptationSet* AdaSet = new AdaptationSet();
            if(j == 0)
            {

                AdaSet->max_width = 2688;
                AdaSet->max_height= 1792;
                AdaSet->max_framerate = 30;
                AdaSet->par = "3:2";
                AdaSet->EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014";
                AdaSet->EssentialProperty.value = "1,0,0,0,0";

//                segTemp->initialization = "dash_tiled_set1_init.mp4";
                AdaSet->segment_template = new SegmentTemplate();
                AdaSet->segment_template->initialization = "dash_tiled_set1_init.mp4"; //set_media_name(string("dash_tiled_set1_init.mp4"));
//                AdaSet.segment_template->initialization.assign("dash_tiled_set1_init.mp4");
                /*
                AdaSet.segment_template = new SegmentTemplate();
                AdaSet.segment_template->initialization = "dash_tiled_set1_init.mp4";
                */
                //std::cout << AdaSet.segment_template->initialization << '\n';
                             //printf("%s\n", AdaSet.segment_template->media);

                Representation *rep = new Representation();
                rep->id = "1";
                rep->mimeType = "video/mp4";
                rep->width = 2688; rep->height = 1792; rep->frame_rate = 30; rep->sar="1:1"; rep->bandwidth=7648;

                rep->segment_template = new SegmentTemplate();
                rep->segment_template->media = "1_tiled_dash_track1_$Number$.m4s";
                rep->segment_template->timescale = 30000;
                rep->segment_template->start_number = 1;
                rep->segment_template->duration = 30000;
                AdaSet->representations.push_back(rep);
            }
            /*
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

            period->adaptationSets.push_back(AdaSet);
        }
                //std::cout << AdaSet.representations.at(j).segment_template->media << '\n';
           //period.adaptationSets.push_back(AdaSet);
        newMPD.periods.push_back(period);
    }
    std::cout << newMPD.periods.at(0)->adaptationSets.at(0)->segment_template->initialization<< '\n';
    std::cout << newMPD.periods.at(0)->adaptationSets.at(0)->representations.at(0)->segment_template->media<< '\n';


//resolve and download init part
    int err;
    int AdaSetID = 0, repID=0, download_seg_index=1;
    MPD_URLResolveType resolve_type = MPD_RESOLVE_URL_INIT;
    uint64_t out_segment_duration_in_ms;
    string out_url;
    err = newMPD.mpd_resolve_url(AdaSetID, repID, download_seg_index, resolve_type, &out_segment_duration_in_ms, &out_url);
    std::cout << out_url << '\n';
    // call download with out_url


    bool go_on = true;
    while(go_on){

        for(int i = 0; i <1; i++){//nb_seg_in_rep; i++){
            int AdaSetID = 0, repID=0, download_seg_index= i ;
            MPD_URLResolveType resolve_type = MPD_RESOLVE_URL_MEDIA;
            uint64_t out_segment_duration_in_ms;
            string out_url;
            err = newMPD.mpd_resolve_url(AdaSetID, repID, download_seg_index, resolve_type, &out_segment_duration_in_ms, &out_url);
            std::cout << out_url << '\n';
            // call download with out_url
        }
        go_on = false;
    }

    QCoreApplication a(argc, argv);
    return a.exec();
}
