#include "configureinputdialog.h"
#include "ui_configureinputdialog.h"
#include <QKeyEvent>
#include "config.h"

ConfigureInputDialog::ConfigureInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureInputDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->configure_button, &QPushButton::released, this, &ConfigureInputDialog::handleConfigureInput);
    touch_labels_.resize(BUTTON_COUNT);
    initTouchLabels();
    Config conf;
    input_ = conf.input();
    printTouchLabels();
}

ConfigureInputDialog::~ConfigureInputDialog()
{
    delete ui;
}

void ConfigureInputDialog::initTouchLabels(){
    touch_labels_[0] = ui->up_touch;
    touch_labels_[1] = ui->down_touch;
    touch_labels_[2] = ui->left_touch;
    touch_labels_[3] = ui->right_touch;
    touch_labels_[4] = ui->select_touch;
    touch_labels_[5] = ui->start_touch;
    touch_labels_[6] = ui->b_touch;
    touch_labels_[7] = ui->a_touch;
}

void ConfigureInputDialog::printTouchLabels(){
    touch_labels_[0]->setText(QKeySequence(input_.up).toString(QKeySequence::NativeText));
    touch_labels_[1]->setText(QKeySequence(input_.down).toString(QKeySequence::NativeText));
    touch_labels_[2]->setText(QKeySequence(input_.left).toString(QKeySequence::NativeText));
    touch_labels_[3]->setText(QKeySequence(input_.right).toString(QKeySequence::NativeText));
    touch_labels_[4]->setText(QKeySequence(input_.select).toString(QKeySequence::NativeText));
    touch_labels_[5]->setText(QKeySequence(input_.start).toString(QKeySequence::NativeText));
    touch_labels_[6]->setText(QKeySequence(input_.b).toString(QKeySequence::NativeText));
    touch_labels_[7]->setText(QKeySequence(input_.a).toString(QKeySequence::NativeText));
}

void ConfigureInputDialog::handleConfigureInput(){
    configuring_ = true;
    current_touch_index_ = 0;
    printTouchLabels();
    touch_labels_[current_touch_index_]->setText("Waiting for input...");
    ui->configure_button->clearFocus(); //avoid "entering" configure again when typing enter
}

void ConfigureInputDialog::keyReleaseEvent(QKeyEvent *e){
    QDialog::keyReleaseEvent(e);
    if(configuring_){
        Q_ASSERT(current_touch_index_ < touch_labels_.size());
        Q_ASSERT(touch_labels_[current_touch_index_] != nullptr);
        switch(current_touch_index_){
        case 0:
            input_.up = e->key();
            break;
        case 1:
            input_.down = e->key();
            break;
        case 2:
            input_.left = e->key();
            break;
        case 3:
            input_.right = e->key();
            break;
        case 4:
            input_.select = e->key();
            break;
        case 5:
            input_.start = e->key();
            break;
        case 6:
            input_.b = e->key();
            break;
        case 7:
            input_.a = e->key();
            break;
        default:
            Q_ASSERT(false); // not implemented
        }

        touch_labels_[current_touch_index_]->setText(QKeySequence(e->key()).toString(QKeySequence::NativeText));
        current_touch_index_++;
        if(current_touch_index_ == BUTTON_COUNT){
            configuring_ = false;
        }
        else {
            touch_labels_[current_touch_index_]->setText("Waiting for input...");
        }
    }
}

void ConfigureInputDialog::keyPressEvent(QKeyEvent *e){
    if (configuring_ && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)){
        return;
    }
    QDialog::keyPressEvent(e);
}

const Input &ConfigureInputDialog::input() const {
    return input_;
}
