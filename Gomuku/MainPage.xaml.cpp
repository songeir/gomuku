//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Gomuku;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System::Threading;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板


MainPage::MainPage()
{
	InitializeComponent();

	InitBoard();

	Comb->SelectedValuePath = "简单";
	
	board = new Board();
	board->player[1] = new Player(board,1);
	board->player[2] = new Player(board,2);

}

void MainPage::InitBoard()
{
	for (int i = 0; i < 15; i++)
	{
		auto horizenLine = ref new Line();

		horizenLine->X1 = 30;
		horizenLine->X2 = 450;
		horizenLine->Y1 = 30 + i * 30;
		horizenLine->Y2 = 30 + i * 30;
		horizenLine->Stroke = ref new SolidColorBrush(Windows::UI::Colors::Blue);

		LineBoard->Children->Append(horizenLine);

		auto verticalLine = ref new Line();

		verticalLine->X1 = 30 + i * 30;
		verticalLine->X2 = 30 + i * 30;
		verticalLine->Y1 = 30;
		verticalLine->Y2 = 450;
		verticalLine->Stroke = ref new SolidColorBrush(Windows::UI::Colors::Blue);

		LineBoard->Children->Append(verticalLine);
	}

	for (int i = 0; i<15; i++)
		for (int j = 0; j < 15; j++)
		{
			RadioButton^ btn = ref new RadioButton();

			btn->Opacity = 0.01;
			btn->BorderBrush = ref new SolidColorBrush(Windows::UI::Colors::Black);

			btn->Width = 30;
			btn->Height = 30;
			btn->Margin = { 5,0,5,0 };

			btn->Tag = i * 15 + j;

			btn->Click += ref new RoutedEventHandler(this, &MainPage::Btn_Click);

			ChessBoard->Children->Append(btn);
			ChessBoard->SetRow(btn, i + 1);
			ChessBoard->SetColumn(btn, j + 1);

		}

	return;
}

void MainPage::paintChess()
{
	ListBoard->Children->Clear();
	for (int i = 0; i<15; i++)
		for (int j = 0; j < 15; j++)
		{
			if (board->board[i][j] == 0)
				continue;

			Ellipse^ x = ref new Ellipse();

			x->Width = 30;
			x->Height = 30;
			
			if (board->board[i][j] == 1)
			{
				x->Fill = ref new SolidColorBrush(Windows::UI::Colors::Black);
			}else {
				x->Fill = ref new SolidColorBrush(Windows::UI::Colors::White);
				x->Stroke = ref new SolidColorBrush(Windows::UI::Colors::Black);
			}

			ListBoard->Children->Append(x);
			ListBoard->SetRow(x, i + 1);
			ListBoard->SetColumn(x, j + 1);
		}
}

void MainPage::Btn_Click(Object^ sender, RoutedEventArgs^ e)
{
	RadioButton^ btn = dynamic_cast<RadioButton^>(sender);
	int i = (int)(btn->Tag) / 15;
	int j = (int)(btn->Tag) % 15;

	if (endGame)
		return;

	if (board->player[board->currentPlayer]->makeMove(Node(i, j)))
	{
		board->currentPlayer = (board->currentPlayer == 1) ? 2 : 1;
		
		paintChess();
		//TextB->Text = "电脑正在下棋中......";

		if (board->isEnd())
		{
			endGame = true;
			TextB->Text = "你赢了！\n按[重新开始]按钮\n再来一局";
			return;
		}

		RobotMove();
	}
}

void MainPage::RobotMove()
{
	Node t;
	t = Robot::makeAutoMove(board->player[board->currentPlayer], 0);
	board->player[board->currentPlayer]->makeMove(t);
	board->currentPlayer = (board->currentPlayer == 1) ? 2 : 1;
	paintChess();
	//TextB->Text = "电脑下棋完毕!"; 

	if (board->isEnd())
	{
		endGame = true;
		TextB->Text = "你输了！\n按[重新开始]按钮\n再来一局";
		return;
	}
	
	return;
}

void Gomuku::MainPage::Restart(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	delete board;
	board = new Board();
	board->player[1] = new Player(board, 1);
	board->player[2] = new Player(board, 2);
	endGame = false;

	if (Comb->SelectedIndex == 0)
	{
		Robot::maxDepth = 3;
	}
	else if (Comb->SelectedIndex == 1)
	{
		Robot::maxDepth = 4;
	}
	else if (Comb->SelectedIndex == 2)
	{
		Robot::maxDepth = 5;
	}
	TextB->Text = "";

	paintChess();
}

void Gomuku::MainPage::ClickBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	board->player[board->currentPlayer]->takeBack();
	endGame = false;

	paintChess();
}
