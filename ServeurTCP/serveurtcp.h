#ifndef SERVEURTCP_H
#define SERVEURTCP_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDateTime>
#include <QProcess>
#include <QHostAddress>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ServeurTCP; }
QT_END_NAMESPACE

class ServeurTCP : public QMainWindow
{
    Q_OBJECT
    QTcpServer socketServeur;
    QTcpSocket* socketClient;
    QList<QTcpSocket*> listeSocketsClients;


public:
    ServeurTCP(QWidget *parent = nullptr);
    ~ServeurTCP();

private:
    Ui::ServeurTCP *ui;

private slots:
    void nouveauClient();
    void recevoirRequete();
    void deconnexionClient();
    void deconnexionServeur();
    void clear();
};
#endif // SERVEURTCP_H
