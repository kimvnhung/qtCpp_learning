#ifndef ICONSUMER_H
#define ICONSUMER_H

#include "iframe.h"
#include <QThread>

class IConsumer : public QThread
{
public:

    void stop();
    // QThread interface
    std::shared_ptr<IFrame> frame() const;
    void setFrame(const std::shared_ptr<IFrame> &newFrame);

    QString name() const;
    void setName(const QString &newName);

protected:
    void run() override;
    virtual void renderFrame() = 0;
private:
    std::shared_ptr<IFrame> m_frame;
    QString m_name{};
    bool m_running{false};
};

#endif // ICONSUMER_H
