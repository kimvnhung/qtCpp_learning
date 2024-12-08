#include "common.h"

void acceptClickEvents(QWidget *widget, bool isAccept)
{
    // Disable all mouse interactions
    widget->setAttribute(Qt::WA_TransparentForMouseEvents, !isAccept);

    // Optionally disable focus
    widget->setFocusPolicy(isAccept ? Qt::StrongFocus : Qt::NoFocus);
    widget->setAttribute(Qt::WA_AcceptTouchEvents, isAccept);
    // Loop in children
    for (auto child : widget->findChildren<QWidget *>())
    {
        child->setAttribute(Qt::WA_AcceptTouchEvents, isAccept);
    }
}
