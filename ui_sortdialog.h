#ifndef UI_SORTDIALOG_H
#define UI_SORTDIALOG_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SortDialog : public QObject
{
    Q_OBJECT

public:
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_2;
    QPushButton *okButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QPushButton *moreButton;
    QPushButton *cancelButton;
    QGroupBox *primaryGroupBox;
    QGridLayout *gridLayout;
    QComboBox *primaryColumnCombo;
    QComboBox *primaryOrderCombo;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *secondaryGroupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_3;
    QComboBox *SecondaryColumnCombo;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QComboBox *SecondaryOrderCombo;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *tertiaryGroupBox;
    QGridLayout *gridLayout_6;
    QLabel *label_5;
    QComboBox *TertiaryColumnCombo;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_6;
    QComboBox *TertiaryOrderCombo;
    QSpacerItem *horizontalSpacer_9;

    void setupUi(QDialog *SortDialog)
    {
        if (SortDialog->objectName().isEmpty())
            SortDialog->setObjectName(tr("SortDialog"));
        SortDialog->resize(402, 379);
        SortDialog->setMaximumSize(QSize(402, 379));
        gridLayout_5 = new QGridLayout(SortDialog);
        gridLayout_5->setObjectName(tr("gridLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(tr("gridLayout_2"));
        okButton = new QPushButton(SortDialog);
        okButton->setObjectName(tr("okButton"));
        okButton->setDefault(true);

        gridLayout_2->addWidget(okButton, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 2, 1, 2, 1);

        verticalSpacer_2 = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 3, 1);

        moreButton = new QPushButton(SortDialog);
        moreButton->setObjectName(tr("moreButton"));

        gridLayout_2->addWidget(moreButton, 4, 1, 1, 1);

        cancelButton = new QPushButton(SortDialog);
        cancelButton->setObjectName(tr("cancelButton"));

        gridLayout_2->addWidget(cancelButton, 1, 1, 1, 1);

        primaryGroupBox = new QGroupBox(SortDialog);
        primaryGroupBox->setObjectName(tr("primaryGroupBox"));
        gridLayout = new QGridLayout(primaryGroupBox);
        gridLayout->setObjectName(tr("gridLayout"));
        primaryColumnCombo = new QComboBox(primaryGroupBox);
        primaryColumnCombo->setObjectName(tr("primaryColumnCombo"));

        gridLayout->addWidget(primaryColumnCombo, 0, 1, 1, 1);

        primaryOrderCombo = new QComboBox(primaryGroupBox);
        primaryOrderCombo->setObjectName(tr("primaryOrderCombo"));

        gridLayout->addWidget(primaryOrderCombo, 1, 1, 1, 1);

        label_2 = new QLabel(primaryGroupBox);
        label_2->setObjectName(tr("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(primaryGroupBox);
        label->setObjectName(tr("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(85, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(85, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 2, 1, 1);


        gridLayout_2->addWidget(primaryGroupBox, 0, 0, 3, 1);


        gridLayout_5->addLayout(gridLayout_2, 0, 0, 1, 1);

        secondaryGroupBox = new QGroupBox(SortDialog);
        secondaryGroupBox->setObjectName(tr("secondaryGroupBox"));
        secondaryGroupBox->setMaximumSize(QSize(308, 78));
        gridLayout_3 = new QGridLayout(secondaryGroupBox);
        gridLayout_3->setObjectName(tr("gridLayout_3"));
        label_3 = new QLabel(secondaryGroupBox);
        label_3->setObjectName(tr("label_7"));

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        SecondaryColumnCombo = new QComboBox(secondaryGroupBox);
        SecondaryColumnCombo->setObjectName(tr("SecondaryColumnCombo"));

        gridLayout_3->addWidget(SecondaryColumnCombo, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(85, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        label_4 = new QLabel(secondaryGroupBox);
        label_4->setObjectName(tr("label_8"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        SecondaryOrderCombo = new QComboBox(secondaryGroupBox);
        SecondaryOrderCombo->setObjectName(tr("SecondaryOrderCombo"));

        gridLayout_3->addWidget(SecondaryOrderCombo, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(85, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 1, 2, 1, 1);


        gridLayout_5->addWidget(secondaryGroupBox, 1, 0, 1, 1);

        tertiaryGroupBox = new QGroupBox(SortDialog);
        tertiaryGroupBox->setObjectName(tr("tertiaryGroupBox"));
        tertiaryGroupBox->setEnabled(true);
        tertiaryGroupBox->setMaximumSize(QSize(308, 78));
        gridLayout_6 = new QGridLayout(tertiaryGroupBox);
        gridLayout_6->setObjectName(tr("gridLayout_6"));
        label_5 = new QLabel(tertiaryGroupBox);
        label_5->setObjectName(tr("label_11"));

        gridLayout_6->addWidget(label_5, 0, 0, 1, 1);

        TertiaryColumnCombo = new QComboBox(tertiaryGroupBox);
        TertiaryColumnCombo->setObjectName(tr("TertiaryColumnCombo"));

        gridLayout_6->addWidget(TertiaryColumnCombo, 0, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(150, 14, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        label_6 = new QLabel(tertiaryGroupBox);
        label_6->setObjectName(tr("label_12"));

        gridLayout_6->addWidget(label_6, 1, 0, 1, 1);

        TertiaryOrderCombo = new QComboBox(tertiaryGroupBox);
        TertiaryOrderCombo->setObjectName(tr("TertiarySortCombo"));

        gridLayout_6->addWidget(TertiaryOrderCombo, 1, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(150, 14, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_9, 1, 2, 1, 1);


        gridLayout_5->addWidget(tertiaryGroupBox, 2, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        label_2->setBuddy(primaryOrderCombo);
        label->setBuddy(primaryColumnCombo);
        label_3->setBuddy(SecondaryColumnCombo);
        label_4->setBuddy(SecondaryOrderCombo);
        label_5->setBuddy(SecondaryColumnCombo);
        label_6->setBuddy(SecondaryOrderCombo);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(primaryColumnCombo, primaryOrderCombo);
        QWidget::setTabOrder(primaryOrderCombo, TertiaryOrderCombo);
        QWidget::setTabOrder(TertiaryOrderCombo, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(SortDialog);
        QObject::connect(okButton, SIGNAL(clicked()), SortDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), SortDialog, SLOT(close()));
        QObject::connect(moreButton, SIGNAL(clicked(bool)), secondaryGroupBox, SLOT(show()));//
        QObject::connect(moreButton, SIGNAL(clicked(bool)), tertiaryGroupBox, SLOT(show()));

        QMetaObject::connectSlotsByName(SortDialog);
    } // setupUi

    void retranslateUi(QDialog *SortDialog)
    {
        SortDialog->setWindowTitle(QApplication::translate("SortDialog", "Sort", 0));
        okButton->setText(QApplication::translate("SortDialog", "\345\260\261\350\277\231\346\240\267", 0));
        moreButton->setText(QApplication::translate("SortDialog", "\346\233\264\345\244\232", 0));
        cancelButton->setText(QApplication::translate("SortDialog", "\347\256\227\344\272\206\345\220\247", 0));
        primaryGroupBox->setTitle(QApplication::translate("SortDialog", "&Primary Key", 0));
        primaryColumnCombo->clear();
        primaryColumnCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "None", 0)
        );
        primaryOrderCombo->clear();
        primaryOrderCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "Ascending", 0)
         << QApplication::translate("SortDialog", "Desceding", 0)
        );
        label_2->setText(QApplication::translate("SortDialog", "Order:", 0));
        label->setText(QApplication::translate("SortDialog", "Column:", 0));
        secondaryGroupBox->setTitle(QApplication::translate("SortDialog", "&Secondary Key", 0));
        label_3->setText(QApplication::translate("SortDialog", "Column:", 0));
        SecondaryColumnCombo->clear();
        SecondaryColumnCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "None", 0)
        );
        label_4->setText(QApplication::translate("SortDialog", "Order:", 0));
        SecondaryOrderCombo->clear();
        SecondaryOrderCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "Ascending", 0)
         << QApplication::translate("SortDialog", "Desceding", 0)
        );
        tertiaryGroupBox->setTitle(QApplication::translate("SortDialog", "&Tertiary Key", 0));
        label_5->setText(QApplication::translate("SortDialog", "Column:", 0));
        TertiaryColumnCombo->clear();
        TertiaryColumnCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "None", 0)
        );
        label_6->setText(QApplication::translate("SortDialog", "Order:", 0));
        TertiaryOrderCombo->clear();
        TertiaryOrderCombo->insertItems(0, QStringList()
         << QApplication::translate("SortDialog", "Ascending", 0)
         << QApplication::translate("SortDialog", "Desceding", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class SortDialog: public Ui_SortDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SORTDIALOG_H
