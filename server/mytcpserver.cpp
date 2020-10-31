#include "mytcpserver.h"
myTcpserver::myTcpserver()
{
    m_sql=new Datactrl;
    read_pointer=0;
    db_Tcpserver=QSqlDatabase::database("my_connection");
    query=QSqlQuery(db_Tcpserver);
    qDebug()<<"构造了 myTcpserver";
    p1=new play1();
}

void myTcpserver::incomingConnection(qintptr socketDes){
    qDebug()<<"incommingConnection...";
    m_socketList.insert(socketDes,"unKnown");
    myThread *new_thread=new myThread(socketDes);
    connect(this,SIGNAL(SendData(int,QString)),new_thread,SLOT(transfer_sendDataSLOT(int,QString)));
    connect(new_thread,SIGNAL(transfer_disconnect(int)),this,SLOT(cilentLeaveSLOT(int)));
    connect(new_thread,SIGNAL(transfer_recvData(int,QString)),this,SLOT(Recv_Data(int,QString)));
    connect(new_thread,SIGNAL(finished()),new_thread,SLOT(deleteLater()));

    new_thread->start();
}

void myTcpserver::Recv_Data(int socketDes,QString data){
    read_socketDes=socketDes;
    qDebug()<<"rec Data from socketDes:"<<socketDes;
    qDebug()<<"and the Data is:"<<data;
    list=data.split("#");
    read_len=list.length();
    Handle_Data();

}

//rgst -a
//log  -b
//board -c
//break-submit -d
//build-submit -e
//break-ask-usr -f
//build-ask-usr -g
//break-word h
//break-update i
//builder-update j
//board-builder k
//board-breaker l
//delete-usr m
//search n 下面还有很多细分类
//作为客户端 sendUsrList  o
//invitate -p 邀请的信息（发出邀请，接收邀请）
//是否接受邀请？ -q
//双人游戏向服务器要个单词(请求&&接受) -r
//双人游戏，有人退出      -s

void myTcpserver::Handle_Data(){
    if(list[read_pointer]==""){//一个小节的开始或结束（也是另一个小节的开始），直接跳过，进入正文
        read_pointer++;
    }
    if(list[read_pointer]=="a"){//收到注册请求
        handle_rgst();
    }
    else if(list[read_pointer]=="b"){//收到登录请求
        handle_log();
    }
    else if(list[read_pointer]=="c"){//想查看刷新一次的数据库
        handle_board();
    }
    else if(list[read_pointer]=="d"){//闯关者提交
        handle_break();
    }
    else if(list[read_pointer]=="e"){//出题者提交
        handle_build();
    }
    else if(list[read_pointer]=="f"){//要求在客户端建立闯关者
        handle_breaker_ask();
    }
    else if(list[read_pointer]=="g"){//要求在客户端建立出题者
        handle_builder_ask();
    }
    else if(list[read_pointer]=="h"){//要求给一个单词给客户端的闯关者
        handle_word_ask();
    }
    else if(list[read_pointer]=="i"){//更新闯关者信息
        handle_breaker_update();
    }
    else if(list[read_pointer]=="j"){//更新出题者信息
        handle_builder_update();
    }
    else if(list[read_pointer]=="k"){//客户端要求查看出题者排名
        handle_builder_board();
    }
    else if(list[read_pointer]=="l"){//客户端要求查看闯关者排名
        handle_breaker_board();
    }
    else if(list[read_pointer]=="m"){//客户端要求注销用户
        handle_delete_usr();
    }
    else if(list[read_pointer]=="n"){//客户端要求查询用户信息
        handle_search();
    }
    else if(list[read_pointer]=="p"){//客户端想邀请另外一个在线玩家联机对战
        handle_invite();
    }
    else if(list[read_pointer]=="q"){//另一个客户端回送接收或者拒绝邀请
        transfer_invite_result();
    }
    else if(list[read_pointer]=="r"){//一个客户端代表两个客户端（双人游戏）来要个单词
        send_word_to_two();
    }
    else if(list[read_pointer]=="s"){//一个客户端已经退出联机模式，要求另一个客户端也退出
        handle_online_exit();
    }
    else {
        qDebug()<<"头是:"<<list[read_pointer];
        qDebug()<<"Handle_Data:收到无法识别的信息";
    }
}

void myTcpserver::handle_breaker_update(){//read_p+1,read_p+2,read_p+3,read_p+4，read_p+5
    query.exec(QString("replace into breakers values('%1','%2',%3,%4,%5)").arg(list[read_pointer+1]).arg(list[read_pointer+2])
                                                                           .arg(list[read_pointer+3].toInt()).arg(list[read_pointer+4].toInt()).
                                                                            arg(list[read_pointer+5].toInt()));
    tail_handle(6);
}

void myTcpserver::handle_builder_update(){//read_p+1,read_p+2,read_p+3,read_p+4,read_p+5
    query.exec(QString("replace into builders values('%1','%2',%3,%4,%5)").arg(list[read_pointer+1]).arg(list[read_pointer+2])
                                                                          .arg(list[read_pointer+3].toInt()).arg(list[read_pointer+4].toInt())
                                                                           .arg(list[read_pointer+5].toInt()));
    tail_handle(6);
}

void myTcpserver::handle_delete_usr(){//用到read_p+1
    //    g_ctrl.delete_usr(this->usr);
    //    if(!g_ctrl.search_usr(this->usr)){
    //        QMessageBox::information(this,"tips",QString("注销成功!"));
    //        on_returnBtn_clicked();
    //    }
    //    else  QMessageBox::information(this,"tips",QString("注销失败!"));
    m_sql->delete_usr(list[read_pointer+1]);
    if(!m_sql->search_usr(list[read_pointer+1])){
        QString data="#m#1#";//删除成功
        this->SendData(read_socketDes,data);
    }
    else{
        QString data="#m#2#";
        this->SendData(read_socketDes,data);
    }


}

void myTcpserver::handle_breaker_board(){//此时read_p指向"l"
    query.exec("select count(*) from breakers");
    query.next();
    int sum=query.value(0).toInt();
    QString data=QString("#l#%1#").arg(sum);
    query.exec("select * from breakers order by break_rounds desc");
    QString _nick_name;int _break_rounds,_exp,_level;
    while(query.next()){
        _nick_name=query.value(1).toString();
        _break_rounds=query.value(2).toInt();
        _exp=query.value(3).toInt();
        _level=query.value(4).toInt();
        data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_break_rounds).arg(_exp).arg(_level);
    }
    emit this->SendData(read_socketDes,data);
    tail_handle(1);
}

void myTcpserver::handle_builder_board(){//此时read_p指向"k"
    query.exec("select count(*) from builders");
    query.next();
    int sum=query.value(0).toInt();
    QString data=QString("#k#%1#").arg(sum);
    query.exec("select * from builders order by build_words desc");
    QString _nick_name;int _build_words,_exp,_level;
    while(query.next()){
        _nick_name=query.value(1).toString();
        _build_words=query.value(2).toInt();
        _exp=query.value(3).toInt();
        _level=query.value(4).toInt();
        data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_build_words).arg(_exp).arg(_level);
    }
    emit this->SendData(read_socketDes,data);
    tail_handle(1);
}

void myTcpserver::sendUsrList(){//需要在o的后面加上在线用户的个数
    qDebug()<<"sendUsrList...";
    QString data="#o#";
    int count=m_socketList.size();
    data+=QString("%1#").arg(count);
    QMap<int,QString>::iterator iter=m_socketList.begin();
    while(iter!=m_socketList.end()){
        query.exec(QString("select * from userinfo where usr='%1'").arg(iter.value()));
        query.next();
        QString _nick=query.value(1).toString();
        data+=QString("%1#%2#%3#").arg(iter.key()).arg(iter.value()).arg(_nick);
        iter++;
    }
    emit SendData(-520,data);//全部转发
}

void myTcpserver::cilentLeaveSLOT(int socketDes){
    QString usrname=m_socketList[socketDes];
    query.exec(QString("select * from userinfo where usr='%1'").arg(usrname));
    query.next();
    QString nickname=query.value(1).toString();
    QString usrinfo=QString("%1 %2 %3").arg(socketDes).arg(usrname).arg(nickname);//也算是固定格式了
    emit Someone_leave(usrinfo);//把mainwindow中的listWidget的相应表项删除
    m_socketList.remove(socketDes);//把myTcpserver中的map中的相应表项删除
    sendUsrList();//若有人离开，也应该要向其它所有在线用户 发出广播
}

void myTcpserver::transfer_invite_result(){//用到了read_p+1和read_p+2
    int socket_Dest=list[read_pointer+1].toInt();
    int result=list[read_pointer+2].toInt();
    QString data="#q#";
    data+=QString("%1#").arg(result);
    emit this->SendData(socket_Dest,data);
    tail_handle(3);
}

void myTcpserver::handle_rgst(){//进入此函数前 read_pointer已经指向了"a",注册应是要 read_p+1,read_p+2,read_p+3,read_p+4
    qDebug()<<"收到注册请求";
    int ret=m_sql->reg(list[read_pointer+1],list[read_pointer+2],list[read_pointer+3],list[read_pointer+4]);
    QString sendData;
    sendData="#a";
    if(ret==1){
        sendData+="#1#";//"该账号已经被注册!"
    }
    else if(ret==2){
        sendData+="#2#";//"两次输入密码不一致!"
    }
    else if(ret==4){
        sendData+="#4#";//"昵称已经被注册了"
    }
    else if(ret==3){
        sendData+="#3#";//"注册成功"
    }
    else{
        sendData+="#5#";//"注册失败，原因未知"
    }
    emit this->SendData(read_socketDes,sendData);
//    read_pointer+=5;//把结尾也算进来
//    if(read_pointer<read_len-1){//read_pointer从0开始，所以和read_len-1比较
//        qDebug()<<"read_pointter:"<<read_pointer<<"read_len:"<<read_len;
//        Handle_Data();//循环继续
//    }
//    else {//读取的全部结束了,则必要重置
//        qDebug()<<"读取结束";
//        read_pointer=0;
//    }
    tail_handle(5);
}

void myTcpserver::handle_word_ask(){//进入此函数前read_p指向了"h",出题要read_p+1
    qDebug()<<"handle word ask";
    int round=list[read_pointer+1].toInt();
    qDebug()<<"round:"<<round;
    p1->set_current_word(round);//
    QString data="#h#";
    data+=p1->current_word+"#"+p1->builder_nickname+"#";
    emit SendData(read_socketDes,data);
}

void myTcpserver::handle_online_exit(){//进入此函数前read_p指向了"s",要用read_p+1
    qDebug()<<"handle online exit";
    int to_close_socket_num=list[read_pointer+1].toInt();
    QString data="#s#";
    emit SendData(to_close_socket_num,data);//向 want to 关闭的socket num的客户端发过去信号
    tail_handle(2);
}


//void MainWindow::SLOT_ask_for_word_online(){
//    qDebug()<<"SLOT_ask_for_word_online";
//    QString data=QString("#r#%1#%2#").arg(mysocket_num).arg(g->companion_socket_num);
//    qDebug()<<"mainwindow send data:"<<data;
//    mysocket->write(data.toUtf8());
//}
void myTcpserver::send_word_to_two(){//此时read_p指向了"r",用到了read_p+1和read_p+2
    qDebug()<<"send word to two";
    int socket1=list[read_pointer+1].toInt();//胜者（主动要求方）是socket1
    int socket2=list[read_pointer+2].toInt();//败者是socket2
    QString randomword=m_sql->get_Random_word();
    QString data=QString("#r#%1#").arg(randomword);
    emit SendData(socket1,data);
    emit SendData(socket2,data);
    tail_handle(3);
}


void myTcpserver::handle_log(){//进入此函数前 read_pointer已经指向了"b",登陆应是要 read_p+1,read_p+2
    qDebug()<<"收到登录请求";
    int ret=m_sql->verifyusr(list[read_pointer+1],list[read_pointer+2]);
    QString sendData="#b";
    if(ret){//如果账号信息正确，应回答客户端
        sendData+=QString("#1#%1").arg(read_socketDes);//成功
        //QSqlQuery("create table if not exists userinfo(usr varchar primary key,nickname varchar,pwd varchar)",db);
        query.exec(QString("select * from userinfo where usr='%1'").arg(list[read_pointer+1]));
        query.next();
        QString _nick=query.value(1).toString();
        QString usrinf=QString("%1 %2 %3").arg(read_socketDes).arg(list[read_pointer+1]).arg(_nick);//也算是固定格式了
        m_socketList[read_socketDes]=list[read_pointer+1];
        emit this->Someone_arrive(usrinf);//用于提醒服务器端
    }
    else {
        sendData+="#2#";//失败
    }
    emit this->SendData(read_socketDes,sendData);
    if(ret) sendUsrList();//注意顺序
    tail_handle(3);
}

void myTcpserver::handle_board(){//进入此函数前 read_pointer已经指向了"b",应是要 read_p+1
    qDebug()<<"收到查询排行榜信息";
    tail_handle(2);

}

void myTcpserver::handle_invite(){//read_p指向"p",要用read_p+1    作为服务器转发
    int be_invited_socket=list[read_pointer+1].toInt();
    qDebug()<<"收到邀请请求  邀请者socket:"<<read_socketDes<<" 被邀请者socket:"<<be_invited_socket;
    QString data="#p#";
    data+=QString("%1#").arg(read_socketDes);
    emit this->SendData(be_invited_socket,data);//向被邀请的客户端发出被邀请的信息
    tail_handle(2);
}

void myTcpserver::handle_break(){

}

void myTcpserver::handle_build(){//此时read_pointer指向"e",用到了read_p+1,read_p+2,read_p+3
    qDebug()<<"收到出题者提交的单词";
    QString word=list[read_pointer+1];
    QString builder_nickname=list[read_pointer+3];
    QString usrname=list[read_pointer+2];
    int b=m_sql->add_word(word,usrname,builder_nickname);
    QString data="#e#";
    if(b==1){//该单词已经存在
        data+="1#";
    }
    else if(b==2){//出题成功
        data+="2#";
    }
    else{//出题失败，未知错误
        data+="3#";
    }
    emit this->SendData(read_socketDes,data);
    tail_handle(4);
}

//rgst -a
//log  -b
//board -c
//break-submit -d
//build-submit -e
//break-ask-usr -f
//build-ask-usr -g
//break-word h
//break-update i
void myTcpserver::handle_breaker_ask(){//进入此函数前 read_pointer已经指向了"f"
    qDebug()<<"收到要求闯关者信息";
    read_pointer++;
    QString nickname;
    int break_rounds;
    int exp;
    int level;
    QString asked_usrname=list[read_pointer];//询问的用户的账号
    qDebug()<<"asked_usrname:"<<asked_usrname;
    query.exec(QString("select * from breakers where usr='%1'").arg(asked_usrname));
    int b=query.next();
    qDebug()<<"query.next():"<<b;
    //QSqlQuery("create table if not exists breakers(usr varchar primary key,nickname varchar,break_rounds int,exp int,level int)",db);
    if(b){
        nickname=query.value(1).toString();
        break_rounds=query.value(2).toInt();
        exp=query.value(3).toInt();
        level=query.value(4).toInt();
    }
    else {//没找到，说明要重新创建
        query.exec(QString("select * from userinfo where usr='%1'").arg(asked_usrname));
        int bb=query.next();
        if(!bb) qDebug()<<"handle_breaker_ask:没救了";
        else {
            QString usr=query.value(0).toString();
            nickname=query.value(1).toString();
            break_rounds=0;
            exp=0;
            level=0;
            query.exec(QString("insert into breakers values('%1','%2',%3,%4,%5)").arg(usr).arg(nickname).arg(0).
                                                                             arg(0).arg(0));
        }
    }
    QString data="#f";
    data+="#"+nickname+QString("#%1#%2#%3#").arg(break_rounds).arg(exp).arg(level);
    emit this->SendData(read_socketDes,data);
    tail_handle(1);//已经++过了
}

void myTcpserver::handle_builder_ask(){//只会用到read_p+1
    qDebug()<<"收到要求出题者信息";
    QString nickname;
    int build_words;
    int exp;
    int level;
    QString usrname=list[read_pointer+1];
    query.exec(QString("select * from builders where usr='%1'").arg(usrname));
    int b=query.next();
    if(!b){//没找到，需要重新创建
        query.exec(QString("select * from userinfo where usr='%1'").arg(usrname));
        int bb=query.next();
        if(!bb) qDebug()<<"handle builder ask 没救了";
        else {
            nickname=query.value(1).toString();
            build_words=0;
            exp=0;
            level=0;
            query.exec(QString("insert into builders values('%1','%2',%3,%4,%5)").arg(usrname).arg(nickname).arg(build_words).
                                                                                  arg(exp).arg(level));

        }
    }
    else {//找到了
        nickname=query.value(1).toString();
        build_words=query.value(2).toInt();
        exp=query.value(3).toInt();
        level=query.value(4).toInt();
    }
    QString data="#g";
    data+="#"+nickname+QString("#%1#%2#%3#").arg(build_words).arg(exp).arg(level);
    emit this->SendData(read_socketDes,data);
    tail_handle(2);
}

//if(ui->comboBox->currentText()==QString("出题者-昵称")){
//    qDebug()<<"出题者-昵称";
//    emit SIGNAL_search(1,s);
//}
//else if(ui->comboBox->currentText()==QString("出题者-出题数")){
//    qDebug()<<"出题者-出题数";
//    emit SIGNAL_search(2,s);
//}
//else if(ui->comboBox->currentText()==QString("出题者-经验")){
//    qDebug()<<"出题者-经验";
//    emit SIGNAL_search(3,s);
//}
//else if(ui->comboBox->currentText()==QString("出题者-等级")){
//    qDebug()<<"出题者-等级";
//    emit SIGNAL_search(4,s);
//}
//else if(ui->comboBox->currentText()==QString("闯关者-昵称")){
//    qDebug()<<"闯关者-昵称";
//    emit SIGNAL_search(5,s);
//}
//else if(ui->comboBox->currentText()==QString("闯关者-闯关数")){
//    qDebug()<<"闯关者-闯关数";
//    emit SIGNAL_search(6,s);
//}
//else if(ui->comboBox->currentText()==QString("闯关者-经验")){
//    qDebug()<<"闯关者-经验";
//    emit SIGNAL_search(7,s);
//}
//else if(ui->comboBox->currentText()==QString("闯关者-等级")){
//    qDebug()<<"闯关者-等级";
//    emit SIGNAL_search(8,s);
//}
void myTcpserver::handle_search(){//此时read_p指向"n",用到了read_p+1和read_p+2
    qDebug()<<"收到客户端要求根据属性查询用户";
    int type=list[read_pointer+1].toInt();
    QString s=list[read_pointer+2];
    qDebug()<<"查询要求:"<<type<<" "<<s;
//    QSqlQuery("create table if not exists breakers(usr varchar primary key,nickname varchar,break_rounds int,exp int,level int)",db);
//    QSqlQuery("create table if not exists builders(usr varchar primary key,nickname varchar,build_words int,exp int,level int)",db);
    switch (type) {
    case 1:{//出题者-昵称
        query.exec(QString("select count(*) from builders where nickname='%1'").arg(s));
        query.next();
        int b=query.value(0).toInt();//若不存在，则b为0
        QString data="#n#";
        data+=QString("%1#").arg(b);
        if(b){
            query.exec(QString("select * from builders where nickname='%1'").arg(s));
            query.next();
            QString _nick_name=query.value(1).toString();
            int _bld_words=query.value(2).toInt();
            int _exp=query.value(3).toInt();
            int _level=query.value(4).toInt();
            data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_bld_words).arg(_exp).arg(_level);
        }
        this->SendData(read_socketDes,data);
        break;
    }
    case 2:{//出题者-出题数
        search_function(1,"build_words",s.toInt());
        break;
    }
    case 3:{//出题者-经验
        search_function(1,"exp",s.toInt());
        break;
    }
    case 4:{//出题者-等级
        search_function(1,"level",s.toInt());
        break;
    }
    case 5:{//闯关者-昵称
        query.exec(QString("select count(*) from breakers where nickname='%1'").arg(s));
        query.next();
        int b=query.value(0).toInt();//若不存在，则b=0
        QString data="#n#";
        data+=QString("%1#").arg(b);
        if(b){
            query.exec(QString("select * from breakers where nickname='%1'").arg(s));
            query.next();
            QString _nick_name=query.value(1).toString();
            int _break_rounds=query.value(2).toInt();
            int _exp=query.value(3).toInt();
            int _level=query.value(4).toInt();
            data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_break_rounds).arg(_exp).arg(_level);
        }
        this->SendData(read_socketDes,data);
        break;
    }
    case 6:{//闯关者-闯关数
        search_function(2,"break_rounds",s.toInt());
        break;
    }
    case 7:{//闯关者-经验
        search_function(2,"exp",s.toInt());
        break;
    }
    case 8:{//闯关者-等级
        search_function(2,"level",s.toInt());
        break;
    }
    }
    tail_handle(3);
}

void myTcpserver::tail_handle(int i){
        read_pointer=read_pointer+i;//把后面的空格也算进去
        if(read_pointer<read_len-1){//read_pointer从0开始，所以和read_len-1比较
            qDebug()<<"read_pointter:"<<read_pointer<<"read_len:"<<read_len;
            Handle_Data();//循环继续
        }
        else {//读取的全部结束了,则必要重置
            qDebug()<<"读取结束";
            read_pointer=0;
        }
}

void myTcpserver::search_function(int builder_OR_breaker, QString attribute,int condition){//builder_OR_breaker为1则查找前者
    QString data="#n#";
    QString _nick_name;int _bld_words_OR_break_rounds,_exp,_level;
    if(builder_OR_breaker==1){//查询出题者
        query.exec(QString("select count(*) from builders where %1=%2").arg(attribute).arg(condition));
        query.next();
        int b=query.value(0).toInt();
        data+=QString("%1#").arg(b);
        query.exec(QString("select * from builders where %1=%2").arg(attribute).arg(condition));
        while (query.next()) {
            _nick_name=query.value(1).toString();
            _bld_words_OR_break_rounds=query.value(2).toInt();
            _exp=query.value(3).toInt();
            _level=query.value(4).toInt();
            data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_bld_words_OR_break_rounds).arg(_exp).arg(_level);
        }
    }
    else {//查询闯关者
        query.exec(QString("select count(*) from breakers where %1=%2").arg(attribute).arg(condition));
        query.next();
        int b=query.value(0).toInt();
        data+=QString("%1#").arg(b);
        query.exec(QString("select * from breakers where %1=%2").arg(attribute).arg(condition));
        while (query.next()) {
            _nick_name=query.value(1).toString();
            _bld_words_OR_break_rounds=query.value(2).toInt();
            _exp=query.value(3).toInt();
            _level=query.value(4).toInt();
            data+=QString("%1#%2#%3#%4#").arg(_nick_name).arg(_bld_words_OR_break_rounds).arg(_exp).arg(_level);
        }
    }
    this->SendData(read_socketDes,data);
}
