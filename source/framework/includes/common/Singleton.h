#pragma once
/// <summary>
/// 单例模板类，提供一个全局访问点来获取唯一实例。PS：继承自Singleton的类必须具有默认构造函数，并且不允许复制或赋值。
/// 需要注意的是跨模块使用时，Singleton实例的生命周期和访问方式可能会受到影响，建议在同一模块内使用Singleton模式以避免潜在问题。
/// 如果两个不同的dll都使用了Singleton模式，并且它们之间没有正确的共享机制，那么每个dll可能会创建自己的Singleton实例，导致无法实现真正的单例效果。
/// 因此，在跨模块使用时需要特别小心，确保Singleton实例在所有模块中都是唯一的，并且生命周期得到正确管理。
/// 比如A.dll的类X使用这个单例，B.dll调用X头文件，那么A会有一个X对象，B也会有一个X对象。
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};
