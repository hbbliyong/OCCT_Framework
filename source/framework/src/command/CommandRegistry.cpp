#include "command/CommandRegistry.h"
#include "command/ICommand.h"
#include <string>
#include <iostream>

namespace SongYun {
	CommandRegistry& CommandRegistry::Instance()
	{
		static CommandRegistry instance;
		return instance;
	}
	void CommandRegistry::registerDescriptor(const char* id, std::function<ICommand* ()> factory) {
		// 关键：使用DLL内部的锁（避免跨CRT互斥量问题）
		std::lock_guard<std::mutex> lock(m_mutex);

		// 防止重复注册（重要！）
		if (m_descriptors.find(id) != m_descriptors.end()) {
			// 使用标准输出替代平台特定的调试函数
			std::cerr << "Warning: Command '" << id << "' already registered" << std::endl;
			return;
		}

		// 存储描述符（仅存元数据，不创建实例）
		m_descriptors[id] = CommandDescriptor{
	factory,
	nullptr,
	std::make_unique<std::mutex>() // ✅ 可以安全移动
		};
	}

	ICommand* CommandRegistry::getCommand(const char* id) {
		auto it = m_descriptors.find(id);
		if (it == m_descriptors.end()) return nullptr;

		// 双重检查锁确保线程安全
		if (!it->second.instance) {
			// 解引用unique_ptr获取mutex引用
			std::lock_guard<std::mutex> lock(*it->second.mutex);
			if (!it->second.instance) {
				it->second.instance.reset(it->second.factory()); // 调用工厂函数
			}
		}
		return it->second.instance.get();
	}
}
