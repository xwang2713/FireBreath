/**********************************************************\ 
Original Author: Richard Bateman (taxilian)

Created:    Sep 21, 2010
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 PacketPass, Inc and the Firebreath development team
\**********************************************************/

#include "Document.h"
#include "IDispatchAPI.h"

#include "Window.h"

using namespace AXDOM;

Window::Window(FB::JSObjectPtr obj, IWebBrowser2 *web)
    : m_webBrowser(web), m_htmlWin(as_IDispatchAPI(obj)->getIDispatch()), FB::DOM::Window(obj)
{
    if (!m_htmlWin) {
        throw new std::bad_cast("This is not a valid Window object");
    }
}

Window::~Window()
{
}

FB::DOM::DocumentPtr Window::getDocument()
{
    CComPtr<IHTMLDocument2> htmlDoc;
    m_htmlWin->get_document(&htmlDoc);
    CComQIPtr<IDispatch> htmlDocDisp(htmlDoc);
    FB::JSObjectPtr docAPI(new IDispatchAPI(htmlDocDisp, as_ActiveXBrowserHost(this->m_element->host)));
    return FB::DOM::Document::create(docAPI);
}

void Window::alert(const std::string& str)
{
    m_htmlWin->alert(CComBSTR(FB::utf8_to_wstring(str).c_str()));
}

std::string Window::getLocation()
{
    CComBSTR bstr;
    //m_webBrowser->get_LocationURL(&bstr);
    CComPtr<IHTMLLocation> location;
    m_htmlWin->get_location(&location);
    location->get_href(&bstr);
    return FB::wstring_to_utf8(std::wstring(bstr.m_str));
}