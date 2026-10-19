#include "CCharacterManager.h"

void CCharacterManager::Delete()
{
	//イテレータの生成
	std::vector<CCharacter*>::iterator itr;
	//イテレータを先頭へ
	itr = mpCharacters.begin();
	//最後まで繰り返し
	while (itr != mpCharacters.end())
	{
		if ((*itr)->Enabled())
		{
			//次へ
			itr++;
		}
		else
		{
			//falseの時、インスタンスを削除
			delete* itr;
			//配列から削除
			itr = mpCharacters.erase(itr);
		}
	}
}

void CCharacterManager::Collision()
{
	for (size_t i = 0; i < mpCharacters.size(); i++)
	{	//衝突処理２の呼び出し
		mpCharacters[i]->Collision();
	}
}

void CCharacterManager::Collision(CCharacter* character)
{
	for (size_t i = 0; i < mpCharacters.size(); i++)
	{	//衝突処理４の呼び出し
		character->Collision(character, mpCharacters[i]);
	}
}

void CCharacterManager::Add(CCharacter* c)
{
	mpCharacters.push_back(c);
}

void CCharacterManager::Update()
{
	for (size_t i = 0; i < mpCharacters.size(); i++)
	{
		mpCharacters[i]->Update();
	}
}
void CCharacterManager::Render()
{
	for (size_t i = 0; i < mpCharacters.size(); i++)
	{
		mpCharacters[i]->Render();
	}
}

void CCharacterManager::AllDelete()
{
	//イテレータの生成
	std::vector<CCharacter*>::iterator itr;
	//イテレータを先頭へ
	itr = mpCharacters.begin();
	//最後まで繰り返し
	while (itr != mpCharacters.end())
	{
		//インスタンスを削除
		delete* itr;
		//配列から削除
		itr = mpCharacters.erase(itr);
	}
}
