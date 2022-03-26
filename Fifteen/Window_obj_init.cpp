#include "Window_obj_init.h"

Window_obj_init::Window_obj_init(){ //конструктор - создание окна - блок исключений
	using std::wstring;
	using std::string;

	try {
		init_window();
	}
	catch (const std::exception& e) {
		string expt_data = e.what();
		MessageBox(nullptr, wstring(begin(expt_data),
			end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}
}


void Window_obj_init::init_window() {
	//создание класса окна и его заполнение
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(nullptr); //GetModuleHandle возвращает дескриптор файла, используемого для создания вызывающего процесса.
	wc.lpfnWndProc = Window_obj_init::application_proc;
	wc.lpszClassName = name_class_window.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) //регистрация класса окна
		throw std::runtime_error("Error, can't register main window class!");

	m_hwnd = CreateWindowEx(0, name_class_window.c_str(),
		name_header_window.c_str(), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ,
		(GetSystemMetrics(SM_CXSCREEN) - width_window) / 2, //расположение окна в центре экрана по горизонтали
		(GetSystemMetrics(SM_CYSCREEN) - height_window) / 2, //расположение окна в центре экрана по вертикали
		width_window, height_window, nullptr, nullptr, nullptr, this);
	
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	width_window += width_window - (rect.right - rect.left);
	height_window += height_window - (rect.bottom - rect.top);

	SetWindowPos(m_hwnd, HWND_TOP, 0, 0, width_window, height_window, SWP_NOMOVE | SWP_NOZORDER);


	if (!m_hwnd)
		throw std::runtime_error("Error, can't create main window!");
}


int Window_obj_init::Run() { //вечный цикл - отлавливаю исключения
	try {
		return run();
	}
	catch (const std::exception& e) {
		std::string expt_data = e.what();
		MessageBox(nullptr, std::wstring(begin(expt_data),
			end(expt_data)).c_str(), L"Ошибка", MB_ICONERROR | MB_OK);
		ExitProcess(EXIT_FAILURE);
	}

}


int Window_obj_init::run() { //цикл обработки сообщений
	MSG msg{};

	app.init();

	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //первый показ окна
	UpdateWindow(m_hwnd); //обновление окна
	

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

//статическая функция обратного вызова
LRESULT Window_obj_init::application_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window_obj_init* p_wobj;
	if (uMsg == WM_NCCREATE) {
		p_wobj = static_cast<Window_obj_init*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_wobj))) {
			if (GetLastError() != 0)
				return false;
		}
	}
	else {
		//достаю указатель на объект "Window_obj_init" помещенный туда при конструировании объекта
		p_wobj = reinterpret_cast<Window_obj_init*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (p_wobj) {
		p_wobj->m_hwnd = hwnd;
		return p_wobj->window_proc(uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//объектная функция обратного вызова
LRESULT Window_obj_init::window_proc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;
		HBRUSH  brush, old_brush;
		HFONT hFont, old_hFont;
		std::wstring t;

		switch (uMsg)
		{
		case WM_PAINT:
			return app.wm_paint(m_hwnd);

		case WM_KEYDOWN:
			return app.wm_keydown(wParam, m_hwnd);

		case WM_MOUSEMOVE:
			return app.wm_mousemove(m_hwnd);

		case WM_LBUTTONDOWN:
			return app.wm_lbuttondown(lParam, m_hwnd);

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}