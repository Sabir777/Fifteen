#include "App.h"

int App::wm_paint(HWND m_hwnd) {
	
	PAINTSTRUCT ps;
	RECT rect;
	HDC hDC = BeginPaint(m_hwnd, &ps); //получаю контекст устройства
	GetClientRect(m_hwnd, &rect); //получаю ширину и высоту клиентской области

	/*------------------------рисую фон---------------------------*/
	HBRUSH brush = CreateSolidBrush(RGB(25, 25, 61)); //создаю кисть для фона
	HBRUSH old_brush = (HBRUSH)SelectObject(hDC, brush); //устанавливаю кисть в контекст устройства
	FillRect(hDC, &rect, brush); //закрашиваю фон

	/*----------------------рисую пятнашки------------------------*/
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(188, 32, 212)); //создаю карандаш
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen); // Выбираю свой Pen в DC, запоминаю старый Pen

	brush = CreateSolidBrush(RGB(84, 14, 35));//создаю кисть для квадратов
	SelectObject(hDC, brush); 
	const int size_number_cells = 4; //количество ячеек на игровом поле

	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold"); //создаю новый шрифт
	HFONT old_hFont = (HFONT)SelectObject(hDC, hFont); //устанавливаю шрифт в контекст устройства
	SetTextColor(hDC, RGB(250, 174, 22)); //устанавливаю цвет текста
	SetBkColor(hDC, RGB(84, 14, 35)); //устанавливаю цвет фона под текстом

	for(int j = 0; j < size_number_cells; ++j)
		for (int i = 0; i < size_number_cells; ++i)
			if (arr[j][i]) {
				int left = i * ceil_size + i + 1;
				int right = left + ceil_size - 1;
				int top = j * ceil_size + j + 1;
				int bottom = top + ceil_size - 1;
				Rectangle(hDC, left, top, right, bottom);

				RECT rc{ left - 2, top, right, bottom + 4 };
				DrawText(hDC, std::to_wstring(arr[j][i]).c_str(), -1, &rc, 
					DT_SINGLELINE | DT_CENTER | DT_VCENTER); //вывожу номер пятнашки
			}

	/*----------------------рисую курсор------------------------*/
	if (!mouse_focus_state) { //если нет мыши в фокусе - рисую курсор

		hPen = CreatePen(PS_SOLID, 5, RGB(38, 245, 34)); //создаю зеленый карандаш
		SelectObject(hDC, hPen);

		//удаляю свою кисть - устанавливаю прозрачную кисть
		DeleteObject(SelectObject(hDC, GetStockObject(NULL_BRUSH)));

		//рисую круг курсора
		int i{ cursor_x }, j{ cursor_y };
		int left = i * ceil_size + i + 1;
		int top = j * ceil_size + j + 1;

		//задаю квадрат для курсора, определяющий размер круга
		int left_cursor = left + (ceil_size - cursor_size) / 2;
		int right_cursor = left_cursor + cursor_size - 1;
		int top_cursor = top + (ceil_size - cursor_size) / 2;
		int bottom_cursor = top_cursor + cursor_size - 1;
		Ellipse(hDC, left_cursor, top_cursor, right_cursor, bottom_cursor);
		
		SelectObject(hDC, old_brush); //устанавливаю в контекст стоковую кисть 
	}
	else {
		DeleteObject(SelectObject(hDC, old_brush)); //удаляю созданную кисть
	}

	/*-----------------Удаляю использованные объекты------------------*/
	DeleteObject(SelectObject(hDC, hOldPen)); // Удаляю Pen
	DeleteObject(SelectObject(hDC, old_hFont)); //удаляю созданный шрифт
	
	EndPaint(m_hwnd, &ps); //освобождаю контекст устройства
	return 0;
}

void App::init() {
	const int number_cells{ 4 };
	int count{ 0 };
	for(int j = 0; j < number_cells; ++j)
		for (int i = 0; i < number_cells; ++i)
			arr[j][i] = (++count == 16) ? 0 : count;
	mix();
	cursor_y = zero_y;
	cursor_x = zero_x;
}

void App::mix() { //перемешиваю пятнашки
	srand(time(NULL));
	for (int i = 0; i < 1000; ++i) {
		/*-----------------угловые пустые клетки----------------*/
		if (zero_x == 0 && zero_y == 0) {
			if (rand() % 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}
			else {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}

		}
		else if (zero_x == 3 && zero_y == 0) {
			if (rand() % 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}
			else {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}

		}
		else if (zero_x == 3 && zero_y == 3) {
			if (rand() % 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}
			else {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}

		}
		else if (zero_x == 0 && zero_y == 3) {
			if (rand() % 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}
			else {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}

		}
		/*-----------------боковые пустые клетки----------------*/
		else if (zero_x == 0) {
			int var_x = 1 + rand() % 3;
			if (var_x == 1) {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}
			else if (var_x == 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}
			else if (var_x == 3) {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}
		}
		else if (zero_y == 0) {
			int var_x = 1 + rand() % 3;
			if (var_x == 1) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}
			else if (var_x == 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}
			else if (var_x == 3) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}
		}
		else if (zero_x == 3) {
			int var_x = 1 + rand() % 3;
			if (var_x == 1) {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}
			else if (var_x == 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}
			else if (var_x == 3) {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}
		}
		else if (zero_y == 3) {
			int var_x = 1 + rand() % 3;
			if (var_x == 1) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}
			else if (var_x == 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}
			else if (var_x == 3) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}
		}
		/*-----------------средние пустые клетки----------------*/
		else {
			int var_x = 1 + rand() % 4;
			if (var_x == 1) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x - 1]);
				--zero_x;
			}
			else if (var_x == 2) {
				std::swap(arr[zero_y][zero_x], arr[zero_y - 1][zero_x]);
				--zero_y;
			}
			else if (var_x == 3) {
				std::swap(arr[zero_y][zero_x], arr[zero_y][zero_x + 1]);
				++zero_x;
			}
			else if (var_x == 4) {
				std::swap(arr[zero_y][zero_x], arr[zero_y + 1][zero_x]);
				++zero_y;
			}
		}
	}		
}
	

int App::wm_keydown(WPARAM wParam, HWND m_hwnd) {
	mouse_focus_state = false; //выключаю фокус мыши при нажатии клавиши

	int static old_cursor_x, old_cursor_y;

	switch (wParam)
	{
		case VK_UP: //стрелка вверх
			cursor_y = (cursor_y == 0) ? 0 : cursor_y - 1;
			break;

		case VK_DOWN: //стрелка вниз
			cursor_y = (cursor_y == 3) ? 3 : cursor_y + 1;
			break;

		case VK_LEFT: //стрелка влево
			cursor_x = (cursor_x == 0) ? 0 : cursor_x - 1;
			break;

		case VK_RIGHT: //стрелка вправо
			cursor_x = (cursor_x == 3) ? 3 : cursor_x + 1;
			break;

		case VK_RETURN: //Enter
		case VK_SPACE: //Пробел
			
			//если произошел сдвиг фишки - обновить окно
			if (move_square(cursor_x, cursor_y)) {
				InvalidateRect(m_hwnd, NULL, TRUE); 
				UpdateWindow(m_hwnd);
				return 0;
			}
			else return 0;
			
	}

	//обновляю окно при смещении курсора
	if (old_cursor_x != cursor_x || old_cursor_y != cursor_y) {
		InvalidateRect(m_hwnd, NULL, TRUE);
		UpdateWindow(m_hwnd);
	}

	old_cursor_x = cursor_x;
	old_cursor_y = cursor_y;

	return 0;
}

bool App::move_square(int x_curs, int y_curs) {
	if (abs(zero_x - x_curs) == 1 && zero_y == y_curs) {
		std::swap(arr[y_curs][x_curs], arr[zero_y][zero_x]);
		zero_x = x_curs;
		return true;
	}
	else if (abs(zero_y - y_curs) == 1 && zero_x == x_curs) {
		std::swap(arr[y_curs][x_curs], arr[zero_y][zero_x]);
		zero_y = y_curs;
		return true;
	}
	return false;
}

int App::wm_mousemove(HWND m_hwnd) {

	cursor_y = zero_y;
	cursor_x = zero_x;
	
	if (!mouse_focus_state) {
		mouse_focus_state = true;
		
		InvalidateRect(m_hwnd, NULL, TRUE);
		UpdateWindow(m_hwnd);
	}

	return 0;
}

int App::wm_lbuttondown(LPARAM lParam, HWND m_hwnd) {

	//узнаю в какой квадрат указывает курсор
	//перевожу координаты мыши в логические координаты
	int x = GET_X_LPARAM(lParam) / (ceil_size + 1);
	int y = GET_Y_LPARAM(lParam) / (ceil_size + 1);

	//корректирую выход за пределы диапазона
	x = (x > 3) ? 3 : x;
	y = (y > 3) ? 3 : y;

	if (move_square(x, y)) {
		InvalidateRect(m_hwnd, NULL, TRUE);
		UpdateWindow(m_hwnd);
	}

	return 0;
}