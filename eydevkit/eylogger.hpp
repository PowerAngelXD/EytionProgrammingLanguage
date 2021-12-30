#include "eydkit.h"
namespace eylogger{
    void log(const char text[]) {cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    void log(string text){cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    void log(int text){cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    void log(float text){cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    void log(double text){cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    void log(long text){cout<<_FONT_YELLOW<<text<<_NORMAL<<endl;}
    template<class TYPE>
    void log(vector<TYPE> text){
        for(auto t : text) cout<<_FONT_YELLOW<<t<<_NORMAL<<endl;
    }
}