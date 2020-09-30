// NimotsuKun.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

void readFile(char** buffer, int* size, const char* filename);

//模拟二维数组,实现动态
template<class T>class Array2D {
public:
    Array2D() :mArray(0) {}
    ~Array2D() {
        delete[] mArray;
        mArray = 0;
    }
    void setSize(int x, int y) {
        mSizeX = x;
        mSizeY = y;
        mArray = new T[x * y];
    }
    //重载()
    T& operator()(int x, int y) {
        return mArray[y * mSizeX + x];
    }
    const T& operator()(int x, int y) const {
        return mArray[y * mSizeX + x];
    }
private:
    T* mArray;
    int mSizeX;
    int mSizeY;
};

//状态类
class State {
public:
    State(const char* stageData, int size);
    void update(char input);
    void draw() const;
    bool hasCleared() const;
private:
    enum Object {
        OBJ_SPACE,
        OBJ_WALL,
        OBJ_BLOCK,
        OBJ_MAN,

        OBJ_UNKNOWN,
    };
    void setSize(const char* stageData, int size);

    int mWidth;
    int mHeight;
    Array2D< Object > mObjects;
    Array2D< bool > mGoalFlags;
};

//-------------------------主函数-----------------------------------------
int main(int argc, char**argv)
{
    const char* filename = "stageData.txt";
    if (argc >= 2) {
        filename = argv[1];
    }
    char* stageData;
    int fileSize;
    readFile(&stageData, &fileSize, filename);
    if (!stageData) {
        cout << "error:stage file can't be read!" << endl;
        return 1;
    }
    State* state = new State(stageData, fileSize);

    //主循环
    while (true)
    {
        state->draw();
        //通关检测
        if (state->hasCleared()) {
            break;
        }
        cout << "A:left S:down D:right W:up. Command?" << endl;
        char input;
        cin >> input;
        state->update(input);
    }
    cout << "You win!" << endl;
    delete[] stageData;
    stageData = 0;
    return 0;

    //防止游戏结束直接退出
    while (true) {
        ;
    }
    return 0;
}

//--------------------------函数定义部分--------------------------------------
void readFile(char** buffer, int* size, const char* filename) {
    ifstream in(filename);
    if (!in) {
        *buffer = 0;
        *size = 0;
    }
    else {
        in.seekg(0, ifstream::end);
        *size = static_cast<int>(in.tellg());
        in.seekg(0, ifstream::beg);
        *buffer = new char[*size];
        in.read(*buffer, *size);
    }
}

State::State(const char* stageData, int size) {
    //确保容量
    setSize(stageData, size);
    //确保空间
    mObjects.setSize(mWidth, mHeight);
    mGoalFlags.setSize(mWidth, mHeight);
    //预设初始值
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            mObjects(x, y) = OBJ_WALL; //多余部分都设置为墙壁
            mGoalFlags(x, y) = false; //非终点
        }
    }
    int x = 0;
    int y = 0;
    for (int i = 0; i < size; ++i) {
        Object t;
        bool goalFlag = false;
        switch (stageData[i]) {
        case '#': t = OBJ_WALL; break;
        case ' ': t = OBJ_SPACE; break;
        case 'o': t = OBJ_BLOCK; break;
        case 'O': t = OBJ_BLOCK; goalFlag = true; break;
        case '.': t = OBJ_SPACE; goalFlag = true; break;
        case 'p': t = OBJ_MAN; break;
        case 'P': t = OBJ_MAN; goalFlag = true; break;
        case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break;
        default: t = OBJ_UNKNOWN; break;
        }
        if (t != OBJ_UNKNOWN) { //遇到未定义的元素值就跳过
            mObjects(x, y) = t; //写入
            mGoalFlags(x, y) = goalFlag;
            ++x;
        }
    }
}
void State::setSize(const char* stageData, int size) {
    mWidth = mHeight = 0; //初始化
    //当前位置
    int x = 0;
    int y = 0;
    for (int i = 0; i < size; ++i) {
        switch (stageData[i]) {
        case '#': case ' ': case 'o': case 'O':
        case '.': case 'p': case 'P':
            ++x;
            break;
        case '\n':
            ++y;
            //更新最大值
            mWidth = max(mWidth, x);
            mHeight = max(mHeight, y);
            x = 0;
            break;
        }
    }
}

void State::draw() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            Object o = mObjects(x, y);
            bool goalFlag = mGoalFlags(x, y);
            if (goalFlag) {
                switch (o) {
                case OBJ_SPACE: cout << '.'; break;
                case OBJ_WALL: cout << '#'; break;
                case OBJ_BLOCK: cout << 'O'; break;
                case OBJ_MAN: cout << 'P'; break;
                }
            }
            else {
                switch (o) {
                case OBJ_SPACE: cout << ' '; break;
                case OBJ_WALL: cout << '#'; break;
                case OBJ_BLOCK: cout << 'o'; break;
                case OBJ_MAN: cout << 'p'; break;
                }
            }
        }
        cout << endl;
    }
}

void State::update(char input) {
    //移动量变换
    int dx = 0;
    int dy = 0;
    switch (input) {
    case 'a': dx = -1; break; //向左
    case 'd': dx = 1; break; //右
    case 'w': dy = -1; break; //上。Y朝下为正
    case 's': dy = 1; break; //下。
    }

    int w = mWidth;
    int h = mHeight;
    Array2D< Object >& o = mObjects;
    //查找玩家的坐标
    int x, y;
    x = y = -1;
    bool found = false;
    for (y = 0; y < mHeight; ++y) {
        for (x = 0; x < mWidth; ++x) {
            if (o(x, y) == OBJ_MAN) {
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }
    //移动后的坐标
    int tx = x + dx;
    int ty = y + dy;
    //判断坐标
    if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
        return;
    }
    //该方向上是空白或者终点,则移动
    if (o(tx, ty) == OBJ_SPACE) {
        o(tx, ty) = OBJ_MAN;
        o(x, y) = OBJ_SPACE;
        //该方向上是箱子,并且该方向的下下个格子是空白或者终点,则允许移动
    }
    else if (o(tx, ty) == OBJ_BLOCK) {
        //检测同方向上的下下个格子是否位于合理值范围
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //按键无效
            return;
        }
        if (o(tx2, ty2) == OBJ_SPACE) {
            //按顺序替换
            o(tx2, ty2) = OBJ_BLOCK;
            o(tx, ty) = OBJ_MAN;
            o(x, y) = OBJ_SPACE;
        }
    }
}

//只要还存在一个goalFlag值为false就不能判定为通关
bool State::hasCleared() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            if (mObjects(x, y) == OBJ_BLOCK) {
                if (mGoalFlags(x, y) == false) {
                    return false;
                }
            }
        }
    }
    return true;
}