#include "BearWindow/Window.h"

#if __has_include("List.h") || __has_include("BearList/List.h")
	#define BearDynamicArrayHasInclude
	#include <BearList/List.h>
#else
	#include <vector>
#endif

namespace Bear
{
	#ifdef BearDynamicArrayHasInclude
		static Bear::DynamicArray<Window*> windows;
	#else
		static std::vector<Window*> windows;
	#endif

	Window::Vector::Vector()
		:x(0), y(0)
	{
	}

	Window::Vector::Vector(const int& x, const int& y)
		: x(x), y(y)
	{
	}

	Window* SearchWindow(const HWND& instance)
	{
		#ifdef BearDynamicArrayHasInclude
		for (Bear::DynamicArrayUInt i = 0; i < windows.Count(); i++)
		{
			if (instance == windows[i]->GetAttachment())
				return windows[i];
		}
		#else
		for (uint32_t i = 0; i < windows.size(); i++)
		{
			if (instance == windows[i]->GetAttachment())
				return windows[i];
		}
		#endif

		return nullptr;
	}

	LRESULT WinProc(HWND instance, UINT msg, WPARAM firstParam, LPARAM secondParam)
	{
		switch (msg)
		{
			case WM_PAINT:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnUpdateCallback)
						window->OnUpdateCallback(window);
				}
				break;
			}
			case WM_MOUSEMOVE:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnMouseMoveCallback)
						window->OnMouseMoveCallback(window, { LOWORD(secondParam), HIWORD(secondParam) });
				}

				break;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_XBUTTONDOWN:
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
						else if (firstParam & MK_XBUTTON1)
							mouseButton = Window::MouseButton::XButton1;
						else if(firstParam & MK_XBUTTON2)
							mouseButton = Window::MouseButton::XButton2;

						window->OnMouseClickCallback(window, mouseButton, { LOWORD(secondParam), HIWORD(secondParam) }, firstParam & MK_CONTROL, firstParam & MK_SHIFT);
					}
				}

				break;
			}
			case WM_MOUSEWHEEL:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnMouseScrollCallback)
						window->OnMouseScrollCallback(window, (HIWORD(firstParam) == WHEEL_DELTA));
				}

				break;
			}
			case WM_CHAR:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnKeyClickCallback)
						window->OnKeyClickCallback(window, (Window::KeyCode)firstParam);
				}

				break;
			}
			case WM_MOVE:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnMoveCallback)
						window->OnMoveCallback(window, { LOWORD(secondParam), HIWORD(secondParam) });
				}

				break;
			}
			case WM_SIZE:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnResizeCallback)
					{
						Window::State state = Window::State::Restored;

						switch (firstParam)
						{
							case SIZE_RESTORED:
							{
								state = Window::State::Restored;
								break;
							}
							case SIZE_MINIMIZED:
							{
								state = Window::State::Minimized;
								break;
							}
							case SIZE_MAXIMIZED:
							{
								state = Window::State::Maximized;
								break;
							}
							default:
								break;
						}

						window->OnResizeCallback(window, { LOWORD(secondParam), HIWORD(secondParam) }, state);
					}
				}

				break;
			}
			case WM_CLOSE:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnCloseCallback)
						window->OnCloseCallback(window);
				}

				DestroyWindow(instance);

				break;
			}
			case WM_DESTROY:
			{
				if (Window* window = SearchWindow(instance))
				{
					if (window->OnDestroyCallback)
						window->OnDestroyCallback(window);

					window->destroyed = true;
				}

				bool&& quit = true;

				for (const auto& window : windows)
				{
					if (!window->destroyed)
						quit = false;
				}

				if(quit)
					PostQuitMessage(0);

				break;
			}
			default:
				return DefWindowProc(instance, msg, firstParam, secondParam);
		}

		return 0;
	}

	Window::Window(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage, const State& WindowState, const Style& WindowStyle)
		: style(WindowStyle), destroyed(false)
	{
		const wchar_t* className = !ClassName ? Title : ClassName;

		WNDCLASSEX wc{};
		wc.cbClsExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(nullptr, MAKEINTRESOURCE(PointerType));
		wc.hIcon = PathToTaskBarImage ? (HICON)LoadImage(nullptr, PathToTaskBarImage, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED) : LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = PathToImage ? (HICON)LoadImage(nullptr, PathToImage, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED) : LoadIcon(nullptr, IDI_APPLICATION);
		wc.hInstance = nullptr;
		wc.lpfnWndProc = WinProc;
		wc.lpszClassName = className;
		wc.lpszMenuName = nullptr;
		wc.style = 0;

		if (!RegisterClassEx(&wc))
			return;

		this->attachment = CreateWindowEx(0, className, Title, (DWORD)WindowStyle, Position.x, Position.y, Size.x, Size.y, nullptr, nullptr, nullptr, nullptr);

		if (!this->attachment)
			return;

		this->SetState(WindowState);
		UpdateWindow(this->attachment);

		#ifdef BearDynamicArrayHasInclude
		windows.Add(this);
		#else
		windows.push_back(this);
		#endif

		if (Parent)
			SetParent(this->attachment, Parent->attachment);

		this->title = (wchar_t*)Title;

		UnregisterClass(className, GetModuleHandle(nullptr));
	}

	Window::Window(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage, const State& WindowState, const Style& WindowStyle)
		: style(WindowStyle), destroyed(false)
	{
		const wchar_t* className = !ClassName ? Title : ClassName;

		WNDCLASSEX wc{};
		wc.cbClsExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = (HCURSOR)LoadCursorFromFile(PointerFileName);
		wc.hIcon = PathToTaskBarImage ? (HICON)LoadImage(nullptr, PathToTaskBarImage, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED) : LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = PathToImage ? (HICON)LoadImage(nullptr, PathToImage, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED) : LoadIcon(nullptr, IDI_APPLICATION);
		wc.hInstance = nullptr;
		wc.lpfnWndProc = WinProc;
		wc.lpszClassName = className;
		wc.lpszMenuName = nullptr;
		wc.style = 0;

		if (!RegisterClassEx(&wc))
			return;
		
		this->attachment = CreateWindowEx(0, className, Title, (DWORD)WindowStyle, Position.x, Position.y, Size.x, Size.y, nullptr, nullptr, nullptr, nullptr);

		if (!this->attachment)
			return;

		this->SetState(WindowState);
		UpdateWindow(this->attachment);

		#ifdef BearDynamicArrayHasInclude
		windows.Add(this);
		#else
		windows.push_back(this);
		#endif

		if (Parent)
			SetParent(this->attachment, Parent->attachment);

		this->title = (wchar_t*)Title;

		UnregisterClass(className, GetModuleHandle(nullptr));
	}

	Window::~Window()
	{
		destroyed = true;

		#ifdef BearDynamicArrayHasInclude
		if (windows.Data())
			windows.Remove(this);
		#else
		if (windows.data())
			windows.erase(std::find(windows.begin(), windows.end(), this));
		#endif
	}

	const HWND Window::GetAttachment() const
	{
		return attachment;
	}

	const HINSTANCE Window::GetInstance() const
	{
		return (HINSTANCE)GetModuleHandle(nullptr);
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

	const Window::Vector Window::GetSize() const
	{
		RECT point;
		GetClientRect(attachment, &point);

		return { point.right, point.bottom };
	}

	void Window::SetSize(const Vector& NewSize)
	{
		SetWindowPos(attachment, nullptr, 0, 0, NewSize.x, NewSize.y, SWP_NOMOVE);
	}

	const Window::Vector Window::GetPosition() const
	{
		RECT point;
		GetWindowRect(attachment, &point);

		return { point.left + 8, point.top + 31 };
	}

	void Window::SetPosition(const Vector& NewPosition)
	{
		SetWindowPos(attachment, nullptr, NewPosition.x, NewPosition.y, 0, 0, SWP_NOSIZE);
	}

	const Window::State Window::GetState() const
	{
		WINDOWPLACEMENT windowState{};
		windowState.length = sizeof(WINDOWPLACEMENT);

		GetWindowPlacement(this->attachment, &windowState);

		return (Window::State)windowState.showCmd;
	}

	void Window::SetState(const State& NewState)
	{
		if (NewState == State::FullScreen)
		{
			SetWindowLong(attachment, GWL_STYLE, WS_VISIBLE);
			ShowWindow(this->attachment, (int)State::Maximized);
		}	
		else
		{	
			SetWindowLong(attachment, GWL_STYLE, (LONG)style);
			ShowWindow(this->attachment, (int)NewState);
		}
	}

	void Window::SetCursor(const PointerType& type)
	{
		::SetCursor(LoadCursor(nullptr, MAKEINTRESOURCE(type)));
	}

	void Window::SetCursor(const wchar_t* pointerFileName)
	{
		::SetCursor(LoadCursorFromFile(pointerFileName));
	}

	void Window::Update() const
	{
		MSG msg{};

		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	const Window::Vector Window::GetMonitorResolution()
	{
		return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
}