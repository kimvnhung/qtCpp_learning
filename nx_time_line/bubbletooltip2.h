#ifndef BUBBLETOOLTIP2_H
#define BUBBLETOOLTIP2_H

#include "qmlwidget.h"

#include <QObject>
#include <QQuickWidget>
#include <QSharedPointer>

class BubbleToolTipModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(qreal normalizedPointerPos READ normalizedPointerPos WRITE setNormalizedPointerPos NOTIFY normalizedPointerPosChanged FINAL)
    Q_PROPERTY(int pointerEdge READ pointerEdge WRITE setPointerEdge NOTIFY pointerEdgeChanged FINAL)

public:
    BubbleToolTipModel(QObject *parent = NULL);
    ~BubbleToolTipModel();

    enum class State
    {
        shown,
        hidden,
        suppressed
    };
    Q_ENUM(State)

    State state();
    void setState(State state);

    QString text() const;
    void setText(const QString &newText);

    qreal normalizedPointerPos() const;
    void setNormalizedPointerPos(qreal newNormalizedPointerPos);
    int pointerEdge() const;
    void setPointerEdge(int newPointerEdge);

signals:
    void stateChanged();

    void textChanged();

    void normalizedPointerPosChanged();
    void pointerEdgeChanged();

private:
    State m_state;
    QString m_text;
    qreal m_normalizedPointerPos;
    int m_pointerEdge;
    bool m_visible;



};

class BubbleToolTip2 : public QmlWidget
{
    Q_OBJECT
    using base_type = QmlWidget;
    using State = BubbleToolTipModel::State;
public:
    explicit BubbleToolTip2(QObject *parent = nullptr);

    void show();
    void hide(bool immediately = false);
    void suppress(bool immediately = false);

    QString text() const;
    void setText(const QString& value);

    Qt::Orientation orientation() const; //< Default Qt::Horizontal.
    void setOrientation(Qt::Orientation value);

    bool suppressedOnMouseClick() const; //< Default true.
    void setSuppressedOnMouseClick(bool value);

    // Target and enclosing rect are specified in global coordinates.
    void setTarget(const QRect& targetRect);
    void setTarget(const QPoint& targetPoint);
    void setEnclosingRect(const QRect& value);

    virtual BubbleToolTipModel* model() const;

    State state() const;


    QQuickWidget* widget() const;

signals:
protected:
    BubbleToolTip2(const QUrl& componentUrl, QObject* parent = NULL,bool isInit = true);

private:
    class Private;
    QSharedPointer<Private> d;

    // QmlWidget interface
protected:
    void registerQmlType() override;
    void setUpQmlUrl(const QUrl &componentUrl) override ;
    QObject *rootObject() const override;

};



#endif // BUBBLETOOLTIP2_H
