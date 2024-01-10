#pragma once

#include <QObject>
#include "QCompleter"

class custom_q_completer : public QCompleter
{
    Q_OBJECT
public:
    explicit custom_q_completer(QObject *parent = nullptr);
    static const int CompleteRole;
    QString get_foo_token_number_for_selected(const QModelIndex &index) const;
    virtual QString pathFromIndex(const QModelIndex &index) const;

signals:

};
