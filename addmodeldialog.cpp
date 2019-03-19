#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog(QWidget *parent) :
    QDialog(parent),
    ui_add_model_dialog(new Ui::AddModelDialog)
{
    ui_add_model_dialog->setupUi(this);
    setModal (true);

    model_lb = new QLabel("Модель",this);
    model_te = new QTextEdit(this);
    model_layout = new QHBoxLayout(this);
    model_layout->addWidget (model_lb);
    model_layout->addWidget (model_te);

//    season_lb = new QLabel("Сезон",this);
//    season_te = new QTextEdit(this);
//    season_layout = new QHBoxLayout(this);
//    season_layout->addWidget (season_lb);
//    season_layout->addWidget (season_te);

//    main_layout->addLayout (model_layout);
//    main_layout->addLayout (season_layout);

}

AddModelDialog::~AddModelDialog()
{
    delete ui_add_model_dialog;
}
