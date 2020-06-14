class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	void Reset(); // 在消息循环之前调用
	void Start(); // 在取消暂停的时候调用
	void Stop();  // 在暂停的时候调用
	void Tick();  // 在每一帧的时候调用
	float DeltaTime();		// 帧间隔时间

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};