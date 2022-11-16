#ifndef MULTICLIENTS_H
#define MULTICLIENTS_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDateTime>
#include <QProcess>
#include <QHostAddress>
#include <QAbstractSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class multiClients; }
QT_END_NAMESPACE

class multiClients : public QMainWindow
{
    Q_OBJECT
    QTcpServer socketServeur;
    //QTcpSocket* socketClient;
    QList<QTcpSocket*> listeSocketsClients;


public:
    multiClients(QWidget *parent = nullptr);
    ~multiClients();

private:
    Ui::multiClients *ui;

private slots:
    void nouveauClient();
    void recevoirRequete();
    void deconnexionClient();
    void deconnexionServeur();
    void clear();
    void difusermsg();
};
#endif // MULTICLIENTS_H
