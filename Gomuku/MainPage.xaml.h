//
// MainPage.xaml.h
// MainPage 类的声明。
//

#pragma once

#include "MainPage.g.h"
#include "gomuku.h"
#include "robot.h"

namespace Gomuku
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();


	private:
		void InitBoard();
		void paintChess();
		void Btn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Restart(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ClickBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RobotMove();

		Board* board;
		bool endGame = false;


	};
}
