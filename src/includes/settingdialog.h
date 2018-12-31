#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include<QDialog>
#include<QTabWidget>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QSpinBox>

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(int _fontSize, QWidget *perent = nullptr);
    int fontSize;
public slots:
    void okClicked();
private:
    QTabWidget *tab;

    QPushButton *ok;
    QPushButton *cancel;
    QSpinBox *spin;
};

#endif // SETTINGDIALOG_H
