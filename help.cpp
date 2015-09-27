//Подключаем библиотеку hhctrl.lib к своему проекту
// добавить параметр в ини-файл, указывающий расположение HelpFile

#include <htmlhelp.h>

//В тексте программы вызов

    HtmlHelp(GetDesktopWindow(),
    "help.chm::/about servise config.htm",
    HH_DISPLAY_TOPIC,NULL);
 