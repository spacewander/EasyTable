/**
 *@file
 * find action controller
 */
#ifndef FIND_H
#define FIND_H

class EasyTable;
#include <QObject>

class Find : public QObject
{
    Q_OBJECT
public:
    explicit Find(EasyTable *parent = 0);
    
    bool isAble() const;
signals:
    void matched();
public slots:
    void findNext(const QString &str,Qt::CaseSensitivity cs);
    void findPrevious(const QString &str,Qt::CaseSensitivity cs);
    void findInAll(const QString &str,Qt::CaseSensitivity cs);
    void findFromHere(const QString &str,Qt::CaseSensitivity cs);

    void findNext(const QRegExp &re);
    void findPrevious(const QRegExp &re);
    void findInAll(const QRegExp &re);
    void findFromHere(const QRegExp &re);

    void replaceSelectedCell(const QString&str);

private:
    EasyTable *parent;

    void didFind(int row, int column);
    bool _find(int row,int column,const QString &str, Qt::CaseSensitivity cs);
    bool _find(int row, int column, const QRegExp &re);
};

#endif // FIND_H
