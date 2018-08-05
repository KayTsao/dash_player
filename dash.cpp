#include "dash.h"
using namespace mpd;
#include <iostream>

dash_group::dash_group(){
    id = 0;
    serverURL = "";
}

// deconstruc dash_group before MPD_Representation
dash_group::~dash_group() {
}

uint32_t dash_group::setup_dash_group(MPD *Mpd, uint32_t group_id) {
    id = group_id;
    //duration = Mpd->periods.at(Mpd->active_period_index)->duration_in_ms;
    serverURL = Mpd->basic_URL;
    AdaptationSet* AS;
    int i, as_count;

    if(group_id == 0) { //LR
        AS = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.at(0);
        if(!AS->is_basic_AS){
            return 1;//error
        }
        width = AS->max_width;
        height = AS->max_height;
        initUrl = AS->InitFileUrl;
        Tracks.push_back(AS->representations.at(0));
    }
    else{ //HR1 HR2 HR3 BLK
        as_count = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.size();
        for(i = 1; i < as_count; i++){
            AS = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.at(i);
            if(!AS)
                return 1;//error
            Representation * rep ;
            if(AS->is_basic_AS){
                width = AS->max_width;
                height = AS->max_height;
                initUrl = AS->InitFileUrl;
                rep = AS->representations.at(0);
                Tracks.push_back(rep);
            }
            else{
                rep = AS->representations.at(group_id - 1);
                if(!rep)
                    return 1; //error
                Tracks.push_back(rep);
            }
        }
    }
    return 0;
}






dash::dash(MPD *mpd){
    int i;
    for(i = 0; i < 5; i++){
        dash_group *group = new dash_group();
        group->setup_dash_group(mpd, i);
        DASH_Groups.push_back(group);
    }
}

dash::~dash(){
    int i, group_count;
    group_count = DASH_Groups.size();
    for(i = 0; i < group_count; i++){
        delete(DASH_Groups[i]);
    }
    DASH_Groups.clear();
}

string dash::get_init_url(uint32_t group_id){
    if(group_id < 5 )
        return DASH_Groups.at(group_id)->initUrl;
    else
        return "";
}

string dash::get_media_url(uint32_t group_id, uint32_t track_id, uint32_t segment_id){

    if(group_id>=5)
        return "";
    string out_url;
    Representation * track = DASH_Groups[group_id]->Tracks[track_id];
    track->get_media_url(segment_id, &out_url);
    return out_url;
}


/*
void dash::download_m4s_resources(){
    list<Representation*>::iterator it;
    int i;
    for(i = 0; i < 5; i++){
        for(it = DASH_Group[i].Tracks.begin(); it!=DASH_Group[i].Tracks.end(); ++it){
            Representation* track = *it;
            if(track->downloadFlag)
               cout << track->m4s_url_tmp << '\n';
        }
    }
}*/

//为每个representation设置downloadFlag
void dash::FOV_Based_Adaptation_Algo(const float cam_dir[3] ){
    float dir[3];
    int row_id, col_id, group_i, track_j;
    int track_count;

//    for(it = DASH_Group[0].Tracks.begin(); it!=DASH_Group[0].Tracks.end(); ++it){
//        Representation* track = *it;
//        track->downloadFlag = true;
//    }
    // Group[0]和[4]需要全部下载？
    for(group_i = 1; group_i < 4; group_i++){
        track_count = DASH_Groups[group_i]->Tracks.size();
        for(track_j = 0; track_j < track_count; track_j++){
            Representation* track = DASH_Groups[group_i]->Tracks[track_j];
            if(track_j == 0)
                track->downloadFlag = true;
            else{
                row_id = track->srd_row_idx;
                col_id = track->srd_col_idx;
                dir[0] = Dir_Table_3D[group_i-1][row_id][col_id][0];
                dir[1] = Dir_Table_3D[group_i-1][row_id][col_id][1];
                dir[2] = Dir_Table_3D[group_i-1][row_id][col_id][2];
                float cosTheta = dir[0]*cam_dir[0] + dir[1]*cam_dir[1] + dir[2]*cam_dir[2];
                if(cosTheta >= cosFOVAngle){
                    track->downloadFlag = true;
                }
                else
                    track->downloadFlag = false;
            }
        }
    }
}

// TODO
void dash::Bandwidth_Based_Adaptation_Algo(){
}
