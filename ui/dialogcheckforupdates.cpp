#include "dialogcheckforupdates.h"
#include "ui_dialogcheckforupdates.h"

#include "EnsembleAPI.h"

using namespace std;

dialogcheckforupdates::dialogcheckforupdates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogcheckforupdates)
{
    ui->setupUi(this);

    string cur_version = Ensemble_Update_Get_Current_Version();
    QString qstr_cur_version = "Current ver: " + QString(cur_version.c_str());
    ui->label_curVersion->setText(qstr_cur_version);
}

dialogcheckforupdates::~dialogcheckforupdates()
{
    delete ui;
}

void dialogcheckforupdates::on_pushButton_Checkforupdates_clicked()
{
    string version_strings = Ensemble_Update_Get_Version_List();
    vector<string> version_list = split(version_strings, '/');
    ui->comboBox_versionList->clear();

    for( int i = 0 ; i < version_list.size() ; i++ )
    {
        QString version_name = version_list[i].c_str();
        ui->comboBox_versionList->addItem(version_name);
    }
}

void dialogcheckforupdates::on_pushButton_Apply_Restart_clicked()
{
    QString select_Version = ui->comboBox_versionList->currentText();
    if( select_Version == "" )
    {
        QMessageBox msgBox;
        msgBox.setText("Version name is empty");
        msgBox.exec();
    }
    else
    {
        Ensemble_Update_Set_Version(select_Version.toStdString());
        this->close();
    }
}

vector<string> dialogcheckforupdates::split(string str, char delimiter)
{
    vector<string> internal;
    stringstream ss(str);
    string temp;

    while (getline(ss, temp, delimiter)) {
        internal.push_back(temp);
    }   
    return internal;
}

