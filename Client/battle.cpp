#include "battle.h"
#include "ui_battle.h"
#include <QTest>

const QList<int> expToGain={0,30,80,130,160,
                       200,240,280,320,360,
                       390,430,470,520,560,650};

Battle::Battle(QString name, quint16 port, QList<PokeMon *> allPM, QWidget *parent) :
    QWidget(parent), allPM(allPM),
    ui(new Ui::Battle)
{
    ui->setupUi(this);
    QIcon icon(QPixmap(":/new/prefix1/image/pikachu.png"));
    setWindowIcon(icon);
    socket=new QUdpSocket(this);
    battlePort=port+200;
    this->name=name;
    socket->bind(battlePort);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setWindowTitle("Battle!");
    setWindowModality(Qt::WindowModal);
    hide();
    ui->tableView_chellenges->hide();
    ui->label_type->hide();
    ui->textBrowser_info->hide();
    ui->pushButton_back->hide();
    ui->pushButton_Battle->hide();
    ui->pushButton_skip->hide();
    ui->label_HPA->hide();
    ui->label_HPB->hide();
    ui->progressBar_A->hide();
    ui->progressBar_B->hide();
    ui->label_infoA->hide();
    ui->label_infoB->hide();
    ui->spinBox_level->setMaximum(15);
    ui->spinBox_level->setMinimum(1);
    ui->pushButton_UpTour->setIcon(QIcon(":/new/prefix1/image/upgrade.png"));
    ui->pushButton_UpTour->setIconSize(QSize(64,64));
    ui->pushButton_DuRace->setIcon(QIcon(":/new/prefix1/image/duel.png"));
    ui->pushButton_DuRace->setIconSize(QSize(64,64));
    qsrand(QTime::currentTime().msec());
}

Battle::~Battle()
{
    delete ui;
}

void Battle::sendResult(int id, bool result, int level, PMRarity rarity)
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=SENDRESULT;

    outStream<<type
            <<name
           <<battleType
          <<id
         <<result;

    if(result)//win a race
    {
        int exp = expToGain[level]*((int)rarity+1);
        outStream<<exp;
    }
    else if(battleType==2)   //duel race
    {
        ;
    }

    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void Battle::getTrophy(PokeMon *opponent)
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=GETTROPHY;

    Rename rename;
    if(rename.exec())
        opponent->name=rename.value();

    outStream<<type<<name;
//    outStream<<opponent;
    outStream<<(int)opponent->type<<opponent->name<<opponent->level
            <<opponent->attack<<opponent->defence<<opponent->maxHealth
            <<opponent->speed<<opponent->exp<<(int)opponent->rarity<<(int)opponent->limitBreak<<opponent->id;


    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void Battle::on_pushButton_back_clicked()
{
    ui->pushButton_back->hide();
    ui->tableView_chellenges->hide();
    ui->pushButton_Battle->hide();
    ui->label_HPA->hide();
    ui->label_HPB->hide();
    ui->progressBar_A->hide();
    ui->progressBar_B->hide();
    ui->label_infoA->hide();
    ui->label_infoB->hide();
    ui->textBrowser_info->hide();
    ui->pushButton_skip->hide();
    ui->label_type->hide();
    ui->pushButton_UpTour->show();
    ui->pushButton_DuRace->show();
    ui->comboBox_rarity->show();
    ui->spinBox_level->show();
    ui->pushButton_close->show();

}

void Battle::on_pushButton_UpTour_clicked()
{
    ui->pushButton_back->show();
    ui->tableView_chellenges->show();
    ui->pushButton_Battle->show();
    ui->label_type->setText("Upgrade Tournament");
    this->battleType=1;
    ui->label_type->show();
    ui->pushButton_UpTour->hide();
    ui->pushButton_DuRace->hide();
    ui->comboBox_rarity->hide();
    ui->spinBox_level->hide();
    ui->pushButton_close->hide();
}

void Battle::on_pushButton_DuRace_clicked()
{
    ui->pushButton_back->show();
    ui->tableView_chellenges->show();
    ui->pushButton_Battle->show();
    ui->label_type->setText("Duel Race");
    this->battleType=2;
    ui->label_type->show();
    ui->pushButton_UpTour->hide();
    ui->pushButton_DuRace->hide();
    ui->comboBox_rarity->hide();
    ui->spinBox_level->hide();
    ui->pushButton_close->hide();
}

void Battle::on_pushButton_Battle_clicked()
{
    ui->tableView_chellenges->hide();
    ui->pushButton_Battle->hide();
    ui->pushButton_back->hide();
    ui->pushButton_skip->show();
    ui->textBrowser_info->show();
    ui->label_HPA->show();
    ui->label_HPB->show();
    ui->progressBar_A->show();
    ui->progressBar_B->show();
    ui->label_infoA->show();
    ui->label_infoB->show();
    isSkipped=false;

    int opLevel=ui->spinBox_level->value();
    PMRarity rarity=(PMRarity)ui->comboBox_rarity->currentIndex();
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=GETOPPONENT;
    outStream<<type<<battlePort<<name<<opLevel<<(int)rarity;
    socket->writeDatagram(datagram,serverAddress,serverPort);

    if(socket->waitForReadyRead(500))
    {
        QByteArray receiveData;
        receiveData.resize(socket->pendingDatagramSize());
        socket->readDatagram(receiveData.data(),receiveData.size());

        QDataStream inStream(receiveData);
        PokeMon *opponent;

        inStream>>opponent;

        Fighting fight(self,opponent);
        auto procs=fight.procedure();

        bool result=fight.isVictory();

        sendResult(self->id,result,opponent->level,opponent->rarity);

        auto infos=fight.generateInfo(procs);
        ui->label_HPA->setText(QString("%1").arg((int)self->maxHealth));
        ui->progressBar_A->setMaximum((int)self->maxHealth);
        ui->progressBar_A->setValue((int)self->maxHealth);
        ui->label_HPB->setText(QString("%1").arg((int)opponent->maxHealth));
        ui->progressBar_B->setMaximum((int)opponent->maxHealth);
        ui->progressBar_B->setValue((int)opponent->maxHealth);
        QString init=tr("决斗开始:\n你的HP:%1,对方的HP:%2\n").arg(self->maxHealth).arg(opponent->maxHealth);
        ui->textBrowser_info->append(init);

        quint32 preTime=0;
        for(int i=0;i<procs.length();i++){

            QElapsedTimer t;
            t.start();
            while(t.elapsed()<procs[i].time-preTime&&!isSkipped)
                QCoreApplication::processEvents();

            ui->label_HPA->setText(QString("%1").arg(procs[i].HP_A));
            ui->progressBar_A->setValue(procs[i].HP_A);
            ui->label_HPB->setText(QString("%1").arg(procs[i].HP_B));
            ui->progressBar_B->setValue(procs[i].HP_B);
            ui->textBrowser_info->append(infos[i]);
            preTime=procs[i].time;
        }
        if(!isSkipped)
            on_pushButton_skip_clicked();

        QMessageBox::information(this,"战斗结果",result?"胜利":"失败");

        if(result)//gainExp
        {
            int exp=expToGain[opLevel]*(rarity+1);
            int preLv=self->level;
            self->gainExp(exp);
            QString info=tr("获得经验值%1\n").arg(exp);
            if(self->level>preLv)
                info+=self->getInfomation();
            QMessageBox::information(this,"获得经验值",info);
            if(battleType==2)
            {
                getTrophy(opponent);
            }
        }
        else if(battleType==2)            //lose a dual race
        {

            auto tmpPMs=allPM;
            selectedPM.clear();
            for(int i=0;i<3;i++)
            {
                int key=qrand()%(tmpPMs.length());
                selectedPM.append(tmpPMs[key]);
                tmpPMs.removeAt(key);
                if(tmpPMs.empty())
                    break;
            }

            Death death(selectedPM,this);
            if(death.exec())
            {
                int id=death.value();
                on_killPM(id);
            }
        }
        delete opponent;
    }
    else
    {
        QMessageBox::warning(this,"警告","连接超时");
    }

}

void Battle::on_pushButton_skip_clicked()
{
    if(ui->pushButton_skip->text()=="skip")
    {
        isSkipped=true;
        ui->pushButton_skip->setDisabled(true);
        ui->pushButton_skip->setEnabled(true);
        ui->pushButton_skip->setText("back");
    }
    else
    {
        ui->pushButton_skip->setText("skip");
        ui->textBrowser_info->clear();
        on_pushButton_back_clicked();
        this->hide();
        emit initMonsTable();
    }
}

void Battle::recvPM(PokeMon *src)
{
    self=src;
}

void Battle::on_killPM(int id)
{
    QByteArray datagram;//datagram to send
    QDataStream outStream(&datagram,QIODevice::ReadWrite);
    datagramType type=KILLPM;
    bool newRandom=false;
    if(allPM.length()==1)
        newRandom=true;
    outStream<<type<<id<<name<<newRandom;
    socket->writeDatagram(datagram,serverAddress,serverPort);

}

void Battle::on_pushButton_close_clicked()
{
    close();
}
