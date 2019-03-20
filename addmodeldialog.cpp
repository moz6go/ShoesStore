#include "addmodeldialog.h"
#include "ui_addmodeldialog.h"

AddModelDialog::AddModelDialog(QWidget *parent) :
    QDialog(parent),
    ui_add_model_dialog(new Ui::AddModelDialog)
{
    ui_add_model_dialog->setupUi(this);
    setModal (true);

    /*model_lb = new QLabel ("Модель",this);
    model_le = new QLineEdit (this);
    model_lb->setFixedWidth (LABEL_WID);
    model_le->setFixedWidth (LINE_WID);
    model_layout = new QHBoxLayout();
    model_layout->addWidget (model_lb);
    model_layout->addWidget (model_le);

    season_lb = new QLabel("Сезон",this);
    season_le = new QLineEdit(this);
    season_lb->setFixedWidth (LABEL_WID);
    season_le->setFixedWidth (LINE_WID);
    season_layout = new QHBoxLayout();
    season_layout->addWidget (season_lb);
    season_layout->addWidget (season_le);

    brand_lb = new QLabel("Виробник",this);
    brand_le = new QLineEdit(this);
    brand_lb->setFixedWidth (LABEL_WID);
    brand_le->setFixedWidth (LINE_WID);
    brand_layout = new QHBoxLayout();
    brand_layout->addWidget (brand_lb);
    brand_layout->addWidget (brand_le);

    wholesaleprice_lb = new QLabel("Ціна оптова, грн",this);
    wholesaleprice_le = new QLineEdit(this);
    wholesaleprice_le->setValidator (&PRICE_VALIDATOR);
    wholesaleprice_lb->setFixedWidth (LABEL_WID);
    wholesaleprice_le->setFixedWidth (LINE_WID);
    wholesaleprice_layout = new QHBoxLayout();
    wholesaleprice_layout->addWidget (wholesaleprice_lb);
    wholesaleprice_layout->addWidget (wholesaleprice_le);

    retailprice_lb = new QLabel("Ціна роздрібна, грн",this);
    retailprice_le = new QLineEdit(this);
    retailprice_le->setValidator (&PRICE_VALIDATOR);
    retailprice_lb->setFixedWidth (LABEL_WID);
    retailprice_le->setFixedWidth (LINE_WID);
    retailprice_layout = new QHBoxLayout();
    retailprice_layout->addWidget (retailprice_lb);
    retailprice_layout->addWidget (retailprice_le);

    main_layout = new QVBoxLayout(this);
    main_layout->addLayout (model_layout);
    main_layout->addLayout (season_layout);
    main_layout->addLayout (brand_layout);
    main_layout->addLayout (wholesaleprice_layout);
    main_layout->addLayout (retailprice_layout);

    setLayout (main_layout);*/

    ui_add_model_dialog->retailpr_le->setValidator (&PRICE_VALIDATOR);
    ui_add_model_dialog->wholesalepr_le->setValidator (&PRICE_VALIDATOR);
}

AddModelDialog::~AddModelDialog()
{
    delete ui_add_model_dialog;
}
