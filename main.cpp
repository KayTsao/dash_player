#include <QCoreApplication>
#include "MPD/src/MPD.h"
//#include <string>
#include <iostream>
using namespace mpd;
using namespace std;

int main(int argc, char *argv[])
{
    string MPD_URL( "http://10.3.57.202:8000/dash/rabbit/dash_tiled.mpd" );
    MPD newMPD(MPD_URL);// = new MPD(MPD_URL);
    newMPD.setup_mpd();//set_mpd_url(MPD_URL);
// parseData(File* mpd_file, MPD newMPD);
    newMPD.xmlns = "urn:mpeg:dash:schema:mpd:2011";
    newMPD.set_duration("PT0H2M4.000S", MEDIA_PRESENTATION_DURATION);
    newMPD.set_duration("PT0H0M1.000S", MAX_SEGMENT_DURATION);
    newMPD.type = MPD_TYPE_STATIC;
    int PeriodCount = 1; int AdaptationCount = 3; int RepresentationCount= 3;
    int i,j,k;

    for (i = 0; i < PeriodCount ; i++)
    {
        Period* period = new Period();
        period->duration_in_ms = 3000;
        for(j = 0; j <AdaptationCount ; j++){
            AdaptationSet* AdaSet = new AdaptationSet();
            AdaSet->id = j;
            if(j == 0)
            {

                AdaSet->max_width = 2688; AdaSet->max_height= 1792; AdaSet->max_framerate = 30; AdaSet->par = "3:2";
                AdaSet->EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014"; AdaSet->EssentialProperty.value = "1,0,0,0,0";
                //AdaSet->solve_property();

                AdaSet->segment_template = new SegmentTemplate();
                AdaSet->segment_template->initialization = "dash_tiled_set1_init.mp4";

                Representation *rep = new Representation();
                rep->id = "1"; rep->mimeType = "video/mp4"; rep->width = 2688; rep->height = 1792; rep->frame_rate = 30; rep->sar="1:1"; rep->bandwidth=7648;
                rep->segment_template = new SegmentTemplate();
                rep->segment_template->media = "1_tiled_dash_track1_$Number$.m4s";
                rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                AdaSet->representations.push_back(rep);
            }
            else
            {
                AdaSet->max_width = AdaSet->max_height = 448;
                AdaSet->max_framerate = 30;
                AdaSet->par = "1:1";
                AdaSet->SupplementalProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014";
                //AdaSet->EssentialProperty.value = "1,0,0,0,0";
                for(k = 0; k<RepresentationCount; k++)
                {
                    Representation *rep = new Representation();
                    rep->id = "1"; rep->width = rep->height = 448;
                    char target[128];
                    sprintf(target, "%d_tiled_dash_track%d_$Number$.m4s",k+1, j+1);
                    string str(target);
                    rep->segment_template = new SegmentTemplate();
                    rep->segment_template->media = str;
                    cout<<rep->segment_template->media <<'\n';
                    AdaSet->representations.push_back(rep);
                }
            }

            period->adaptationSets.push_back(AdaSet);
        }
        newMPD.periods.push_back(period);
    }
//resolve init part
    int err;
    int AdaSetID = 0, repID=0, download_seg_index=1;
    uint64_t out_segment_duration_in_ms;
    string out_url;
    err = newMPD.get_resolved_url(AdaSetID, repID, download_seg_index, MPD_RESOLVE_URL_INIT, &out_segment_duration_in_ms, &out_url);
    cout << out_url << '\n';

//resolve the rest
    bool go_on = true;
    while(go_on){
        int AdaCount = newMPD.periods.at(0)->adaptationSets.size();
        for(i = 0; i < AdaCount; i++){
            int RepCount = newMPD.periods.at(0)->adaptationSets.at(i)->representations.size();
            for(j = 0; j < RepCount; j++){
                uint64_t out_segment_duration_in_ms;
                string out_url;
                err = newMPD.get_resolved_url(i, j, 1, MPD_RESOLVE_URL_MEDIA, &out_segment_duration_in_ms, &out_url);
                std::cout << out_url << '\n';
            }
        }
        go_on = false;
    }
    int periodCount;
    periodCount = newMPD.periods.size();
    for(i =0; i < periodCount; i++)
    {
        delete(newMPD.periods[i]);
    }
    newMPD.periods.clear();
    QCoreApplication a(argc, argv);
    return a.exec();
}
