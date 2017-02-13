//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CopenHoldemStatusbar.h"

#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CFunctionCollection.h"
#include "CIteratorThread.h"
#include "CScraper.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineIsOmaha.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "..\StringFunctionsDLL\string_functions.h"

COpenHoldemStatusbar *p_openholdem_statusbar = NULL;

COpenHoldemStatusbar::COpenHoldemStatusbar(CWnd *main_window){
	_main_window = main_window;
  InitStatusbar();
  SetLastAction("");
  SetPrWin(0, 0, 0);
  SetIterations(0, 0);
}

COpenHoldemStatusbar::~COpenHoldemStatusbar() {
}

void COpenHoldemStatusbar::InitStatusbar() {
	_status_bar.Create(_main_window);
	_status_bar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
  int position = 0;
	_status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_ACTION, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_HANDRANK, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_PRWIN, NULL, 100);
  ++position;
  _status_bar.SetPaneInfo(position, ID_INDICATOR_STATUS_NIT, SBPS_STRETCH, 240);
}

void COpenHoldemStatusbar::GetWindowRect(RECT *statusbar_position) {
	_status_bar.GetWindowRect(statusbar_position);
}

void COpenHoldemStatusbar::OnUpdateStatusbar() {
  _status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), LastAction());
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), _status_handrank);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), _status_prwin);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NIT), _status_nit);
}

CString COpenHoldemStatusbar::LastAction() {
  if (p_symbol_engine_userchair == NULL)	{
		// Very early phase of initialization
		// Can't continue here.
		return "Not playing";
	}
	if (!p_symbol_engine_userchair->userchair_confirmed()) 	{
		return "Not playing";
	}
  if (p_symbol_engine_time->elapsedauto() > 5) {
    // Reset display of last action after 5 seconds
    return "";
  }
  // Return the last saved action.
  // This value should get set exactly once after autoplayer/actions
  // to avoid multiple evaluations of the autoplayer-functions,
  // especially at different heartbeats.
  return _last_action;
}

void COpenHoldemStatusbar::SetPrWin(double prwin, double prtie, double prlos) {
  // Turn probabilities into percentages;
  prwin *= 100;
  prtie *= 100;
  prlos *= 100;
  // Format them for display
  _status_prwin.Format("%3.1f/%3.1f/3.1f", prwin, prtie, prlos);
}

void COpenHoldemStatusbar::SetIterations(int calculated, int total) {
  _status_nit.Format("%i/%i", calculated, total);
}