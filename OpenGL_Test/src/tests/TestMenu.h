#pragma once

#include "Test.h"

namespace test
{
	class TestMenu : public Test
	{
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register test: " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};
}