/**
*@file
*store the message for sorting
*it is a functor only used in sort() and its associated functions
*/
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
	/// there are three key words at most
    enum{KeyCount = 3};
    int keys[KeyCount];
	/// if true , order is ascending;else order is descending
    bool ascending[KeyCount];
};
#endif // EASYTABLECOMPARE_H
