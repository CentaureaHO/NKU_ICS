# ICS2017 Programming Assignment

This project is the programming assignment of the class ICS(Introduction to Computer System) in Department of Computer Science and Technology, Nanjing University.

For the guide of this programming assignment,
refer to http://nju-ics.gitbooks.io/ics2017-programming-assignment/content/

To initialize, run
```bash
bash init.sh
```

The following subprojects/components are included. Some of them are not fully implemented.
* [NEMU](https://github.com/NJU-ProjectN/nemu)
* [Nexus-am](https://github.com/NJU-ProjectN/nexus-am)
* [Nanos-lite](https://github.com/NJU-ProjectN/nanos-lite)
* [Navy-apps](https://github.com/NJU-ProjectN/navy-apps)

# Git提交记录

由于在2025年3月3日下午使用--allow-empty后出现了仓库损坏问题，因此后续手动提交时避免使用此参数，在此次进行记录：  

手动提交节点：
* 25.03.03:22:05: 修改./nemu/src/monitor/debug/expc.c，添加token定义与词法匹配，尚未完整测试；