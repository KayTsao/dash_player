#include <QCoreApplication>
#include "MPD/src/MPD.h"
using namespace dash;

int main(int argc, char *argv[])
{


    MPD newMPD;
    int PeriodCount = 1;
    int AdaptationCount = 3;
    int RepresentationCount=3;
    int i,j,k;

    newMPD.xmlns = "urn:mpeg:dash:schema:mpd:2011";
    newMPD.media_presentation_duration = 60000;
    newMPD.max_segment_duration = 1000;
    newMPD.type = MPD_TYPE_STATIC;
    for (i = 0; i < PeriodCount ; i++)
    {
        Period period;
        for(j = 0; j <AdaptationCount ; j++){
            AdaptationSet AdaSet;
            AdaSet.id = i;
            AdaSet.max_width = AdaSet.max_height = 448;
            AdaSet.max_framerate = 30;
            AdaSet.par = "1:1";
            AdaSet.EssentialProperty.scheme_Id_Uri="urn:mpeg:dash:srd:2014";
            AdaSet.EssentialProperty.value = "1,0,0,0,0";
            for(k = 0; k<RepresentationCount; k++)
            {
                Representation rep;
                rep.id = "1"; rep.width = rep.height = 448;
                AdaSet.representations.push_back(rep);
            }
            period.adaptationSets.push_back(AdaSet);
        }
        newMPD.periods.push_back(period);
     }


    QCoreApplication a(argc, argv);
    //FILE *f = fopen("");


    return a.exec();
}
