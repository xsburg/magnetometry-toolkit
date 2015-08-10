The following actions should be made to rebuild Qt MYSQL plugin.

1. cd qt\5.3\Src\qtbase\src\plugins\sqldrivers\mysql
2. qmake INCLUDEPATH+='C:\MySQL\MySQLConnectorC6.1\include' LIBS+="C:\MySQL\MySQLConnectorC6.1\lib\libmysql.lib" mysql.pro
# paths must not contain whitespaces
3. nmake (in dev console)