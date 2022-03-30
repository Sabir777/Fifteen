#include "Window_obj_init.h"

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
	
	Func f = Window_obj_init::application_proc;
	using WOB = Window_obj_init;

	m_hwnd = window_create<WOB>(f, name_class_window, name_header_window,
								width_window, height_window, this);
	
	ShowWindow(m_hwnd, SW_SHOWDEFAULT); //первый показ окна
	UpdateWindow(m_hwnd); //обновление окна
	
	MSG msg{};

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
		/*
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;
		HBRUSH  brush, old_brush;
		HFONT hFont, old_hFont;
		std::wstring t;
		*/

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