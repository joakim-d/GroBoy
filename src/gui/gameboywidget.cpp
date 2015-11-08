#include "gameboywidget.h"
#include <QPainter>
#include <QKeyEvent>
#include "config.h"
#include "../joypad.h"

GameboyWidget::GameboyWidget(QWidget *parent) : QWidget(parent){
    Config conf;
    input_ = conf.input();
}

GameboyWidget::~GameboyWidget(){}

void GameboyWidget::updateImage(const QImage &image){
    current_image_ = image.scaled(this->width(), this->height());
    update();
}

void GameboyWidget::paintEvent(QPaintEvent *e){
    QWidget::paintEvent(e);

    QPainter painter(this);

    //int width = current_image_.width();
    //int height = current_image_.height();
    //int posx = this->width() - (width >> 1);
    //int posy = this->height() - (height >> 1);
    painter.drawPixmap(0, 0, QPixmap::fromImage(current_image_));
}

void GameboyWidget::mouseDoubleClickEvent(QMouseEvent *e){
    QWidget::mouseDoubleClickEvent(e);

    if(isFullScreen()){
        setParent(old_parent_);
        showNormal();
    }
    else {
        old_parent_ = parentWidget();
        setParent(0);
        showFullScreen();
    }
}

void GameboyWidget::keyPressEvent(QKeyEvent *e){

    if(e->key() == input_.up){
        gb_input_ |= Joypad::UP;
    }
    else if(e->key() == input_.down){
        gb_input_ |= Joypad::DOWN;
    }
    else if(e->key() == input_.left){
        gb_input_ |= Joypad::LEFT;
    }
    else if(e->key() == input_.right){
        gb_input_ |= Joypad::RIGHT;
    }
    else if(e->key() == input_.select){
        gb_input_ |= Joypad::SELECT;
    }
    else if(e->key() == input_.start){
        gb_input_ |= Joypad::START;
    }
    else if(e->key() == input_.b){
        gb_input_ |= Joypad::B;
    }
    else if(e->key() == input_.a){
        gb_input_ |= Joypad::A;
    }
    else{
        return;
    }
    emit newInput(gb_input_);
}

void GameboyWidget::keyReleaseEvent(QKeyEvent *e){
    if(e->key() == input_.up){
        gb_input_ &= ~Joypad::UP;
    }
    else if(e->key() == input_.down){
        gb_input_ &= ~Joypad::DOWN;
    }
    else if(e->key() == input_.left){
        gb_input_ &= ~Joypad::LEFT;
    }
    else if(e->key() == input_.right){
        gb_input_ &= ~Joypad::RIGHT;
    }
    else if(e->key() == input_.select){
        gb_input_ &= ~Joypad::SELECT;
    }
    else if(e->key() == input_.start){
        gb_input_ &= ~Joypad::START;
    }
    else if(e->key() == input_.b){
        gb_input_ &= ~Joypad::B;
    }
    else if(e->key() == input_.a){
        gb_input_ &= ~Joypad::A;
    }
    else{
        return;
    }
    emit newInput(gb_input_);
}

void GameboyWidget::setInput(const Input &input){
    input_ = input;
}
