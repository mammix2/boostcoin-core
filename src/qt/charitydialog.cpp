#include "charitydialog.h"
#include "ui_charitydialog.h"

#include "clientmodel.h"
#include "notificator.h"
#include "version.h"

#include "wallet.h"
#include "walletdb.h"
#include "main.h"
#include "init.h"

#include <QMessageBox>

charityDialog::charityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::charityDialog)
{
    ui->setupUi(this);
}

charityDialog::~charityDialog()
{
    delete ui;
}

void charityDialog::setModel(ClientModel *model)
{

}

void charityDialog::on_buttonBox_accepted()
{
    QMessageBox msgBox;
    CBitcoinAddress address = ui->charityAddressEdit->text().toStdString();
    QString str = ui->charityPercentEdit->text();
    bool fIntConversion;
    unsigned int nCharityPercent = str.toInt(&fIntConversion, 10);


                   if (!address.IsValid())
                   {
                       msgBox.setText("Invalid BOST address");
                       msgBox.exec();
                       return;
                   }
                   if (nCharityPercent < 1)
                   {
                        msgBox.setText("Invalid parameter, expected valid percentage");
                        msgBox.exec();
                        return;
                   }
                   if (pwalletMain->IsLocked())
                   {
                       msgBox.setText("Error: Please enter the wallet passphrase with walletpassphrase first.");
                       msgBox.exec();
                       return;
                   }

                   //Turn off if we set to zero.
                   //Future: After we allow multiple addresses, only turn of this address
                   if(nCharityPercent == 0)
                   {
                       pwalletMain->fStakeForCharity = false;
                       pwalletMain->StakeForCharityAddress = "";
                       pwalletMain->nStakeForCharityPercent = 0;

                       msgBox.setText("0 Percent Selected, void");
                       msgBox.exec();
                       return;
                   }


                   //For now max percentage is 50.
                   if (nCharityPercent > 50 )
                      nCharityPercent = 50;

                   pwalletMain->StakeForCharityAddress = address;
                   pwalletMain->nStakeForCharityPercent = nCharityPercent;
                   pwalletMain->fStakeForCharity = true;

                   msgBox.setText("Stake For Charity Set");
                   msgBox.exec();
}

void charityDialog::on_buttonBox_rejected()
{
    close();
}

void charityDialog::on_pushButton_clicked()
{
    QMessageBox msgBox;
    pwalletMain->fStakeForCharity = false;
    pwalletMain->StakeForCharityAddress = "";
    pwalletMain->nStakeForCharityPercent = 0;

    msgBox.setText("Stake For Charity Disabled");
    msgBox.exec();
}

