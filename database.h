#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDebug>
#include <QDate>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

class DataBase : public QObject
{
    Q_OBJECT
    //service variable
    //access from QML
    Q_PROPERTY(int _barcode READ _barcode WRITE _set_barcode NOTIFY _barcode_changed)
    Q_PROPERTY(QString _name READ _name WRITE _set_name NOTIFY _name_changed)
    Q_PROPERTY(QString _type READ _type WRITE _set_type NOTIFY _type_changed)
    Q_PROPERTY(int _count READ _count WRITE _set_count NOTIFY _count_changed)
    Q_PROPERTY(int _price READ _price WRITE _set_price NOTIFY _price_changed)
    Q_PROPERTY(int _total_price READ _total_price WRITE _set_total_price NOTIFY _total_price_changed)
 //   Q_PROPERTY(QString all_data_session READ all_data_session WRITE _all_data_session NOTIFY all_data_session_changed)


public:
    explicit DataBase(QObject *parent = nullptr);
    //variables to new lines in main database
    int id=0,
        barcode=0;
    QString name = "",
            type = "";
    int count = 0,
        price = 0;
    //service variables
    int _barcode();
    QString _name();
    QString _type();
    int _count();
    int _price();
    int _total_price();

signals:
//service signals
void _barcode_changed();
void _name_changed();
void _type_changed();
void _count_changed();
void _price_changed();
void _total_price_changed();
//void all_data_session_changed();


public slots:
    bool connectToDb();
    bool insertLine(int barcode
                    ,QString name
                    ,QString type
                    ,int count
                    ,int price);

    bool deleteLine(int id);
    bool updateDb();
    bool selectLine(int id);
    //cashbox
    bool addToCashbox(int barcode
                      ,QString name
                      ,QString type
                      ,int count
                      ,int price);
    bool all_price();
    bool set_count(int id_for_set_count, int count_for_set_count);
    bool cashbox_clear();
    bool select_line_from_cashbox();
    //service slots
    void _set_barcode(int new_barcode);
    void _set_name(QString new_name);
    void _set_type(QString new_type);
    void _set_count(int new_count);
    void _set_price(int new_price);
    void _set_total_price(int new_price);
    //login
    bool login_in(QString login, QString password);
    bool new_user(QString login, QString password);
    bool start_work();
    bool end_work();
    bool calculation_current_session();
    //for all data session
    QString all_data_session(QString column);//user, start_time, end_time, start_date, end_date, total
    //internet test

private:

    //QSqlQuery query;
    QSqlDatabase db;
    //service variables
    int _m_barcode;
    QString _m_name;
    QString _m_type;
    int _m_count;
    int _m_price;
    int _m_total_price;
    //
    QString salt = "$2a$12$Q5NV/Xg2OHshZ7CNDc6IZO";//do not change. key to passwords in the database!!!ё
    QString last_user;//used in start_work() n end_work
    int current_session_id;//used in start_work() n end_work



};

#endif // DATABASE_H
