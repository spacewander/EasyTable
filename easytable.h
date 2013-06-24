#ifndef EasyTable_H
#define EasyTable_H

#include <QTableWidget>

class Cell;
class EasyTableCompare;

class EasyTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit EasyTable(QWidget *parent = 0);
    bool autoRecalculate() const{return autoRecalc;}
    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;
    void clear();
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    void sort(const EasyTableCompare &compare);
signals:
    void modified();
public slots:
    void cut();
    void copy();
    void paste();
    void del();
    void selectCurrentRow();
    void selectCurrentColumn();
    void recalculate();
    void setAutoRecalculate(bool recalc);
    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
private slots:
    void somethingChanged();
private:
    enum{MagicNumber =0x7F51C883,RowCount=199,ColumnCount=26};//set the range of sheet
    Cell* cell(int row,int column) const;
    QString text(int row,int column) const;
    QString formula(int row,int column) const;
    void setFormula(int row,int column,const QString &formula);
    bool autoRecalc;
};

class EasyTableCompare
{
public:
    bool operator()(const QStringList &row1,const QStringList
                &row2) const;
    enum{KeyCount=3};
    int keys[KeyCount];
    bool ascending[KeyCount];
};
#endif // EasyTable_H
