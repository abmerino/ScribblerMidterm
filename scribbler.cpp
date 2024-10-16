#include "scribbler.h"
#include <QtWidgets>

MouseEvent::MouseEvent(int _action, QPointF _pos, quint64 _time)
    : action(_action),pos(_pos),time(_time) { }

QDataStream &operator<<(QDataStream &out, const MouseEvent &evt) {
    return out << evt.action << evt.pos << evt.time;
}

QDataStream &operator>>(QDataStream &in, MouseEvent &evt) {
    return in >> evt.action >> evt.pos >> evt.time;
}

Scribbler::Scribbler(QWidget *parent) : QGraphicsView(parent), lineWidth(4.0) {
    setScene(&scene);
    setSceneRect(QRectF(0.0, 0.0, 800.0, 600.0));
    setMinimumSize(QSize(800, 600));
    setRenderHint(QPainter::Antialiasing, true);
    scene.addRect(sceneRect());
}

void Scribbler::mouseMoveEvent(QMouseEvent *evt) {
    QGraphicsView::mouseMoveEvent(evt);
    QPointF p = mapToScene(evt->pos());
    scene.addLine(QLineF(lastPoint, p), QPen(Qt::white , lineWidth, Qt::SolidLine, Qt::FlatCap));
    scene.addEllipse(QRectF(p - QPointF(0.5*lineWidth, 0.5*lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);
    lastPoint = p;

    events << MouseEvent(MouseEvent::Press, p, evt->timestamp());
}

void Scribbler::mousePressEvent(QMouseEvent *evt) {
    QGraphicsView::mousePressEvent(evt);
    QPointF p = mapToScene(evt->pos());
    lastPoint = p;
    scene.addEllipse(QRectF(p - QPointF(0.5*lineWidth, 0.5*lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);

    events << MouseEvent(MouseEvent::Move, p, evt->timestamp());
}

void Scribbler::mouseReleaseEvent(QMouseEvent *evt) {
    QGraphicsView::mouseReleaseEvent(evt);
    QPointF p = mapToScene(evt->pos());

    events << MouseEvent(MouseEvent::Release, p, evt->timestamp());
}
