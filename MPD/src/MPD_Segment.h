#ifndef SEGMENT_H
#define SEGMENT_H
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;
namespace mpd
{
class SegmentTemplate{
public:
    SegmentTemplate(){
        initialization=string();
        media = string();
        bitstream_switching = string();
        start_number = 0;
        duration = 0;
        timescale = 0;
//        index = string();
    }
    string media;
    string initialization;
    uint32_t timescale;
    uint32_t duration;
    uint32_t start_number;
    string bitstream_switching;
    // string index;
};
}

#endif // SEGMENT_H
