#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>


//#include "someclass.h"
#include "database.h"
#include "listmodel.h"
#include "cashbox.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    DataBase main_db;
    ListModel listModel;
    Cashbox cashbox;

    //main_db.selectLine(35);
    //qDebug()<<"select:"<<main_db.name;


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    //QQmlContext *rootContext = engine.rootContext();

    engine.rootContext()->setContextProperty("database", &main_db);
    engine.rootContext()->setContextProperty("listmodel", &listModel);
    engine.rootContext()->setContextProperty("cashbox", &cashbox);
    //rootContext->setContextProperty("classA", &testClass);


    return app.exec();
}
