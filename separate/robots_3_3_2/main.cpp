#include "manager.h"

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
