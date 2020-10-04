/*
 * Copyright © 2009-2020 Frictional Games
 * 
 * This file is part of Amnesia: The Dark Descent.
 * 
 * Amnesia: The Dark Descent is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version. 

 * Amnesia: The Dark Descent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Amnesia: The Dark Descent.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "LuxCompletionCountHandler.h"

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

cLuxCompletionCountHandler::cLuxCompletionCountHandler() : iLuxUpdateable("LuxCompletionCountHandler")
{
	mlQuestCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "QuestCompletionValue",0);
	mlItemCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "ItemCompletionValue",0);
	mlNoteCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "NoteCompletionValue",0);
	mlDiaryCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "DiaryCompletionValue",0);
	mlFlashbackCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "FlashbackCompletionValue",0);
	mlChestCompletionValue = gpBase->mpGameCfg->GetInt("Quest", "ChestCompletionValue",0);

	mpFont = NULL;
}

//-----------------------------------------------------------------------

cLuxCompletionCountHandler::~cLuxCompletionCountHandler()
{
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::LoadFonts()
{
	tString sFontFile = gpBase->mpMenuCfg->GetString("CompletionCounter","DefaultFont","");
	mpFont = LoadFont(sFontFile);
}

void cLuxCompletionCountHandler::OnStart()
{
	
}

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::Reset()
{
	mbActive = false;
	mfAlpha=0;
	mfDelay =0;

	mfNewAmount =0;
	mfDisplayedAmount =0;
}

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::Update(float afTimeStep)
{
	return;

	/////////////////////
	//Decrease alpha
	if(mbActive==false)
	{
		mfAlpha -= 0.5f * afTimeStep;
		if( mfAlpha < 0.0f ) mfAlpha = 0.0f;
		return;
	}

	if( mfDelay > 0.0f )
	{
		mfDelay -= afTimeStep;
		return;
	}

	/////////////////////
	//Increase alpha
	mfAlpha += 1.2f * afTimeStep;
	if( mfAlpha > 1.0f ) mfAlpha = 1.0f;

	/////////////////////
	//Increase count
	if(mfAlpha > 0.2f)
	{
		if(mfDisplayedAmount < mfNewAmount)
		{
			float fDistance = 0.0f;//(mfNewAmount - mfDisplayedAmount);
			mfDisplayedAmount += (fDistance+0.001f) * 30.0f * afTimeStep;
			if(mfDisplayedAmount > mfNewAmount)mfDisplayedAmount = mfNewAmount;
		}
		else
		{
			mfDisableDelay -=afTimeStep;
			if(mfDisableDelay <= 0.0f )
			{
				mbActive = false;
			}
		}
	}
}

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::OnDraw(float afFrameTime)
{
	return;
	
	float fPrecent = mfDisplayedAmount * 100.0f;
	gpBase->mpGameHudSet->DrawFont(mpFont, cVector3f(400.0f,20.0f,1.0f),21.0f,cColor(1.0f,mfAlpha), eFontAlign_Right,eGuiMaterial_FontNormal,
									_W("%.1f%%"), fPrecent);
	gpBase->mpGameHudSet->DrawFont(mpFont, cVector3f(400.0f,20.0f,1.0f),21.0f,cColor(1.0f,mfAlpha), eFontAlign_Left,eGuiMaterial_FontNormal,
									_W(" %ls"), kTranslate("CompletionCount", "Completed").c_str());
	
}

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::OnMapExit(cLuxMap *apMap)
{
	Reset();
}

//-----------------------------------------------------------------------

void cLuxCompletionCountHandler::ShowCompletionIncrease(float afPrevius, float afNew, float afDelay)
{
	return;

	if(afPrevius == afNew) return;

	mbActive = true;

	//If the count is already shown then do not set a new value.
	if(mfAlpha <=0)
	{
		mfDelay = afDelay;
		mfDisplayedAmount = afPrevius;
	}

    mfNewAmount = afNew;

	mfDisableDelay = 2.0f;
}


//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
