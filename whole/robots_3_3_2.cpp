// 正式版1（变量均为public，仍需修改）

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <memory>

// Robot抽象类
class Robot {
public:
    int _max_blood=0; // 血量上限
    int _max_heat=0; // 热量上限
    int _blood=0; // 血量
    int _heat=0; // 热量
    int _team=0; // 所属队伍
    int _id=0; // 机器人标识符
    virtual void func() = 0; // 构造纯虚函数，使Robot成为抽象类
    virtual ~Robot()=default; // 编译器自动生成虚析构函数
};

// 步兵子类
class Infantry : virtual public Robot {
public:
    void func() override {} // 重写纯虚函数以实例化对象
    Infantry()=default; // 编译器自动生成默认构造函数
    Infantry(int team,int id) // 初始化机器人有参构造
    {
        _max_blood=100;
        _max_heat=100;
        _blood=100;
        _heat=0;
        _team=team;
        _id=id;
    }
    Infantry(int max_blood,int max_heat,int blood,int heat,int team,int id) // 完整机器人属性有参构造
    {
        _max_blood=max_blood;
        _max_heat=max_heat;
        _blood=blood;
        _heat=heat;
        _team=team;
        _id=id;
    }
};

// 工程子类
class Engineer : virtual public Robot {
public:
    void func() override {} // 重写纯虚函数以实例化对象
    Engineer()=default; // 编译器自动生成默认构造函数
    Engineer(int team,int id) // 初始化机器人有参构造
    {
        _max_blood=300;
        _max_heat=0;
        _blood=300;
        _heat=0;
        _team=team;
        _id=id;
    }
    Engineer(int max_blood,int max_heat,int blood,int heat,int team,int id) // 完整机器人属性有参构造
    {
        _max_blood=max_blood;
        _max_heat=max_heat;
        _blood=blood;
        _heat=heat;
        _team=team;
        _id=id;
    }
};

// Manager管理类
class Manager {
private:
    std::vector<std::shared_ptr<Robot>> robots; // 存储所有机器人对象的数组
    std::list<std::shared_ptr<Robot>> working; // 正常（工作的）机器人列表
    std::list<std::shared_ptr<Robot>> destroyed; // 被摧毁的机器人列表

    // 参数 < 血量下降前_blood的值reduce_blood , < 所属队伍_team，机器人标识符_id > >
    // 按照reduce_blood的值从小到大有序储存在同一段机器人枪口热量降低的时间内被摧毁的机器人
    std::map<int,std::pair<int,int>> reduce_heat_destroyed;

    bool whether_Engineer(int max_heat) // 判断是否为工程
    {
        switch (max_heat)
        {
            case 0: // 热量上限为0的只有工程
                return true;
            default: // 热量上限不为0的均为步兵
                return false;
        }
    }
    void add_new_robot(std::shared_ptr<Robot> robot)
    {
        robots.push_back(robot); // 加入到所有机器人数组
        working.push_back(robot); // 加入到正常机器人列表
    }
    void add_working_robot(int max_blood,int max_heat,int team,int id) // 将该机器人移入正常机器人列表
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
    void add_destroyed_robot(int max_blood,int max_heat,int blood,
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
    auto find_working_robot(int team,int id) // 查找正常机器人
    {
        std::list<std::shared_ptr<Robot>>::iterator it; // 正常机器人列表迭代器
        for (it=working.begin();it!=working.end();it++) // 遍历所有正常机器人
        {
            if ((*it)->_team==team && (*it)->_id==id) // 找到机器人
                break;
        }
        return it; // 返回迭代器指针
    }
    auto find_destroyed_robot(int team,int id) // 查找被摧毁的机器人
    {
        std::list<std::shared_ptr<Robot>>::iterator it; // 被摧毁的机器人列表迭代器
        for (it=destroyed.begin();it!=destroyed.end();it++) // 遍历所有被摧毁的机器人
        {
            if ((*it)->_team==team && (*it)->_id==id) // 找到机器人
                break;
        }
        return it; // 返回迭代器指针
    }
public:
    void reduce_heat(int time) // 机器人枪口热量随时间降低
    {
        std::list<std::shared_ptr<Robot>>::iterator it; // 正常机器人列表迭代器
        for (it=working.begin();it!=working.end();) // 遍历所有正常机器人
        {
            if ((*it)->_heat>0) // 机器人热量为正
            {
                if ( (*it)->_heat > (*it)->_max_heat ) // 机器人热量大于热量上限
                {
                    bool flag=0; // 判断血量下降的方式（初始：在时间结束时机器人热量未减小到最大热量）
                    int reduce_blood; // 血量下降前(*it)->_blood的值，该值越小，机器人被摧毁越早，越早输出
                    if ( (*it)->_heat - (*it)->_max_heat <= time ) // 在时间结束前或时间结束时恰好机器人热量减小到最大热量
                    {
                        (*it)->_blood -= (*it)->_heat - (*it)->_max_heat ; // 每秒钟造成1点伤害
                        flag=1; // 判断血量下降的方式切换为：在时间结束前或时间结束时恰好机器人热量减小到最大热量
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
            // 遍历在同一段机器人枪口热量降低的时间内被摧毁的机器人
            for (auto it=reduce_heat_destroyed.begin();it!=reduce_heat_destroyed.end();it++)
                std::cout<<"D "<<it->second.first<<' '<<it->second.second<<std::endl; // 输出：D 所属队伍 机器人标识符
            reduce_heat_destroyed.clear(); // 清空在同一段机器人枪口热量降低的时间内被摧毁的机器人
        }
    }
    void command_A(int team,int id,int type) // 添加一个新的机器人（type=0为步兵，type=1为工程）
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
    void command_F(int team,int id,int damage) // 对该机器人造成damage点伤害
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
    void command_H(int team,int id,int add_heat) // 该机器人枪口热量上升指定值add_heat
    {
        auto it=find_working_robot(team,id); // 查找正常机器人
        if (it!=working.end()) // 找到机器人
        {
            bool flag=whether_Engineer((*it)->_max_heat); // 判断是否为工程
            if (flag==0) // 是步兵
                (*it)->_heat+=add_heat; // 该机器人枪口热量上升指定值add_heat
        }
    }
    void command_U(int team,int id,int lv) // 将该机器人设定为该等级lv
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
};

int main()
{
    Manager manager;
    int N; // 命令数
    std::cin>>N;
    int pre_time=0; // 上一次命令执行的时间
    int now_time=0; // 当前命令执行的时间
    for (int i=0;i<N;i++) // 命令执行N次
    {
        pre_time=now_time;
        std::cin>>now_time;
        int time=now_time-pre_time; // 时间差
        manager.reduce_heat(time); // 机器人枪口热量随时间降低
        char command; // 命令字
        std::cin>>command;
        int team; // 所属队伍
        std::cin>>team;
        int id; // 机器人标识符
        std::cin>>id;
        int param_3; // 第3个参数
        std::cin>>param_3;
        switch (command)
        {
            case 'A':
                manager.command_A(team,id,param_3); // 添加一个新的机器人（param_3=0为步兵，param_3=1为工程）
                break;
            case 'F':
                manager.command_F(team,id,param_3); // 对该机器人造成param_3点伤害
                break;
            case 'H':
                manager.command_H(team,id,param_3); // 该机器人枪口热量上升指定值param_3
                break;
            case 'U':
                manager.command_U(team,id,param_3); // 将该机器人设定为该等级param_3
                break;
            default:{}
        }
    }
    return 0;
}