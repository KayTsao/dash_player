#include "dash.h"
using namespace mpd;
#include <iostream>
static float get_v3_len(Vec3 v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}
static float v3_dot(Vec3 v1, Vec3 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z ;
}
static void v3_norm(Vec3 *v){
    float len = get_v3_len(*v);
    v->x = v->x/len;
    v->y = v->y/len;
    v->z = v->z/len;
}

//DASH_GROUP
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
        AS->representations.at(0)->Needed = true;
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
                if(group_id == 4)
                    rep->Needed = true;
                Tracks.push_back(rep);
            }
        }
    }
    return 0;
}

//DASH
dash::dash(MPD *mpd){
    int i;
    cosFOVAngle =  0.707;
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

uint32_t dash::get_init_url(uint32_t group_id, string* out_url, string* out_file){
    uint32_t err = 0;
    if(group_id < 5 ){
        *out_url = DASH_Groups.at(group_id)->initUrl;
        string str_tmp = DASH_Groups.at(group_id)->initUrl;
        size_t found  = str_tmp.find(DASH_Groups.at(group_id)->serverURL);
        if(found != string::npos)
            str_tmp.erase(found, DASH_Groups.at(group_id)->serverURL.length());
        *out_file = str_tmp;
    }
    else
        err = 1;
    return err;
}
uint32_t dash::get_media_url(uint32_t group_id, uint32_t track_id, uint32_t segment_id, string* out_url, string* out_file){
    uint32_t err = 0;
    if(group_id>=5){
        err = 1;
        return err;
    }
    int track_count = DASH_Groups[group_id]->Tracks.size();
    if(track_id < track_count){
        Representation * track = DASH_Groups[group_id]->Tracks[track_id];
        err = track->get_media_url(segment_id, out_url, out_file);
      //  cout << '[' << track->srd_row_idx << ',' << track->srd_col_idx << ']' << '\n';
    }
    else{
        err = 1;
        printf("Track_id[%d] out of range[%d]\n", track_id, track_count);
    }
    return err;
}

//为每个representation设置Needed
void dash::FOV_Based_Adaptation_Algo(const float cam_dir[3] ){
    Vec3 cam_vec;
    cam_vec.x = cam_dir[0];
    cam_vec.y = cam_dir[1];
    cam_vec.z = cam_dir[2];
    v3_norm(&cam_vec);

    int row_id, col_id, group_i, track_i;
    int track_count;
    // Group[0]和[4]需要全部下载？
    for(group_i = 1; group_i < 4; group_i++){
        track_count = DASH_Groups[group_i]->Tracks.size();
        for(track_i = 0; track_i < track_count; track_i++){
            Representation* track = DASH_Groups[group_i]->Tracks[track_i];
            //track->Needed = false;
            //group 1 2 3的track 0 都是"*track1_$Number$.m4s"
            if(track_i == 0)
            {
                track->Needed = true;
            }
            else{
                row_id = track->srd_row_idx;
                col_id = track->srd_col_idx;
                Vec3 track_dir;
                track_dir.x =  Dir_Table_3D[group_i-1][row_id][col_id][0];
                track_dir.y =  Dir_Table_3D[group_i-1][row_id][col_id][1];
                track_dir.z =  Dir_Table_3D[group_i-1][row_id][col_id][2];
                v3_norm(&track_dir);

                float cosTheta = v3_dot(track_dir, cam_vec);//dir[0]*cam_dir[0] + dir[1]*cam_dir[1] + dir[2]*cam_dir[2];
                if(cosTheta >= cosFOVAngle){
                    track->Needed = true;
                }
                else{
                    track->Needed = false;
                }
            }
        }
    }
}

// TODO
void dash::Bandwidth_Based_Adaptation_Algo(){
}
