##### 设计思路

大循环的主要逻辑如下：

首先我们读入本局进行的关卡，为了保证程序具有鲁棒性，采用字符串匹配的方式判断输入。

```C++
std::string level_;
do {
    do {
        std::cout << "请输入本局您希望进行的关卡" << std::endl;
        std::getline(std::cin, level_);
    } while (level_ != std::to_string(atoi(level_.c_str())) or
             std::to_string(record->card[record_id].level) < level_);
} while (std::find(config.begin(), config.end(), level_) == config.end());
clear_screen();
int level = atoi(level_.c_str());
```

接下来我们需要读入对应关卡配置。我们定义了两个匿名函数用于处理读入的逻辑：

```c++
auto readJsonFile = [](const std::string &filename) -> Json::Value {...};
auto read = [&readJsonFile](int level, std::vector<int> &input, std::vector<int> &output, int &ground, std::vector<std::string> &instructions) -> auto {...};
```

并从json文件读入了游戏配置。我们将输入、输出、可用指令保存在列表中。

```c++
// 读入输入输出、地面的大小、可用的指令
auto p = read(level, input, output, ground, available_instructions);
```

有两种读入指令的方式：从输入和从文件。

##### 工程结构

工程结构如图所示，各个部分的内容有：

```
|   .gitignore
|   CMakeLists.txt
|   README.md
+---ans					关卡答案
|       1
|       2
|       3
|       4
|       5
|       6
|       7
|       
+---bin					编译后的二进制文件
|       game.exe
|       
|               
+---include				头文件
|       debug.h			用于debug的头文件
|       hrm_gui.h		绘制图形界面的头文件
|       json-forwards.h	读取json文件
|       json.h			读取json文件
|       utils.h			定义游戏中所有类
|       
+---level				关卡信息
|       1.json
|       2.json
|       3.json
|       4.json
|       5.json
|       6.json
|       7.json
|       
+---record				游戏记录
|       1.rec
|       2.rec
|       3.rec
|       Template.rec
|       
\---src					所有游戏代码
        hrm_gui.cpp		绘制图形界面、执行主体逻辑
        jsoncpp.cpp		读取json文件
        main.cpp		游戏入口
```

##### 整体游戏界面的设计

进入游戏界面：包含选择存档、选择是否新建存档。

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010148.png)

选择关卡界面：绿色表示已经通过的关卡，红色表示正在进行中的关卡，灰色表示还没有解锁的关卡。

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010931.png)

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010153.png)

输入指令界面：每个关卡有详细的指示，包括本关卡的目标、可用的指令、输入输出序列、可用空地。

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010232.png)

游戏进行界面：顶部是游戏单步进行和连续执行的对话框，输入0/1表示希望的操作。左、右的箱子为输入和输出序列，中间的箱子为空地。画面最右边还有执行中的指令序列，正在执行的下一条指令前有星号标记。

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010243.png)

闯关结果界面

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010250.png)

![屏幕截图 2024-01-06 010148](media/屏幕截图 2024-01-06 010311.png)

##### 游戏测试

##### 自由创新关卡

##### 特色功能

我们制作了一个游戏存档功能，能够分别保存三位不同玩家的性别（头像）、名字、闯关进度。同时支持新建玩家与替换原有玩家。

##### 小组分工