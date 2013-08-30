/**
*@file
*a special tableWidgetItem for EasyTable
*/
#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem
{
//    Q_OBJECT
public:
    explicit Cell();
    QTableWidgetItem *clone() const;

    void setData(int role, const QVariant &value);
    QVariant data(int role) const;
    void setDirty();

    void setFormula(const QString& formula);
    QString formula() const;

    void setDefaultAlignment(bool ok = false);
private:
	/// store the value of Cell
    QVariant value() const;

    QVariant evalExpression(const QString& str,int& pos) const;
    QVariant evalTerm(const QString& str,int& pos) const;
    QVariant evalFactor(const QString& str,int& pos) const;

    mutable QVariant cachedValue;
	/// show whether the value of Cell has been changed
    mutable bool cacheIsDirty;
	/// store whether defaultAlignment is used
    bool defaultAlignment;
signals:

public slots:
    
};

#endif // CELL_H
