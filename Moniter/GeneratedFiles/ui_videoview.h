/********************************************************************************
** Form generated from reading UI file 'videoview.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOVIEW_H
#define UI_VIDEOVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_videoviewClass
{
public:
    QPushButton *openfile;
    QLabel *label;
    QPushButton *calculate_cars;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *lane_finder;
    QPushButton *cross_lane;
    QPushButton *change_light;

    void setupUi(QWidget *videoviewClass)
    {
        if (videoviewClass->objectName().isEmpty())
            videoviewClass->setObjectName(QStringLiteral("videoviewClass"));
        videoviewClass->resize(1130, 896);
        openfile = new QPushButton(videoviewClass);
        openfile->setObjectName(QStringLiteral("openfile"));
        openfile->setGeometry(QRect(980, 730, 101, 31));
        label = new QLabel(videoviewClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(480, 670, 361, 191));
        calculate_cars = new QPushButton(videoviewClass);
        calculate_cars->setObjectName(QStringLiteral("calculate_cars"));
        calculate_cars->setGeometry(QRect(980, 770, 101, 31));
        label_2 = new QLabel(videoviewClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 20, 1031, 601));
        label_3 = new QLabel(videoviewClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 670, 351, 211));
        lane_finder = new QPushButton(videoviewClass);
        lane_finder->setObjectName(QStringLiteral("lane_finder"));
        lane_finder->setGeometry(QRect(980, 810, 101, 31));
        cross_lane = new QPushButton(videoviewClass);
        cross_lane->setObjectName(QStringLiteral("cross_lane"));
        cross_lane->setGeometry(QRect(980, 850, 101, 31));
        change_light = new QPushButton(videoviewClass);
        change_light->setObjectName(QStringLiteral("change_light"));
        change_light->setGeometry(QRect(870, 790, 93, 28));

        retranslateUi(videoviewClass);

        QMetaObject::connectSlotsByName(videoviewClass);
    } // setupUi

    void retranslateUi(QWidget *videoviewClass)
    {
        videoviewClass->setWindowTitle(QApplication::translate("videoviewClass", "videoview", Q_NULLPTR));
        openfile->setText(QApplication::translate("videoviewClass", "\346\211\223\345\274\200\350\247\206\351\242\221", Q_NULLPTR));
        label->setText(QString());
        calculate_cars->setText(QApplication::translate("videoviewClass", "\350\275\246\346\265\201\351\207\217\347\273\237\350\256\241", Q_NULLPTR));
        label_2->setText(QString());
        label_3->setText(QString());
        lane_finder->setText(QApplication::translate("videoviewClass", "\350\275\246\351\201\223\346\243\200\346\265\213", Q_NULLPTR));
        cross_lane->setText(QApplication::translate("videoviewClass", "\344\270\215\346\214\211\350\275\246\351\201\223\350\241\214\351\251\266", Q_NULLPTR));
        change_light->setText(QApplication::translate("videoviewClass", "\350\256\276\347\275\256\344\270\272\347\272\242\347\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class videoviewClass: public Ui_videoviewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOVIEW_H
