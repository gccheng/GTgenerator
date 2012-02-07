#include "gtvideo.h"

GTVideo::GTVideo(QObject *parent) :
    QObject(parent)
{
}

GTVideo::GTVideo(SourceType t, QString path)
    :type(t),filePath(path)
{
}
