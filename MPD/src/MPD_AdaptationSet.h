#ifndef ADAPTATIONSET_H
#define ADAPTATIONSET_H
#include "MPD_Segment.h"
#include "MPD_Representation.h"
#include <cstring>
using namespace std;
namespace mpd
{
struct Property{
    string scheme_Id_Uri;
    string value;
    string *id;
};

class AdaptationSet
{
public:
    AdaptationSet():
        max_width(0),
        max_height(0),
        max_framerate(0),
        srd_row_idx(0),
        srd_col_idx(0),
        par(""),
        lang(""),
        segment_template(NULL){}

    ~AdaptationSet(){
        int i, RepCount;
        RepCount = representations.size();
        for(i =0; i < RepCount; i++)
        {
            delete(representations[i]);
        }
        representations.clear();
        if(segment_template) delete(segment_template);
    }

    uint32_t id;

    bool segment_alignment;
    bool bitstream_switching;
    uint32_t max_width;
    uint32_t max_height;
    uint32_t max_framerate;
    string par;
    string lang;
    Property SupplementalProperty;
    Property EssentialProperty;
    SegmentTemplate *segment_template;
    vector< Representation * >   representations;

    bool is_basic_AS;
    string basic_rep_id;//basic rep
    string dependency_id;// non basic rep

    // info out of MPD
 //   uint32_t active_rep_index;
 //   uint32_t prev_active_rep_index;
 //   uint32_t download_start_time;
    uint32_t srd_x, srd_y, srd_w, srd_h;
    int srd_col_idx, srd_row_idx;

    string InitFileUrl;
    string basic_url;
    uint64_t duration_in_ms;

    uint32_t setup_adaptationset(string url, uint64_t duration){
        uint32_t err = 0;
        basic_url = url;
        err = solve_property();
        if(!err){
            int i, rep_count;
            duration_in_ms = duration;
            rep_count = representations.size();
            for(i =0; i < rep_count; i++){
                Representation* rep = representations.at(i);
                err = rep->setup_representation(url, duration);
                if(err) return err;
            }
        }
        return err;
    }


private:
    uint32_t solve_property(){
        uint32_t err=0;
        //Essential&Supplemental: tile_based
        if(!EssentialProperty.scheme_Id_Uri.empty()){
            if(EssentialProperty.scheme_Id_Uri.compare("urn:mpeg:dash:srd:2014") == 0){
                char * val = new char [EssentialProperty.value.length()+1];
                strcpy (val, EssentialProperty.value.c_str());
                sscanf(val, "%d,%d,%d,%d,%d", &id, &srd_x, &srd_y, &srd_w, &srd_h);
                delete[] val;
            }
        }
        if(!SupplementalProperty.scheme_Id_Uri.empty()){
            if(SupplementalProperty.scheme_Id_Uri.compare("urn:mpeg:dash:srd:2014") == 0){
                char * val = new char [SupplementalProperty.value.length()+1];
                strcpy (val, SupplementalProperty.value.c_str());
                sscanf(val, "%d,%d,%d,%d,%d", &id, &srd_x, &srd_y, &srd_w, &srd_h);
                if(srd_w && srd_h){
                    srd_col_idx = srd_x/srd_w;
                    srd_row_idx = srd_y/srd_h;
                    int count = representations.size();
                    for(int i = 0; i < count; i++){
                        representations.at(i)->set_srd_coord_info(srd_row_idx, srd_col_idx);
                    }
                }
                delete[] val;
            }
        }
        Representation *rep = representations.at(0);
        if(rep){
            if(rep->dependency_id.empty()){
                is_basic_AS = true;
                basic_rep_id = rep->id;
                if(segment_template){
                    if(segment_template->initialization.size()){
                        InitFileUrl = basic_url;
                        InitFileUrl.append(segment_template->initialization);
                    }
                }
                else if(rep->segment_template){
                    if(rep->segment_template->initialization.size()){
                        InitFileUrl = basic_url;
                        InitFileUrl.append(rep->segment_template->initialization);
                    }
                }
                else{
                    printf("ERROR: no initial filename found\n");
                    err = 1;
                }
            }
            else{
                is_basic_AS = false;
                dependency_id = rep->dependency_id;
            }
        }
        return err;
    }
};
}
#endif // ADAPTATIONSET_H
