#pragma once

#include "core/CoreMacro.hpp"
#include "common/Singleton.h"
#include <functional>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>

namespace SongYun
{
	class ICommand;

	class  CommandRegistry :public Singleton<CommandRegistry>
	{
	public:
		SONGYUN_API static CommandRegistry& Instance();
		// 注册命令描述符（仅存元信息，不创建实例）
		SONGYUN_API void registerDescriptor(const char* id, std::function<ICommand* ()> factory);

		// 获取命令（首次调用时惰性初始化）
		SONGYUN_API ICommand* getCommand(const char* id);

	private:
		struct CommandDescriptor
		{
			std::function<ICommand* ()> factory;
			std::unique_ptr<ICommand> instance;
			mutable std::unique_ptr<std::mutex> mutex; // ✅ 使用unique_ptr包裹

			// 添加构造函数
			CommandDescriptor() : factory(nullptr), instance(nullptr),
				mutex(std::make_unique<std::mutex>()) {}

			CommandDescriptor(std::function<ICommand* ()> f,
				std::unique_ptr<ICommand> inst,
				std::unique_ptr<std::mutex> mtx)
				: factory(std::move(f)), instance(std::move(inst)), mutex(std::move(mtx)) {}
		};

		CommandRegistry() = default;
		~CommandRegistry() = default;
		CommandRegistry(const CommandRegistry&) = delete;
		CommandRegistry& operator=(const CommandRegistry&) = delete;
	private:
		std::unordered_map<std::string, CommandDescriptor> m_descriptors;
		std::mutex m_mutex;
	};
} // namespace SongYun
