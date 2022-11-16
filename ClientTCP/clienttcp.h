#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientTCP; }
QT_END_NAMESPACE

class ClientTCP : public QMainWindow
{
    Q_OBJECT
    QTcpSocket socket;



public:
    ClientTCP(QWidget *parent = nullptr);
    ~ClientTCP();

private:
    Ui::ClientTCP *ui;
private slots:
    void recevoirMessage();
    void deconnexionServeur();
    void envoyerMessage();
    void connexion();
    void clear();
    void afficherStatut();

};
#endif // CLIENTTCP_H
