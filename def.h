//
// Created by linhongyun on 2018/7/19.
//

#ifndef NATIVE_CODEC_DEF_H
#define NATIVE_CODEC_DEF_H

#include <cstdlib>
#include <string>
#include <list>

using namespace std;

typedef struct {
    uint32_t dts;
    uint32_t index;
    uint32_t frameNum;
    char *data = NULL;
    uint32_t size = 0;
} AccessUnit;

enum MediaType {
    UNKOWN = 0,
    INITMP4,
    M4S,
    UNION,
};

// enough ?
enum NodeType {
    S_START = 0,
    S_MID,
    S_END,
};

enum MediaStatus {
    UNDOWN_NONEED = 0,
    UNDOWN_NEED,
    DOWN_NEED,
    DOWN_NONEED,
    UNCOMBINED,
    COMBINED,
    DELETED,
};

//AU4..., 4K ?
enum AdaptiveSetType {
    ADAPT_8K_LR = 0,
    ADAPT_8K_HR_AU1,
    ADAPT_8K_HR_AU2,
    ADAPT_8K_HR_AU3,
};

typedef struct {
    MediaType mediaType;
    string mediaUrl;
    string cacheName;
    void *cachePtr = NULL;
    MediaStatus mediaStatus;
    list<AccessUnit> AU;
} MediaCache;

#define MAX_ADAPT_SET_NUM 4   //LR + n*AU
#define MAX_ADAPT_SET_RANK 4  //Represention(1M,4M,9M,13M),(原始分辨率，1/2分辨率，1/4分辨率)
#define MAX_ADAPT_SET_W 10    //?
#define MAX_ADAPT_SET_H 13    //?

typedef struct {
    uint32_t adaptiveSetNum;       //
    uint32_t adaptiveSetRank;      //
    uint32_t tile_w;
    uint32_t tile_h;
    uint32_t statusRange[4][4] = {0};    //{1,1,1,1; 2,2,8,4; 3,2,8,4; 4,2,8,4}
    MediaStatus status[MAX_ADAPT_SET_NUM][MAX_ADAPT_SET_RANK][MAX_ADAPT_SET_W][MAX_ADAPT_SET_H] = {UNDOWN_NONEED};
} SegmentStatus;


typedef struct {
    double timeStamp;
    double linear_acceleration[3];
    double angular_velocity[3];
} IMUData;


typedef struct {
    uint32_t dts;
    SegmentStatus *segmentStatus; //?
    uint32_t index;
    uint32_t frameNum;
    uint32_t format; //? yuv420,yuv422,yuv444 ...
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    char *data;
    uint32_t size;
} DecodeBuffer;

enum PlayerStatus {
    P_START = 0,
    P_PLAYING,
    P_SEEKING,
    P_PAUSE,
    P_STOP,
    P_QUIT,
};

#endif //NATIVE_CODEC_DEF_H
