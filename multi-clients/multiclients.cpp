#include "multiclients.h"
#include "ui_multiclients.h"

#include <QApplication>

multiClients::multiClients(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::multiClients)
{
    ui->setupUi(this);
    //titre
    this->setWindowTitle("Serveur multi-clients TCP");

    //btn quiter
    connect(ui->pushButton_Quiter,&QPushButton::clicked, this, &multiClients::close);
    // Si on ferme le serveur avec le bouton quiter
    connect(ui->pushButton_Quiter, &QPushButton::clicked, this, &multiClients::deconnexionServeur);
    //Bouton clear
    connect(ui->pushButton_clear, &QPushButton::clicked, this, &multiClients::clear);
    //Bouton diffuser message
    connect(ui->pushButton_dif_msg, &QPushButton::clicked, this, &multiClients::difusermsg);


    const unsigned short PORT_SERVEUR = 12345;
    bool listen = socketServeur.listen(QHostAddress::AnyIPv4, PORT_SERVEUR);
    QHostAddress adSrv = socketServeur.serverAddress();
    quint16 port = socketServeur.serverPort();

    if (listen==true) {
        ui->label_serveur_ecoute->setText("Serveur en écoute sur:"+adSrv.toString()+":"+QString::number(port));
        connect(&socketServeur, &QTcpServer::newConnection, this, &multiClients::nouveauClient);
    } else
    {
        ui->label_serveur_ecoute->setText("Erreur connexion:"+socketServeur.errorString());
    }
    //mettre en gris le bouton quand il y a 0 clients
    ui->pushButton_dif_msg->setEnabled(false);

}

multiClients::~multiClients()
{
    delete ui;
}


void multiClients::nouveauClient()
{
    QTcpSocket* socketNouveauClient;

    socketNouveauClient = socketServeur.nextPendingConnection();
    listeSocketsClients.append(socketNouveauClient);

    QHostAddress adresse =  socketNouveauClient->peerAddress();
    quint16 port = socketNouveauClient->peerPort();
    connect(socketNouveauClient, &QTcpSocket::readyRead, this, &multiClients::recevoirRequete);
    connect(socketNouveauClient, &QTcpSocket::disconnected, this, &multiClients::deconnexionClient);
    //Afficher le bouton quand il y a au moins 1 client
    ui->pushButton_dif_msg->setEnabled(true);
    ui->textEdit->append(QString("Client Connectés avec l'adresse:"+adresse.toString()+" et sur le port:"+QString::number(port)));

}
void multiClients::recevoirRequete()
{
    //QByteArray requete;// recue avec readAll()
    QByteArray reponse; // Envoyé avec write() puis flush
    QByteArray message;
    QTcpSocket* socketClient = qobject_cast<QTcpSocket*>(sender());




    message=socketClient->readAll();//Lire le message recu
    if (socketClient->state()){
        ui->textEdit->append(QString(message));//utiliser le message du client
        socketClient->flush();

        if (message=="time"||message=="time\r\n"){
            QDateTime dt=QDateTime::currentDateTime();
            reponse=dt.toString("dd/MM/yyyy HH:mm:ss").toUtf8();
            socketClient->write(message.toUpper()+"\n");
            socketClient->write(reponse);

        }
        else if (message=="cpu"||message=="cpu\r\n"){
            QProcess proc;
            proc.start("lireInfoOS.sh",QStringList("cpu"));
            proc.waitForFinished();
            reponse="Charge cpu ="+proc.readAll();
            socketClient->write(message.toUpper()+"\n");
            socketClient->write(reponse);

        }

        else if (message=="tasks"||message=="tasks\r\n"){
            QProcess tasks;
            tasks.start("lireInfoOS.sh",QStringList("tasks"));
            tasks.waitForFinished();
            reponse="tasks ="+tasks.readAll();
            socketClient->write(message.toUpper()+"\n");
            socketClient->write(reponse);
        }
        else if (message=="fin"||message=="fin\r\n"){
            socketClient->write(message.toUpper()+"\n");
            socketClient->disconnectFromHost();

        }
        else {
            socketClient->write("Commande Invalide: "+message.toUpper());

        }

    } else {
        ui->textEdit->append("Probleme de connexion avec le Client !");
    }

}
void multiClients::deconnexionClient()
{
    QTcpSocket* socketClient = qobject_cast<QTcpSocket*>(sender());

    socketClient->disconnectFromHost();
    ui->textEdit->append(QString("Client Déconnectés"));
    listeSocketsClients.removeAll(socketClient);
    //Si la deconnexion est le dernier clients on met en gris le bouton de diffusion de message
    int nbClient = listeSocketsClients.size();
    if(nbClient==0){
        ui->pushButton_dif_msg->setEnabled(false);
    }

}
void multiClients::deconnexionServeur()
{
    socketServeur.close();
}

void multiClients::clear()
{
 ui->textEdit->clear();
}

void multiClients::difusermsg(){
    int nbClient = listeSocketsClients.size();
    for (int i = 0; i < nbClient; ++i) {
        // s'il y a seulement une personne de connecté
        if(nbClient==1){
          listeSocketsClients[i]->write("Vous êtes le seul cient connecté ! (nombre de socket: "+QByteArray::number(nbClient)+")");
        }
        // s'il y a plusieurs personnes de connectés
        else {
            listeSocketsClients[i]->write("Il y a: "+QByteArray::number(nbClient)+" clients connectés");
        }
    }
}
