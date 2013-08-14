#ifndef EASYTABLECOMPARE_H
#define EASYTABLECOMPARE_H
#include <QStringList>

class EasyTableCompare
{
public:
    bool operator()(const QStringList &row1,const QStringList
                &row2) const;
    bool larger(const QString &a, const QString &b) const;
    bool smaller(const QString &a, const QString &b) const;
    enum{KeyCount = 3};
    int keys[KeyCount];
    bool ascending[KeyCount];
};
#endif // EASYTABLECOMPARE_H
