#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include "general.h"
#include "database.h"

namespace Ui {
class DictionaryDialog;
}

class DictionaryDialog : public QDialog
{
    Q_OBJECT
    Ui::DictionaryDialog *ui_dict;
    DataBase* sdb;
    QString dict_table;
    QString col;
    QSqlQueryModel* query_model;
private slots:
    void EnableAddButton();
    void EnableDelButton ();
    void AddButtonClicked();
    void DelButtonClicked();
    void DictTypeChanged(int indx);
    void UpdateDictView(QString text);
public:
    explicit DictionaryDialog(DataBase* data_base, QWidget *parent = nullptr);
    ~DictionaryDialog();

};

#endif // DICTIONARYDIALOG_H
