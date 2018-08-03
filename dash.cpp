#include "dash.h"
using namespace mpd;
#include <iostream>

dash_group::dash_group(MPD *Mpd, uint32_t group_id) {

    id = group_id;
    duration = Mpd->periods.at(Mpd->active_period_index)->duration_in_ms;
    //nb_seg =
    serverURL = Mpd->basic_URL;
    AdaptationSet* AS;
    int i, as_count;

    if(group_id == 0) { //LR
        AS = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.at(0);
        if(!AS->is_basic_AS){
            return;//error
        }
        width = AS->max_width;
        height = AS->max_height;
        initUrl = AS->InitFileUrl;
        Tracks.push_back(AS->representations.at(i));
    }
    else{ //HR1 HR2 HR3 BLK
        as_count = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.size();
        for(i = 1; i < as_count; i++){
            AS = Mpd->periods.at(Mpd->active_period_index)->adaptationSets.at(i);
            if(!AS) return;//error
            if(AS->is_basic_AS){
                width = AS->max_width;
                height = AS->max_height;
                initUrl = AS->InitFileUrl;
            }
            else{
                Representation * rep = AS->representations.at(group_id - 1);
                if(!rep) return; //error
                Tracks.push_back(rep);
            }

//            list<Representation*>::iterator it;
//            for(it=Tracks.begin(); it!=Tracks.end(); ++it){
//                Representation* track = *it;
//            }
        }
    }
}

dash_group::~dash_group() {
}


void dash::download_init_resources(){
    list<Representation*>::iterator it;

    for(int i = 0; i < 4; i++)
    {
        if (DASH_Group[i].initUrl.size())
        {
            //download initURL
        }
    }
}

string dash::get_init_url(uint32_t group_id){
    if(group_id<5 && group_id>=0)
        return DASH_Group[group_id].initUrl;
    else
        return "";
}

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
}

//为每个representation设置downloadFlag
void dash::FOV_Based_Adaptation_Algo(const float cam_dir[3] ){
    float dir[3];
    int group_id, row_id, col_id, i;
    list<Representation*>::iterator it;
//    for(it = DASH_Group[0].Tracks.begin(); it!=DASH_Group[0].Tracks.end(); ++it){
//        Representation* track = *it;
//        track->downloadFlag = true;
//    }
    // Group[0]和[4]需要全部下载？
    for(i = 1; i < 4; i++){
        for(it = DASH_Group[i].Tracks.begin(); it!=DASH_Group[i].Tracks.end(); ++it){
            Representation* track = *it;
            if(it == DASH_Group[i].Tracks.begin())
                track->downloadFlag = true;
            else{
                group_id = i;
                row_id = track->srd_row_idx;
                col_id = track->srd_col_idx;
                dir[0] = Dir_Table_3D[group_id-1][row_id][col_id][0];
                dir[1] = Dir_Table_3D[group_id-1][row_id][col_id][1];
                dir[2] = Dir_Table_3D[group_id-1][row_id][col_id][2];
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
