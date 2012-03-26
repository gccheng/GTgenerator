#ifndef CONFIGPARAM_H
#define CONFIGPARAM_H

#include <QString>
#include "sharedtypes.h"

struct ConfigParam{
    QString strGroundtruthPath;
    QString strOrigFramePath;
    TrackType trackAlgo;
    bool morphopen;
    int strelsize;
    ConfigParam():strGroundtruthPath(""),
        strOrigFramePath(""),
        trackAlgo(SNAKE),
        morphopen(true),
        strelsize(3){}

    ConfigParam(QString groundtruthPath, QString origPath, TrackType algo, bool postproc, int strel)
        : strGroundtruthPath(groundtruthPath),
        strOrigFramePath(origPath),
        trackAlgo(algo),
        morphopen(postproc),
        strelsize(strel){}
};

#endif // CONFIGPARAM_H
