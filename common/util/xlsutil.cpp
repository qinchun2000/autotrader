#include <string.h>
#include <xlslib/xlslib.h>

using namespace xlslib_core;
using namespace std;

int main (int argc, char *argv[]) 
{
workbook wb;
xf_t* xf = wb.xformat();
worksheet* ws;
ws = wb.sheet("sheet1");
string label = "Hello, World!";
ws->label(1,2,label,xf); // 从0开始数，第1行，第2列，即C3
wb.Dump("workbook.xls");
return 0;
}
