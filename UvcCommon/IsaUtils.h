#include <Windows.h>


class UvcRect;

// UvcRect�ࣺ
// �洢��һЩ������Ⱦ���ж��ľ���
class UvcRect
{
public:
	UvcRect() { rect = { 0 }; return; };
	UvcRect(RECT r) :UvcRect() { rect = r; return; };
	~UvcRect() {};

public:
	RECT rect;
	UINT msg;
};