# elog
efficient log library

## 设计原则
- 高效
- 可变
- 可控
- 无锁

采用每cpu工作，可选排序/乱序日志，可统一生成单日志文件，也可以每cpu日志文件，
单日志文件大小上限可设置，日志创建规则可设置。

![img_1.png](z-img%2Fimg_1.png)
![img.png](z-img%2Fimg.png)