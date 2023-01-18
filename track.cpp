#include "track.h"

Track::Track(QWidget *parent) : QWidget(parent) {

}

void Track::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    calculateDimens();

    static const QPen white(QColor("#FFFFFF"), 1, Qt::SolidLine, Qt::RoundCap);
    static const QPen gray(QColor("#A0A0A0"), 1, Qt::SolidLine, Qt::RoundCap);
    static const QPen black(QColor("#000000"), 1, Qt::SolidLine, Qt::RoundCap);

    static const QColor WHITE("#FFFFFF");
    static const QColor GRAY("#A0A0A0");
    static const QColor BLACK("#000000");

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = event->rect();
    rect.setRect(marginX, marginY, rect.width() - 2 * marginX, rect.height() - 2 * marginY);
    painter.fillRect(rect, WHITE);

    QFont font;
    font.setPixelSize(getScaled(400));
    painter.setFont(font);
    painter.setPen(black);

    QBrush defaultBrush = painter.brush();

    for (int i = 0; i < game.checkpoints.size(); i++) {
        auto & p = game.checkpoints[i];
        if (i == game.current) {
            QBrush brush(Qt::yellow);
            painter.setBrush(brush);
            painter.drawEllipse(QPointF(marginX + getScaled(p.x), marginY + getScaled(p.y)), getScaled(600), getScaled(600));
            painter.setBrush(defaultBrush);
        }
        painter.drawEllipse(QPointF(marginX + getScaled(p.x), marginY + getScaled(p.y)), getScaled(600), getScaled(600));
        if ((i+1)%(game.checkpoints.size()) > 9) {
            painter.drawText(QPointF(marginX + getScaled(p.x - 200), marginY + getScaled(p.y + 100)), QString::number((i+1)%(game.checkpoints.size())));
        } else {
            painter.drawText(QPointF(marginX + getScaled(p.x - 100), marginY + getScaled(p.y + 100)), QString::number((i+1)%(game.checkpoints.size())));
        }
    }

    QRectF rectf(marginX + getScaled(game.car.x - 300), marginY + getScaled(game.car.y - 150), getScaled(600), getScaled(300));
    rotate(&painter,rectf, game.car.angle, true);
    painter.drawRect(rectf);
    painter.drawLine(marginX + getScaled(game.car.x), marginY + getScaled(game.car.y), marginX + getScaled(game.car.x+300), marginY + getScaled(game.car.y));

}

void Track::calculateDimens() {
    float width = QWidget::width();
    float height = QWidget::height();
    float s = width / height;
    if (s >= 16.0f/9.0f) {
        scale = width / 16000.0f;
        marginX = ((height * s) - ((16 * height) / 9));
        marginX /= 2;
        marginY = 0;
        scale = (width - 2 * marginX) / 16000.0f;
    } else {
        scale = height / 9000.0f;
        marginX = 0;
        marginY = (width - (9.0f/16.0f) * s * width) / (2 * s);
        scale = (height - 2 * marginY) / 9000.0f;
    }
}

int Track::getScaled(int toScale) {
    return (int)(scale * toScale);
}

float Track::getScaled(float toScale) {
    return (scale * toScale);
}

void Track::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        game = Game(rand());
    }
    repaint();
}
