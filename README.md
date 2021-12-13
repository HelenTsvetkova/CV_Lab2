
Для выполнения разных частей лабы можно менять переменную labPart, чтобы выполнялись разные cases.
```C
enum {
    BOXFILTER,
    GAUSSIAN,
    UNSHARPMASK_G_BOX,
    LAPLACIAN,
    UNSHARPMASK_l
};

int labPart = UNSHARPMASK_l;
```
