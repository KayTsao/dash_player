#include <QCoreApplication>
#include "MPD/src/MPD.h"
#include "dash.h"
//#include <string>
#include <iostream>
using namespace mpd;
using namespace std;

static void parse_mpd(MPD* newMPD){
    newMPD->xmlns = "urn:mpeg:dash:schema:mpd:2011";
    //newMPD->media_presentation_duration = "PT0H2M4.000S";
    //newMPD->max_segment_duration="PT0H0M1.000S";
    newMPD->type = MPD_TYPE_STATIC;
    int PeriodCount = 1; int AdaptationCount = 4; int RepresentationCount= 4;
    int i,j,k;
    for (i = 0; i < PeriodCount ; i++)
    {
        Period* period = new Period();
        period->duration =  "PT0H2M4.000S";

        for(j = 0; j <AdaptationCount ; j++){
            AdaptationSet* AdaSet = new AdaptationSet();
            AdaSet->id = j;
            if(j == 0)
            {
//                <AdaptationSet segmentAlignment="true" maxWidth="896" maxHeight="1344" maxFrameRate="30" par="896:1344" lang="und">
//                  <Representation id="1" mimeType="video/mp4" codecs="hev2.1.6.L186.80" width="896" height="1344" frameRate="30" sar="1:1" startWithSAP="1" bandwidth="3706378">
//                   <SegmentTemplate media="LR_dash$Number$.m4s" timescale="30000" startNumber="1" duration="30000" initialization="LR_dashinit.mp4"/>
//                  </Representation>
//                 </AdaptationSet>
                AdaSet->max_width = 896; AdaSet->max_height= 1344; AdaSet->max_framerate = 30; AdaSet->par = "3:2";
                Representation *rep = new Representation();
                rep->id = "1"; rep->mimeType = "video/mp4"; rep->width = 896; rep->height = 1344; rep->frame_rate = 30; rep->sar="1:1"; rep->bandwidth=3706378;
                rep->segment_template = new SegmentTemplate();
                rep->segment_template->media = "LR_dash$Number$.m4s";
                rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                rep->segment_template->initialization="LR_dashinit.mp4";
                AdaSet->representations.push_back(rep);
            }
            if(j == 1)
            {
//                <AdaptationSet segmentAlignment="true" bitstreamSwitching="true" maxWidth="3584" maxHeight="1792" maxFrameRate="30" par="2:1" lang="und">
//                   <EssentialProperty schemeIdUri="urn:mpeg:dash:srd:2014" value="1,0,0,0,0"/>
//                   <SegmentTemplate initialization="dash_tiled_set2_init.mp4"/>
//                   <Representation id="2" mimeType="video/mp4" codecs="hev2.1.6.L186.80" width="3584" height="1792" frameRate="30" sar="1:1" startWithSAP="1" bandwidth="7933">
//                    <SegmentTemplate media="HR1_dash_track1_$Number$.m4s" timescale="30000" startNumber="1" duration="30000"/>
//                   </Representation>
//                  </AdaptationSet>
                AdaSet->max_width = 3584; AdaSet->max_height= 1792; AdaSet->max_framerate = 30; AdaSet->par = "2:1";
                AdaSet->EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014"; AdaSet->EssentialProperty.value="1,0,0,0,0";
                AdaSet->segment_template = new SegmentTemplate();
                AdaSet->segment_template->initialization = "dash_tiled_set2_init.mp4";

                Representation *rep = new Representation();
                rep->id = "2"; rep->mimeType = "video/mp4"; rep->width = 3584; rep->height = 1792; rep->frame_rate = 30; rep->sar="1:1"; rep->bandwidth=7933;
                rep->segment_template = new SegmentTemplate();
                rep->segment_template->media = "HR1_dash_track1_$Number$.m4s";
                rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                AdaSet->representations.push_back(rep);
            }
            if(j == 2){
                AdaSet->max_width = 448; AdaSet->max_height= 448; AdaSet->max_framerate = 30; AdaSet->par = "1:1";
                AdaSet->SupplementalProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014"; AdaSet->SupplementalProperty.value="1,0,0,448,448";
                for(k = 0; k < 4;k++){
                    Representation *rep = new Representation();
                    rep->mimeType = "video/mp4"; rep->width = 448; rep->height = 448; rep->frame_rate = 30;rep->sar="1:1";
                    rep->dependency_id ="2";
                    if(k == 0){
                        rep->id = "2_3";  rep->bandwidth=177913;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR1_dash_track2_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 1){
                        rep->id = "2_35";  rep->bandwidth=296745;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR2_dash_track2_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 2){
                        rep->id = "2_67";  rep->bandwidth=211516;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR3_dash_track2_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 3){
                        rep->id = "2_99";  rep->bandwidth=10792;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "BLK_HR_dash_track2_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    AdaSet->representations.push_back(rep);
                }
            }
            if(j == 3){
                AdaSet->max_width = 448; AdaSet->max_height= 448; AdaSet->max_framerate = 30; AdaSet->par = "1:1";
                AdaSet->SupplementalProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014"; AdaSet->SupplementalProperty.value="1,448,0,448,448";
                for(k = 0; k < 4;k++){
                    Representation *rep = new Representation();
                    rep->mimeType = "video/mp4"; rep->width = 448; rep->height = 448; rep->frame_rate = 30;rep->sar="1:1";
                    rep->dependency_id ="2";
                    if(k == 0){
                        rep->id = "2_4";  rep->bandwidth=405926;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR1_dash_track3_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 1){
                        rep->id = "2_36";  rep->bandwidth=451514;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR2_dash_track3_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 2){
                        rep->id = "2_68";  rep->bandwidth=353658;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "HR3_dash_track3_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    if(k == 3){
                        rep->id = "2_100";  rep->bandwidth=11032;
                        rep->segment_template = new SegmentTemplate();
                        rep->segment_template->media = "BLK_HR_dash_track3_$Number$.m4s";
                        rep->segment_template->start_number = 1; rep->segment_template->timescale = 30000; rep->segment_template->duration = 30000;
                    }
                    AdaSet->representations.push_back(rep);
                }
            }
            period->adaptationSets.push_back(AdaSet);
        }
        newMPD->periods.push_back(period);
    }
}

int main(int argc, char *argv[])
{
    string MPD_URL( "http://10.3.57.202:8000/dash/rabbit/dash_tiled.mpd" );
    MPD newMPD(MPD_URL);// = new MPD(MPD_URL);

    parse_mpd(&newMPD);
    newMPD.setup_mpd();//set_mpd_url(MPD_URL);
    dash DASH(&newMPD);

    string init_url, media_url;
    int i;
    //0-5
    for(i=0; i < 4; i++)
    {
        init_url = DASH.get_init_url(i);
        cout << init_url <<'\n';
    }
    for(i = 2 ; i < 4; i++){
        media_url = DASH.get_media_url(i,1,1);
        cout << media_url <<'\n';
    }


//    string get_media_url(uint32_t group_id, uint32_t track_id, uint32_t segment_id);

//    int periodCount;
//    periodCount = newMPD.periods.size();
//    for(i =0; i < periodCount; i++)
//    {
//        delete(newMPD.periods[i]);
//    }
//    newMPD.periods.clear();
    QCoreApplication a(argc, argv);
    return a.exec();
}
