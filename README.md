# snake
C++ Linux CMD Snake Game，C++ 贪吃蛇游戏

CMD下的游戏，练手项目，由于没有使用任何CMD的图像库，全部都是手工捕获、刷新、定位输出，所以兼容性不敢保证，仅在 Ubuntu16.04和18.04 运行下没有问题

编译：
g++ -std=c++0x  main.cpp Game.cpp Snake.cpp  -lpthread -o Snake
