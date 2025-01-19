#ifndef ROBOTS_4_1_6_MANAGER_H
#define ROBOTS_4_1_6_MANAGER_H

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include "robots.h"

// Manager管理类
class Manager {
private:
    std::vector<std::shared_ptr<Robot>> robots; // 存储所有机器人对象的数组
    std::list<std::shared_ptr<Robot>> working; // 正常（工作的）机器人列表
    std::list<std::shared_ptr<Robot>> destroyed; // 被摧毁的机器人列表

    // 参数 < 血量下降前_blood的值reduce_blood , < 所属队伍_team，机器人标识符_id > >
    // 按照reduce_blood的值从小到大有序储存在同一段机器人枪口热量降低的时间内被摧毁的机器人
    std::map<int,std::pair<int,int>> reduce_heat_destroyed;

    static bool whether_Engineer(int max_heat); // 判断是否为工程
    void add_new_robot(const std::shared_ptr<Robot>& robot); // 添加一个新的机器人
    void add_working_robot(int max_blood,int max_heat,int team,int id); // 将该机器人移入正常机器人列表
    void add_destroyed_robot(int max_blood,int max_heat,int blood,
                             int heat,int team,int id); // 将该机器人移入被击毁的机器人列表
    auto find_working_robot(int team,int id); // 查找正常机器人
    auto find_destroyed_robot(int team,int id); // 查找被摧毁的机器人
public:
    void reduce_heat(int time); // 机器人枪口热量随时间降低
    void command_A(int team,int id,int type); // 添加一个新的机器人（type=0为步兵，type=1为工程）
    void command_F(int team,int id,int damage); // 对该机器人造成damage点伤害
    void command_H(int team,int id,int add_heat); // 该机器人枪口热量上升指定值add_heat
    void command_U(int team,int id,int lv); // 将该机器人设定为该等级lv
};

#endif //ROBOTS_4_1_6_MANAGER_H
