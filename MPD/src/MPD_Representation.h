#ifndef REPRESENTATION_H
#define REPRESENTATION_H
#include "MPD_Segment.h"
#include <cstring>
#include <iostream>
using namespace std;

namespace mpd
{
class Representation
{
public:
    Representation():
        id(""),
        width(0),
        height(0),
        frame_rate(0),
        bandwidth       (0),
        srd_row_idx(0),
        srd_col_idx(0),
        segment_template (NULL),
        Needed(false),
        Requested(false),
        Downloaded(false),
        basic_url(""),
        m4s_url_tmp(""){}

    ~Representation(){
        if(segment_template) delete(segment_template);
   }

    string id; /*MANDATORY*/
    string mimeType;
    string codecs;
    uint32_t width;
    uint32_t height;
    uint32_t frame_rate;
    string sar;
    int startWithSAP;
    uint32_t bandwidth; /*MANDATORY*/
    string dependency_id;
    SegmentTemplate *segment_template;

    int srd_col_idx, srd_row_idx;
    double seg_time_ms;
    uint32_t nb_seg_in_rep;
    string basic_url;
    string m4s_url_tmp;
    bool Needed;
    bool Requested;
    bool Downloaded;
//    string init_url;


    //设置rep行列坐标 由AS层调用
    void set_srd_coord_info(int row, int col){
        srd_row_idx = row;
        srd_col_idx = col;
    }

    uint32_t setup_representation(string url, uint64_t duration){
        uint32_t err = 0;
        basic_url = url;
        if(segment_template->duration && segment_template->timescale){
            seg_time_ms = (double)segment_template->duration / segment_template->timescale * 1000;
        }
        else
            err = 1;
        if(!err){
            //set seg时长&个数
            double mediaDuration = (double)duration;
            /*duration is given in ms*/
            double nb_seg = mediaDuration/1000;
            nb_seg *= segment_template->timescale;
            nb_seg /= segment_template->duration;
            nb_seg_in_rep = (uint32_t)nb_seg;
            if(nb_seg_in_rep < nb_seg){
                nb_seg_in_rep++;
            }
            //设置媒体文件模板m4s_url_tmp
            if(segment_template->media.empty()){
                err = 1;
            }
            else{
                m4s_url_tmp = url;
                m4s_url_tmp.append(segment_template->media);
            }
            Needed = false;
        }
        return err;
    }

    uint32_t get_media_url(uint32_t seg_idx, string* out_url, string* out_file_name){
        uint32_t err = 0;
        if(seg_idx > nb_seg_in_rep){
            printf("segment_id[%d] out of range[%d]\n", seg_idx, nb_seg_in_rep);
            err = 1;
            return err;
        }
        char* url_to_solve = new char[m4s_url_tmp.length()+1];
        char* solved_template = new char[m4s_url_tmp.length()+1];

        if(!err){
            char *first_sep;
            strcpy(url_to_solve, m4s_url_tmp.c_str());
            strcpy(solved_template, m4s_url_tmp.c_str());

            first_sep = strchr(solved_template, '$');
            if(first_sep) first_sep[0] = 0;         // solved_template: "1_tiled_dash_track2_"
            first_sep = strchr(url_to_solve, '$');  // first_sep:       "$Number$.m4s"

            while(first_sep){
                char PrintFormat[10];
                char szFormat[10];
                char*second_sep = strchr(first_sep+1, '$'); //second_sep == "$.m4s "
                if(!second_sep){
                    err = 1;
                    break;
                }
                second_sep[0] = 0; //first_sep == "$Number"
                strcpy(PrintFormat, "%d");
                //通配符为Number
                if (!strcmp(first_sep+1, "Number")){
                    sprintf(szFormat, PrintFormat, seg_idx);
                    strcat(solved_template, szFormat); // solved_template: "1_tiled_dash_track2_N"
                }
                second_sep[0] = '$';  //first_sep： $Number$.m4s        second_sep：$.m4s"
                first_sep = strchr(second_sep+1, '$');
                if(first_sep) first_sep[0] = 0;
                if(strlen(second_sep+1))
                    strcat(solved_template, second_sep+1);// solved_template: "1_tiled_dash_track2_N.m4s"
                if(first_sep) first_sep[0] = '$';
            }
        }
        if(!err){
            string solved_str(solved_template);
           // string file_str(solved_template);
            *out_url = solved_str;

            size_t found  = solved_str.find(basic_url);
            if(found != string::npos)
                solved_str.erase(found, basic_url.length());
            *out_file_name = solved_str;
       //     std::cout <<*out_url<< '\t' << * out_file_name << '\n';
        }
        delete[] url_to_solve;
        delete[] solved_template;
        return err;
    }
};
}

#endif // REPRESENTATION_H
