/*��� ������ ��������� � 2 ������.
��������� ������, ������������� ������ ���������, ����
��������� ENG_LANG, ��� ������, ���� �� ���������
*/

#ifndef __LANG_STR_H__
#define __LANG_STR_H__

#ifdef ENG_LANG
#define LSTR(E, R) E
#else
#define LSTR(E, R) R
#endif
#endif