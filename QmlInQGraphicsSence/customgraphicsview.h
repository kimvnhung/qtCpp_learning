#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
    using base_type = QGraphicsView;
public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~CustomGraphicsView() = default;

protected:
    void wheelEvent(QWheelEvent *event) override;
private:
    void zoomIn();
    void zoomOut();
};

#endif // CUSTOMGRAPHICSVIEW_H
