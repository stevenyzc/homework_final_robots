#ifndef ROBOTS_4_1_6_ROBOTS_H
#define ROBOTS_4_1_6_ROBOTS_H

// Robot抽象类
class Robot {
private:
    int _max_blood=0; // 血量上限
    int _max_heat=0; // 热量上限
    int _blood=0; // 血量
    int _heat=0; // 热量
    int _team=0; // 所属队伍
    int _id=0; // 机器人标识符
public:
    virtual void func() = 0; // 构造纯虚函数，使Robot成为抽象类
    virtual ~Robot()=default; // 编译器自动生成虚析构函数
    // 设置6个参数的值
    void set_max_blood  (int max_blood) {_max_blood = max_blood ; }
    void set_max_heat   (int max_heat ) {_max_heat  = max_heat  ; }
    void set_blood      (int blood    ) {_blood     = blood     ; }
    void set_heat       (int heat     ) {_heat      = heat      ; }
    void set_team       (int team     ) {_team      = team      ; }
    void set_id         (int id       ) {_id        = id        ; }
    // 输出6个参数的值
    int get_max_blood() const { return _max_blood; }
    int get_max_heat () const { return _max_heat ; }
    int get_blood    () const { return _blood    ; }
    int get_heat     () const { return _heat     ; }
    int get_team     () const { return _team     ; }
    int get_id       () const { return _id       ; }
};

// 步兵子类
class Infantry : virtual public Robot {
public:
    void func() override {} // 重写纯虚函数以实例化对象
    Infantry()=default; // 编译器自动生成默认构造函数
    Infantry(int team,int id) // 初始化机器人有参构造
    {
        set_max_blood(100);
        set_max_heat(100);
        set_blood(100);
        set_heat(0);
        set_team(team);
        set_id(id);
    }
    Infantry(int max_blood,int max_heat,int blood,int heat,int team,int id) // 完整机器人属性有参构造
    {
        set_max_blood(max_blood);
        set_max_heat(max_heat);
        set_blood(blood);
        set_heat(heat);
        set_team(team);
        set_id(id);
    }
};

// 工程子类
class Engineer : virtual public Robot {
public:
    void func() override {} // 重写纯虚函数以实例化对象
    Engineer()=default; // 编译器自动生成默认构造函数
    Engineer(int team,int id) // 初始化机器人有参构造
    {
        set_max_blood(300);
        set_max_heat(0);
        set_blood(300);
        set_heat(0);
        set_team(team);
        set_id(id);
    }
    Engineer(int max_blood,int max_heat,int blood,int heat,int team,int id) // 完整机器人属性有参构造
    {
        set_max_blood(max_blood);
        set_max_heat(max_heat);
        set_blood(blood);
        set_heat(heat);
        set_team(team);
        set_id(id);
    }
};

#endif //ROBOTS_4_1_6_ROBOTS_H
