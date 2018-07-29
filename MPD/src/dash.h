#ifndef DASH_H
#define DASH_H
#include "MPD_AdaptationSet.h"
#include "MPD_Period.h"
namespace dash
{
class dash
{
public:
    dash();
    ~dash();
    std::vector<AdaptationSet> adaptation_sets;
};
}
#endif // DASH_H
