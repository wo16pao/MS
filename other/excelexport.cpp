#include "excelexport.h"

ExcelExport::ExcelExport()
{

}
void ExcelExport::newExcel(const QString &fileName)
{
    pApplication = new QAxObject();
    //连接Excel控件
    pApplication->setControl("Excel.Application");
    //false不显示窗体（看具体过程）
    pApplication->dynamicCall("SetVisible(bool)", false);
    //不显示任何警告信息
    pApplication->setProperty("DisplayAlerts", false);

    pWorkBooks = pApplication->querySubObject("Workbooks");

    QFile file(fileName);
    if (file.exists())
    {
        pWorkBook = pWorkBooks->querySubObject(
            "Open(const QString &", fileName);
    }else {
        pWorkBooks->dynamicCall("Add");
        pWorkBook =
            pApplication->querySubObject("ActiveWorkBook");
    }
    //默认有一个worksheet
    pSheets = pWorkBook->querySubObject("Sheets");
    pSheet = pSheets->querySubObject("Item(int)",1);
}

void ExcelExport::setCellValue(int row, int column
    , const QString &value)
{
    QAxObject *pRange = pSheet->querySubObject("Cells(int,int)"
        , row, column);
    pRange->dynamicCall("Value", value);
    //内容居中
    pRange->setProperty("HorizontalAlignment", -4108);
    pRange->setProperty("VerticalAlignment", -4108);
}


void ExcelExport::saveExcel(const QString &fileName)
{
    pWorkBook->dynamicCall("SaveAs(const QString &)",
        QDir::toNativeSeparators(fileName));

    pApplication->dynamicCall("Quit(void)");  //退出
    delete pApplication;
}

ExcelExport::~ExcelExport()
{
}
