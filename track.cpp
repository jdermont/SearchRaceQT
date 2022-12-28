#include "track.h"

Track::Track(QWidget *parent) : QWidget(parent) {

}

void Track::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    static const QPen white(QColor("#FFFFFF"), 1, Qt::SolidLine, Qt::RoundCap);
    static const QPen gray(QColor("#A0A0A0"), 1, Qt::SolidLine, Qt::RoundCap);
    static const QPen black(QColor("#000000"), 1, Qt::SolidLine, Qt::RoundCap);

    static const QColor WHITE("#FFFFFF");
    static const QColor GRAY("#A0A0A0");
    static const QColor BLACK("#000000");

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = event->rect();
    painter.fillRect(rect, WHITE);

    QFont font;
    font.setPixelSize(320 / SCALE);
    painter.setFont(font);
    painter.setPen(black);

    QBrush defaultBrush = painter.brush();

    for (int i = 0; i < game.checkpoints.size(); i++) {
        auto & p = game.checkpoints[i];
        if (i == game.current) {
            QBrush brush(Qt::yellow);
            painter.setBrush(brush);
            painter.drawEllipse(QPointF(p.x/SCALE, p.y/SCALE), 600/SCALE, 600/SCALE);
            painter.setBrush(defaultBrush);
        }
        painter.drawEllipse(QPointF(p.x/SCALE, p.y/SCALE), 600/SCALE, 600/SCALE);
        if ((i+1)%(game.checkpoints.size()) > 9) {
            painter.drawText(QPointF(p.x/SCALE - 12, p.y/SCALE + 4), QString::number((i+1)%(game.checkpoints.size())));
        } else {
            painter.drawText(QPointF(p.x/SCALE - 6, p.y/SCALE + 4), QString::number((i+1)%(game.checkpoints.size())));
        }
    }

    QRectF rectf(game.car.x/SCALE - 300/SCALE,game.car.y/SCALE - 150/SCALE,600/SCALE,300/SCALE);
    rotate(&painter,rectf, game.car.angle, true);
    painter.drawRect(rectf);
    painter.drawLine(game.car.x/SCALE,game.car.y/SCALE,game.car.x/SCALE+300/SCALE,game.car.y/SCALE);

}
