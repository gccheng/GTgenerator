#ifndef LOADVIDEOTHREAD_H
#define LOADVIDEOTHREAD_H

#include <QObject>
#include <QThread>

class LoadVideoThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadVideoThread(const QString &path, QThread *parent = 0);

signals:
    void completeLoading(bool result);
public slots:
    void startLoading();

protected:
    void run();

private:
    QString filePath;
};

#endif // LOADVIDEOTHREAD_H
