#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Map.h"
#include "World.h"


void UI::DrawUI()
{
	Rendering::Image::Component img;
	Rendering::Text::Component text;

	// Exp 게이지바
	{
		img.Content = "UI/EXPBar/backgrnd";
		img.Length = img.GetLength();
		img.Length[0] = 1280.f;
		img.Location[1] = (img.Length[1] - 720.f) / 2;
		img.Draw(false);

		img.Content = "UI/EXPBar/layerback";
		img.Length = img.GetLength();
		img.Location[1] = (img.Length[1] - 720.f) / 2;
		img.Draw(false);


		float exp_prob = (float)PLAYER.exp / PLAYER.needExp[PLAYER.level];
		img.Content = "UI/EXPBar/layergauge";
		img.Length = img.GetLength();
		img.Length[0] = (1280.f - 15.0f) * (exp_prob);
		img.Location[0] = -(1280.f / 2.0f);
		img.Location[0] += (img.Length[0] / 2.0f) + 15.0f;
		img.Draw(false);

		std::string show_exp = "";
		std::string content = "UI/EXPBar/";
		exp_prob *= 10000.f;
		exp_prob = floor(exp_prob);
		exp_prob /= 100.f;
		std::to_string(static_cast<double>(PLAYER.exp));
		show_exp.append(std::to_string(PLAYER.exp));
		show_exp.append("[");
		show_exp.append(std::to_string(exp_prob));
		show_exp = show_exp.substr(0, show_exp.size() - 4);
		show_exp.append("%]");
		float total_len = 0;
		for (int i = 0; i < show_exp.size(); i++)
		{
			if (show_exp.at(i) == '.')
				img.Content = "UI/EXPBar/p";
			else {
				img.Content = content + show_exp.at(i);
			}
			total_len += img.GetLength()[0];
		}
		img.Location[0] = -(total_len / 2.0f);
		for (int i = 0; i < show_exp.size(); i++)
		{
			if (show_exp.at(i) == '.')
				img.Content = "UI/EXPBar/p";
			else
				img.Content = content + show_exp.at(i);
			img.Length = img.GetLength();
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
			img.Draw(false);
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
		}
	}
	// Hp, Mp 게이지바 
	{
		img.Location[0] = 0.0f;

		img.Content = "UI/status/backgrnd";
		img.Length = img.GetLength();
		img.Location[1] = ((img.Length[1] - 720.f) / 2.0f) + 15.f;
		img.Draw(false);

		img.Location[0] = 10.0f;

		float prob = (float)PLAYER.hp / PLAYER.maxHp;
		img.Content = "UI/status/hp";
		img.Length = img.GetLength();
		img.Length[0] *= prob;
		img.Location[0] = (img.Length[0] / 2.0f) - 85.5f;
		img.Location[1] = ((img.Length[1] - 720.f) / 2.0f) + 39.f;
		img.Draw(false);

		prob = (float)PLAYER.mp / PLAYER.maxMp;
		img.Content = "UI/status/mp";
		img.Length = img.GetLength();
		img.Length[0] *= prob;
		img.Location[0] = (img.Length[0] / 2.0f) - 85.5f;
		img.Location[1] = ((img.Length[1] - 720.f) / 2.0f) + 23.f;
		img.Draw(false);

		img.Location[0] = 0.0f;
		img.Content = "UI/status/cover";
		img.Length = img.GetLength();
		img.Location[1] = ((img.Length[1] - 720.f) / 2.0f) + 10.0f;
		img.Draw(false);

		img.Location = { -70.0f, 68.0f - (720.f / 2.0f) };
		img.Content = "UI/status/Lv";
		img.Length = img.GetLength();
		img.Draw(false);


		img.Location[0] += img.Length[0];
		std::string status_str = std::to_string(PLAYER.level);
		std::string content = "UI/status/lv";

		for (int i = 0; i < status_str.size(); i++)
		{
			img.Content = content + status_str.at(i);
			img.Length = img.GetLength();
			img.Draw(false);

			img.Location[0] += img.Length[0];
		}

		img.Location = { 10.0f, ((img.Length[1] - 720.f) / 2.0f) + 40.5f };
		content = "UI/status/";
		status_str = std::to_string(PLAYER.hp);
		status_str.append("s");
		status_str.append(std::to_string(PLAYER.maxHp));
		float total_len = 0;
		for (int i = 0; i < status_str.size(); i++)
		{
			img.Content = content + status_str.at(i);
			total_len += img.GetLength()[0];
		}
		img.Location[0] = 10.f - (total_len / 2.0f);
		for (int i = 0; i < status_str.size(); i++)
		{
			img.Content = content + status_str.at(i);
			img.Length = img.GetLength();
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
			img.Draw(false);
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
		}

		img.Location = { 10.0f, ((img.Length[1] - 720.f) / 2.0f) + 25.5f };
		status_str = std::to_string(PLAYER.mp);
		status_str.append("s");
		status_str.append(std::to_string(PLAYER.maxMp));
		total_len = 0;
		for (int i = 0; i < status_str.size(); i++)
		{
			img.Content = content + status_str.at(i);
			total_len += img.GetLength()[0];
		}
		img.Location[0] = 10.f - (total_len / 2.0f);
		for (int i = 0; i < status_str.size(); i++)
		{
			img.Content = content + status_str.at(i);
			img.Length = img.GetLength();
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
			img.Draw(false);
			img.Location[0] += (float)(img.GetLength()[0] / 2.0f);
		}
	}
	// miniMap
	{
		vector<2> mmLength;
		img.Content = "Map/miniMap/" + std::to_string(CURRENT_MAP->mapId);
		img.Length = img.GetLength();
		mmLength = img.Length;
		img.Location[0] = ((-1280.f + img.Length[0]) / 2.0f) + 9.0f;
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 21.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/n";
		img.Length = img.GetLength();
		img.Length[0] = mmLength[0] - 110.0f;
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f) + 64.0f;
		img.Location[1] = (720.f - 21.0f) / 2.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/s";
		img.Length = img.GetLength();
		img.Length[0] = mmLength[0] - 110.0f;
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f) + 64.0f;
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 21.0f - mmLength[1];
		img.Draw(false);

		img.Content = "Map/miniMap/base/w";
		img.Length = img.GetLength();
		img.Length[1] = mmLength[1] - 24.0f;
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f);
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 27.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/e";
		img.Length = img.GetLength();
		img.Length[1] = mmLength[1] - 24.0f;
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f) + 9.0f + mmLength[0];
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 27.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/nw";
		img.Length = img.GetLength();
		img.Location[0] = (-1280.f + img.Length[0]) / 2.0f;
		img.Location[1] = (720.f - img.Length[1]) / 2.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/ne";
		img.Length = img.GetLength();
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f) - 46.0f + mmLength[0];
		img.Location[1] = (720.f - img.Length[1]) / 2.0f;
		img.Draw(false);

		img.Content = "Map/miniMap/base/sw";
		img.Length = img.GetLength();
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f);
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 3.0f - mmLength[1];
		img.Draw(false);

		img.Content = "Map/miniMap/base/se";
		img.Length = img.GetLength();
		img.Location[0] = ((img.Length[0] - 1280.f) / 2.0f) - 46.0f + mmLength[0];
		img.Location[1] = ((720.f - img.Length[1]) / 2.0f) - 3.0f - mmLength[1];
		img.Draw(false);

		img.Content = "Map/miniMap/base/yellow";
		img.Length = img.GetLength();
		img.Location[0] = (-1280.f / 2.0f) + 9.0f;
		img.Location[1] = ((720.f / 2.0f) - 21.0f) - mmLength[1] + 6.0f;
		img.Location[0] += mmLength[0] * (PLAYER.position[0] + (CURRENT_MAP->mapSize[0] / 2.0f)) / CURRENT_MAP->mapSize[0];
		img.Location[1] += mmLength[1] * (PLAYER.position[1] + (CURRENT_MAP->mapSize[1] / 2.0f)) / CURRENT_MAP->mapSize[1];
		img.Draw(false);

	}
	// 캐릭터 닉네임
	{
		text.str = "캐릭터 닉네임";
		text.Color = { 255, 255, 255 };
		text.Location = { (1280.f / 2.0f) , 720.f - 93.0f };
		text.Length = { 80, 30 };
		text.Font.Name = "malgun";
		text.Font.Size = 11;
		text.Draw();
	}
}
