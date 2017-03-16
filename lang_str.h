/*ƒл€ работы программы в 2 €хыках.
ќпределен макрос, подставл€юший первое вырадение, если
определен ENG_LANG, или второе, если не определен
*/

#ifndef __LANG_STR_H__
#define __LANG_STR_H__

#ifdef ENG_LANG
#define LSTR(E, R) E
#else
#define LSTR(E, R) R
#endif
#endif