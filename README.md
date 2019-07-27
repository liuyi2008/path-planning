zig-zag.cpp
在凸多边形实现zig-zag填充，多边形是鼠标点出坐标点后生成
PolyScan()、PolyScan1()函数等，里面大体相同，除了末尾的点的排列方式不同，主要用来画水平线，左边的线，右边的线。

triangle.cpp
新的变化填充曲线 三角变化填充线

square.cpp
正方形变形填充曲线

hexagon.cpp
六边形变化填充曲线，但是理论上不成立

triangle to gcode.cpp
生成Gcode文件，但是不成熟，仅作参考，gcode只适合makerbot replicator 2