#pragma once
#if defined (_WIN32) || defined(_WIN64)
	#include <Windows.h>
	#define BEAR_WINDOW_FOR_WINDOWS
#elif defined (__linux__)
	#error "Bear Window haven't support Linux yet"
#endif

namespace Bear
{
	class Window
	{
	private:
		#ifdef BEAR_WINDOW_FOR_WINDOWS
			HWND attachment;
		#endif

		const wchar_t* title;
	public:
		enum class MouseButton
		{
			Left = 1, Right = 2, Middle = 16
		};

		enum class State
		{
			Restored = 1, Minimized = 2, Maximized = 3, FullScreen = 4
		};

		enum class PointerType
		{
			Arrow = 32512, Crosshair = 32515, Hand = 32649, Help = 32651, IBeam = 32513, Blocked = 32648, Move = 32646, SizeNESW = 32643, SizeNS = 32645, SizeNWSE = 32642, SizeWE = 32644, UpArrow = 32516, Wait = 32514
		};
	public:
		~Window();
	public:
		struct Vector
		{
			int x, y;

			Vector();
			Vector(const int& x, const int& y);
		};
	public:
		typedef void(*OnUpdate)(const Window* window);
		typedef void(*OnMouseMove)(const Window* window, const Vector mousePosition);
		typedef void(*OnMouseClick)(const Window* window, const MouseButton mouseButton, const Vector mousePosition, const bool controlClicked, const bool shiftClicked);
		typedef void(*OnMouseScroll)(const Window* window, const bool offset);
		typedef void(*OnKeyClick)(const Window* window, const char key);
		typedef void(*OnMove)(const Window* window, const Vector position);
		typedef void(*OnResize)(const Window* window, const Vector size, const State state);
		typedef void(*OnClose)(const Window* window);
		typedef void(*OnDestroy)(const Window* window);
	public:
		OnUpdate OnUpdateCallback;
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
			const HINSTANCE GetInstance() const;
		#endif

		const wchar_t* GetTitle() const;
		void SetTitle(const wchar_t* NewTitle);

		const Vector GetSize() const;
		void SetSize(const Vector& NewSize);

		const Vector GetPosition() const;
		void SetPosition(const Vector& NewPosition);

		const State GetState() const;
		void SetState(const State& NewState);
	public:
		void Update() const;
	public:
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent, const wchar_t* ClassName);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent, const wchar_t* ClassName);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage, const State& WindowState);
		static Window* CreateBearWindow(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName, const Window* Parent, const wchar_t* ClassName, const wchar_t* PathToTaskBarImage, const wchar_t* PathToImage, const State& WindowState);
	};
}