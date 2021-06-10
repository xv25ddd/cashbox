#include "database.h"
#include "qtbcrypt.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    if(connectToDb()){
        updateDb();
    }
    all_price();
}


bool DataBase::connectToDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./database.db");
    if(!db.open()){
        qDebug()<<"db not opened";
        return 0;
    }
    else{
        qDebug()<<"db opened";
        return 1;
    }

}

//insert line in main
bool DataBase::insertLine(int barcode
                          ,QString name
                          ,QString type
                          ,int count
                          ,int price)
{
    QSqlQuery query;
    if(!query.prepare("INSERT INTO main (barcode, name, type, count, price) "
                     "VALUES (:barcode, :name, :type, :count, :price)")
    )return 0;
    query.bindValue(":barcode", barcode);
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":count", count);
    query.bindValue(":price", price);
    query.exec();
    return 1;
}//end insert line

//delete line in main table
bool DataBase::deleteLine(int id)
{
   QSqlQuery query;
   QString _id = QString::number(id);
   if(!query.exec("DELETE FROM main WHERE id="+_id+";"))return 0;
   else return 1;

}

bool DataBase::updateDb()
{
    QSqlQuery query;
    if(!query.exec("SELECT barcode, name, type, count, price FROM main")){
        qDebug()<<"not working";
        return 0;
    }

    while(query.next()){
        QString barcode = query.value(0).toString();
        QString name = query.value(1).toString();
        QString type = query.value(2).toString();
        QString count = query.value(3).toString();
        qDebug()<<"============";
        qDebug()<<"=main table=";
        qDebug()<<"============";
        qDebug()<<barcode;
        qDebug()<<name;
        qDebug()<<type;
        qDebug()<<count;
        qDebug()<<"=================";
    }
    return 1;
}

//select line from main table
bool DataBase::selectLine(int id)
{
    QString _id = QString::number(id);
    QSqlQuery query;
    if(!query.exec("SELECT id, barcode, name, type, count, price FROM main WHERE id="+_id)){
            qDebug()<<"not working select_line";
            return 0;
        }

    query.next();
    id = query.value(0).toInt();
    _set_barcode(query.value(1).toInt());
    _set_name(query.value(2).toString());
    _set_type(query.value(3).toString());
    _set_count(query.value(4).toInt());
    _set_price(query.value(5).toInt());
    //qDebug()<<barcode;
    return 1;
}

bool DataBase::addToCashbox(int barcode
                            ,QString name
                            ,QString type
                            ,int count
                            ,int price)
  {
      int total_price = 0;
      QSqlQuery query;
      if(!query.prepare("INSERT INTO cashbox (barcode, name, type, count, price, total_price) "
                       "VALUES (:barcode, :name, :type, :count, :price, :total_price)")
      )return 0;
      query.bindValue(":barcode", barcode);
      query.bindValue(":name", name);
      query.bindValue(":type", type);
      query.bindValue(":count", 0);
      query.bindValue(":price", price);
      query.bindValue(":total_price", total_price);
      query.exec();
      return 1;
}

//update green number bottom left
bool DataBase::all_price()
{
    QSqlQuery query;
    if(!query.exec("SELECT SUM(total_price) FROM cashbox")){
        qDebug()<<"error all price";
    }

    query.next();
    //_m_total_price = 0;
    _set_total_price(query.value(0).toInt());
    return 1;
}

//set count in cashbox
bool DataBase::set_count(int id_for_set_count, int count_for_set_count)
{
    QSqlQuery query;
    QString _id_in_set_count, _count_in_set_count;
    _id_in_set_count = QString::number(id_for_set_count);
    _count_in_set_count = QString::number(count_for_set_count);

    if(!query.exec("UPDATE cashbox set count="+_count_in_set_count+" WHERE id="+_id_in_set_count))
    {
        qDebug()<<"error";
        return 0;
    }
    else{
        query.exec("UPDATE cashbox set total_price=count*price WHERE id="+_id_in_set_count);

        return 1;
    }

}

bool DataBase::cashbox_clear()
{
    QSqlQuery query;
    if(!query.exec("DELETE FROM cashbox")){
        qDebug()<<"error clear cashbox";
    }
    return 1;
}

bool DataBase::select_line_from_cashbox()
{
    QString _id = QString::number(id);
    QSqlQuery query;
    if(!query.exec("SELECT name, count, price, total_price FROM cashbox WHERE id="+_id)){
            qDebug()<<"not working select_line";
            return 0;
        }
    query.next();
    _set_name(query.value(0).toString());
    _set_count(query.value(1).toInt());
    _set_price(query.value(2).toInt());
    _set_total_price(query.value(3).toInt());
    return 1;
}

/////////////////////////////




bool DataBase::login_in(QString login, QString password)
{
    QSqlQuery query;
    QString hashedPassword = QtBCrypt::hashPassword(password, salt);
    last_user = login;
    if(!query.exec("SELECT login, password FROM users")){
            qDebug()<<"not working login";
            return 0;
        }

//for some reason it was not possible to request by login. manual search is used

    while(query.next()){
        if(query.value(0).toString() == login){
            break;
        }
    }
    if(query.value(1).toString() != hashedPassword){
        return 0;
    }
    else{
        last_user = login;
        start_work();
        return 1;
    }
}

bool DataBase::new_user(QString login, QString password)
{
    QSqlQuery query;
    password = QtBCrypt::hashPassword(password, salt);
    if(!query.prepare("INSERT INTO users (login, password)"
                   "VALUES (:login, :password)")){
        return 0;
    }
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    if(query.exec()){
        return 1;
    }
    else{
      return 0;
    }
}

//open session
bool DataBase::start_work()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QSqlQuery query;
    query.prepare("INSERT INTO service (user, start_work_time, start_work_date, total_session)"
                  "VALUES (:user, :start_work_time, :start_work_date, :total_session)");
    query.bindValue(":user", last_user);
    query.bindValue(":start_work_time", time);
    query.bindValue(":start_work_date", date);
    query.bindValue(":total_session", 0);
    if(query.exec()){
        query.exec("SELECT id FROM service");
        query.last();
        current_session_id = query.value(0).toInt();
        qDebug()<<"===============";
        qDebug()<<"start work";
        qDebug()<<current_session_id;
        qDebug()<<"===============";
        return 1;
    }
    else return 0;
}

//close session
bool DataBase::end_work()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    QSqlQuery query;
    query.prepare("UPDATE service set end_work_time=:time, end_work_date=:date WHERE id=:_id");

    query.bindValue(":time", time);
    query.bindValue(":date", date);
    query.bindValue(":_id", current_session_id);
    qDebug()<<"===============";
    qDebug()<<"end work";
    qDebug()<<current_session_id;
    qDebug()<<"===============";
    if(query.exec())return 1;
    else return 0;
}

//sums up every check
bool DataBase::calculation_current_session()
{
    QSqlQuery query;

    query.prepare("UPDATE service set total_session=(:total_price+total_session) WHERE id=:_id");
    query.bindValue(":total_price", _total_price());
    query.bindValue(":_id", current_session_id);
    qDebug()<<"===============";
    qDebug()<<"calculation_current_session";
    qDebug()<<current_session_id;
    qDebug()<<"===============";
    if(query.exec())return 1;
    else return 0;
}

QString DataBase::all_data_session(QString column)//user, start_time, end_time, start_date, end_date, total
{
    //SELECT login, password FROM users
    QSqlQuery query;
    query.prepare("SELECT user, start_work_time, end_work_time, start_work_date, end_work_date, total_session FROM service WHERE id=:_id");
    query.bindValue(":_id", current_session_id);
    qDebug()<<"===============";
    qDebug()<<"all data session";
    qDebug()<<current_session_id;
    qDebug()<<"===============";
    query.exec();
    query.next();
    //query.value(0).toString()
    if(column == "user"){
        return query.value(0).toString();
    }
    else if(column == "start_time"){
        return query.value(1).toString();
    }
    else if(column == "end_time"){
        return query.value(2).toString();
    }
    else if(column == "start_date"){
        return query.value(3).toString();
    }
    else if(column == "end_date"){
        return query.value(4).toString();
    }
    else if(column == "total"){
        return query.value(5).toString();
    }
}





///////////
//service//
///////////
void DataBase::_set_barcode(int new_barcode)
{
    if(_m_barcode!=new_barcode){
        _m_barcode = new_barcode;
        emit _barcode_changed();
    }
}

void DataBase::_set_name(QString new_name)
{
    if(_m_name!=new_name){
        _m_name = new_name;
        emit _name_changed();
    }
}

void DataBase::_set_type(QString new_type)
{
    if(_m_type!=new_type){
        _m_type = new_type;
        emit _type_changed();
    }
}

void DataBase::_set_count(int new_count)
{
    if(_m_count!=new_count){
        _m_count = new_count;
        emit _count_changed();
    }
}

void DataBase::_set_price(int new_price)
{
    if(_m_price!=new_price){
        _m_price = new_price;
        emit _price_changed();
    }
}
void DataBase::_set_total_price(int new_total_price){
    if(_m_total_price!=new_total_price){
        _m_total_price = new_total_price;
        emit _total_price_changed();
    }
}


int DataBase::_barcode()
{
    return _m_barcode;
}

QString DataBase::_name()
{
    return _m_name;
}

QString DataBase::_type()
{
    return _m_type;
}

int DataBase::_count()
{
    return _m_count;
}

int DataBase::_price()
{
    return _m_price;
}

int DataBase::_total_price()
{
    return _m_total_price;
}
///////////////
//end service//
///////////////
//_total_price
//query.exec("UPDATE cashbox set total_price=count*price WHERE id="+_id_in_set_count);

/*
if(!query.prepare("INSERT INTO main (barcode, name, type, count, price) "
                 "VALUES (:barcode, :name, :type, :count, :price)")
)return 0;
query.bindValue(":barcode", barcode);
*/
/*
if(!query.exec("SELECT name, count, price, total_price FROM cashbox WHERE id="+_id)){
        qDebug()<<"not working select_line";
        return 0;
    }
*/
////////////////////







//end insert line
