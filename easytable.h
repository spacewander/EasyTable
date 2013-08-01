#ifndef EasyTable_H
#define EasyTable_H

#include <QTableWidget>

class Cell;
class EasyTableCompare;
class QTextDocument;

class EasyTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit EasyTable(QWidget *parent = 0);

    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;
    void clear();
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);

    void sort(const EasyTableCompare &compare);

    void setFont(const QFont &font);
    void setAlignment(int alignment);
    void setTextColor(QColor &textColor);
    void setBackgroundColor(QColor &backgroundColor);

    bool getDefaultAlignment();
    bool autoRecalculate() const{return autoRecalc;}
signals:
    void modified();
public slots:
    void cut();
    void copy();
    void paste();
    void del();

    void rowInsert();
    void columnInsert();

    void selectCurrentRow();
    void selectCurrentColumn();

    void recalculate();
    void setAutoRecalculate(bool recalc);

    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
    void findInAll(const QString &str,Qt::CaseSensitivity cs);

    void setDefaultAlignment(bool ok);
    QTextDocument* getContext();//get context for print
private slots:
    void somethingChanged();
private:
    enum{RECOGNITIONNUMBER =0x7F51C883};//set the magicnumber of sheet
    static int RowCount,ColumnCount;

    Cell* cell(int row,int column) const;
    QString text(int row,int column) const;
    QString formula(int row,int column) const;
    void setFormula(int row,int column,const QString &formula);

    void columnSort(const EasyTableCompare &compare,const QTableWidgetSelectionRange &range);
    void defaultSort(const EasyTableCompare &compare,const QTableWidgetSelectionRange &range);

    void setHeaderItem();

    bool autoRecalc;
    bool defaultAlignment;
    QTextDocument* context;
};

class EasyTableCompare
{
public:
    bool operator()(const QStringList &row1,const QStringList
                &row2) const;

    enum{KeyCount = 3};
    int keys[KeyCount];
    bool ascending[KeyCount];
};

#endif // EasyTable_H
