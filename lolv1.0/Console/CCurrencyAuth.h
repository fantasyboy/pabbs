// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装器类

#import "..\\vd\\AllpurAuthentic.dll" no_namespace
// CCurrencyAuth 包装器类

class CCurrencyAuth : public COleDispatchDriver
{
public:
	CCurrencyAuth() {} // 调用 COleDispatchDriver 默认构造函数
	CCurrencyAuth(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCurrencyAuth(const CCurrencyAuth& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// ICurrencyAuth 方法
public:
	long Initialize(LPCTSTR product)
	{
		long result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, product);
		return result;
	}
	long UserAuth(LPCTSTR user, LPCTSTR pwd)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, user, pwd);
		return result;
	}
	long Auth(LPCTSTR serial)
	{
		long result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, serial);
		return result;
	}
	CString GetCode()
	{
		CString result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void Update(LPCTSTR cmd)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, cmd);
	}
	CString GetBulletin()
	{
		CString result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL IsValid()
	{
		BOOL result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	short DeductPoint(short point)
	{
		short result;
		static BYTE parms[] = VTS_I2;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I2, (void*)&result, parms, point);
		return result;
	}
	CString GetValidity()
	{
		CString result;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	unsigned short GetProductDataSize()
	{
		unsigned short result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_UI2, (void*)&result, NULL);
		return result;
	}
	unsigned char GetProductData(unsigned short pos)
	{
		unsigned char result;
		static BYTE parms[] = VTS_UI2;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_UI1, (void*)&result, parms, pos);
		return result;
	}
	long PutData(LPCTSTR data)
	{
		long result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, data);
		return result;
	}
	unsigned char GetData(unsigned char pos)
	{
		unsigned char result;
		static BYTE parms[] = VTS_UI1;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_UI1, (void*)&result, parms, pos);
		return result;
	}
	unsigned char GetUserType()
	{
		unsigned char result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_UI1, (void*)&result, NULL);
		return result;
	}
	CString GetVersion()
	{
		CString result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long AddTime(LPCTSTR card, LPCTSTR buyer, LPCTSTR super, short * days, short * point)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_PI2 VTS_PI2;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, card, buyer, super, days, point);
		return result;
	}
	long Unbind()
	{
		long result;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long UserRegister(LPCTSTR user, LPCTSTR pwd, unsigned char type, unsigned char bind, unsigned char multi, short point)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_UI1 VTS_UI1 VTS_UI1 VTS_I2;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, user, pwd, type, bind, multi, point);
		return result;
	}
	long ChangePassword(LPCTSTR Old, LPCTSTR New)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Old, New);
		return result;
	}
	BOOL IsConnected()
	{
		BOOL result;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString GetCurrTime()
	{
		CString result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long DeductHour(unsigned long hour)
	{
		long result;
		static BYTE parms[] = VTS_UI4;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, hour);
		return result;
	}
	long GetTotalLen()
	{
		long result;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetDownloadLen()
	{
		long result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void LeaveMsg(unsigned char type, LPCTSTR msg)
	{
		static BYTE parms[] = VTS_UI1 VTS_BSTR;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms, type, msg);
	}
	void InputVNC(long address, long len)
	{
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, address, len);
	}
	CString Encrypt(unsigned char type, LPCTSTR src, LPCTSTR key)
	{
		CString result;
		static BYTE parms[] = VTS_UI1 VTS_BSTR VTS_BSTR;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, type, src, key);
		return result;
	}
	CString Decrypt(unsigned char type, LPCTSTR src, LPCTSTR key)
	{
		CString result;
		static BYTE parms[] = VTS_UI1 VTS_BSTR VTS_BSTR;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, type, src, key);
		return result;
	}

	// ICurrencyAuth 属性
public:

};
