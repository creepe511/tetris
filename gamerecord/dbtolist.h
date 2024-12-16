// dbtolist - 头文件

#ifndef DBTOLIST_H
#define DBTOLIST_H

#include "linkedlist.h"

//创建并将新记录加入到数据库中
void add_to_db();

//删除节点以删除数据库中的记录
void del_in_db();


//修改节点内的“备注”以修改数据库内的“备注”
void edit_in_db();

//使数据库内的游戏记录按“游戏时间”排序
void sort_in_start_time();

//使数据库内的游戏记录按“分数”排序
void sort_in_score();

//使数据库内的游戏记录按“游戏时长”排序
void sort_in_duration();





#endif // DBTOLIST_H