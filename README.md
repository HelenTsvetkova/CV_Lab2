
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

На вход программе нужно передавать аргумент: путь к фотографии, которую собираемся фильтровать. В репозиторий закинула тестовое фото
