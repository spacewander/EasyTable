#ifndef HELPSEARCHDIALOG_H
#define HELPSEARCHDIALOG_H

#include <QDialog>
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class HelpSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HelpSearchDialog(QWidget *parent, QStringList& strList);

signals:
    void actionSelected(QString& sender);

public slots:
    void changeActionList(QString str);
    void demoAction();

private:
    QStringList actionList;
    QLineEdit *lineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *lineLabel;
    QLabel *textLabel;
    QTableWidget *table;
    /// the sum of rows in the table
    enum{ITEMS = 12};

    void triggerMenu(QString& str);
    void setTable(int row, const QString &s);
};

#endif // HELPSEARCHDIALOG_H
