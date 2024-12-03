#include <QFrame>
#include <QMouseEvent>
#include <QEvent>
#include <QGestureEvent>

class interactiveCard : public QFrame
{
    Q_OBJECT

public:
    explicit interactiveCard(const QString &title, QWidget *parent = nullptr);
    ~interactiveCard() override = default;
    void setOnClick(std::function<void()> callback);

signals:
    void cardClicked(const QString &title);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

private:
    QString cardTitle;
    bool gestureEvent(QGestureEvent *event);
    std::function<void()> onClickCallback;
};