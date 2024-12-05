#include "interactive_card.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QGestureEvent>

#include "common.hpp"

interactiveCard::interactiveCard(const QString &title,QWidget* cardWidget , QWidget *parent)
    : QFrame(parent)
    , cardTitle(title)
    , cardWidget(cardWidget)
{
    setStyleSheet("QFrame { border-radius: 10px; background-color: #7ac2cf; }"
                  "QFrame:hover { background-color: #7079db; }");
    setAttribute(Qt::WA_AcceptTouchEvents);

    QVBoxLayout *layout = new QVBoxLayout(this);

    if(cardWidget)
    {
        layout->addWidget(cardWidget);
        // Disable click event for the card widget
        cardWidget->setAttribute(Qt::WA_AcceptTouchEvents, false);
    }
    else
    {
        QLabel *titleLabel = new QLabel(title, this);
        titleLabel->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(14);
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        layout->addWidget(titleLabel);
    }
}

void interactiveCard::setOnClick(std::function<void()> callback)
{
    onClickCallback = std::move(callback);
}

void interactiveCard::reloadCardWidget()
{
    cardWidget->update();
    layout()->addWidget(cardWidget);
    acceptClickEvents(cardWidget,false);
}

bool interactiveCard::hasCardWidget() const
{
    return cardWidget;
}

void interactiveCard::mousePressEvent(QMouseEvent *event)
{
    if (onClickCallback)
    {
        onClickCallback();
    }
    emit cardClicked(cardTitle);
    QFrame::mousePressEvent(event);
}

bool interactiveCard::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        return gestureEvent(static_cast<QGestureEvent *>(event));
    }
    return QFrame::event(event);
}

bool interactiveCard::gestureEvent(QGestureEvent *event)
{
    if (QGesture *tap = event->gesture(Qt::TapGesture))
    {
        if (onClickCallback)
        {
            onClickCallback();
        }
        emit cardClicked(cardTitle);
        return true;
    }
    return false;
}
