#include "serveurtcp.h"
#include "ui_serveurtcp.h"

ServeurTCP::ServeurTCP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServeurTCP)
{
    ui->setupUi(this);

        //titre
        this->setWindowTitle("ServeurTCP");

        //btn quiter
        connect(ui->pushButton_Quiter,&QPushButton::clicked, this, &ServeurTCP::close);
        // Si on ferme le serveur avec le bouton quiter
        connect(ui->pushButton_Quiter, &QPushButton::clicked, this, &ServeurTCP::deconnexionServeur);
        //Bouton clear
        connect(ui->pushButton_clear, &QPushButton::clicked, this, &ServeurTCP::clear);


        const unsigned short PORT_SERVEUR = 12345;
        bool listen = socketServeur.listen(QHostAddress::AnyIPv4, PORT_SERVEUR);
        QHostAddress adSrv = socketServeur.serverAddress();
        quint16 port = socketServeur.serverPort();

        if (listen==true) {
            ui->label_serveur_ecoute->setText("Serveur en écoute sur:"+adSrv.toString()+":"+QString::number(port));
            connect(&socketServeur, &QTcpServer::newConnection, this, &ServeurTCP::nouveauClient);
        } else
        {
            ui->label_serveur_ecoute->setText("Erreur connexion:"+socketServeur.errorString());
        }



}

ServeurTCP::~ServeurTCP()
{
    delete ui;
}

void ServeurTCP::nouveauClient()
{

    socketClient = socketServeur.nextPendingConnection();

    QHostAddress adresse =  socketClient->peerAddress();
    quint16 port = socketClient->peerPort();
    connect(socketClient, &QTcpSocket::readyRead, this, &ServeurTCP::recevoirRequete);
    connect(socketClient, &QTcpSocket::disconnected, this, &ServeurTCP::deconnexionClient);

    ui->textEdit->append(QString("Client Connectés avec l'adresse:"+adresse.toString()+" et sur le port:"+QString::number(port)));

}
void ServeurTCP::recevoirRequete()
{
    //QByteArray requete;// recue avec readAll()
    QByteArray reponse; // Envoyé avec write() puis flush
    QByteArray message;

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
void ServeurTCP::deconnexionClient()
{
    socketClient->disconnectFromHost();
    ui->textEdit->append(QString("Client Déconnectés"));
}
void ServeurTCP::deconnexionServeur()
{
    socketServeur.close();
}

void ServeurTCP::clear()
{
 ui->textEdit->clear();
}
