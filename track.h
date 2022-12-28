#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QThread>

#include "game.h"

#define SCALE 20.0

class Track : public QWidget {
    Q_OBJECT
public:
    explicit Track(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *);
//    virtual void keyPressEvent(QKeyEvent *);
//    Game game = Game("1000 4500;2500 3905;4000 5095;5500 3905;7000 5095;8500 3905;10000 5095;11500 3905");
//    Game game = Game(15);

private:
    void rotate(QPainter* p, const QRectF& r, qreal angle, bool clock_wise) {
        p->translate(r.center());
        p->rotate(clock_wise ? angle : -angle);
        p->translate(-r.center());
    }

    Game game;
};

#endif // TRACK_H
