# ICS_x86_PA

ICS x86 PA实验，以x86架构为核心实现了对计算机执行的模拟。
请勿将这份代码作为您的作业提交，后果自负。实验的要求每年都在变化，过往的实现参考价值有限。

项目结构：

```bash
icspa-public/
├── game                   // 包含游戏相关代码
├── include                // PA整体依赖的一些文件
│   ├── config.h          // 一些配置用的宏
│   └── trap.h            // trap相关定义，不可改动
├── kernel                 // 一个微型操作系统内核
├── Makefile               // 帮助编译和执行工程的Makefile
├── Makefile.git           // 和git有关的部分
├── nemu                   // NEMU
│   └── src
│       └── main.c        // NEMU入口
└── testcase               // 测试用例
└── scripts                // 框架代码功能脚本，其中包含objdump4nemu-i386反汇编工具
└── libs                   // 框架代码所使用的库，不可改动
└── docs                   // 包含本教程
```

- PA_1: 寄存器，alu，flu的模拟
- PA_2: 指令集的实现，ELF装载，调试器完善
- PA_3: Cache模拟，保护模式，分页
- PA_4: I/O中断，实际程序载入测试
