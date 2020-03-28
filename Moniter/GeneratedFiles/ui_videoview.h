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
#include <QtWidgets/QComboBox>
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
    QComboBox *light_kind;
    QLabel *label_4;

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
        label_2->setGeometry(QRect(30, 20, 1031, 601));
        label_3 = new QLabel(videoviewClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(80, 650, 351, 211));
        lane_finder = new QPushButton(videoviewClass);
        lane_finder->setObjectName(QStringLiteral("lane_finder"));
        lane_finder->setGeometry(QRect(980, 810, 101, 31));
        cross_lane = new QPushButton(videoviewClass);
        cross_lane->setObjectName(QStringLiteral("cross_lane"));
        cross_lane->setGeometry(QRect(980, 850, 101, 31));
        light_kind = new QComboBox(videoviewClass);
        light_kind->setObjectName(QStringLiteral("light_kind"));
        light_kind->setGeometry(QRect(870, 750, 101, 31));
        label_4 = new QLabel(videoviewClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(870, 730, 72, 15));

        retranslateUi(videoviewClass);
        QObject::connect(light_kind, SIGNAL(currentIndexChanged(int)), videoviewClass, SLOT(on_light_kind_changed()));

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
        light_kind->clear();
        light_kind->insertItems(0, QStringList()
         << QApplication::translate("videoviewClass", "\345\217\257\351\200\232\350\241\214", Q_NULLPTR)
         << QApplication::translate("videoviewClass", "\344\273\205\345\217\257\347\233\264\350\241\214", Q_NULLPTR)
         << QApplication::translate("videoviewClass", "\344\273\205\345\217\257\345\267\246\350\275\254", Q_NULLPTR)
         << QApplication::translate("videoviewClass", "\344\270\215\345\217\257\351\200\232\350\241\214", Q_NULLPTR)
        );
        label_4->setText(QApplication::translate("videoviewClass", "\351\200\232\350\241\214\347\212\266\346\200\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class videoviewClass: public Ui_videoviewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOVIEW_H
