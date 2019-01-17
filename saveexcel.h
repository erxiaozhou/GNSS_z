#include <QAxObject>
#include <QDir>
#include <QFile>
class ExcelHelper
{
    public:
        ExcelHelper();

        void newExcel(const QString &fileName);
        void appendSheet(const QString &sheetName);
        void setCellValue(int row, int column, const QString &value);
        void saveExcel(const QString &fileName);
        void freeExcel();

        QAxObject *pApplication;
        QAxObject *pWorkBooks;
        QAxObject *pWorkBook;
        QAxObject *pSheets;
        QAxObject *pSheet;
};
