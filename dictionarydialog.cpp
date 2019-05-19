#include "dictionarydialog.h"
#include "ui_dictionarydialog.h"


DictionaryDialog::DictionaryDialog(DataBase* data_base, QWidget *parent) :
    QDialog(parent),
    ui_dict(new Ui::DictionaryDialog)
{
    ui_dict->setupUi(this);
    setModal (true);
    sdb = data_base;
    dict_table = BRANDS_TABLE;
    col = BRAND;

    query_model = new QSqlQueryModel(this);
    ui_dict->add_pb->setDisabled (true);
    ui_dict->del_pb->setDisabled (true);
    ui_dict->warning_lbl->setVisible (false);
    ui_dict->dict_cb->addItems (DICTIONARIES);
    UpdateDictView(DICTIONARIES[0]);

    QObject::connect (ui_dict->dict_cb, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DictionaryDialog::DictTypeChanged);
    QObject::connect (ui_dict->dict_cb, &QComboBox::currentTextChanged, this, &DictionaryDialog::UpdateDictView);
    QObject::connect (ui_dict->new_value_le, &QLineEdit::textChanged, this, &DictionaryDialog::EnableAddButton);
    QObject::connect (ui_dict->add_pb, &QPushButton::clicked, this, &DictionaryDialog::AddButtonClicked);
    QObject::connect (ui_dict->cancel_pb, &QPushButton::clicked, this, &DictionaryDialog::reject);
    QObject::connect (ui_dict->del_pb, &QPushButton::clicked, this, &DictionaryDialog::DelButtonClicked);
    QObject::connect (ui_dict->dict_view, &QTableView::clicked, this, &DictionaryDialog::EnableDelButton);
}

void DictionaryDialog::EnableAddButton() {
    bool text_ok = !ui_dict->new_value_le->text ().isEmpty ();
    bool model_ok = !sdb->SelectCount (dict_table, col, "=", ui_dict->new_value_le->text ());

    if(text_ok && model_ok) {
        ui_dict->warning_lbl->setVisible (false);
        ui_dict->add_pb->setEnabled (true);
    }
    else if (model_ok) {
        ui_dict->warning_lbl->setVisible (false);
        ui_dict->add_pb->setDisabled (true);
    }
    else {
        ui_dict->warning_lbl->setVisible (true);
        ui_dict->add_pb->setDisabled (true);
    }
}

void DictionaryDialog::EnableDelButton() {
    ui_dict->del_pb->setEnabled (true);
}

void DictionaryDialog::AddButtonClicked() {
    QVariantList data = QVariantList() << ui_dict->new_value_le->text ();
    QStringList columns = QStringList() << col;
    sdb->InsertDataIntoTable (sdb->GenerateInsertQuery (dict_table, columns),
                            sdb->GenerateBindValues (columns),
                            data);
    UpdateDictView(ui_dict->dict_cb->currentText ());
    ui_dict->new_value_le->clear ();
}

void DictionaryDialog::DelButtonClicked() {
    if(sdb->SelectCount (MODELS_TABLE,
                         col,
                         "=",
                         query_model->data (query_model->index (ui_dict->dict_view->currentIndex ().row (), 0)).toString ())) {
        QMessageBox::warning (this, "Помилка!", "Неможливо видалити, є наявні моделі зв'язані з цим записом!");
    }
    else{
        QString to_delete =  query_model->data (query_model->index (ui_dict->dict_view->currentIndex ().row (), 0)).toString ();

        sdb->DeleteRow (dict_table, col, to_delete);
        UpdateDictView(ui_dict->dict_cb->currentText ());
        ui_dict->new_value_le->clear ();
    }
}

void DictionaryDialog::DictTypeChanged(int indx){
    switch (indx) {
    case DictTypes::BRANDS:
        dict_table = BRANDS_TABLE;
        col = BRAND;
        break;
    case DictTypes::SEASONS:
        dict_table = SEASONS_TABLE;
        col = SEASON;
        break;
    case DictTypes::CATEGORIES:
        dict_table = CATEGORIES_TABLE;
        col = CATEGORY;
    }
}

void DictionaryDialog::UpdateDictView(QString text) {
    query_model->setQuery ("SELECT * FROM " + dict_table);
    query_model->setHeaderData (0, Qt::Horizontal, text);
    ui_dict->dict_view->verticalHeader ()->setDefaultSectionSize (18);
    ui_dict->dict_view->verticalHeader()->setVisible(false);
    ui_dict->dict_view->resizeColumnsToContents();
    ui_dict->dict_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_dict->dict_view->horizontalHeader()->setStretchLastSection(true);
    ui_dict->dict_view->horizontalHeader ()->resizeSections (QHeaderView::ResizeToContents);
    ui_dict->dict_view->setSortingEnabled (true);
    ui_dict->dict_view->setModel (query_model);
    ui_dict->dict_view->show ();
}

DictionaryDialog::~DictionaryDialog() {
    delete ui_dict;
}
