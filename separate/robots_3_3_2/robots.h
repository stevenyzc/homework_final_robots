#ifndef ROBOTS_3_3_2_ROBOTS_H
#define ROBOTS_3_3_2_ROBOTS_H

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

#endif //ROBOTS_3_3_2_ROBOTS_H
