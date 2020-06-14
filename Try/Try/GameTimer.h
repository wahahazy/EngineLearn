class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	void Reset(); // ����Ϣѭ��֮ǰ����
	void Start(); // ��ȡ����ͣ��ʱ�����
	void Stop();  // ����ͣ��ʱ�����
	void Tick();  // ��ÿһ֡��ʱ�����
	float DeltaTime();		// ֡���ʱ��

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};