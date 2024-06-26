﻿#include <crtdbg.h>
#include <ecm/ecm.hpp>
#include <ecm/ecm_math.hpp>
#include <ecm/algorithm/random.h>
#include <ecm/ecm_window.hpp>
#include <ecm/ecm_opengl.hpp>

#pragma warning(disable : 4996)

namespace ec = ecm::console;

#include <thread>
#include <mutex>

std::atomic<bool> thread1_ready{ false };
std::atomic<bool> thread2_ready{ false };

void test_Threading()
{
	std::thread thread1{
		[]() {
			int i{ 0 };
			while (i < 10)
			{
				ec::Write("Dies ist eine Testnachricht des Threads 1.");
				ec::WriteLine(" Dies ist Teil 2.");
				//_sleep(95);
				i++;
			}
			thread1_ready = true;
		}
	};
	
	std::thread thread2{
		[]() {
			int i{ 0 };
			while (i < 10)
			{
				ec::Write("Dies ist eine Testnachricht des Threads 2.");
				ec::WriteLine(" Dies ist Teil 2.");
				//_sleep(105);
				i++;
			}
			thread2_ready = true;
		}
	};
	
	thread1.detach();
	thread2.detach();

	bool succeeded{ false };
	while (!succeeded)
	{
		_sleep(100);
		if (thread1_ready && thread2_ready) succeeded = true;
	}
}

void test_ConsoleSystem()
{
	ec::WriteLine("%sHello World! Black",			ec::ANSI::BG_Black);
	ec::WriteLine("%sHello World! Red",				ec::ANSI::BG_Red);
	ec::WriteLine("%sHello World! Green",			ec::ANSI::BG_Green);
	ec::WriteLine("%sHello World! Yellow",			ec::ANSI::BG_Yellow);
	ec::WriteLine("%sHello World! Blue",			ec::ANSI::BG_Blue);
	ec::WriteLine("%sHello World! Magenta",			ec::ANSI::BG_Magenta);
	ec::WriteLine("%sHello World! Cyan",			ec::ANSI::BG_Cyan);
	ec::WriteLine("%sHello World! LightGray",		ec::ANSI::BG_LightGray);
	ec::WriteLine("%sHello World! Gray",			ec::ANSI::BG_DarkGray);
	ec::WriteLine("%sHello World! LightRed",		ec::ANSI::BG_LightRed);
	ec::WriteLine("%sHello World! LightGreen",		ec::ANSI::BG_LightGreen);
	ec::WriteLine("%sHello World! LightYellow",		ec::ANSI::BG_Yellow);
	ec::WriteLine("%sHello World! LightBlue",		ec::ANSI::BG_LightBlue);
	ec::WriteLine("%sHello World! LightMagenta",	ec::ANSI::BG_LightMagenta);
	ec::WriteLine("%sHello World! LightCyan",		ec::ANSI::BG_LightCyan);
	ec::WriteLine("%sHello World! White,%s",		ec::ANSI::BG_White, ec::ANSI::Reset);
	
	ec::WriteLine("%sHello World! Black",			ec::ANSI::FG_Black);
	ec::WriteLine("%sHello World! Red",				ec::ANSI::FG_Red);
	ec::WriteLine("%sHello World! Green",			ec::ANSI::FG_Green);
	ec::WriteLine("%sHello World! Yellow",			ec::ANSI::FG_Yellow);
	ec::WriteLine("%sHello World! Blue",			ec::ANSI::FG_Blue);
	ec::WriteLine("%sHello World! Magenta",			ec::ANSI::FG_Magenta);
	ec::WriteLine("%sHello World! Cyan",			ec::ANSI::FG_Cyan);
	ec::WriteLine("%sHello World! LightGray",		ec::ANSI::FG_LightGray);
	ec::WriteLine("%sHello World! Gray",			ec::ANSI::FG_DarkGray);
	ec::WriteLine("%sHello World! LightRed",		ec::ANSI::FG_LightRed);
	ec::WriteLine("%sHello World! LightGreen",		ec::ANSI::FG_LightGreen);
	ec::WriteLine("%sHello World! LightYellow",		ec::ANSI::FG_Yellow);
	ec::WriteLine("%sHello World! LightBlue",		ec::ANSI::FG_LightBlue);
	ec::WriteLine("%sHello World! LightMagenta",	ec::ANSI::FG_LightMagenta);
	ec::WriteLine("%sHello World! LightCyan",		ec::ANSI::FG_LightCyan);
	ec::WriteLine("%sHello World! White%s",			ec::ANSI::FG_White, ec::ANSI::Reset);

	ec::WriteLine("%sHello World! Bold%s",			ec::ANSI::Bold, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Semi-Transparent%s", ec::ANSI::SemiTransparent, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Italic%s",		ec::ANSI::Italic, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Underlined%s",	ec::ANSI::Underlined, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Blinking%s",		ec::ANSI::SlowBlink, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Rapid Blink%s",	ec::ANSI::RapidBlink, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Swaped%s",		ec::ANSI::Swapped, ec::ANSI::Reset);
	ec::WriteLine("%sHello World! Invisible%s",		ec::ANSI::Invisible, ec::ANSI::Reset);
	ec::WriteLine();
}

void test_WindowSystem()
{
	ecm::GraphicsAPI gApi{ ecm::GRAPHICSAPI_OPENGL };

	ecm::Window window = ecm::CreateWindow("Test window", { 800, 600 },
		ecm::WINDOWFLAG_RESIZABLE, ecm::WINDOWMODE_SHOWN, gApi);

	ecm::ContextBase* context = nullptr;
	if (gApi == ecm::GRAPHICSAPI_SDLRENDERER) context = new ecm::SDLRendererContext();
	else if (gApi == ecm::GRAPHICSAPI_OPENGL) context = new ecm::gl::OpenGLContext();
	else context = new ecm::SDLRendererContext();

	context->Initialize(window);
	context->SetColor(ecm::ColorF(0x1A304Cff));
	context->SetVSyncMode(ecm::VSYNC_ENABLED);

	while (!window.IsClosed())
	{
		ecm::Event e{};
		while (ecm::PollEvent(e));

		context->ClearBuffers();
		context->SwapBuffers();
	}

	context->Shutdown();
	delete context;
	ecm::DestroyWindow(window);
}

#undef main
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	test_Threading();
	test_ConsoleSystem();
	test_WindowSystem();
	
	system("pause");
	return 0;
}
