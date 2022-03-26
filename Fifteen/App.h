#pragma once
#include <Windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <cmath>

//макросы для определения координат мыши из библиотеки windowsx.h
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

class App {
public:
	void init(); //заполняю массив
	void mix(); //перемешиваю массив
	bool move_square(int x, int y); //двигать фишку
	int wm_paint(HWND m_hwnd); //прорисовываю окно
	int wm_keydown(WPARAM wParam, HWND m_hwnd); //обрабатываю события клавиатуры
	int wm_mousemove(HWND m_hwnd); //включение фокуса мыши
	int wm_lbuttondown(LPARAM lParam, HWND m_hwnd); //обрабатываю левый клик мыши

private:
	int arr[4][4]; //двумерный массив - клеточное поле
	const int ceil_size{ 100 }; //размер клетки
	const int cursor_size{ 80 }; //диаметр курсора
	int zero_y{ 3 }; //координаты пустой клетки - y
	int zero_x{ 3 }; //координаты пустой клетки - x
	int cursor_y{ 3 }; //координаты курсора - y
	int cursor_x{ 3 }; //координаты курсора - x
	bool mouse_focus_state{ true }; //приоритет мыши над клавиатурой - если мышь в окне курсор гаснет
};
