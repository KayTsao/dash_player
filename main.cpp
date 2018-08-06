#include <QCoreApplication>
#include "MPD/src/MPD.h"
#include "dash.h"
//#include <string>
#include <iostream>
#include <time.h>
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
    uint32_t err;
    string init_url, media_url, file_url;
    int i;
    //0-5
    /*
    for(i=0; i < 4; i++)
    {
        //get_init_url(uint32_t group_id, string* out_url, string* out_file)
        err = DASH.get_init_url(i,&init_url,&file_url);
        cout << init_url <<'\n'<< file_url << '\n';
    }
    cout << '\n';
    for(i = 0; i<5; i++){
        err = DASH.get_media_url(i,0,2, &media_url,&file_url);
        cout << media_url << '\n' << file_url << '\n';
    }

    cout << '\n';
    for(i = 1 ; i < 5; i++){
        err = DASH.get_media_url(i,1,1, &media_url,&file_url);
        cout << media_url << '\n' << file_url << '\n';
    }

    cout << '\n';
    for(i = 1 ; i < 5; i++){
        err = DASH.get_media_url(i,2,1, &media_url,&file_url);
        cout << media_url << '\n' << file_url << '\n';
    }

    cout << '\n';
    */

    float cam_dir[3];
    cam_dir[0] = 0;    cam_dir[1] = 0;    cam_dir[2] = -1;

    int group_i, track_i, track_count;
    int seg_idx = 1;

    //Download initfile
    string initLR_url, initLR_fname;
    string initHR_url, initHR_fname;
    string m4s_url, m4s_fname;

    DASH.get_init_url(0, &initLR_url, &initLR_fname);
    DASH.get_init_url(0, &initHR_url, &initHR_fname);
    //call download

clock_t start = clock();
clock_t diff;
float dt;
    err = 0;
    while(!err){
        DASH.FOV_Based_Adaptation_Algo(cam_dir);
        diff = clock() - start;
        dt = ((float)diff)/CLOCKS_PER_SEC;
        if(dt>0.99){
            for(group_i = 0; group_i < 5; group_i ++){
                track_count = DASH.DASH_Groups[group_i]->Tracks.size();
                for(track_i=0; track_i < track_count; track_i++){
                    Representation* track = DASH.DASH_Groups[group_i]->Tracks[track_i];
                    //需要下载
                    if(track->Needed){
                        if(track_i == 0){
                            if(group_i != 1)
                                continue;
                        }
                        err = DASH.get_media_url(group_i,track_i,seg_idx, &m4s_url, &m4s_fname );
                        cout << m4s_url <<'\n';//<< m4s_fname << '\n';
                        //call download
                    }
                }
            }
            seg_idx ++;
            printf("seg_id:%d\n", seg_idx);
            start = clock();
        }
    }
    printf("end!!!!seg_idx:%d\n", seg_idx);

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
