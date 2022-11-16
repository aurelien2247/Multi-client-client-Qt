#include "clienttcp.h"
#include "ui_clienttcp.h"

ClientTCP::ClientTCP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientTCP)
{
    ui->setupUi(this);

    //titre
    this->setWindowTitle("ClientTCP");

    //btn quiter
    connect(ui->pushButton_Quiter,&QPushButton::clicked, this, &ClientTCP::close);

    // socket pour connexion
    connect(ui->pushButton_connecter, &QPushButton::clicked, this, &ClientTCP::connexion);
    // Fermeture de la socket
    connect(ui->pushButton_deconnecter, &QPushButton::clicked, this, &ClientTCP::deconnexionServeur);
    // bouton envoyer message
    connect(ui->pushButton_envoie_msg, &QPushButton::clicked, this, &ClientTCP::envoyerMessage);
    connect(ui->lineEdit_contenant_msg, &QLineEdit::returnPressed , this, &ClientTCP::envoyerMessage);
    //Afficher status connection
    connect(&socket, &QTcpSocket::stateChanged, this, &::ClientTCP::afficherStatut);

    //Bouton clear
    connect(ui->pushButton_clear, &QPushButton::clicked, this, &ClientTCP::clear);

    //Met les boutons de deconnexion et msg en grisé
    ui->pushButton_deconnecter->setEnabled(false);
    ui->pushButton_envoie_msg->setEnabled(false);

}

ClientTCP::~ClientTCP()
{
    delete ui;
}

void ClientTCP::recevoirMessage()
{
    QByteArray message=socket.readAll();

    ui->textEdit_log->append(QString(message));//Afficher dans l'ihm

}
void ClientTCP::envoyerMessage(){
    //recupere la valeur du message a envoyer dans la variable message
    QString message=ui->lineEdit_contenant_msg->text();

    //Regarde la socket de communication et regarde son etat
    if (socket.state()){
        socket.write(message.toUtf8());
        socket.flush();
        //Netoyage du QLineEdit et focus apres l'envoie
        ui->lineEdit_contenant_msg->clear();
        ui->lineEdit_contenant_msg->setFocus();

    } else {
        ui->textEdit_log->append("Probleme de connexion avec le serveur !");
    }

}

void ClientTCP::deconnexionServeur()
{
    socket.disconnectFromHost();
    //Met les boutons de deconnexion et msg en grisé et active le bouton de connexion
    ui->pushButton_connecter->setEnabled(true);
    ui->pushButton_deconnecter->setEnabled(false);
    ui->pushButton_envoie_msg->setEnabled(false);
}
void ClientTCP::connexion(){
    //QString adresseServeur = "127.0.0.1"; //Adresse IP ou nom DNS
    //unsigned short PORT_SERVEUR = 12345;
    //Récupere les valeurs qu'entre l'utilisateur
    QString adresseServeur = ui->lineEdit_ip->text();
    QString port_text = ui->lineEdit_port->text();

    socket.connectToHost(adresseServeur, port_text.toUShort());
    connect(&socket, &QTcpSocket::readyRead, this, &ClientTCP::recevoirMessage);

    //Attend et vérifie la connexion d'un client sur un serveur
    if (socket.waitForConnected(5000)){
        QHostAddress adresseClient = socket.localAddress();
        unsigned short portClient = socket.localPort();

        ui->textEdit_log->append("Client Connecté avec "+adresseClient.toString()+":"+QString::number(portClient));
        ui->pushButton_connecter->setEnabled(false);
        ui->pushButton_deconnecter->setEnabled(true);
        ui->pushButton_envoie_msg->setEnabled(true);
    }
    else {
        ui->textEdit_log->append("ERREUR connexion :"+socket.errorString());

    }
}
void ClientTCP::clear(){
    //Nettoie les logs
    ui->textEdit_log->clear();
}

void ClientTCP::afficherStatut()
{
    socket.state();
    unsigned short index = socket.state();
    QString tabState[7]={"UnconnectedState", "HostLookupState", "ConnectingState", "ConnectedState", "BoundState", "ListeningState", "ClossingState"};
    ui->textEdit_log->append(QString::number(index) +":" +tabState[index]);
}
