#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	//манифест

#pragma once
#include <Windows.h>
#include <string>
#include <exception>
#include "App.h"

class App; //имя главного класса приложения

class Window_obj_init {
/*-------------------------------------------------------------
						Методы окна
--------------------------------------------------------------*/
public:
	Window_obj_init(); //конструктор: создаю окно - отлавливаю исключения
	~Window_obj_init() {} //деструктор
	void init_window(); //создаю окно 
	
	//статическая функция CALLBACK - передает вызов объектной функции CALLBACK
	static LRESULT CALLBACK application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
/*-------------------------------------------------------------
					    Настройки окна
--------------------------------------------------------------*/
private:
	const std::wstring name_class_window{ L"My_Window" }; //задаю имя класса окна
	const std::wstring name_header_window{ L"Пятнашки" }; //задаю заголовок окна
	HWND m_hwnd{}; //дескриптор главного окна приложения
	int width_window{ 405 }; //ширина окна
	int height_window{ 405 }; //высота окна
	const int ID_TIMER{ 1 }; //ID таймера - номер таймера
	
/*-------------------------------------------------------------
					   Методы приложения
--------------------------------------------------------------*/
public:
	LRESULT CALLBACK window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam); //объектная функция CALLBACK
	int Run(); //запуск оконного приложения - отлавливаю исключения
	
private:
	int run(); //запуск оконного приложения - вечный цикл обработки
/*-------------------------------------------------------------
					   Данные приложения
--------------------------------------------------------------*/
private:
App app;
};