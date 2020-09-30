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
    initialize(state, gStageWidth, gStageHeight, gStateData);
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
            Object o = state[y * width + x];
            cout << font[o];
        }
        cout << endl;
    }
}