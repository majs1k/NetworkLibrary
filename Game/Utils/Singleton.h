// -------------------------------------------------------------------
// 
// 싱글톤
// 
// 매니저 클래스는 이 클래스 상속 받아서 사용
// 상속 받을때 앞에 public 꼭 붙이기
//
// -------------------------------------------------------------------
#pragma once

template <class T>
class Singleton
{
private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);

protected:
	Singleton() {}
	~Singleton() {}

public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}
};
