#include <string>
#include <sstream>
using namespace std;


BOOL CALLBACK FindCalcProc(      
    HWND hwnd,
    LPARAM lParam);

BOOL CALLBACK EnumCalcChildByCIDProc(      
    HWND hwnd,
    LPARAM lParam);


typedef struct FindWnd {
	unsigned long opcode;
	HWND resultWnd;
} FINDWND, *PFINDWND;

class CalcInterface : public std::stringstream {
	HWND GetWndByName(char name);
	void DoClick(char name);
	void MoveMouse(RECT &area);
	
public:
	void Clear();
	bool isError();
	char *GetAnswer(char *szResult);
	CalcInterface();
	~CalcInterface();
	char *Execute(char *szResult);
};


class CalcHandle {
	HWND hCalcWnd_;

	static CalcHandle* pInstance_;
	static bool destroyed_;
	static void Create() {
		static CalcHandle theInstance;
		pInstance_ = &theInstance;
	}
	CalcHandle() {
	}
	~CalcHandle() {
		
		SendMessage(hCalcWnd_, WM_CLOSE,0,0);
		//DestroyWindow(hCalcWnd_);
		pInstance_ = 0;

	}
	friend BOOL CALLBACK FindCalcProc(HWND hwnd, LPARAM lParam);

	void setCalcWnd(HWND hCalcWnd) {
		hCalcWnd_ = hCalcWnd;
	}
public:
	static CalcHandle& Instance() {
		if (!pInstance_) {
			if (destroyed_) {
				exit(-1);
			} else {
				Create();
			}
		}
		return *pInstance_;
	}

	HWND &getCalcWnd();
};


//CommandProcessor& operator<<(CommandProcessor& obj, std::string);


