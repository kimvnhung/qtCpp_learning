#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H

class FrameListener {
public:
    virtual void onFrameAvailable(const char* frame, int dataSize) = 0;
};

#endif // FRAMELISTENER_H
