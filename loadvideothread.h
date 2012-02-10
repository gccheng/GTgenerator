#ifndef LOADVIDEOTHREAD_H
#define LOADVIDEOTHREAD_H

/*
 * Description: LoadVideoThread deals with loading user-specified
 *              video using another thread.
 * Create Date:
 * Last Update: ##/##/####: ************
 *
 */

#include <QObject>
#include <QThread>

#include "gtvideo.h"

class LoadVideoThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadVideoThread(const QString &path, SourceType t, GTVideo *v, QThread *parent = 0);

protected:
    void run();

private:
    void loadFromVideo();
    void loadFromImages();

signals:
    void completeLoading(bool result);
public slots:
    void startLoading();

private:
    SourceType type;        // Video source type: VIDEO/IMAGES
    QString filePath;       // Video path: file/directory
    GTVideo *gtv;           // To which the video resides in memory
};

#endif // LOADVIDEOTHREAD_H
