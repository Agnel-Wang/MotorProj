# Author: _Agnel_ &emsp; for Motor Driver

## 待做

+ 更改串口屏程序
+ 电磁阀舵机程序
+ CAN1也加入报文列表

## 注意事项

+ 一个主控板不要控制太多
+ 电机结构体里面，虽然大体参数相同，但还是有些小差别，所以每种都是分开写的
+ 程序里面电机过多，所以一定要通过初定义进行注释，否则易出现中断堵塞
+ 大疆电机结构体里面是value, ELMO和VESC是val，懒得改了
+ 实话说，流程号真没用过，以前看过现在也忘的差不多了，不过它的设计初衷还是很有意义
+ TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M。
而配置里面的都是按照72Mhz计算的'虚假'1ms，实际上是0.857ms，历史遗留问题了，关键有CAN出队函数我现在也没时间去修改这个
+ 未防止电机参数的频繁修改，在各种motorInit函数里面通过预定义将参数初始化设定可好多遍，虽然有些麻烦，但暂时这样没多问题

### DJ电机相关事项

+ DJ电机电流是一起发的，包括所有电机。所以多个板子控制电机的时候不要将can2线连在一起
+ DJ电机开始位置记录因为在`pulse_caculate()`函数里面开始distance会先运行一次才会在对`valuePrv`进行赋值，所以会有`0~8192`的脉冲误差
+ 注意DJ电机回去其报文以及反馈的时候需要进行一次`ChangeData()`
+ motor.status.overspeed暂时没用

### ELMO相关事项

+ 电机模式枚举体因ELMO的电流、速度、位置为固定1，2，5，所以别改顺序
+ ELMO的变参函数虽然感觉很骚，但用着似乎有些麻烦，不方便，参数格式记不住
+ ELMO直接命令赋值所以很多参数、函数定义的都没必要
+ 关于ElmoCANopen协议详见文档9.1.1.1
+ 动作执行放在了TIM2里面，严格定时
+ enable只是一个状态

### VESC相关事项

+ VESC[Github](github.com/vedderb)地址
+ VESC[论坛](vesc-project.com/vesc_tool)地址
+ VESC需要定时赋值否则会自动释放
+ VESC暂时只用到了速度模式，所以程序基本没写多少

---  

## 程序配置

### marco.h

1. 选择车类型
2. 选择为转向电机程序或上层机构程序（T车今年没有转向电机）
3. 选择使用哪些电机

### motor.h / vesc.h / elmo.h

在初始化里面设置使用哪些编号的电机及模式

---  

## 中断优先级

|中断/事件|抢占优先级|响应优先级|用途|
|:-:|:-:|:-:|:-:|
|TIM3_IRQn|1|0|DJ电机驱动|
|TIM2_IRQn|1|1|VESC电机驱动|
|CAN2_RX0_IRQn|2|0|DJ报文获取|
|CAN2_RX1_IRQn|2|1|ELMO/VESC报文获取|
|CAN1_RX0_IRQn|3|0|主控|
|USART2_IRQn|4|1|串口屏|
|USART2_DMA|7|1||
|USART1_DMA|5|1|虚拟示波器|
|USART3_IRQn|4|0|编码器|

---  

## 机构执行动作分配

### DJ电机

#### P车上层机构

|电机序号|用途|电机序号|用途|
|:-:|:-:|:-:|:-:|
|||||

#### T车上层机构

|电机序号|用途|电机序号|用途|
|:-:|:-:|:-:|:-:|
|||||

### ELMO

### VESC

### 电磁阀

||||
|:-:|:-:|:-:|
1|手爪|PA6  
0|气动手指 | PA7

---  

## 问题记录

+ __DATA:__  2020.08.12  
  __问题描述:__  `motor[id].valuePrv.pulseRead≠motor[id].valueReal.pulseRead`致使`motor[id].valueReal.pulse`一直在变化  
  __解决方法:__ 不能将 `motor[id].valuePrv=motor[id].valueReal;` 赋值语句放在can中断里面，应放在tim中断

+ __DATA:__  2020.07.25  
  __问题描述:__ 运行后wheel.BG值不改变  
  程序修改后未编译进入Debug运行的是未修改的程序，但却显示修改之后的代码，理应注意。及时编译

+ __DATA:__  2020.07.21  
  __问题描述:__ 程序功能选择中，想用到const，但是忘了预处理命令是在程序运行之前定义的，所以只能用#define，#ifndef  

+ __DATA:__  2020.07.20  
  __问题描述：__  显示 *.h not found  
  __解决方法：__ **已解决**  
  这个在设置里面clang.diagnostic.enable关了就好  

+ __DATA:__  2020.07.20  
  __问题描述：__  settting.jdon内容莫名清空  
  __所做尝试：__  还不清楚问什么，只能重新添加  
  __解决方法：__ **未解决**  

+ __DATA:__ 2020.07.20  
  __问题描述：__  编译后突然出现数十errors与warnings  
  __所做尝试：__  原来是修改程序改太多忘记编译错误累计太多了，吓死了，还以为什么呢  
  __解决方法：__ **已解决**  

+ __DATA:__ 2020.07.20  
  __问题描述：__ 为什么变参函数va_arg的type变量在VSCode上说要输入表达式以及不允许使用类型名？keil却不报错？  
  __所做尝试：__  

  1. 在.json文件里面将编译器从gcc改为clang  **（未解决）**  
   观察头文件keil里面是define了clang，VSCode里面没有，两者用的是两个函数。  
   可是这似乎并没有什么关系啊，参数一样的，并不关乎表达式  
  2. 试着将va_arg()变为__builtin_va_arg()，同时根据提示将va_list()也变为__builtin_va_list()，果然这次不报错了。  
    所以最根本的问题还是未定义_clang_  

  __解决方法：__ **已解决**  
&emsp; 真慌，在setting.json里面加各种clang配置却始终还是在stdarg.h里面没有__clang__的宏定义，最后在c_cpp_properities.json的define里面手动添加__clang__的宏定义才解决了  

---

## 积极工作记录

### 2020.08.18

1. 好久没用ELMO，自己写的格式都忘了，InConGrpFlag在第三位，记成最后一位了

### 2020.08.13

1. 关于P车的转向电机程序，艾云飞的CAN报文是和杨震匹配的。锁点是通过手动设置当前位置为位置模式的设定位置。不过这种方式有一个缺陷，将位置的设定值修改了，下次跑位置就需要重新对valueSet.angle进行赋值，而不是恢复原来的现场。  
借鉴ELMO的BG，通过设置一个begin的标志位，可以实现这种方式。  
但目前通信协议里面，原来的是一直在MO=1，无关系；现在如果ST后，begin=0；之后赋位置不去设置begin=1是不会动的，暂时将它加到了0x320指令中
2. 将大疆电机的currentInput(i)提出到模式运行外，避免过多无意义的重复电流赋值发送，缓解CAN2压力
3. 学到一波新知识，底盘转向电机单纯在码盘的作用下四个电机转向角度一致，这是用于X、Y方向，如果加上陀螺仪，这是用于自身旋转，那么0x320所赋的电机角度就不一样了，会有差异。

### 2020.07.31

1. 从零开始，一个新的旅程。

### 2020.07.26

1. 调DT的时候迷上寄存器了

### 2020.07.25

1. 达成VESC成就

### 2020.07.22

1. 取了杨震手柄，好骚啊

### 2020.07.21

1. 这个无法代码自动补全的一直解决不了，烦
2. 完成了P车上三个M2006的任务，论写代码有多么快乐
3. 这也太惨了吧，晚上加班工作，只带了神奇小本回去，这才发现原来VSCode的同名高亮竟然是有的，还算清晰。可我睁大眼睛仔细寻找，没错，这显示屏上啥都没变化，不愧是你。  

### 2020.07.20

1. 解决va_arg()报错问题
2. 这，吐了。就添加个lcd不知道怎么83个错误，这程序你没了  
   我去，懵了。原来是settting.jdon里面配置全没了。不知道是不是自己刚才拔线的问题。心态炸裂  
   呼，解决了，问题不大，不慌。不是setting的事，但它确实没了  

### version 3.0.1 2020.07.20  

1. 完善README
2. 单独添加了VESC模块，以后用VESC就用新写的代码了，不用之前的

### version 3.0.0 2020.07.19  

1. 将程序移植到了VSCode上
2. 初步根据杨震的程序添加了marco和lcd文件
3. 自行添加了README文件  

### version 2.2.0  

+ 改善ELMO反馈  

### version 2.1.0  

+ 增加大编码器接收数据模块  

### version 1.6.0  

+ 增加CAN1与主控通信报文  

### version 1.5.0  

+ 实现ELMO变参函数的更改  

### version 1.4.0  

+ 增加了流程号  

### version 1.3.0  

+ 增加了虚拟示波器  

### version 1.2.0  

+ 增加了延时检测  

### version 1.1.0  

+ 程序中添加了ELMO模块  
