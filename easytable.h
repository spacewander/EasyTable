/**
*@file
*EasyTable is the class about the editable sheet.
*/
#ifndef EasyTable_H
#define EasyTable_H

#include <QTableWidget>
#include "easytablecompare.h"

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
    bool saveFileAsTxt(const QString &fileName);
    bool saveFileAsEt(const QString &fileName);
    bool saveFileAsCsv(const QString &fileName);


    void sort(const EasyTableCompare &compare,bool defaultChoose);

    void setFont(const QFont &font);
    void setAlignment(int alignment);
    void setTextColor(QColor &textColor);
    void setBackgroundColor(QColor &backgroundColor);
    void setGrid(Qt::PenStyle &gridStyle);

    bool getDefaultAlignment()const{return defaultAlignment;}
    bool autoRecalculate() const{return autoRecalc;}
    bool getAutoResize() const{return autoResize;}
    bool getAutoTip() const{return autoTip;}
    void setAutoTip(bool ok) {autoTip = ok;}
    bool getTipDirty() const{return tipDirty;}
    void setTipDirty(bool ok) {tipDirty = ok;}

    int getRowCount()const{return RowCount;}
    int getColumnCount()const{return ColumnCount;}
    void getColumnContext(int column,QSet<QString> &strSet,QVector<int> &maxRow);

    QMap<QString,int> tipMap;/// key is the text of cell,value is the column of cell
    void initialTipMap();
    QString getCurrentText();
signals:
    void modified();

public slots:
    void connectSignalsAndSlots();
    void cut();
    void copy();
    void paste();
    void del();

    void rowInsert();
    void columnInsert();
    void rowRemove();
    void columnRemove();
    void rowHide();
    void columnHide();
    void showHiddenRanges();
    void hideRowUnlike(int column,QString str,int range);

    void selectCurrentRow();
    void selectCurrentColumn();

    void recalculate();
    void resizeCell(QTableWidgetItem *item);
    void setAutoRecalculate(bool recalc);
    void setAutoResize(bool resize);

    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
    void findInAll(const QString &str,Qt::CaseSensitivity cs);
    void findFromHere(const QString &str,Qt::CaseSensitivity cs);
    void replaceSelectedCell(const QString&str);

    void setDefaultAlignment(bool ok);
    QTextDocument* getContextForPrint();//get context for print

    void useFunction();

    void addTipMapItem(int row,int column);
    void finish(const QString &str);
private slots:
    void somethingChanged();
private:
    enum{RECOGNITIONNUMBER = 0x7F51C883};/// set the magicnumber of sheet
    int RowCount,ColumnCount;/// decide the range of a sheet

    Cell* cell(int row,int column) const;
    QString text(int row,int column) const;
    QString formula(int row,int column) const;
    void setFormula(int row,int column,const QString &formula);

    void columnSort(const EasyTableCompare &compare,
                    const QTableWidgetSelectionRange &range);
    void defaultSort(const EasyTableCompare &compare,
                     const QTableWidgetSelectionRange &range);

    void setHeaderItem();

    bool autoRecalc;
    bool autoTip;
    bool tipDirty;
    bool defaultAlignment;
    QTextDocument* context;

    bool autoResize;
    void storeCellSize(int row, int column);
    int Width,Height;
    void cellSizeChange(int row, int column, qreal width, qreal height);
	
/// set the function recognition code
    enum Function{Cancell = 0,Count = 1,Sum = 2,Average = 3};
    Function functionCode;
    int count;
    double sum,average;
    Function getFunctionCode();
    void displayResults();

    void functionCount();
    void functionSum();
    void functionAverage();
    //the functions above change the value of count,sum,average...
};
#endif // EasyTable_H
