//---------------------------------------------------------------------------

#ifndef TimeCalcH
#define TimeCalcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ProfGrid.hpp"
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include "LMDBaseEdit.hpp"
#include "LMDCalendarComboBox.hpp"
#include "LMDCustomBevelPanel.hpp"
#include "LMDCustomComponent.hpp"
#include "LMDCustomControl.hpp"
#include "LMDCustomEdit.hpp"
#include "LMDCustomExtCombo.hpp"
#include "LMDCustomMaskEdit.hpp"
#include "LMDCustomPanel.hpp"
#include "LMDDateTimeRangeValidator.hpp"
#include "LMDDBCalendarComboBox.hpp"
#include "LMDVldBase.hpp"
#include "LMDBaseControl.hpp"
#include "LMDBaseGraphicButton.hpp"
#include "LMDBaseGraphicControl.hpp"
#include "LMDBaseLabel.hpp"
#include "LMDControl.hpp"
#include "LMDCustomLabel.hpp"
#include "LMDCustomSpeedButton.hpp"
#include "LMDLabel.hpp"
#include "LMDSimplePanel.hpp"
#include "LMDSpeedButton.hpp"

#define END_TIME 18.0/24.0
#define START_TIME 8.0/24.0

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TProfGrid *HojaCalculo;
  TDateTimePicker *BeginTimePicker;
  TDateTimePicker *BeginDatePicker;
  TLMDSimplePanel *LMDSimplePanel1;
  TLMDLabel *LMDLabel1;
  TDateTimePicker *EndTimePicker;
  TDateTimePicker *EndDatePicker;
  TLMDLabel *LMDLabel2;
  TLMDSpeedButton *InsertBT;
  TLMDSpeedButton *CalcBT;
  void __fastcall InsertBTClick(TObject *Sender);
  void __fastcall CalcBTClick(TObject *Sender);
  void __fastcall HojaCalculoCellChanged(TProfGrid *Sender, int ACol,
          int ARow, const WideString OldText, WideString &NewText,
          bool &AllowExit);
  void __fastcall FormatRow(TObject *Sender);
  void __fastcall highlight(TObject *Sender);
  void __fastcall downlight(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);

  void AddTimeRow (int RowNum, TProfGrid *PG);
  double CalculaHoras (TDateTime first, TDateTime second);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
