#include "custom_q_completer.h"
const int custom_q_completer::CompleteRole = Qt::UserRole + 1;

custom_q_completer::custom_q_completer(QObject *parent)
    : QCompleter{parent}
{

}

QString custom_q_completer::pathFromIndex(const QModelIndex &index) const{
    return index.data().toString();
    //return index.data(CompleteRole).toString(); //if you want to dsiplay value instead text enable this
}

QString custom_q_completer::get_foo_token_number_for_selected(const QModelIndex &index) const
{
    QString code = index.data(CompleteRole).toString();
    return code;
}
