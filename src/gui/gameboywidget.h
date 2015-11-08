#ifndef _GAMEBOY_WIDGET_H_
#define _GAMEBOY_WIDGET_H_

#include <QWidget>
#include "input.h"

class GameboyWidget : public QWidget{
    Q_OBJECT
    Q_DISABLE_COPY(GameboyWidget)
public:
    GameboyWidget(QWidget *parent = 0);
    ~GameboyWidget();

    void setInput(const Input &input);

    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

public slots:
    void updateImage(const QImage &image);

signals:
    void newInput(uint8_t input);

private:
    QImage current_image_;
    QWidget *old_parent_;
    Input input_;
    uint8_t gb_input_ {};
};

#endif
