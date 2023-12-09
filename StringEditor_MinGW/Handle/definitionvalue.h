#ifndef DEFINITIONVALUE_H
#define DEFINITIONVALUE_H


/* 数据库表格显示R数据 */
typedef struct TableData
{
    bool is_updated;    //是否更新过
    QString path;    //路径
    qlonglong index;    //位置
    QString replace_str;    //修改字符串
    QString source_str;    //原字符串
} TableData;

/* 用户操作动作 */
typedef struct UserAct
{
    short type;
    QString value1;
    QString value2;
    UserAct(const short &user_type, const QString &v1, const QString &v2) :
        type(user_type), value1(v1), value2(v2) {}
} UserAct;


/* 用户操作, 用于撤回与恢复 */
#define TABLE_CHANGE              0         //表格修改
#define REPLACE_NOW               1         //当前表格替换修改
#define REPLACE_ALL               2         //所有字符串替换修改

#define PREVIOUS                  0         //上一个操作
#define NEXT                      1         //下一个操作


/* 打开文件和sql属性 */
#define FILE                      0          //代码文件属性
#define FOLDER                    1          //文件夹属性
#define MEAT_DATA                 2          //meat_data属性
#define JSON                      3          //json属性


/* 主线程操作模式 */
#define OPEN_FILE                 0          //打开文件模式
#define OPEN_FOLDER               1          //打开文件夹模式
#define OPEN_JSON                 2          //打开Json模式
#define OPEN_MEAT                 3          //打开Meat-Data模式

#define EXPORT_FILE                 4          //导出文件模式
#define EXPORT_FOLDER               5          //导出文件夹模式
#define EXPORT_JSON                 6          //导出JSON模式
#define EXPORT_MEAT                 7          //导出Meat-Data模式

#endif // DEFINITIONVALUE_H
