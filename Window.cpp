#include "Window.h"

namespace Bear
{
	WindowVector::WindowVector()
		:x(0), y(0)
	{
	}

	WindowVector::WindowVector(const int& x, const int& y)
		: x(x), y(y)
	{
	}

	Bear::List<Window*> windows;

	Window* SearchWindow(HWND instance)
	{
		for (Bear::BearListInt i = 0; i < windows.Count(); i++)
		{
			if (instance == windows[i]->GetAttachment())
				return windows[i];
		}

		return nullptr;
	}

#ifdef BEAR_WINDOW_FOR_WINDOWS
	long long int WinProc(HWND instance, unsigned int msg, WPARAM firstParam, LPARAM secondParam)
	{
		switch (msg)
		{
		case WM_PAINT:
		{
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnMouseMoveCallback)
					window->OnMouseMoveCallback({ LOWORD(secondParam), HIWORD(secondParam) });
			}

			break;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnMouseClickCallback)
				{
					Window::MouseButton mouseButton = Window::MouseButton::Left;

					if (firstParam & MK_LBUTTON)
						mouseButton = Window::MouseButton::Left;
					else if (firstParam & MK_RBUTTON)
						mouseButton = Window::MouseButton::Right;
					else if (firstParam & MK_MBUTTON)
						mouseButton = Window::MouseButton::Middle;

					window->OnMouseClickCallback(mouseButton, { LOWORD(secondParam), HIWORD(secondParam) }, firstParam & MK_CONTROL, firstParam & MK_SHIFT);
				}
			}

			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnMouseScrollCallback)
					window->OnMouseScrollCallback((HIWORD(firstParam) == WHEEL_DELTA));
			}

			break;
		}
		case WM_KEYDOWN:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnKeyClickCallback)
					window->OnKeyClickCallback((char)firstParam);
			}

			break;
		}
		case WM_MOVE:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnMoveCallback)
					window->OnMoveCallback({ LOWORD(secondParam), HIWORD(secondParam) });
			}

			break;
		}
		case WM_SIZE:
		{
			if (Window* window = SearchWindow(instance))
			{
				if (window->OnResizeCallback)
				{
					Window::WindowState state = Window::WindowState::Restored;
					
					if (firstParam < 3)
						state = (Window::WindowState)firstParam;

					window->OnResizeCallback({ LOWORD(secondParam), HIWORD(secondParam) }, state);
				}
			}

			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(instance);

			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);

			break;
		}
		default:
			return DefWindowProc(instance, msg, firstParam, secondParam);
		}

		return 0;
	}
#endif

	Window::~Window()
	{
#ifdef BEAR_WINDOW_FOR_WINDOWS
		DestroyWindow(this->attachment);
#endif
		windows.Remove(this);

		delete this;
	}

#ifdef BEAR_WINDOW_FOR_WINDOWS
	Window* Window::CreateBearWindow(const WindowVector& Size, const WindowVector& Position, const wchar_t* Title, const Window* Parent, const wchar_t* ClassName)
	{
		Window* window = new Window();

		const wchar_t* className = !ClassName ? Title : ClassName;

		WNDCLASSEX wc{};
		wc.cbClsExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hInstance = nullptr;
		wc.lpfnWndProc = WinProc;
		wc.lpszClassName = className;
		wc.lpszMenuName = nullptr;
		wc.style = 0;

		if (!RegisterClassEx(&wc))
			return nullptr;

		window->attachment = CreateWindowEx(0, className, Title, WS_OVERLAPPEDWINDOW, Position.x, Position.y, Size.x, Size.y, nullptr, nullptr, nullptr, nullptr);

		if (!window->attachment)
			return nullptr;

		ShowWindow(window->attachment, true);
		UpdateWindow(window->attachment);

		if(Parent)
			SetParent(window->attachment, Parent->attachment);

		window->title = (wchar_t*)Title;

		windows.Add(window);

		return window;
	}

	const HWND Window::GetAttachment() const
	{
		return attachment;
	}

	const wchar_t* Window::GetTitle() const
	{
		return title;
	}

	void Window::SetTitle(const wchar_t* NewTitle)
	{
		SetWindowText(attachment, NewTitle);
		title = (wchar_t*)NewTitle;
	}

	const WindowVector Window::GetSize() const
	{
		RECT point;
		GetWindowRect(attachment, &point);

		return { point.right - point.left,point.bottom - point.top };
	}

	void Window::SetSize(const WindowVector& NewSize)
	{
		SetWindowPos(attachment, nullptr,0, 0, NewSize.x, NewSize.y, SWP_NOMOVE);
	}

	const WindowVector Window::GetPosition() const
	{
		RECT point;
		GetWindowRect(attachment, &point);

		return { point.left,point.top };
	}

	void Window::SetPosition(const WindowVector& NewPosition)
	{
		SetWindowPos(attachment, nullptr, NewPosition.x, NewPosition.y, 0, 0, SWP_NOSIZE);
	}

	#ifdef BEAR_LIST_HAS_ADDED
	void Window::Update(void(*Update)(const Bear::List<void*>& params), const Bear::List<void*>& params) const
	#else
	void Window::Update(void(*Update)(const std::vector<void*>& params), const std::vector<void*>& params) const
	#endif
	{
		MSG msg{};

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Update(params);
		}
	}

	void Window::Update(void(*Update)()) const
	{
		MSG msg{};

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Update();
		}
	}
#endif
}