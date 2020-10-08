#pragma once
#if defined (_WIN32) || defined(_WIN64)
	#include <Windows.h>
	#define BEAR_WINDOW_FOR_WINDOWS
#elif defined (__linux__)
	#error "Bear Window haven't support Linux yet"
#endif

#if __has_include(<BearList/List.h>)
	#include <BearList/List.h>
	#define  BEAR_LIST_HAS_ADDED
#elif
	#include <vector>
#endif

namespace Bear
{
	struct WindowVector
	{
		int x, y;

		WindowVector();
		WindowVector(const int& x, const int& y);
	};

	class Window
	{
	private:
#ifdef BEAR_WINDOW_FOR_WINDOWS
		HWND attachment;
#endif
		wchar_t* title;
	public:
		~Window();
	public:
		enum class MouseButton
		{
			Left = 1, Right = 2, Middle = 16
		};

		enum class WindowState
		{
			Restored = 0, Minimized = 1, Maximized = 2
		};
	public:
		typedef void(*OnMouseMove)(const WindowVector mousePosition);
		typedef void(*OnMouseClick)(const MouseButton mouseButton, const WindowVector mousePosition, const bool controlClicked, const bool shiftClicked);
		typedef void(*OnMouseScroll)(const bool offset);
		typedef void(*OnKeyClick)(const char key);
		typedef void(*OnMove)(const WindowVector position);
		typedef void(*OnResize)(const WindowVector size, const WindowState state);
		typedef void(*OnClose)();
		typedef void(*OnDestroy)();
	public:
		OnMouseMove OnMouseMoveCallback;
		OnMouseClick OnMouseClickCallback;
		OnMouseScroll OnMouseScrollCallback;
		OnKeyClick OnKeyClickCallback;
		OnMove OnMoveCallback;
		OnResize OnResizeCallback;
		OnClose OnCloseCallback;
		OnDestroy OnDestroyCallback;
	public:
#ifdef BEAR_WINDOW_FOR_WINDOWS
		const HWND GetAttachment() const;
#endif
		const wchar_t* GetTitle() const;
		void SetTitle(const wchar_t* NewTitle);

		const WindowVector GetSize() const;
		void SetSize(const WindowVector& NewSize);

		const WindowVector GetPosition() const;
		void SetPosition(const WindowVector& NewPosition);
	public:
		#ifdef BEAR_LIST_HAS_ADDED
		void Update(void(*Update)(const Bear::List<void*>& params), const Bear::List<void*>& params) const;
		#else
		void Update(void(*Update)(const std::vector<void*>& params), const std::vector<void*>& params) const;
		#endif
		void Update(void(*)()) const;
	public:
		static Window* CreateBearWindow(const WindowVector& Size, const WindowVector& Position = { 0, 0 }, const wchar_t* Title = L"Bear Window", void(*OnCreate)() = nullptr, const Window* Parent = nullptr, const wchar_t* ClassName = nullptr);
	};
}