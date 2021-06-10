import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQml 2.2
import QtQuick.Controls 1.4


Window {
    id: main_window
    width: 640
    height: 480
    visibility: Window.Maximized

    //top panel for creating new records
    RowLayout{
        id: rowLayout
        visible: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
        spacing: 4
        Text {text: qsTr("barcode")}
        TextField {id: barcode ;width: 70}
        Text {text: qsTr("name")}
        TextField {id: name ;width: 70}
        Text {text: qsTr("type")}
        TextField {id: type ;width: 70}
        Text {text: qsTr("count")}
        TextField {id: count ;width: 70}
        Text {text: qsTr("price")}
        TextField {id: price ;width: 70}

        Button{
            text: qsTr("add")

            onClicked:{
                database.insertLine(barcode.text
                                    , name.text
                                    , type.text
                                    , count.text
                                    , price.text);
                listmodel.updateModel();
                barcode.text = "";
                name.text = "";
                type.text = "";
                count.text = "";
                price.text = "";
            }
        }//end button


    }//end row layout



///////////////////
//main table view//
///////////////////
    TableView{
        id: tableView

        visible: true
        anchors.top: rowLayout.bottom
        anchors.bottom: btn_1.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        TableViewColumn{
            role: "barcode"
            title: "barcode"
        }
        TableViewColumn{
            role: "name"
            title: "name"
        }
        TableViewColumn{
            role: "type"
            title: "type"
        }
        TableViewColumn{
            role: "count"
            title: "count"
        }
        TableViewColumn{
            role: "price"
            title: "price"
        }

        model: listmodel

        rowDelegate: Rectangle{
            id: row_delegate_1
            anchors.fill: parent
            color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
            MouseArea{
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: {
                    tableView.selection.clear();
                    tableView.selection.select(styleData.row);
                    tableView.currentRow = styleData.row;
                    tableView.focus = true;

                    switch(mouse.button){
                    case Qt.RightButton:
                        contextMenu.popup();
                        break;

                    default:
                        break;
                    }
                }
                onDoubleClicked: {
                    database.selectLine(listmodel.getId(tableView.currentRow));
                    database.addToCashbox(database._barcode
                                          ,database._name
                                          ,database._type
                                          ,database._count
                                          ,database._price);
                    cashbox.updateModel();
                }

            }//end mouse area
        }//end row_delegate_1(rectangle)
    }//end tableview
///////////////////////
//end main table view//
///////////////////////

///////////
//cashbox//
///////////

    TableView{
        id: cashbox_table

        visible: false
        anchors.top: rowLayout.bottom
        anchors.bottom: btn_1.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        //Font.pixelSize: 20

        TableViewColumn{
            role: "barcode"
            title: "barcode"
            width: 200
        }
        TableViewColumn{
            role: "name"
            title: "name"
        }
        TableViewColumn{
            role: "type"
            title: "type"
        }
        TableViewColumn{
            role: "count"
            title: "count"
        }
        TableViewColumn{
            role: "price"
            title: "price"
        }
        TableViewColumn{
            role: "total_price"
            title: "total price"
        }
        model: cashbox
//
        rowDelegate: Rectangle{
            id: row_delegate_2
            anchors.fill: parent
            color: styleData.selected ? 'red' : (styleData.alternate ? 'whitesmoke' : 'white');
            MouseArea{
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                onClicked: {
                    cashbox_table.selection.clear();
                    cashbox_table.selection.select(styleData.row);
                    cashbox_table.currentRow = styleData.row;
                    cashbox_table.focus = true;
                }
                onDoubleClicked: {
                    edit_line_in_cashbox_dialog.open();
                    //console.log(cashbox.getId(cashbox_table.currentRow));
                }

            }//end mouse area
        }
    }//end tableview cashbox

    Dialog{
        id: edit_line_in_cashbox_dialog
        standardButtons:StandardButton.Ok|StandardButton.Cancel

        Text {
            id: count_text_in_dialog;
            font.pixelSize: 50
            anchors.margins: 2;
            text: qsTr("enter count");
        }
        TextField{
            id: text_input_in_edit_line;
            font.pixelSize: 50
            anchors.margins: 2
            anchors.top: count_text_in_dialog.bottom
            text: "count";
        }
        onAccepted: {
            database.set_count(cashbox.getId(cashbox_table.currentRow), text_input_in_edit_line.text);
            database.all_price();
            listmodel.updateModel();
            cashbox.updateModel();
        }
    }

///////////////
//end cashbox//
///////////////


/////////////////////////////////////////////
//context menu to delete line in main table//
/////////////////////////////////////////////
    Menu{//context menu
        id: contextMenu

        MenuItem{
            text: qsTr("delete")
            onTriggered: {

                dialogDelete.open();
            }
        }
        MenuItem{
            text: qsTr("add")
            onTriggered: {

            }
        }
    }
    MessageDialog{
        id: dialogDelete
        title: qsTr("delete line")
        text: qsTr("delete?")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok | StandardButton.Cancel

        onAccepted: {
            database.deleteLine(listmodel.getId(tableView.currentRow))
            //console.log(tableView.currentRow)
            listmodel.updateModel();
        }
    }//end context menu

/////////////////////////////////////////////////
//end context menu to delete line in main table//
/////////////////////////////////////////////////


            //green number bottom left
            Text {
                id: total_price
                text: qsTr("all price: "+database._total_price.toString())
                //text: "The date is: " + new Date().toLocaleTimeString(Qt.locale("de_DE"))
                font.pixelSize: 20
                color: "green"
                anchors.left: clear_cashbox.right
                anchors.top: tableView.bottom
                anchors.margins: 5
            }

            //switched main table n check table
            Button{
                id:btn_1
                text: "to switch"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                //x: 50
                //y: 50
                onClicked: {
                    database.all_price();
                    if(tableView.visible){
                        //rowLayout.visible = false;
                        tableView.visible = false;
                        cashbox_table.visible = true;
                    }
                    else{
                        //rowLayout.visible = true;
                        tableView.visible = true;
                        cashbox_table.visible = false;

                    }
                }
            }

///////////////
//print check//
///////////////

            Window{
                id: window_print_check
                width: 315
                height: 300
                //width: table_view_in_print_check
                //standardButtons: StandardButton.Ok
                flags: Qt.FramelessWindowHint
                modality: Qt.ApplicationModal
                color: "whitesmoke"

                Text {
                    id: date_string_in_dialog_print_check;
                    text: qsTr("**************\n"+new Date().toLocaleDateString(Qt.locale));
                    font.pixelSize: 20
                }
                Text {
                    id: time_string_in_dialog_print_check;
                    text: qsTr(new Date().toLocaleTimeString(Qt.locale));
                    anchors.top: date_string_in_dialog_print_check.bottom
                    font.pixelSize: 20
                }
                Text {
                    id: all_cost_in_dialog_print_check
                    text: qsTr("all price: "+database._total_price.toString()+"\n**************")
                    anchors.top: time_string_in_dialog_print_check.bottom
                    font.pixelSize: 20
                }
                TableView{
                    id: table_view_in_print_check
                    anchors.top: all_cost_in_dialog_print_check.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right


                    TableViewColumn{
                        role: "name"
                        title: "name"
                        width: 150
                    }
                    TableViewColumn{
                        role: "count"
                        title: "count"
                        width: 50
                    }
                    TableViewColumn{
                        role: "price"
                        title: "price"
                        width: 50
                    }
                    TableViewColumn{
                        role: "total_price"
                        title: "total_price"
                        width: 50
                    }
                    model: cashbox
                }
                Button{
                    id: print_check_ok
                    anchors.top: table_view_in_print_check.bottom
                    anchors.right: parent.right
                    text: "print"
                    onClicked: {
                        //window_print_check.visible = false
                        //main_window.visible = true
                        window_print_check.close();
                        database.calculation_current_session();
                        database.cashbox_clear();
                        cashbox.updateModel();
                    }
                }
                Button{
                    id: print_check_cancel
                    text: "cancel"
                    anchors.top: table_view_in_print_check.bottom
                    anchors.right: print_check_ok.left
                    onClicked: {
                        window_print_check.close();
                    }
                }
            }

            Button{
                id: print_check
                text: "print check"
                anchors.left: btn_1.right
                anchors.bottom: parent.bottom
                onClicked: {
                    window_print_check.visible = true
                    cashbox.updateModel();
                }
            }
            Button{
                id: clear_cashbox
                text: "clear cashbox"
                anchors.bottom: parent.bottom
                anchors.left: print_check.right
                onClicked: {
                    database.cashbox_clear();
                    cashbox.updateModel();
                }
            }
///////////////////
//end print check//
///////////////////




/////////
//login//
/////////
    Window{
        id: on_login_window
        height: 104
        visible: true
        modality: Qt.ApplicationModal
        flags: Qt.FramelessWindowHint
        color: "whitesmoke"
        Text {
            id: text_input_login
            text: qsTr("login")
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5
        }
        TextField{
            id: login_str
            anchors.right: parent.right
        }
        Text {
            id: text_input_password
            text: qsTr("password")
            anchors.top: login_str.bottom
            anchors.left: parent.left
            anchors.margins: 5
        }
        TextField{
            id: password_str
            anchors.top: login_str.bottom
            anchors.right: parent.right
            echoMode: TextInput.Password
        }
        Button{
            id: login_btn
            anchors.top: password_str.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            //anchors.margins: 5
            text: "login"
            property int counter_to_wrong_login_or_password: 0;
            onClicked: {
                if(!database.login_in(login_str.text, password_str.text)){
                    counter_to_wrong_login_or_password+=1;
                    if(counter_to_wrong_login_or_password==3)Qt.quit();
                }
                else{
                   //main_window.visible = true;
                   //database.start_work();
                   login_str.text = "";
                   password_str.text = "";
                   on_login_window.close();
                }

            }
        }
        Button{
            id: exit_btn_in_login_window
            anchors.top: login_btn.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            //anchors.margins: 5
            text: "quit"
            onClicked: Qt.quit();
        }
    }

    Button{
        id: logout
        text: "logout"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            database.all_data_session("user")
            database.end_work();
            user_to_all_data_session.text = database.all_data_session("user");
            start_time_to_all_data_session.text = database.all_data_session("start_time");
            end_time_to_all_data_session.text = database.all_data_session("end_time");
            start_date_to_all_data_session.text = database.all_data_session("start_date");
            end_date_to_all_data_session.text = database.all_data_session("end_date");
            total_to_all_data_session.text = database.all_data_session("total");
            all_data_session.show();

        }
    }

    Window{
        id: all_data_session
        modality: Qt.ApplicationModal
        flags: Qt.FramelessWindowHint
        color: "whitesmoke"
        height: 200
        visible: false
        Text {
            id: user_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("user"))
        }
        Text {
            anchors.top: user_to_all_data_session.bottom
            id: start_time_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("start_time"))
        }
        Text {
            anchors.top: start_time_to_all_data_session.bottom
            id: end_time_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("end_time"))
        }
        Text {
            anchors.top: end_time_to_all_data_session.bottom
            id: start_date_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("start_date"))
        }
        Text {
            anchors.top: start_date_to_all_data_session.bottom
            id: end_date_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("end_date"))
        }
        Text {
            anchors.top: end_date_to_all_data_session.bottom
            id: total_to_all_data_session
            font.pixelSize: 20
            text: qsTr(database.all_data_session("total"))
        }

        Button{
            id: ok_btn_from_all_data
            anchors.top: total_to_all_data_session.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "login"
            onClicked: {
                all_data_session.close();
                on_login_window.show();
            }
        }
        Button{
            anchors.top: ok_btn_from_all_data.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "exit"
            onClicked: Qt.quit();
        }
    }
/////////////
//end login//
/////////////

////////////
//new user//
////////////
    Window{
        id: window_new_user
        width: 200
        height: 200
        modality: Qt.ApplicationModal
        flags: Qt.FramelessWindowHint
        color: "whitesmoke"
        //
        Text {
            id: new_user_text_input_login
            text: qsTr("login")
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5
        }
        TextField{
            id: new_user_login_str
            anchors.right: parent.right
        }
        //
        Text {
            id: new_user_text_input_password
            text: qsTr("password")
            anchors.top: new_user_login_str.bottom
            anchors.left: parent.left
            anchors.margins: 5
        }
        TextField{
            id: new_user_password_str
            anchors.top: new_user_login_str.bottom
            anchors.right: parent.right
            echoMode: TextInput.Password
        }
        //
        Text {
            id: double_new_user_text_input_password
            text: qsTr("confirmation")
            anchors.top: new_user_password_str.bottom
            anchors.left: parent.left
            anchors.margins: 5
        }
        TextField{
            id: double_new_user_password_str
            anchors.top: new_user_password_str.bottom
            anchors.right: parent.right
            echoMode: TextInput.Password
        }
        //
        Button{
            id: create_new_user
            text: "create"
            anchors.top: double_new_user_password_str.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            onClicked:{
                if(double_new_user_password_str.text === new_user_password_str.text){
                    database.new_user(new_user_login_str.text, new_user_password_str.text);
                    window_new_user.close();
                }
                else{
                    new_user_password_str.text = "";
                    double_new_user_password_str.text = "";
                }

            }
        }
        Button{
            id: cancel_create_new_user
            anchors.top: create_new_user.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            text: "cancel"
            onClicked: window_new_user.close();
        }
    }

    Button{//its button in main window bottom right
        id: new_user
        text: "new_user"
        anchors.right: logout.left
        anchors.bottom: parent.bottom
        onClicked: {
            window_new_user.show();
            //database.new_user(new_user_login_str.text, new_user_password_str.text);
            new_user_login_str.text = "";
            new_user_password_str.text = "";
            double_new_user_password_str.text = "";
            //window_new_user.close();
        }
    }
    ////////////////
    //end new user//
    ////////////////
}



/*
SomeClass{
    id: myClass
}


Button{
    id: btn1
    anchors.centerIn: parent
    text: "click";
    //onClicked: classA.callMe();
    onClicked:{
        myClass.setSomeVar("ABC");
        myClass.me_int+=1;
        myClass.me_float+=0.5;
        //myClass.db_open();

    }
    //onClicked: myClass.set_me_int(150);

}


Text{
    id: myLabel
    anchors{
        top: parent.top
        horizontalCenter: parent.horizontalCenter
        topMargin: 20
    }
    font.pixelSize: 24
    text: myClass.someVar
}

Text{
    id: myLabel_int
    anchors{
        top: parent.top
        horizontalCenter: parent.horizontalCenter
        //y: 10
        topMargin: 60
    }
    font.pixelSize: 24
    text: myClass.me_int
}
Text{
    id: myLabel_float
    anchors{
        top: parent.top
        horizontalCenter: parent.horizontalCenter
        topMargin: 100
    }
    font.pixelSize: 24
    text: myClass.me_float
}
*/
