#ifndef ISYNCHRONIZER_H
#define ISYNCHRONIZER_H

class ISynchronizer
{
public:
    virtual void processNext() = 0; // gọi trong consumer loop
    virtual void onPause() = 0;
    virtual void onResume() = 0;
    virtual void onSeek(double seconds) = 0;
};

#endif // ISYNCHRONIZER_H
