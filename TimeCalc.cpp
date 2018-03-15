//---------------------------------------------------------------------------

#include <vcl.h>
#include <systdate.h>
#include <math.h>
#pragma hdrstop

#include "TimeCalc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ProfGrid"
#pragma link "LMDBaseEdit"
#pragma link "LMDCalendarComboBox"
#pragma link "LMDCustomBevelPanel"
#pragma link "LMDCustomComponent"
#pragma link "LMDCustomControl"
#pragma link "LMDCustomEdit"
#pragma link "LMDCustomExtCombo"
#pragma link "LMDCustomMaskEdit"
#pragma link "LMDCustomPanel"
#pragma link "LMDDateTimeRangeValidator"
#pragma link "LMDDBCalendarComboBox"
#pragma link "LMDVldBase"
#pragma link "LMDBaseControl"
#pragma link "LMDBaseGraphicButton"
#pragma link "LMDBaseGraphicControl"
#pragma link "LMDBaseLabel"
#pragma link "LMDControl"
#pragma link "LMDCustomLabel"
#pragma link "LMDCustomSpeedButton"
#pragma link "LMDLabel"
#pragma link "LMDSimplePanel"
#pragma link "LMDSpeedButton"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
  // formatear el profgrid
  // col1: begin  col2: end
  // col3: num dias col4: dias fin de semana
  // col5: horas

  TProfGrid *PG = HojaCalculo;
  PG->ColCount = 4;
  PG->RowCount = 1;

  PG->Cols[1]->Width = 150;
  PG->Cols[2]->Width = 150;
  PG->Cols[3]->Width =  90;

  AddTimeRow (2, PG);
}
//---------------------------------------------------------------------------
void TForm1::AddTimeRow (int RowNum, TProfGrid * PG)
{
  // añadir una fila
  PG->RowCount = RowNum;
  RowNum --;
  
  // dar formato a la fila
  PG->Cells[0][RowNum]->Value = 0;
  PG->Cells[1][RowNum]->Color = clYellow;
  PG->Cells[2][RowNum]->Color = clYellow;
  PG->Cells[1][RowNum]->ReadOnly = false;
  PG->Cells[2][RowNum]->ReadOnly = false;
  PG->Cells[3][RowNum]->ReadOnly = true;
  PG->Cells[3][RowNum]->Format = "#.000";
}

//---------------------------------------------------------------------------
void __fastcall TForm1::InsertBTClick(TObject *Sender)
{
  // añade una nueva fila
  TProfGrid *PG = HojaCalculo;
  int NewRow = PG->RowCount + 1;
  AddTimeRow(NewRow, PG);

  System::TDateTime begin, end;
  double DateInput, TimeInput;

  begin = floor(BeginDatePicker->Date) + fmod(BeginTimePicker->Time, 1);
  end   = floor(EndDatePicker->Date  ) + fmod(EndTimePicker->Time  , 1);

  // rellena la fecha + hora begin y end
  PG->Cells[1][NewRow-1]->Value = begin;
  PG->Cells[2][NewRow-1]->Value = end;

  // col 0 valor 0
  PG->Cells[0][NewRow-1]->Value = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CalcBTClick(TObject *Sender)
{
  TProfGrid *PG = HojaCalculo;
  int row;
  System::TDateTime begin, end;

  // para cada fila que la celda 1 contega el valor 0
  for (row = 1; row < PG->RowCount; row++)
  {
    if (PG->Cells[0][row]->Value == 0)
    {
      try
      {
        begin = (TDateTime) PG->Cells[1][row]->Value;
        end   = (TDateTime) PG->Cells[2][row]->Value;
        if (begin > end) PG->Cells[3][row]->Value = "error: fecha de fin anterior a fecha de inicio";
        else PG->Cells[3][row]->Value = CalculaHoras(begin, end);
      }
      catch (...)
      {
        PG->Cells[3][row]->Value = "inputs incorrectos";
      }
      PG->Cells[0][row]->Value = 1;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::HojaCalculoCellChanged(TProfGrid *Sender, int ACol,
      int ARow, const WideString OldText, WideString &NewText,
      bool &AllowExit)
{
  // al modificar col 1 o col 2 la col 0 serellena con un 0.
  TProfGrid *PG = Sender;
  PG->Cells[0][ARow]->Value = 0;
}
//---------------------------------------------------------------------------
double  TForm1::CalculaHoras(TDateTime first, TDateTime last)
{
  double HoursCount = 0.0, HoursTmp = 0.0;
  TDateTime current, start, end;

  // comprobar que last > first
  if(last < first) return 0.0;
  start = (double) first.operator int() + START_TIME;
  end   = (double) last.operator int()  + END_TIME;

  // horas del primer día antes de END_TIME si no es fin de semana
  if (DayOfWeek(first) > 1 && DayOfWeek(first) < 7 )
  {
    HoursCount = floor(first) + END_TIME - (first>start?first:start);
    if (HoursCount < 0) HoursCount = 0;
  }

  // horas del último día después de START_TIME si no es fin de semana
  if (DayOfWeek(last) > 1 && DayOfWeek(last) < 7)
  {
    HoursTmp = (last<end?last:end) - (floor(last) + START_TIME);
    if (HoursTmp < 0) HoursTmp = 0;
    HoursCount = HoursCount + HoursTmp + START_TIME - END_TIME;  // restar END-START porque se los vamos a sumar otra vez en el siguiente paso
  }

  current = first +1;
  while ( current.operator int()  <= last.operator int() )       // integer y <= ???
  {
    switch (DayOfWeek(current))
    {
      case (1) : HoursCount = HoursCount + 0;                     break;   // Sunday
      case (2) : HoursCount = HoursCount + END_TIME - START_TIME; break;   // Monday
      case (3) : HoursCount = HoursCount + END_TIME - START_TIME; break;   // Tuesday
      case (4) : HoursCount = HoursCount + END_TIME - START_TIME; break;   // Wednesday
      case (5) : HoursCount = HoursCount + END_TIME - START_TIME; break;   // Thursday
      case (6) : HoursCount = HoursCount + END_TIME - START_TIME; break;   // Friday
      case (7) : HoursCount = HoursCount + 0;                     break;   // Saturday
    }
    current =current +1;
  }

  if (HoursCount < 0) HoursCount = 0;

  // convertir fracción de dias en horas
  HoursCount = 24*HoursCount;

  return HoursCount;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormatRow(TObject *Sender)
{
  TProfGrid *PG = (TProfGrid *)Sender;

  for (int RowNum = 1; RowNum < PG->RowCount; RowNum++)
  {
    PG->Cells[0][RowNum]->Value = 0;
    PG->Cells[1][RowNum]->Color = clYellow;
    PG->Cells[2][RowNum]->Color = clYellow;
    PG->Cells[1][RowNum]->ReadOnly = false;
    PG->Cells[2][RowNum]->ReadOnly = false;
    PG->Cells[3][RowNum]->ReadOnly = true;
    PG->Cells[3][RowNum]->Format = "#.000";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::highlight(TObject *Sender)
{
  TLMDSpeedButton *BT = (TLMDSpeedButton *)Sender;
  BT->Color = 0x02C7D4FA;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::downlight(TObject *Sender)
{
  TLMDSpeedButton *BT = (TLMDSpeedButton *)Sender;
  BT->Color = clBtnFace;
}
//---------------------------------------------------------------------------

