#pragma once
#include <Windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <cmath>
#include <exception>
#include <stdexcept>
#include "citation.h"

/*-------------------------------------------------------------
			 Глобальная функция - создание окна
--------------------------------------------------------------*/
using Func = LRESULT(CALLBACK*) (HWND, UINT, WPARAM, LPARAM);

using WSTR = std::wstring;
using STR = std::string;

template<typename T>
HWND window_create(Func f, WSTR name_class, WSTR header_win, int width, int height, T *p_obj) {

	//создание класса окна и его заполнение
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = f;
	wc.lpszClassName = name_class.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	WSTR message{ L"Error, can't register class: " };
	message += name_class;
	STR mess(std::begin(message), end(message));

	if (!RegisterClassEx(&wc)) //регистрация класса окна
		throw std::runtime_error(mess.c_str());

	HWND m_hwnd = CreateWindowEx(0, name_class.c_str(),
		header_win.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2, //расположение окна в центре экрана по горизонтали
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2, //расположение окна в центре экрана по вертикали
		width, height, nullptr, nullptr, nullptr, p_obj);

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	width += width - (rect.right - rect.left);
	height += height - (rect.bottom - rect.top);

	SetWindowPos(m_hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

	message = L"Error, can't create window: ";
	message += header_win;
	mess = STR(std::begin(message), end(message));

	if (!m_hwnd)
		throw std::runtime_error(mess.c_str());

	return m_hwnd;
}

/*-------------------------------------------------------------
						   класс App
--------------------------------------------------------------*/
//макросы для определения координат мыши из библиотеки windowsx.h
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

enum class State {PLAY, VICTORY};

class App {
/*-------------------------------------------------------------
			 Обрабатываю события основного окна
--------------------------------------------------------------*/
public:
	App(); //конструктор
	void init(); //заполняю и перемешиваю массив
	void mix(); //перемешиваю массив
	bool move_square(int x, int y); //двигать фишку
	int wm_paint(HWND m_hwnd); //прорисовываю окно
	int wm_keydown(WPARAM wParam, HWND m_hwnd); //обрабатываю события клавиатуры
	int wm_mousemove(HWND m_hwnd); //включение фокуса мыши
	int wm_lbuttondown(LPARAM lParam, HWND m_hwnd); //обрабатываю левый клик мыши
	bool check_victory(); //проверяю решена ли головоломка
	
private:
	int arr[4][4]; //двумерный массив - клеточное поле
	const int ceil_size{ 100 }; //размер клетки
	const int cursor_size{ 80 }; //диаметр курсора
	int zero_y{ 3 }; //координаты пустой клетки - y
	int zero_x{ 3 }; //координаты пустой клетки - x
	int cursor_y{ 3 }; //координаты курсора - y
	int cursor_x{ 3 }; //координаты курсора - x
	int mix_count{ 1000 }; //задаю количество перемешиваний
	bool mouse_focus_state{ true }; //приоритет мыши над клавиатурой - если мышь в окне курсор гаснет
	State state{ State::PLAY }; //состояние приложения

/*-------------------------------------------------------------
				    Создание окна сообщения
--------------------------------------------------------------*/
public:
	void message_window_create();
	static LRESULT CALLBACK application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam);

/*-------------------------------------------------------------
					Контролы окна сообщения
--------------------------------------------------------------*/
private:
enum class CTL_ID { NEW_GAME_ID, EXIT_ID }; //контролы - кнопки
HWND h_btn_new{}; //хэндл кнопки Новая игра
HWND h_btn_exit{}; //хэндл кнопки Выход

/*-------------------------------------------------------------
					 Методы окна сообщения
--------------------------------------------------------------*/
public:
	void message_show(); //показ окна сообщения
	void generator_citation(); //случайный выбор цитаты
	int wm_paint_m(); //прорисовываю окно сообщения
	int wm_send_button(WPARAM wParam); //обрабатываю нажатие кнопки


/*-------------------------------------------------------------
					Данные окна сообщения
--------------------------------------------------------------*/
private:
	HWND h_wnd{}; //дескриптор окна сообщения
	WSTR name_class{L"Name_Message"}; //имя класса окна
	WSTR name_header{ L"Цитаты Виктора Олеговича Пелевина" }; //имя заголовка окна
	int width_window{ 1000 }; //ширина окна
	int height_window{ 500 }; //высота окна
	int n_str_citation{0}; //номер цитаты - которая будет выбрана случайно
};
