#include "crestclient.h"

CRESTClient::CRESTClient(QObject *parent) : QObject(parent)
{

}

QString CRESTClient::getCharacterName()
{
    if (accessCode_ == "") {
        return "";
    }
    return "TyounanMOTI";
}
