#ifndef _GAMEBOY_GUI_H_
#define _GAMEBOY_GUI_H_

#include "../gameboy.h"
#include <QLabel>
#include <QThread>

class GameboyGui : public QThread, public Gameboy {
    Q_OBJECT
public:
    GameboyGui(QObject *parent = 0);
    void run();
    void updateScreen(const unsigned char *buffer, unsigned int width, unsigned int height);

public slots:
    void handleNewInput(uint8_t input);
signals:
    void frameReady(const QImage &frame);
private:
    QLabel label_;
};

#endif
