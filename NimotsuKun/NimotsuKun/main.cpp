// NimotsuKun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;
enum Object {
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,
    OBJ_UNKNOW,
};

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main()
{
    Object* state = new Object[gStageWidth * gStageHeight];
    initialize(state, gStageWidth, gStageHeight, gStageData);
    while (true)
    {
        draw(state, gStageWidth, gStageHeight);
        if (checkClear(state, gStageWidth, gStageHeight)) {
            break;
        }
        cout << "a:left s:down w:right w:up. Command?" << endl;
        char input;
        cin >> input;
        update(state, input, gStageWidth, gStageHeight);
    }
    cout << "You win!" << endl;
    delete[] state;
    state = 0;
    return 0;
}

void initialize(Object* state, int w, int h, const char* stageData)
{
    const char* d = stageData;
    int x = 0, y = 0;
    while (*d != '\0')
    {
        Object t;
        switch (*d)
        {
        case '#': t = OBJ_WALL; break;
        case ' ': t = OBJ_SPACE; break;
        case 'o': t = OBJ_BLOCK; break;
        case 'O': t = OBJ_BLOCK_ON_GOAL; break;
        case '.': t = OBJ_GOAL; break;
        case 'p': t = OBJ_MAN; break;
        case 'P': t = OBJ_MAN_ON_GOAL; break;
        case '\n':
            x = 0;
            ++y;
            t = OBJ_UNKNOW;
            break;
        default: t = OBJ_UNKNOW; break;
        }
        ++d;
        if (t != OBJ_UNKNOW)
        {
            state[y * w + x] = t;
            ++x;
        }
    }
}

void draw(const Object* state, int w, int h)
{
    const char font[] = { ' ', '#', '.', 'o', 'O', 'p','P' };
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Object o = state[y * w + x];
            cout << font[o];
        }
        cout << endl;
    }
}

void update(Object* s, char input, int w, int h)
{
    int dx = 0;
    int dy = 0;
    switch (input)
    {
    case 'a':dx = -1; break;//左
    case 'd':dx = 1; break;//右
    case 'w':dy = -1; break;//上
    case 's':dy = 1; break;//下
    default:
        break;
    }
    //查找玩家坐标
    int i = 0;
    for (i = 0; i < w * h; ++i)
    {
        if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
            break;
        }
    }
    int x = i % w;
    int y = i / w;
    //移动后的坐标
    int tx = x + dx;
    int ty = y + dy;
    if (tx < 0 || tx >= w || ty < 0 || ty >= h)
    {
        return;
    }
    //移动判断
    int p = y * w + x;
    int tp = ty * w + tx;
    if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
    {
        //目标位置图标与玩家位置图标替换判断
        s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
    {
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || tx2 >= w || ty2 < 0 || ty2 >= h)
        {
            return;
        }
        int tp2 = (ty + dy) * w + (tx + dx);
        if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
        {
            s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool checkClear(const Object* s, int w, int h)
{
    for (int i = 0; i < w * h; ++i)
    {
        if (s[i] == OBJ_BLOCK)
        {
            return false;
        }
    }
    return true;
}
