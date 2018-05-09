#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mysqlconnect.h"
#include <unistd.h>

ConnectionPool *pool = ConnectionPool::getInstance("root", "root", "tcp://127.0.0.1:3306", 4);

static std::string GetCurrentDate()
{
    time_t nowtime;
    nowtime = time(NULL); //获取日历时间
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y%m%d",localtime(&nowtime));
    return tmp;
}
std::map<std::string, std::string> name_symbols = {};
static std::map<std::string, std::string> stop_symbols = {};

void split( const string &str, const string &delimiters, std::map<std::string, std::string> &elems) {
    string::size_type pos, prev = 0;
    while ( ( pos = str.find_first_of(delimiters, prev) ) != string::npos ) {

        if ( pos > prev ) {

            //if ( skip_empty && 1 == pos - prev ) break;
            if(str[pos] == '(') {

                std::string userid = str.substr(prev, pos - prev);
                prev = pos + 1;
                size_t  tmp_post = str.find(':', prev);
                size_t  tmp_post2 = str.find(',', prev);
                prev = tmp_post+1;
                std::string symbol, symbols;
                if(tmp_post2 == std::string::npos){
                    symbol = str.substr(prev);
                }else {
                    symbol = str.substr(prev, tmp_post2 - prev);
                    pos = tmp_post2;
                }
                symbols = elems[userid];
                elems[userid] = symbols + symbol + ",";

                cout << "11:" << userid << " " <<  elems[userid] << "\n";
            }
            sleep(1);

        }
        prev = pos + 1;
    }

}

int main(int argc, char *argv[])
{
    std::shared_ptr<Connection>con;
    Statement *state;
    ResultSet *result;
    PreparedStatement *pstmt;

/*    split("1(user_test):RKT/BTC,2(use1):BTC/RKC,2(rerfdd):BTC/ETH,1(user):BTC/ETH","(", name_symbols);
    for (auto j = name_symbols.begin(); j !=  name_symbols.end(); ++j) {
        cout << "firsr:" << j->first << " " << j->second << "\n";
    }

    split("1(user_test):RKT/BTC,2(use1):BTC/RKC","(", stop_symbols);
    for (auto j = stop_symbols.begin(); j !=  stop_symbols.end(); ++j) {
        std::string symbo = j->second;
        size_t position = name_symbols[j->first].find(symbo);

        name_symbols[j->first].erase(position, symbo.length());

    }

    for (auto j = name_symbols.begin(); j !=  name_symbols.end(); ++j) {
        cout << "firsr:" << j->first << " " << j->second << "\n";
    }*/
    //获得一个连接
    con = pool->getConnect();
    //获得一个数据库连接对象
    state = con->createStatement();

    std::string tablename = "TbSubmitOrder_1001_" + GetCurrentDate();
    //查询语句
    std::string sqlstring = "SELECT table_name FROM `INFORMATION_SCHEMA`.`TABLES` WHERE table_name ='"+ tablename +"' AND TABLE_SCHEMA='matching_engine';";
    //查询语句
    result = state->executeQuery(sqlstring);
    std::cout << "result:" << result->rowsCount() << "\n";
    if(result->rowsCount() == 0)
    {
        std::cout << "**************\n";
        std::string creatable = "CREATE TABLE `matching_engine`.`" + tablename  +"` (\n"
                "  `SubOrderID` bigint(8) NOT NULL,\n"
                "  `UserOrderID` varchar(35) NOT NULL,\n"
                "  `UserID` int(4) unsigned zerofill NOT NULL,\n"
                "  `SubmitTime` bigint(8) NOT NULL,\n"
                "  `SubmitType` tinyint(4) NOT NULL,\n"
                "  `OrderType` tinyint(4) NOT NULL,\n"
                "  `TransType` tinyint(4) NOT NULL,\n"
                "  `Symbol` int(4) unsigned zerofill NOT NULL,\n"
                "  `OrderPrice` bigint(8) NOT NULL,\n"
                "  `OrderNumber` bigint(8) NOT NULL,\n"
                "  `OrderIndex` int(4) unsigned zerofill NOT NULL,\n"
                "  PRIMARY KEY (`SubOrderID`,`UserOrderID`)\n"
                ") ENGINE=InnoDB DEFAULT CHARSET=utf8;";
        state->execute(creatable);
    }
    sleep(1);
    delete result;
    delete state;

    std::string insert = "insert into matching_engine." + tablename + "(SubOrderID,UserOrderID,UserID,SubmitTime,"
            "SubmitType,OrderType,TransType,Symbol,OrderPrice,OrderNumber,OrderIndex) value(?,?,?,?,?,?,?,?,?,?,?);";
    pstmt = con->prepareStatement(insert);
    pstmt->setUInt64(1, 1);
    pstmt->setString(2, "1234");
    pstmt->setUInt(3, 1001);
    pstmt->setUInt64(4, 15212344555);
    pstmt->setUInt(5, 0);
    pstmt->setUInt(6, 1);
    pstmt->setUInt(7, 1);
    pstmt->setUInt(8, 21001);
    pstmt->setInt64(9, 539000);
    pstmt->setInt64(10, 1000000);
    pstmt->setUInt(11, 1);
    pstmt->executeUpdate();

    delete pstmt;

    pool->retConnect(con);
    std::cout << pool->getPoolSize() << std::endl;

    return EXIT_SUCCESS;
}
