#include "scribbler.h"
#include <QtWidgets>

MouseEvent::MouseEvent(int _action, QPointF _pos, quint64 _time)
    : action(_action),pos(_pos),time(_time) { }

QDataStream &operator<<(QDataStream &out, const MouseEvent &evt) {
    out << evt.action << evt.pos << evt.time;
    return out;
}

QDataStream &operator>>(QDataStream &in, MouseEvent &evt) {
    in >> evt.action >> evt.pos >> evt.time;
    return in;
}

Scribbler::Scribbler(QWidget *parent) : QGraphicsView(parent), lineWidth(4.0) {
    setScene(&scene);
    // no fixed scene rectangle size here
    setRenderHint(QPainter::Antialiasing, true);
    // set a default minimum size
    setMinimumSize(QSize(400, 300));
}

const QList<MouseEvent>& Scribbler::getMouseEvents() const {
    return events;
}

void Scribbler::setMouseEvents(const QList<MouseEvent> &newEvents) {
    events = newEvents;
}

void Scribbler::mouseMoveEvent(QMouseEvent *evt) {
    QGraphicsView::mouseMoveEvent(evt);
    QPointF p = mapToScene(evt->pos());
    scene.addLine(QLineF(lastPoint, p), QPen(Qt::white , lineWidth, Qt::SolidLine, Qt::FlatCap));
    scene.addEllipse(QRectF(p - QPointF(0.5*lineWidth, 0.5*lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);
    lastPoint = p;

    events << MouseEvent(MouseEvent::Move, p, evt->timestamp());
}

void Scribbler::mousePressEvent(QMouseEvent *evt) {
    QGraphicsView::mousePressEvent(evt);
    QPointF p = mapToScene(evt->pos());
    lastPoint = p;
    scene.addEllipse(QRectF(p - QPointF(0.5*lineWidth, 0.5*lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);

    events << MouseEvent(MouseEvent::Press, p, evt->timestamp());
}

void Scribbler::mouseReleaseEvent(QMouseEvent *evt) {
    QGraphicsView::mouseReleaseEvent(evt);
    QPointF p = mapToScene(evt->pos());

    events << MouseEvent(MouseEvent::Release, p, evt->timestamp());
}

void Scribbler::startCapture() {
    events.clear();
}

void Scribbler::endCapture() {
    emit captureEnded(events); //emit to MainWindow
    events.clear(); //clear events
}
void Scribbler::onSaveTriggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Scribble", "", "Scribble Files (*.scrib)");
    if (!fileName.isEmpty() && !saveToFile(fileName)) {
        QMessageBox::warning(this, "Error", "Unable to save Scribble");
    }
}

void Scribbler::onLoadTriggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "Load Scribble", "", "Stribble File (*.scrib)");
    if (!fileName.isEmpty()) {
        if (!loadFromFile(fileName)) {
            QMessageBox::critical(this, "Save Error", "Unable to save Scribble");
        } else {
            readEvents();
        }
    }
}

bool Scribbler::saveToFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save Error", "Could not save file");
        return false;
    }

    QDataStream stream(&file);
    stream << 1; //version number for compatibility
    stream << events.size();

    for (const auto& event: events) {
        stream << event.action << event.pos;
    }

    file.close();
    return true;
}

bool Scribbler::loadFromFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Load Error", "Could not load this file");
        return false;
    }

    QDataStream stream(&file);
    int version;
    stream >> version;
    if (version != 1) {
        QMessageBox::warning(this, "Load Error", "Incompatible file version");
        return false;
    }

    int numEvents;
    stream >> numEvents;
    events.clear();
    for (int i = 0; i < numEvents; ++i) {
        MouseEvent event;
        stream >> event.action >> event.pos;
        events.append(event);
    }

    file.close();
    return true;
}

void Scribbler::readEvents() {
    // scene.clear();
    // //needs revision
    // foreach (const MouseEvent &evt, events) {
    //     if (evt.action == MouseEvent::Press) {
    //         lastPoint = evt.pos;
    //     } else if (evt.action == MouseEvent::Move){
    //         scene.addLine(QLineF(lastPoint, evt.pos), QPen(Qt::white, lineWidth));
    //         lastPoint = evt.pos;
    //     } else if (evt.action == MouseEvent::Release) {
    //         scene.addEllipse(QRectF(evt.pos - QPointF(0.5 * lineWidth, 0.5 * lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);
    //     }
    // }


    scene.clear();

    if (events.isEmpty()) {
        QMessageBox::warning(this, "Load Error", "No events to load");
        return;
    }

    for (int i = 0; i < events.size(); ++i) {
        const MouseEvent &evt = events[i];

        if (evt.action == MouseEvent::Press) {
            lastPoint = evt.pos;
        }
        else if (evt.action == MouseEvent::Move) {
            if (!lastPoint.isNull()) {
                scene.addLine(QLineF(lastPoint, evt.pos), QPen(Qt::white, lineWidth));
                lastPoint = evt.pos;
            }
        }
        else if (evt.action == MouseEvent::Release) {
            if (!evt.pos.isNull()) {
                scene.addEllipse(QRectF(evt.pos - QPointF(0.5 * lineWidth, 0.5 * lineWidth), QSizeF(lineWidth, lineWidth)), Qt::NoPen, Qt::white);
            }
            lastPoint = QPointF(); //reset lastPoint
        }
    }
}


void Scribbler::setViewMode(ViewMode mode) {
    viewMode = mode;
    update();
}

void Scribbler::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    for (int i = 0; i<events.size(); ++i) {
        const MouseEvent &e = events[i];

        if (viewMode == ViewMode::DotsOnly) {
            painter.drawEllipse(e.pos, 3, 3); //draw dots only
        } else if (viewMode == ViewMode::LineSegments) {
            if (i > 0) {
                painter.drawLine(events[i-1].pos, e.pos);
            }
            painter.drawEllipse(e.pos, 3, 3);
        }
    }
}

void Scribbler::reset() {
    scene.clear();
    events.clear();
}
