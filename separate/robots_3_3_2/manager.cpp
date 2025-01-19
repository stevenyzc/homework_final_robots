#include "manager.h"

bool Manager::whether_Engineer(int max_heat) // 判断是否为工程
{
    switch (max_heat)
    {
        case 0: // 热量上限为0的只有工程
            return true;
        default: // 热量上限不为0的均为步兵
            return false;
    }
}

void Manager::add_new_robot(const std::shared_ptr<Robot>& robot) // 添加一个新的机器人
{
    robots.push_back(robot); // 加入到所有机器人数组
    working.push_back(robot); // 加入到正常机器人列表
}

void Manager::add_working_robot(int max_blood,int max_heat,int team,int id) // 将该机器人移入正常机器人列表
{
    bool flag=whether_Engineer(max_heat); // 判断是否为工程
    if (flag==1) // 是工程
    {
        std::shared_ptr<Engineer> robot
                =std::make_shared<Engineer>(max_blood,max_heat,max_blood,0,
                                            team,id); // 新建工程，并将血量设置为血量上限，热量设置为0
        working.push_back(robot); // 加入到正常机器人列表
    }
    else // 是步兵
    {
        std::shared_ptr<Infantry> robot
                =std::make_shared<Infantry>(max_blood,max_heat,max_blood,0,
                                            team,id); // 新建步兵，并将血量设置为血量上限，热量设置为0
        working.push_back(robot); // 加入到正常机器人列表
    }
}

void Manager::add_destroyed_robot(int max_blood,int max_heat,int blood,
                         int heat,int team,int id) // 将该机器人移入被击毁的机器人列表
{
    bool flag=whether_Engineer(max_heat); // 判断是否为工程
    if (flag==1) // 是工程
    {
        std::shared_ptr<Engineer> robot
                =std::make_shared<Engineer>(max_blood,max_heat,blood,heat,team,id); // 新建工程
        destroyed.push_back(robot); // 加入到被摧毁的机器人列表
    }
    else // 是步兵
    {
        std::shared_ptr<Infantry> robot
                =std::make_shared<Infantry>(max_blood,max_heat,blood,heat,team,id); // 新建步兵
        destroyed.push_back(robot); // 加入到被摧毁的机器人列表
    }
}

auto Manager::find_working_robot(int team,int id) // 查找正常机器人
{
    std::list<std::shared_ptr<Robot>>::iterator it; // 正常机器人列表迭代器
    for (it=working.begin();it!=working.end();it++) // 遍历所有正常机器人
    {
        if ((*it)->_team==team && (*it)->_id==id) // 找到机器人
            break;
    }
    return it; // 返回迭代器指针
}

auto Manager::find_destroyed_robot(int team,int id) // 查找被摧毁的机器人
{
    std::list<std::shared_ptr<Robot>>::iterator it; // 被摧毁的机器人列表迭代器
    for (it=destroyed.begin();it!=destroyed.end();it++) // 遍历所有被摧毁的机器人
    {
        if ((*it)->_team==team && (*it)->_id==id) // 找到机器人
            break;
    }
    return it; // 返回迭代器指针
}

void Manager::reduce_heat(int time) // 机器人枪口热量随时间降低
{
    std::list<std::shared_ptr<Robot>>::iterator it; // 正常机器人列表迭代器
    for (it=working.begin();it!=working.end();) // 遍历所有正常机器人
    {
        if ((*it)->_heat>0) // 机器人热量为正
        {
            if ( (*it)->_heat > (*it)->_max_heat ) // 机器人热量大于热量上限
            {
                bool flag=false; // 判断血量下降的方式（初始：在时间结束时机器人热量未减小到最大热量）
                int reduce_blood; // 血量下降前(*it)->_blood的值，该值越小，机器人被摧毁越早，越早输出
                if ( (*it)->_heat - (*it)->_max_heat <= time ) // 在时间结束前或时间结束时恰好机器人热量减小到最大热量
                {
                    (*it)->_blood -= (*it)->_heat - (*it)->_max_heat ; // 每秒钟造成1点伤害
                    flag=true; // 判断血量下降的方式切换为：在时间结束前或时间结束时恰好机器人热量减小到最大热量
                }
                else // 在时间结束时机器人热量未减小到最大热量，判断血量下降的方式不变
                    (*it)->_blood -= time; // 每秒钟造成1点伤害，共time点
                if ((*it)->_blood<=0) // 被击毁
                {
                    add_destroyed_robot((*it)->_max_blood,(*it)->_max_heat,(*it)->_blood,
                                        (*it)->_heat,(*it)->_team,(*it)->_id); // 将该机器人移入被击毁的机器人列表
                    if (flag==1) // 血量下降的方式为：在时间结束前或时间结束时恰好机器人热量减小到最大热量
                        reduce_blood = (*it)->_blood + (*it)->_max_heat - (*it)->_heat;
                    else // 血量下降的方式为：在时间结束时机器人热量未减小到最大热量
                        reduce_blood = (*it)->_blood + time;
                    // 按照reduce_blood的值从小到大有序插入同一段机器人枪口热量降低的时间内被摧毁的机器人
                    reduce_heat_destroyed.insert ( std::make_pair
                                                           ( reduce_blood , std::make_pair ( (*it)->_team , (*it)->_id ) ) );
                    auto it2=it; // 新迭代器指向原迭代器指向的元素位置
                    it++; // 原迭代器后移一位
                    working.erase(it2); // 使用新迭代器将该机器人移出正常机器人列表
                }
                else // 未被击毁
                {
                    (*it)->_heat -= time ; // 每秒钟降低1点热量，共time点
                    if ((*it)->_heat<0) // 热量不能为负数
                        (*it)->_heat=0;
                    it++; // 迭代器后移一位
                }
            }
            else // 机器人热量小于等于热量上限
            {
                (*it)->_heat -= time ; // 每秒钟降低1点热量，共time点
                if ((*it)->_heat<0) // 热量不能为负数
                    (*it)->_heat=0;
                it++; // 迭代器后移一位
            }
        }
        else // 机器人枪口热量为0
            it++; // 迭代器后移一位
    }
    if (!reduce_heat_destroyed.empty()) // 在同一段机器人枪口热量降低的时间内存在被摧毁的机器人
    {
        for (auto & it : reduce_heat_destroyed) // 遍历在同一段机器人枪口热量降低的时间内被摧毁的机器人
            std::cout<<"D "<<it.second.first<<' '<<it.second.second<<std::endl; // 输出：D 所属队伍 机器人标识符
        reduce_heat_destroyed.clear(); // 清空在同一段机器人枪口热量降低的时间内被摧毁的机器人
    }
}

void Manager::command_A(int team,int id,int type) // 添加一个新的机器人（type=0为步兵，type=1为工程）
{
    auto it=find_destroyed_robot(team,id); // 查找被摧毁的机器人
    if (it!=destroyed.end()) // 找到机器人
    {
        add_working_robot((*it)->_max_blood,(*it)->_max_heat,
                          (*it)->_team,(*it)->_id); // 将该机器人移入正常机器人列表
        destroyed.erase(it); // 将该机器人移出被摧毁的机器人列表
    }
    else // 未找到机器人
    {
        it=find_working_robot(team,id); // 查找正常机器人
        if (it==working.end()) // 未找到机器人
        {
            switch (type)
            {
                case 1: // 是工程
                    add_new_robot(std::make_shared<Engineer>(team,id));
                    return;
                case 0: // 是步兵
                    add_new_robot(std::make_shared<Infantry>(team,id));
                    return;
                default:{}
            }
        }
    }
}

void Manager::command_F(int team,int id,int damage) // 对该机器人造成damage点伤害
{
    auto it=find_working_robot(team,id); // 查找正常机器人
    if (it!=working.end()) // 找到机器人
    {
        (*it)->_blood-=damage; // 对该机器人造成damage点伤害（血量下降damage点）
        if ((*it)->_blood<=0) // 被击毁
        {
            add_destroyed_robot((*it)->_max_blood,(*it)->_max_heat,(*it)->_blood,
                                (*it)->_heat,(*it)->_team,(*it)->_id); // 将该机器人移入被击毁的机器人列表
            std::cout<<"D "<<(*it)->_team<<' '<<(*it)->_id<<std::endl; // 输出：D [所属队伍] [机器人标识符]
            working.erase(it); // 将该机器人移出正常机器人列表
        }
    }
}

void Manager::command_H(int team,int id,int add_heat) // 该机器人枪口热量上升指定值add_heat
{
    auto it=find_working_robot(team,id); // 查找正常机器人
    if (it!=working.end()) // 找到机器人
    {
        bool flag=whether_Engineer((*it)->_max_heat); // 判断是否为工程
        if (flag==0) // 是步兵
            (*it)->_heat+=add_heat; // 该机器人枪口热量上升指定值add_heat
    }
}

void Manager::command_U(int team,int id,int lv) // 将该机器人设定为该等级lv
{
    auto it=find_working_robot(team,id); // 查找正常机器人
    if (it!=working.end()) // 找到机器人
    {
        bool flag=whether_Engineer((*it)->_max_heat); // 判断是否为工程
        if (flag==0 && lv>(*it)->_max_heat/100) // 是步兵且目标等级大于当前等级（等级=热量上限/100）
        {
            (*it)->_max_blood=100*lv-50; // 血量上限=100*等级-50（只考虑等级2、3）
            (*it)->_max_heat=100*lv; // 热量上限=100*等级
            (*it)->_blood=(*it)->_max_blood; // 将血量重置为血量上限
        }
    }
}
