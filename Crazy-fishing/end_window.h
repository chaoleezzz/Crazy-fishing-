#ifndef END_WINDOW_H
#define END_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class End_window;
}

class End_window : public QWidget
{
    Q_OBJECT

public:
    explicit End_window(QWidget *parent = nullptr);
    ~End_window();
    QPushButton* getPushbutton_resume(){return pushbutton_resume;};
    QPushButton* getPushbutton_rstart(){return pushbutton_restart;};
    QPushButton* getPushbutton_queding(){return pushbutton_queding;};
    QLineEdit * getlineedit(){return mylineedit;};
    QLabel *get_score_lable(){return score_lable;};
    QLabel *get_tishi_lable(){return tishi_lable;};

private:
    Ui::End_window *ui;
    QPushButton *pushbutton_resume;
    QPushButton *pushbutton_restart;
    QPushButton *pushbutton_queding;
    QLabel *score_lable;
    QLabel *tishi_lable;
    QLineEdit *mylineedit;

};

#endif // END_WINDOW_H
