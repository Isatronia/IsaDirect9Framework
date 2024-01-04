#include "UvcImage.h"

namespace IsaFrame {
	// Role�ࣺ
	// �洢�������Ϣ��������ͼ��λ��
	// �Լ�һ���ֵ�������Ϣ�����ٶ�
	// ��ǹ�߶�y+36
	class Sprite
	{
		// constuctor & desrturctor
	public:
		Sprite();
		explicit Sprite(int CharaX, int CharaY);
		Sprite(const Sprite&) = delete;

		// public methods
	public:
		// ��ȡ��������
		inline int getX() { return mCharax; };
		inline int getY() { return mCharay; };
		inline POINT getPoint()
		{
			POINT p;
			p.x = mCharax;
			p.y = mCharay;
			return p;
		};

		// ��ȡ����Ⱦ��ͼ�������ڲ������Զ�ѡ��
		UvcImage getBMP();

		// �жϽ�ɫ�Ƿ����ƶ�
		inline bool isMoving() { return (mCharavx) ? 1 : 0; };

		// ��ɫ�ƶ�����
		virtual inline void Jump();
		virtual inline void RunLeft() { mCharavx = -mStdvx; mfaceRight = false; };
		virtual inline void RunRight() { mCharavx = mStdvx; mfaceRight = true; };
		virtual inline void StopRun();

		// call when Role is falling
		inline void Fall() { mFall = true; };
		// Call when Role stop Falling.
		inline void StopFall() { mFall = false; mJumpTimes = 2; };
		inline bool IsFalling() { return mFall; };

		// ��ɫ�ƶ���ʱ��ĸ��º���
		void Move();
		// ������ײ��
		void SetDetectRECT(UINT Width = 65535, UINT Height = 65535);
		// ��������
		void Count();

		inline bool IsFaceRight() { return mfaceRight; };
		inline bool IsFireing() { return mFire; };
		inline void Fireing() { if (LastFireTick - GetTickCount64() < 100 || Getvx())return; mFire = true; LastFireTick = GetTickCount64(); return; };
		inline void Fired() { mFire = false; return; };

		inline void Damage(ULONGLONG Dmg);

	public:
		// getters for debug and Edge-Detect.
		inline LONG Getvy() { return mCharavy; };
		inline void ForceSetVy(LONG v) { mCharavy = v; return; };
		inline LONG Getvx() { return mCharavx; };
		inline void ForceSetVx(LONG v) { mCharavx = v; return; };
		inline POINT GetCenterPos();
		inline int GetDieState() { return mdie; };
		inline void SetDieState(int state) { mdie = state; return; }
		inline void ForceSetPos(LONG X, LONG Y) { mCharax = X; mCharay = Y; return; };
		inline LONG GetStdVx() { return mStdvx; };

	protected:
		void PlotPixel(DDSURFACEDESC2& ddsd, int x, int y, int r, int g = -1, int b = -1, int a = 0);
		static void PlotPixel(UvcImage UImg, int x, int y, int r, int g = -1, int b = -1, int a = 0);

	public:
		// ��ɫ��ͼ
		UvcImage uBmp;
		UvcImage rAnim[5];
		UvcImage lAnim[5];

		RECT detectRect;

	public:
		void SetClientSize(int Width, int Height);
		static int mClientWidth;
		static int mClientHeight;
		/*static DDPIXELFORMAT CanvasPF;
		static DDSURFACEDESC2 CanvasDDSD;*/

	private:
		int mJumpTimes;

		LONG mCharax;
		LONG mCharay;
		LONG mCharavx;
		LONG mCharavy;
		LONG mStdvx;

		int mAnimIndex;
		int mAnimIndexAddCount;
		int mdie;

		bool mfaceRight;
		bool mFall;

		ULONGLONG LastFireTick;
		bool mFire;

		int mAtk;
		UCHAR mHp;
	};
}