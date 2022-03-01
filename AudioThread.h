#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>
#include <QThread>


class AudioThead : public QThread
{
    Q_OBJECT
public:
    explicit AudioThead(QObject *parent = nullptr);
    void run() override;
private:
    ~AudioThead();
signals:

};

#endif // AUDIOTHREAD_H
