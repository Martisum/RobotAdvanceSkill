#ifndef SINGLETON_H
#define SINGLETON_H
template <class T>
class Singleton{
public:
	static	T* getInstance(){
		static T* instance = nullptr;
		if (!instance){
			instance = new T();
		}
		return instance;
	}
private:
	Singleton(){}
	~Singleton(){}
};
#endif