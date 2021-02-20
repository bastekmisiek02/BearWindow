#pragma once
#if defined (_WIN32) || defined(_WIN64)
	#include <Windows.h>
	#define BEAR_WINDOW_FOR_WINDOWS
		#ifdef _WIN64
			typedef unsigned long int BearWindowUInt;
		#else
			typedef unsigned int BearWindowUInt;
		#endif
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
		enum class KeyCode
		{
			Escape = 7,
			Backspace = 8,
			Tab = 9,
			Enter = 13,
			Space = 32,
			ExclamationMark = 33,
			QuotationMark = 34,
			Hash = 35,
			Dollar = 36,
			Percentage = 37,
			Ampersand = 38,
			Apostrophe = 39,
			LeftRoundBracket = 40,
			RightRoundBracket = 41,
			Asterysk = 42,
			Plus = 43,
			Comma = 44,
			Minus = 45,
			FullStop = 46,
			ForwardSlash = 47,
			Number0 = 48, 
			Number1 = 49, 
			Number2 = 50, 
			Number3 = 51, 
			Number4 = 52, 
			Number5 = 53, 
			Number6 = 54, 
			Number7 = 55, 
			Number8 = 56, 
			Number9 = 57,
			Colon = 58,
			Semicolon = 59,
			LessThan = 60,
			Equal = 61,
			GreaterThan = 62,
			QuestionMark = 63,
			At = 64,
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			LeftSquareBracket = 91,
			Backslash = 92,
			RightSquareBracket = 93,
			Caret = 94,
			Hyphen = 95,
			GraveAccent = 96,
			a = 97,
			b = 98,
			c = 99,
			d = 100,
			e = 101,
			f = 102,
			g = 103,
			h = 104,
			i = 105,
			j = 106,
			k = 107,
			l = 108,
			m = 109,
			n = 110,
			o = 111,
			p = 112,
			q = 113,
			r = 114,
			s = 115,
			t = 116,
			u = 117,
			v = 118,
			w = 119,
			x = 120,
			y = 121,
			z = 122,
			LeftBuckle = 123,
			VerticalLine = 124,
			RightBuckle = 125,
			Tilda = 126
		};

		enum class MouseButton
		{
			Left = 1, 
			Right = 2, 
			Middle = 16, 
			XButton1 = 32, 
			XButton2 = 64
		};

		enum class State
		{
			Restored = 1, 
			Minimized = 2, 
			Maximized = 3, 
			FullScreen = 4
		};

		enum class PointerType
		{
			Arrow = 32512, 
			Crosshair = 32515, 
			Hand = 32649, 
			Help = 32651, 
			IBeam = 32513, 
			Blocked = 32648, 
			Move = 32646, 
			SizeNESW = 32643, 
			SizeNS = 32645, 
			SizeNWSE = 32642, 
			SizeWE = 32644, 
			UpArrow = 32516, 
			Wait = 32514
		};

		enum class Style : BearWindowUInt
		{
			Normal = 282001408, 
			PopUp = 2415919104
		};
	private:
		static friend LRESULT WinProc(HWND instance, UINT msg, WPARAM firstParam, LPARAM secondParam);
	private:
		Style style;
		bool destroyed;
	public:
		struct Vector
		{
			int x, y;

			Vector();
			Vector(const int& x, const int& y);
		};
	public:
		Window(const Vector& Size, const Vector& Position, const wchar_t* Title, const PointerType& PointerType = PointerType::Arrow, const Window* Parent = nullptr, const wchar_t* ClassName = nullptr, const wchar_t* PathToTaskBarImage = nullptr, const wchar_t* PathToImage = nullptr, const State& WindowState = State::Restored, const Style& WindowStyle = Style::Normal);
		Window(const Vector& Size, const Vector& Position, const wchar_t* Title, const wchar_t* PointerFileName = nullptr, const Window* Parent = nullptr, const wchar_t* ClassName = nullptr, const wchar_t* PathToTaskBarImage = nullptr, const wchar_t* PathToImage = nullptr, const State& WindowState = State::Restored, const Style& WindowStyle = Style::Normal);
		~Window();
	public:
		typedef void(*OnUpdate)(const Window* window);
		typedef void(*OnMouseMove)(const Window* window, const Vector mousePosition);
		typedef void(*OnMouseClick)(const Window* window, const MouseButton mouseButton, const Vector mousePosition, const bool controlClicked, const bool shiftClicked);
		typedef void(*OnMouseScroll)(const Window* window, const bool offset);
		typedef void(*OnKeyClick)(const Window* window, const KeyCode key);
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
		void SetCursor(const PointerType& type);
		void SetCursor(const wchar_t* pointerFileName);
	public:
		const Vector GetMousePosition() const;
	public:
		void Update() const;
	public:
		static const Vector GetMonitorResolution();
		static const bool IsKeyDown(const char& key);
	};
}