#ifndef LINERS_MODEL_H
#define LINERS_MODEL_H

#include <QAbstractTableModel>
#include <QObject>

class Liners_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Liners_Model(QObject *parent = nullptr);
};

#endif // LINERS_MODEL_H
