#ifndef HISTORYACTION_H
#define HISTORYACTION_H

#include <QDebug>
#include <QStack>

template<typename Type>
class HistoryAction
{

private:
    int length = 0;
    QStack<Type> now_history;    //当前记录
    QStack<Type> undo_history;    //撤回记录

public:
    HistoryAction() {}

    /// 函数功能: 访问记录的位置
    /// index: 访问的位置
    /// 返回: 改位置的对象
    Type at(const int &index) const
    {
        return now_history.at(index);
    }

    /// 函数功能: 获取历史记录长度
    /// 返回: 当前记录的长度
    int getLength() const
    {
        return length;
    }

    /// 函数功能: 添加操作记录
    /// action: 需要被记录的操作
    void append(const Type &action)
    {
        now_history.push(action);
        length++;
        undo_history.clear();
    }

    /// 函数功能: 撤回记录
    /// 返回: 被撤回的操作
    Type undo(bool &is_success = false)
    {
        Type action;
        is_success = now_history.isEmpty() == false;
        if (is_success)
        {
            action = now_history.pop();
            undo_history.push(action);
            length--;
        }
        return action;
    }

    /// 函数功能: 恢复记录
    /// 返回: 被恢复的操作
    Type reload(bool &is_success = false)
    {
        Type action;
        is_success = undo_history.isEmpty() == false;
        if (is_success)
        {
            action = undo_history.pop();
            now_history.push(action);
            length++;
        }
        return action;
    }
};

#endif // HISTORYACTION_H
